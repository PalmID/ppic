// -*- C++ -*-
//===--------------------------- singleton.h ----------------------------------===//
//
// Created by leosocy on 2018/7/18.
// Copyright (c) 2018 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//

#ifndef PPIC_COMMON_SINGLETON_H
#define PPIC_COMMON_SINGLETON_H

#include <memory>

namespace ppic {

template <class T>
class Singleton {
public:
  Singleton& operator=(const Singleton&) = delete;
  Singleton(const Singleton&) = delete;

  static std::shared_ptr<T> instance()
  {
    static std::shared_ptr<T> inst{new T};
    return inst;
  }

protected:
  Singleton() {};
};

} // namespace ppic

#endif //PPIC_COMMON_SINGLETON_H
