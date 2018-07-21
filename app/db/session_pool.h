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

#ifndef PPIC_DB_SESSION_POOL_H_
#define PPIC_DB_SESSION_POOL_H_

#include "common/singleton.h"
#include <string>
#include <cstdint>

namespace ppic {

namespace db {

using std::string;

class SessionPool;
typedef Singleton<SessionPool> SessionPoolSingleton;

class SessionPoolOption {
public:
  SessionPoolOption();
  SessionPoolOption(const string& user, const string& password, const string& host, uint16_t capacity, uint16_t port=33060);
  SessionPoolOption& UrlFromEnv(const char* url_env="MYSQL_CONNECTION_URL");
  SessionPoolOption& set_capacity(uint16_t capacity) { capacity_ = capacity; }
  const string& url() { return url_; }
private:
  friend class SessionPool;

  string url_;
  string user_;
  string password_;
  string host_;
  uint16_t port_;
  uint16_t capacity_;
};

class SessionPool {
public:
private:
  friend class Singleton<SessionPool>;
};

} // namespace db

} // namespace ppic

#endif //PPIC_DB_SESSION_POOL_H_