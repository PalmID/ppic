// -*- C++ -*-
//===--------------------------- user.h ----------------------------------===//
//
// Created by leosocy on 2018/7/18.
// Copyright (c) 2018 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//

#ifndef PPIC_USER_H
#define PPIC_USER_H

#include "common/singleton.h"
#include <string>

namespace ppic {

namespace model {

using std::string;

class User {
public:
  User() = delete;

  uint64_t id() { return id_; }
  const string& name() { return name_; }
  const string& registration_date() { return registration_date_ ; }
protected:
private:
  uint64_t id_;
  string name_;
  string registration_date_;

  friend class UserDbManager;
};

class UserDbManager : public Singleton<UserDbManager> {
public:
  const User& CreateUser(const string& name, const string& registration_date) {}
};

} // namespace model

} // namespace ppic

#endif //PPIC_USER_H
