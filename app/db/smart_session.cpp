/****************************************************************************\
 * Created on Wed Jul 25 2018
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

#include "db/smart_session.h"
#include "db/session_pool.h"

namespace ppic {

namespace db {

using mysqlx::Schema;

SmartSession::SmartSession(const SessionPoolOption& option)
  : Session(option.url()) {
  current_schema_ = std::make_shared<Schema>(createSchema(option.db(), true));
  UseDatabase(option.db());
}

SmartSession& SmartSession::SelectSchema(const char* schema_name) {
  current_schema_ = std::make_shared<Schema>(createSchema(schema_name, true));
  UseDatabase(schema_name);
  return *this;
}

Schema& SmartSession::GetCurrentSchema() const {
  return *current_schema_.get();
}

void SmartSession::UseDatabase(const char* db_name) {
  char sql_str[128] = {0};
  snprintf(sql_str, 128, "USE %s;", db_name);
  sql(sql_str).execute();
}

}   // namespace db

}   // namespace ppic