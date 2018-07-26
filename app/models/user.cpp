/****************************************************************************\
 * Created on Thu Jul 19 2018
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

#include "models/user.h"
#include "db/session_pool.h"
#include "db/smart_session.h"

namespace ppic {

namespace model {

using ppic::db::SessionPoolSingleton;

std::shared_ptr<Table> UserDbManager::GetOrCreateTable(const char* table_name) {
  std::lock_guard<std::mutex> lock(table_mtx_);
  if (table_ && table_name_ != table_name) {
    char msg[128];
    snprintf(msg, 128, "[ERROR] Can't create `user` model twice with different table name.");
    throw std::runtime_error(msg);
  }
  if (!table_) {
    table_name_ = table_name;
  } else {
    return table_;
  }
  
  auto session = SessionPoolSingleton::instance()->ObtainSession();
  char create_sql[1024];
  snprintf(create_sql, 1024,
           "CREATE TABLE IF NOT EXISTS %s (             \
            id INT UNSIGNED NOT NULL AUTO_INCREMENT,    \
            name VARCHAR(128) NOT NULL,                 \
            registration_at TIMESTAMP NOT NULL          \
                DEFAULT CURRENT_TIMESTAMP,              \
            PRIMARY KEY (id)                            \
            );", table_name);
  session->sql(create_sql).execute();
  table_ = std::make_shared<Table>(session->GetCurrentSchema().getTable(table_name));
  SessionPoolSingleton::instance()->ReleaseSession(session);
  return table_;
}

void UserDbManager::DropTable() {
  std::lock_guard<std::mutex> lock(table_mtx_);
  if (!table_) {
    return;
  }
  auto session = SessionPoolSingleton::instance()->ObtainSession();
  session->dropSchema(table_name_);
  SessionPoolSingleton::instance()->ReleaseSession(session);
  table_.reset();
}

std::shared_ptr<User> UserDbManager::CreateUser(const string& name) {
  std::unique_lock<std::mutex> lock(table_mtx_);
  if (!table_) {
    lock.unlock();
    GetOrCreateTable();
    lock.lock();
  }
  auto res = table_->insert("name").values(name).execute();
  uint64_t id = res.getAutoIncrementValue();
  lock.unlock();
  return GetUserById(id);
}

std::shared_ptr<User> UserDbManager::GetUserById(uint64_t id) {
  std::unique_lock<std::mutex> lock(table_mtx_);
  if (!table_) {
    lock.unlock();
    GetOrCreateTable();
    lock.lock();
  }
  auto res = table_->select("id", "name", "registration_at").where("id = :id").bind("id", id).execute();
  auto row = res.fetchOne();
  lock.unlock();
  return RowToUser(row);
}

std::shared_ptr<User> UserDbManager::RowToUser(const mysqlx::Row& row) {
  return std::shared_ptr<User>{new User(row[0], row[1], row[2])};
}

}   // namespace model

}   // namespace ppic