// { dg-do run { target c++11 } }
// { dg-require-cstdint "" }

// 2009-09-14  Benjamin Kosnik  <benjamin@redhat.com>

// Copyright (C) 2009-2023 Free Software Foundation, Inc.
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

#include <ext/vstring.h>
#include <exception/safety.h>

void
value()
{
  // NB: Instantiating with __gnu_cxx::throw_value_limit would be illegal,
  // isn't a POD type.
  typedef char value_type;
  typedef __gnu_cxx::throw_allocator_limit<value_type> allocator_type;
  typedef std::char_traits<value_type> traits_type;

  typedef __gnu_cxx::__versa_string<value_type, traits_type,
    allocator_type, __gnu_cxx::__rc_string_base> test_type_rc;

  typedef __gnu_cxx::__versa_string<value_type, traits_type,
    allocator_type, __gnu_cxx::__sso_string_base> test_type_sso;

  __gnu_test::propagation_consistent<test_type_rc> test_rc;
  __gnu_test::propagation_consistent<test_type_sso> test_sso;
}

// Container requirement testing, exceptional behavior
int main()
{
  value();
  return 0;
}

// The __versa_string destructor triggers a bogus -Wfree-nonheap-object
// due to pr54202.
// { dg-prune-output "\\\[-Wfree-nonheap-object" }
