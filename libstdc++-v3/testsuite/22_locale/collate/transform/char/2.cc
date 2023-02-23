// { dg-require-namedlocale "en_US.ISO8859-1" }
// { dg-require-namedlocale "fr_FR.ISO8859-15" }
// { dg-require-namedlocale "de_DE.ISO8859-15" }

// 2001-08-15 Benjamin Kosnik  <bkoz@redhat.com>

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

// 22.2.4.1.1 collate members

#include <locale>
#include <testsuite_hooks.h>

void test02()
{
  using namespace std;
  typedef std::collate<char>::string_type string_type;

  // basic construction
  locale loc_c = locale::classic();
  locale loc_us = locale(ISO_8859(1,en_US));
  locale loc_fr = locale(ISO_8859(15,fr_FR));
  locale loc_de = locale(ISO_8859(15,de_DE));
  VERIFY( loc_c != loc_de );
  VERIFY( loc_us != loc_fr );
  VERIFY( loc_us != loc_de );
  VERIFY( loc_de != loc_fr );

  // cache the collate facets
  const collate<char>& coll_de = use_facet<collate<char> >(loc_de); 

  // long hash(const charT*, const charT*) cosnt
  const char* strlit3 = "�uglein Augment"; // "C" == "Augment �uglein"
  const char* strlit4 = "Base ba� Ba� Bast"; // "C" == "Base ba� Ba� Bast"

  int i1;
  int i2;
  int size3 = char_traits<char>::length(strlit3) - 1;
  int size4 = char_traits<char>::length(strlit4) - 1;

  string_type str3 = coll_de.transform(strlit3, strlit3 + size3);
  string_type str4 = coll_de.transform(strlit4, strlit4 + size4);
  i1 = str3.compare(str4);
  i2 = coll_de.compare(strlit3, strlit3 + size3, strlit4, strlit4 + size4);
  VERIFY ( i2 == -1 );
  VERIFY ( i1 * i2 > 0 );
}

int main()
{
  test02();
  return 0;
}
