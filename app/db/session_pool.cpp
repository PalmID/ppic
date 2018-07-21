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
#include <cstdlib>
#include <cassert>

namespace ppic {

namespace db {

SessionPoolOption::SessionPoolOption()
  : url_(""), user_(""), password_(""), host_(""), port_(33060), capacity_(16) {
}

SessionPoolOption::SessionPoolOption(const string& user, const string& password, const string& host, uint16_t capacity, uint16_t port)
  : user_(user), password_(password), host_(host), capacity_(capacity), port_(port) {
    char url[512] = {};
    sprintf(url, "%s:%s@%s:%u", user_.c_str(), password_.c_str(), host_.c_str(), port_);
    url_ = url;
}

SessionPoolOption& SessionPoolOption::UrlFromEnv(const char* url_env) {
  assert (url_env != NULL);
  url_ = std::getenv(url_env);
}

} // namespace db

} // namespace ppic