// { dg-do compile }
// 2001-04-30  Benjamin Kosnik  <bkoz@redhat.com>

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

// 24.5.4 template class ostreambuf_iterator

#include <sstream>
#include <iterator>
#include <testsuite_hooks.h>

void test01()
{
  using namespace std;

  // Check for required typedefs
  typedef ostreambuf_iterator<char> test_iterator;
  typedef test_iterator::value_type value_type;
  typedef test_iterator::difference_type difference_type;
  typedef test_iterator::pointer pointer;
  typedef test_iterator::reference reference;
  typedef test_iterator::iterator_category iteratory_category;

  typedef test_iterator::char_type char_type;
  typedef test_iterator::traits_type traits_type;
  typedef test_iterator::ostream_type ostream_type;
  typedef test_iterator::streambuf_type streambuf_type;
}

#if __cplusplus >= 201103L
void test02()
{
  using namespace std;

  using test_type = ostreambuf_iterator<char>;
  static_assert(is_same<test_type::value_type, void>::value, "");
#if __cplusplus <= 201703L
  static_assert(is_same<test_type::difference_type, void>::value, "");
#else
  static_assert(is_same<test_type::difference_type, ptrdiff_t>::value, "");
#endif
  static_assert(is_same<test_type::pointer, void>::value, "");
  static_assert(is_same<test_type::reference, void>::value, "");
  static_assert(is_same<test_type::iterator_category, output_iterator_tag>::value, "");

  static_assert(is_same<test_type::char_type, char>::value, "");
  static_assert(is_same<test_type::traits_type, char_traits<char>>::value, "");
  static_assert(is_same<test_type::ostream_type, ostream>::value, "");
  static_assert(is_same<test_type::streambuf_type, streambuf>::value, "");
}

#ifdef _GLIBCXX_USE_WCHAR_T
void test03()
{
  using namespace std;

  using test_type = ostreambuf_iterator<wchar_t>;
  static_assert(is_same<test_type::value_type, void>::value, "");
#if __cplusplus <= 201703L
  static_assert(is_same<test_type::difference_type, void>::value, "");
#else
  static_assert(is_same<test_type::difference_type, ptrdiff_t>::value, "");
#endif
  static_assert(is_same<test_type::pointer, void>::value, "");
  static_assert(is_same<test_type::reference, void>::value, "");
  static_assert(is_same<test_type::iterator_category, output_iterator_tag>::value, "");

  static_assert(is_same<test_type::char_type, wchar_t>::value, "");
  static_assert(is_same<test_type::traits_type, char_traits<wchar_t>>::value, "");
  static_assert(is_same<test_type::ostream_type, wostream>::value, "");
  static_assert(is_same<test_type::streambuf_type, wstreambuf>::value, "");
}
#endif
#endif
