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

#ifndef PPIC_APP_MODELS_USER_H_
#define PPIC_APP_MODELS_USER_H_

#include "common/singleton.h"
#include "mysqlx/xdevapi.h"
#include <string>
#include <memory>

namespace ppic {

namespace model {

class UserDbManager;
typedef Singleton<UserDbManager> UserDbManagerSingleton;

using std::string;
using mysqlx::Table;

class User {
public:
  User(const User&) = delete;
  User& operator=(const User&) = delete;

  uint64_t id() { return id_; }
  const string& name() { return name_; }
  const string& registration_date() { return registration_date_ ; }
protected:
private:
  friend class UserDbManager;
  User() {}

  uint64_t id_;
  string name_;
  string registration_date_;
};

class UserDbManager {
public:
  UserDbManager(const UserDbManager&) = delete;
  UserDbManager& operator=(const UserDbManager&) = delete;

  std::shared_ptr<Table> table() { return table_; }
  std::unique_ptr<User> CreateUser(const string& name);
private:
  friend class Singleton<UserDbManager>;
  UserDbManager() {}
  ~UserDbManager() {}

  string table_name_;
  std::shared_ptr<Table> table_;
};

} // namespace model

} // namespace ppic

#endif //PPIC_APP_MODELS_USER_H_
