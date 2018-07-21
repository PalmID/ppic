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
#include <stdio.h>

using ppic::db::SessionPoolOption;

TEST(SessionPoolOptionTest, test_url_with_constructor_parameters) {
  auto option = SessionPoolOption("root", "test", "localhost", 16);
  EXPECT_EQ(option.url(), "root:test@localhost:33060");
}

TEST(SessionPoolOptionTest, test_url_with_env) {
  const char* variable = "MYSQL_CONNECTION_URL";
  const char* url = "root:123@localhost:12345";
  setenv(variable, url, 1);
  auto option = SessionPoolOption();
  EXPECT_EQ(option.UrlFromEnv(variable).set_capacity(16).url(), url);
}