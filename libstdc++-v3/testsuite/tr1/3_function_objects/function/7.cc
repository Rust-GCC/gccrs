// 2005-01-15 Douglas Gregor <dgregor@cs.indiana.edu>
//
// Copyright (C) 2005-2023 Free Software Foundation, Inc.
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

// 3.7.2 polymorphic function object wrapper
#include <tr1/functional>
#include <testsuite_hooks.h>
#include <testsuite_tr1.h>

using namespace __gnu_test;

// Put reference_wrappers to function pointers into function<> wrappers
void test07()
{
  using std::tr1::function;
  using std::tr1::ref;
  using std::tr1::cref;

  int (*fptr)(float) = truncate_float;

  function<int(float)> f1(ref(fptr));
  VERIFY( f1 );
  VERIFY( !!f1 );
  VERIFY( !(f1 == 0) );
  VERIFY( !(0 == f1) );
  VERIFY( f1 != 0 );
  VERIFY( 0 != f1 );

  // Invocation
  VERIFY( f1(3.1f) == 3 );

#if __cpp_rtti
  // target_type and target() functions
  const function<int(float)>& f1c = f1;
  VERIFY( typeid(int(*)(float)) == f1.target_type() );
  VERIFY( f1.target<int(*)(float)>() != 0 );
  VERIFY( f1.target<int(*)(float)>() == &fptr );
  VERIFY( f1c.target<int(*)(float)>() != 0 );
  VERIFY( f1c.target<int(*)(float)>() == &fptr );
#endif

  function<int(float)> f2(cref(fptr));
  VERIFY( f2 );
  VERIFY( !!f2 );
  VERIFY( !(f2 == 0) );
  VERIFY( !(0 == f2) );
  VERIFY( f2 != 0 );
  VERIFY( 0 != f2 );

  // Invocation
  VERIFY( f2(3.1f) == 3 );

#if __cpp_rtti
  // target_type and target() functions
  const function<int(float)>& f2c = f2;
  VERIFY( typeid(int(*)(float)) == f2.target_type() );
  VERIFY( f2.target<int(*)(float)>() == 0 );
  VERIFY( f2.target<int(* const)(float)>() == &fptr );
  VERIFY( f2c.target<int(*)(float)>() != 0 );
  VERIFY( f2c.target<int(*)(float)>() == &fptr );
#endif
}

int main()
{
  test07();
  return 0;
}
