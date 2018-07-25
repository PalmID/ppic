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
#include <memory>

namespace {

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
    SessionPoolSingleton::instance()->DestroyPool();
  }
};

TEST_F(UserModelTestFixture, test_create_table) {
  auto sess = SessionPoolSingleton::instance()->ObtainSession();
  SqlResult result = UserDbManagerSingleton::instance()->CreateTable();
}

}   // namespace 
