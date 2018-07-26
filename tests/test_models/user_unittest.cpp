/****************************************************************************\
 * Created on Tue Jul 24 2018
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
#include "gtest/gtest.h"
#include "mysqlx/xdevapi.h"
#include <string>
#include <memory>

namespace {

using std::string;
using mysqlx::SqlResult;

using ppic::db::SessionPool;
using ppic::db::SessionPoolOption;
using ppic::db::SessionPoolSingleton;
using ppic::model::User;
using ppic::model::UserDbManager;
using ppic::model::UserDbManagerSingleton;

class UserModelTestFixture : public testing::Test {
 protected:
  virtual void SetUp() {
    SessionPoolOption option;
    option.FromEnv().set_capacity(4);
    SessionPoolSingleton::instance()->InitPool(option);
  }
  virtual void TearDown() {
    UserDbManagerSingleton::instance()->DropTable();
    SessionPoolSingleton::instance()->DestroyPool();
  }
};

TEST_F(UserModelTestFixture, test_get_or_create_table) {
  const char* user_tbl_name = "user_test";
  auto tbl = UserDbManagerSingleton::instance()->GetOrCreateTable(user_tbl_name);
  EXPECT_EQ(string(tbl->getName()), user_tbl_name);
}

TEST_F(UserModelTestFixture, test_get_or_create_table_twice_with_same_name) {
  const char* user_tbl_name = "user_test";
  auto tbl1 = UserDbManagerSingleton::instance()->GetOrCreateTable(user_tbl_name);
  auto tbl2 = UserDbManagerSingleton::instance()->GetOrCreateTable(user_tbl_name);

  EXPECT_EQ(string(tbl1->getName()), user_tbl_name);
  EXPECT_EQ(string(tbl2->getName()), user_tbl_name);
  EXPECT_EQ(tbl1, tbl2);
}

TEST_F(UserModelTestFixture, test_get_or_create_table_twice_with_different_name) {
  const char* user_tbl_name1 = "user_test";
  const char* user_tbl_name2 = "user_test_diff";
  auto tbl1 = UserDbManagerSingleton::instance()->GetOrCreateTable(user_tbl_name1);

  EXPECT_THROW(UserDbManagerSingleton::instance()->GetOrCreateTable(user_tbl_name2), std::runtime_error);
}

TEST_F(UserModelTestFixture, test_create_user) {
  string user_name("user 1");
  auto user = UserDbManagerSingleton::instance()->CreateUser(user_name);

  EXPECT_EQ(user->name(), user_name);
}   

}   // namespace 
