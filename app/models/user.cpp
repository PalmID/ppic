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

mysqlx::SqlResult UserDbManager::CreateTable(const char* table_name) {
  auto session = SessionPoolSingleton::instance()->ObtainSession();
  std::lock_guard<std::mutex> lock(table_mtx_);
  if (!table_name_.empty() && table_name_ != table_name) {
    char msg[128];
    snprintf(msg, 128, "[ERROR] Can't create `user` model twice with different table name.");
    throw std::runtime_error(msg);
  }
  table_name_ = table_name;
  char create_sql[1024];
  snprintf(create_sql, 1024,
           "CREATE TABLE IF NOT EXISTS %s (             \
            id INT UNSIGNED NOT NULL AUTO_INCREMENT,    \
            name VARCHAR(128) NOT NULL,                 \
            registration_date TIMESTAMP NOT NULL        \
                DEFAULT CURRENT_TIMESTAMP,              \
            PRIMARY KEY (id)                            \
            );", table_name);
  return session->sql(create_sql).execute();
}

std::unique_ptr<User> UserDbManager::CreateUser(const string& name) {
  auto session = SessionPoolSingleton::instance()->ObtainSession();
  std::unique_ptr<User> user{new User()};
  user->id_ = 0;
  user->name_ = name;
  user->registration_date_ = "";
  return user;
}

}   // namespace model

}   // namespace ppic