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

#ifndef PPIC_APP_COMMON_SINGLETON_H_
#define PPIC_APP_COMMON_SINGLETON_H_

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

private:
  Singleton() {}
  ~Singleton() {}
};

} // namespace ppic

#endif //PPIC_APP_COMMON_SINGLETON_H_
