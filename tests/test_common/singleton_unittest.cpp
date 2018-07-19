// -*- C++ -*-
//===--------------------- singleton_unittest.cpp -------------------------===//
//
// Created by leosocy on 2018/7/18.
// Copyright (c) 2018 Leosocy. All rights reserved.
// Use of this source code is governed by a MIT-style license 
// that can be found in the LICENSE file.
//
//===----------------------------------------------------------------------===//

#include "common/singleton.h"
#include "gtest/gtest.h"
#include <stdio.h>

using ppic::Singleton;

TEST(SingletonTest, test_singleton_instance_address_equal) {
  auto p1 = Singleton<int>::instance();
  auto p2 = Singleton<int>::instance();
  EXPECT_EQ(p1, p2);
}

class SingletonTest : public Singleton<SingletonTest> {
public:
  SingletonTest() = default;
};

TEST(SingletonTest, test_singleton_inheritance_address_equal) {
  auto p1 = SingletonTest::instance();
  auto p2 = SingletonTest::instance();
  EXPECT_EQ(p1, p2);
}

TEST(SingletonTest, test_singleton_difference_object_address_equal) {
  auto p1 = Singleton<int>::instance();
  auto p2 = SingletonTest::instance();
  EXPECT_NE((unsigned long*)p1.get(), (unsigned long*)p2.get());
}