// { dg-do run { target c++11 } }
// { dg-options "-g -O0" }

//
// Copyright (C) 2020-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <list>
#include <memory>
#include <testsuite_hooks.h>

struct PredLWG526
{
  PredLWG526(int i) : i_(i) {};
  ~PredLWG526() { i_ = -32767; }

  bool
  operator() (const PredLWG526& p) const { return p.i_ == i_; }

  bool
  operator==(int i) const { return i == i_; }

  bool
  operator()(const PredLWG526& lhs, const PredLWG526& rhs) const
  {
    VERIFY( i_ != -32767 );
    return lhs.i_ == rhs.i_;
  }

  friend bool
  operator==(const PredLWG526& lhs, const PredLWG526& rhs)
  { return lhs.i_ == rhs.i_; }

  int i_;
};

void test01()
{
  int a1[] = {1, 2, 1, 3, 5, 8, 11};
  int a2[] = {2, 3, 5, 8, 11};
  std::list<PredLWG526> l(a1, a1 + 7);

  VERIFY( std::distance(l.begin(), l.end()) == 7 );

  l.remove(l.front());
  VERIFY( std::distance(l.begin(), l.end()) == 5 );
  for (size_t i = 0; !l.empty(); ++i)
    {
      VERIFY( l.front() == a2[i] );
      l.pop_front();
    }
}

void test02()
{
  int a1[] = {1, 2, 1, 3, 5, 8, 11};
  int a2[] = {2, 3, 5, 8, 11};
  std::list<PredLWG526> l(a1, a1 + 7);

  VERIFY( std::distance(l.begin(), l.end()) == 7 );

  l.remove_if(std::cref(l.front()));
  VERIFY( std::distance(l.begin(), l.end()) == 5 );
  for (size_t i = 0; !l.empty(); ++i)
    {
      VERIFY( l.front() == a2[i] );
      l.pop_front();
    }
}

void test03()
{
  int a1[] = {1, 1, 1, 2, 3, 5, 8, 11};
  int a2[] = {1, 2, 3, 5, 8, 11};
  std::list<PredLWG526> l(a1, a1 + 8);

  VERIFY( std::distance(l.begin(), l.end()) == 8 );

  auto it = l.begin();
  ++it;
  l.unique(std::cref(*it));
  VERIFY( std::distance(l.begin(), l.end()) == 6 );
  for (size_t i = 0; !l.empty(); ++i)
    {
      VERIFY( l.front() == a2[i] );
      l.pop_front();
    }
}

int main()
{
  test01();
  test02();
  test03();
  return 0;
}
