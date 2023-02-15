// 2001-04-02  Benjamin Kosnik  <bkoz@redhat.com>

// Copyright (C) 2001-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// 21.4: null-terminiated sequence utilities

#include <string>
#include <cstring>

void test01()
{
  char c = 'a';
  const char cc = 'b';
  char* c1 = &c;
  const char* cc1 = &cc;
  const char* ccarray1 = "san francisco roof garden inspectors";
  const char* ccarray2 = "san francisco sunny-day park inspectors";
  char carray[50];
  std::strcpy(carray, ccarray1);
  void* v = carray;
  const void* cv = ccarray1;
  
  // const char* strchr(const char* s, int c);
  // char* strchr(char* s, int c);
  cc1 = std::strchr(ccarray1, 'c');
  c1 = std::strchr(carray, 'c');

  // const char* strpbrk(const char* s1, const char* s2);
  // char* strpbrk(char* s1, const char* s2);
  cc1 = std::strpbrk(ccarray1, ccarray2);
  c1 = std::strpbrk(carray, ccarray2);

  // const char* strrchr(const char* s, int c);
  // char* strrchr(char* s, int c);
  cc1 = std::strrchr(ccarray1, 'c');
  c1 = std::strrchr(carray, 'c');

  // const char* strstr(const char* s1, const char* s2);
  // char* strstr(char* s1, const char* s2);
  cc1 = std::strstr(ccarray1, ccarray2);
  c1 = std::strstr(carray, carray);

  // const void* memchr(const void* s, int c, size_t n);
  // void* memchr(      void* s, int c, size_t n);
  cv = std::memchr(cv, 'a', 3);
  v = std::memchr(v, 'a', 3);

  cc1 = cc1; // Suppress unused warnings.
  c1 = c1;
}

int main()
{
  test01();
  return 0;
}
