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

#ifndef PPIC_DB_SMART_SESSION_H_
#define PPIC_DB_SMART_SESSION_H_

#include "mysqlx/xdevapi.h"
#include <string>
#include <memory>

namespace ppic {

namespace db {

class SessionPoolOption;

class SmartSession : public mysqlx::Session {
 public:
  SmartSession(const SessionPoolOption&);

  // Same functionality as SQL `USE db_name;`
  // Default schema is `db_name` when it is specified in `SessionPoolOption`
  SmartSession& SelectSchema(const char* schema_name);
  // Return current schema, if there are no schema it will throw an error.
  mysqlx::Schema& GetCurrentSchema() const;
 private:
  std::shared_ptr<mysqlx::Schema> current_schema_;
};

}   // namespace db

}   // namespace ppic

#endif // PPIC_DB_SMART_SESSION_H_