/****************************************************************************\
 * Created on Thu Jul 19 2018
 *
 * The MIT License (MIT)
 * Copyright (c) 2018 leosocy
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the ",Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED ",AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
\*****************************************************************************/

#include "common/singleton.h"
#include "gtest/gtest.h"
#include <stdio.h>

namespace {

using ppic::Singleton;

TEST(SingletonTest, test_singleton_instance_address_equal) {
  auto p1 = Singleton<int>::instance();
  auto p2 = Singleton<int>::instance();
  EXPECT_EQ(p1, p2);
}

class SingletonTest {
public:
  const char* run() {
    return "hello";
  }
private:
  friend class Singleton<SingletonTest>;
  SingletonTest() {}
};

TEST(SingletonTest, test_singleton_inheritance_address_equal) {
  auto p1 = Singleton<SingletonTest>::instance();
  auto p2 = Singleton<SingletonTest>::instance();
  EXPECT_EQ(p1, p2);
  EXPECT_EQ(Singleton<SingletonTest>::instance()->run(), "hello");
}

TEST(SingletonTest, test_singleton_difference_object_address_equal) {
  auto p1 = Singleton<int>::instance();
  auto p2 = Singleton<SingletonTest>::instance();
  EXPECT_NE((unsigned long*)p1.get(), (unsigned long*)p2.get());
}

}   // namespace