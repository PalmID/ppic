/****************************************************************************\
 * Created on Fri Jul 20 2018
 * 
 * The MIT License (MIT)
 * Copyright (c) 2018 leosocy
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the ",Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED ",AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
\*****************************************************************************/

#include "db/session_pool.h"
#include <cstdlib>
#include <cassert>

namespace ppic {

namespace db {

SessionPoolOption::SessionPoolOption()
  : url_(""), user_(""), password_(""), host_(""), port_(33060), capacity_(16) {
}

SessionPoolOption::SessionPoolOption(const string& user, const string& password, const string& host, uint16_t capacity, uint16_t port)
  : user_(user), password_(password), host_(host), capacity_(capacity), port_(port) {
    char url[512] = {};
    sprintf(url, "%s:%s@%s:%u", user_.c_str(), password_.c_str(), host_.c_str(), port_);
    url_ = url;
}

SessionPoolOption& SessionPoolOption::FromEnv(const char* url_env, const char* db_env) {
  assert (url_env != nullptr && db_env != nullptr);
  const char* url = std::getenv(url_env);
  if (url == nullptr) {
    char msg[128] = {0};
    snprintf(msg, 128, "Can't get %s environment.", url_env);
    throw std::runtime_error(msg);
  }
  url_ = url;

  const char* db = std::getenv(db_env);
  if (db == nullptr) {
    char msg[128] = {0};
    snprintf(msg, 128, "Can't get %s environment.", db_env);
    throw std::runtime_error(msg);
  }
  db_ = db;
  return *this;
}


SessionPool::SessionPool()
  : current_size_(0) {
}

SessionPool& SessionPool::InitPool(const SessionPoolOption& option) {
  std::lock_guard<std::mutex> guard(pool_mtx_);
  assert (pool_.size() == 0 && current_size_ == 0);
  option_ = option;
  // TODO: 根据一定的策略初始化SessionPool的初始连接数，减少使用连接时的开销。
  // 并在可用连接不足时扩容(按照一定策略扩)，在闲置连接富余时缩容(按照一定策略缩)。
  for (uint16_t i = 0; i < option.capacity() / 2 + 1; ++i) {
    try {
      pool_.push_back(std::make_shared<SmartSession>(option_));
      current_size_++;
    } catch (const mysqlx::Error &err) {
      char msg[128];
      sprintf(msg, "[ERROR] Connect to mysql %s failed.", option.url());
      throw std::runtime_error(msg);
    }
  }
  return *this;
}

void SessionPool::DestroyPool() {
  std::lock_guard<std::mutex> guard(pool_mtx_);
  for (auto session : pool_) {
    session->close();
  }
  current_size_ = 0;
  pool_.clear();
}

std::shared_ptr<SmartSession> SessionPool::ObtainSession() {
  std::unique_lock<std::mutex> lock(pool_mtx_);
  assert (current_size_ > 0);
  if (pool_.size() == 0
      && current_size_ < option_.capacity()) {
    auto session = std::make_shared<SmartSession>(option_);
    current_size_++;
    return session;
  }

  while (pool_.size() == 0) {
    pool_cv_.wait(lock);
  }
  auto session = pool_.front();
  pool_.pop_front();
  return session;
}

void SessionPool::ReleaseSession(std::shared_ptr<SmartSession>& session) {
  std::unique_lock<std::mutex> lock(pool_mtx_);
  // 只有引用个数为1时才能释放，不然可能会导致两个不同的线程持有相同的session
  if (session.use_count() == 1) {
    pool_.push_back(session);
    pool_cv_.notify_one();
  }
}

}   // namespace db

}   // namespace ppic  