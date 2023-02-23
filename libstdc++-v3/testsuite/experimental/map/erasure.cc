// { dg-do run { target c++14 } }

// Copyright (C) 2015-2023 Free Software Foundation, Inc.
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

#include <experimental/map>
#include <string>
#include <testsuite_hooks.h>

auto is_odd_pair = [](const std::pair<const int, std::string>& p)
{
  return p.first % 2 != 0;
};

void
test01()
{
  std::map<int, std::string> m{ { 10, "A" }, { 11, "B" },
				{ 12, "C" }, { 14, "D" },
				{ 15, "E" }, { 17, "F" },
				{ 18, "G" }, { 19, "H" } };
  std::experimental::erase_if(m, is_odd_pair);
  std::map<int, std::string> t{ { 10, "A" }, { 12, "C" },
				{ 14, "D" }, { 18, "G" } };
  VERIFY( m == t );
}

void
test02()
{
  std::multimap<int, std::string> mm{ { 20, "S" }, { 21, "T" },
				      { 22, "U" }, { 22, "V" },
				      { 23, "W" }, { 23, "X" },
				      { 24, "Y" }, { 25, "Z" } };
  std::experimental::erase_if(mm, is_odd_pair);
  std::multimap<int, std::string> t{ { 20, "S" }, { 22, "U" },
				     { 22, "V" }, { 24, "Y" } };
  VERIFY( mm == t );
}

void
test_pr107850()
{
  // Predicate only callable as non-const and only accepts non-const argument.
  struct Pred { bool operator()(std::pair<const int, int>&) { return false; } };
  const Pred pred; // erase_if parameter is passed by value, so non-const.
  std::map<int, int> m;
  std::experimental::erase_if(m, pred);
  std::multimap<int, int> mm;
  std::experimental::erase_if(mm, pred);
}

int
main()
{
  test01();
  test02();
  test_pr107850();

  return 0;
}
