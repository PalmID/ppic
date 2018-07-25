/****************************************************************************\
 * Created on Thu Jul 26 2018
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

#include "db/smart_session.h"
#include "db/session_pool.h"
#include "gtest/gtest.h"
#include "mysqlx/xdevapi.h"

namespace {

using ppic::db::SmartSession;
using ppic::db::SessionPoolOption;

TEST(SmartSessionTest, test_construct_with_valid_option) {
  SessionPoolOption option;
  option.FromEnv().set_capacity(1);

  EXPECT_NO_THROW(new SmartSession(option));
}

TEST(SmartSessionTest, test_construct_with_invalid_option) {
  const char* url_env = "MYSQL_CONNECTION_URL_TEST";
  const char* url = "root:123@localhost:12345";
  const char* db_env = "MYSQL_DATABASE_TEST";
  const char* db = "test";
  setenv(url_env, url, 1);
  setenv(db_env, db, 1);
  SessionPoolOption option;
  option.FromEnv(url_env, db_env).set_capacity(1);

  EXPECT_THROW(new SmartSession(option), mysqlx::Error);

  unsetenv(url_env);
  unsetenv(db_env);
}

TEST(SmartSessionTest, test_get_current_schema) {
  const char* url_env = "MYSQL_CONNECTION_URL";
  const char* db_env = "MYSQL_DATABASE_TEST";
  const char* db = "test";
  setenv(db_env, db, 1);
  SessionPoolOption option;
  option.FromEnv(url_env, db_env).set_capacity(1);
  
  SmartSession ss(option);
  auto schema = ss.GetCurrentSchema();
  std::string schema_name = schema.getName();
  EXPECT_TRUE(schema_name == db);

  unsetenv(db_env);
  ss.dropSchema(db);
}

TEST(SmartSessionTest, test_select_schema) {
  SessionPoolOption option;
  option.FromEnv().set_capacity(1);
  
  SmartSession ss(option);
  const char* db = "select_schema";
  ss.SelectSchema(db);
  auto schema = ss.GetCurrentSchema();
  std::string schema_name = schema.getName();
  EXPECT_TRUE(schema_name == db);

  ss.dropSchema(db);
}

}   // namespace