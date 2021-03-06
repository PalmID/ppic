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
#include "gtest/gtest.h"
#include "mysqlx/xdevapi.h"
#include <stdio.h>
#include <map>
#include <thread>
#include <chrono>

namespace {

using ppic::db::SessionPoolSingleton;
using ppic::db::SessionPoolOption;

TEST(SessionPoolOptionTest, test_url_with_constructor_parameters) {
  auto option = SessionPoolOption("root", "test", "localhost", 16);
  EXPECT_EQ(option.url(), "root:test@localhost:33060");
  EXPECT_EQ(option.capacity(), 16);
}

TEST(SessionPoolOptionTest, test_url_and_db_with_env) {
  const char* url_env = "MYSQL_CONNECTION_URL_TEST";
  const char* url = "root:123@localhost:12345";
  const char* db_env = "MYSQL_DATABASE_TEST";
  const char* db = "test";
  setenv(url_env, url, 1);
  setenv(db_env, db, 1);
  SessionPoolOption option;
  EXPECT_EQ(option.FromEnv(url_env, db_env).set_capacity(16).url(), url);
  EXPECT_EQ(option.db(), db);
  EXPECT_EQ(option.capacity(), 16);
  unsetenv(url_env);
  unsetenv(db_env);
}

TEST(SessionPoolOptionTest, test_url_with_env_doesnt_exist) {
  const char* url_env = "MYSQL_CONNECTION_URL_TEST";
  SessionPoolOption option;
  EXPECT_THROW(option.FromEnv(url_env).set_capacity(16).url(), std::runtime_error);
  const char* db_env = "MYSQL_DB_TEST";
  EXPECT_THROW(option.FromEnv("MYSQL_CONNECTION_URL", db_env).set_capacity(16).url(), std::runtime_error);
}

TEST(SessionPoolTest, test_incorrect_mysql_url_when_create_session) {
  SessionPoolOption option("root", "incorrect", "mysql", 16);
  EXPECT_THROW(SessionPoolSingleton::instance()->InitPool(option), std::runtime_error);
}

TEST(SessionPoolTest, test_capacity_2_when_3_threads_want_to_obtain_session) {
  SessionPoolOption option;
  option.FromEnv().set_capacity(2);
  SessionPoolSingleton::instance()->InitPool(option);

  std::map<std::thread::id, long> sess_thread;
  std::mutex map_mtx;
  auto entry = [&](int ms) {
    auto sess = SessionPoolSingleton::instance()->ObtainSession();
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    SessionPoolSingleton::instance()->ReleaseSession(sess);
    std::lock_guard<std::mutex> guard(map_mtx);
    sess_thread.insert(std::pair<std::thread::id, long>(std::this_thread::get_id(), (long)sess.get()));
  };
  std::thread t1(entry, 200);
  std::thread t2(entry, 300);
  std::thread::id t1_id = t1.get_id();
  std::thread::id t2_id = t2.get_id();
  t1.join();
  t2.join();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::thread t3(entry, 0);
  std::thread::id t3_id = t3.get_id();
  t3.join();

  EXPECT_TRUE(sess_thread.at(t1_id) == sess_thread.at(t3_id) || sess_thread.at(t2_id) == sess_thread.at(t3_id));

  SessionPoolSingleton::instance()->DestroyPool();
}

}   // namespace