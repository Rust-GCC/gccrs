// { dg-do run { target c++17 } }

// Copyright (C) 2013-2023 Free Software Foundation, Inc.
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

// C++17 24.4.3 Non-member comparison functions [string.view.comparison]

#include <string_view>
#include <testsuite_hooks.h>

void
test01()
{
  std::wstring_view 	str_0(L"costa rica");
  std::wstring_view 	str_1(L"costa marbella");
  std::wstring_view 	str_2(L"cost");
  std::wstring_view	str_3(L"costa ricans");
  std::wstring_view  str_4;

  str_4 = str_0;
  //comparisons between string_view objects
  VERIFY( !(str_0 == str_1) );
  VERIFY( !(str_0 == str_2) );
  VERIFY( !(str_0 == str_3) );
  VERIFY( !(str_1 == str_0) );
  VERIFY( !(str_2 == str_0) );
  VERIFY( !(str_3 == str_0) );
  VERIFY( str_4 == str_0 );
  VERIFY( str_0 == str_4 );

  VERIFY( str_0 != str_1 );
  VERIFY( str_0 != str_2 );
  VERIFY( str_0 != str_3 );
  VERIFY( str_1 != str_0 );
  VERIFY( str_2 != str_0 );
  VERIFY( str_3 != str_0 );
  VERIFY( !(str_0 != str_4) );
  VERIFY( !(str_4 != str_0) );

  VERIFY( str_0 > str_1 ); //true cuz r>m
  VERIFY( str_0 > str_2 );
  VERIFY( !(str_0 > str_3) );
  VERIFY( !(str_1 > str_0) ); //false cuz m<r
  VERIFY( !(str_2 > str_0) );
  VERIFY( str_3 > str_0 );
  VERIFY( !(str_0 > str_4) );
  VERIFY( !(str_4 > str_0) );

  VERIFY( !(str_0 < str_1) ); //false cuz r>m
  VERIFY( !(str_0 < str_2) );
  VERIFY( str_0 < str_3 );
  VERIFY( str_1 < str_0 ); //true cuz m<r
  VERIFY( str_2 < str_0 );
  VERIFY( !(str_3 < str_0) );
  VERIFY( !(str_0 < str_4) );
  VERIFY( !(str_4 < str_0) );

  VERIFY( str_0 >= str_1 ); //true cuz r>m
  VERIFY( str_0 >= str_2 );
  VERIFY( !(str_0 >= str_3) );
  VERIFY( !(str_1 >= str_0) );//false cuz m<r
  VERIFY( !(str_2 >= str_0) );
  VERIFY( str_3 >= str_0 );
  VERIFY( str_0 >= str_4 );
  VERIFY( str_4 >= str_0 );

  VERIFY( !(str_0 <= str_1) );//false cuz r>m
  VERIFY( !(str_0 <= str_2) );
  VERIFY( str_0 <= str_3 );
  VERIFY( str_1 <= str_0 );//true cuz m<r
  VERIFY( str_2 <= str_0 );
  VERIFY( !(str_3 <= str_0) );
  VERIFY( str_0 <= str_4 );
  VERIFY( str_4 <= str_0 );

  //comparisons between string_view object and string_view literal
  VERIFY( !(str_0 == L"costa marbella") );
  VERIFY( !(str_0 == L"cost") );
  VERIFY( !(str_0 == L"costa ricans") );
  VERIFY( !(L"costa marbella" == str_0) );
  VERIFY( !(L"cost" == str_0) );
  VERIFY( !(L"costa ricans" == str_0) );
  VERIFY( L"costa rica" == str_0 );
  VERIFY( str_0 == L"costa rica" );

  VERIFY( str_0 != L"costa marbella" );
  VERIFY( str_0 != L"cost" );
  VERIFY( str_0 != L"costa ricans" );
  VERIFY( L"costa marbella" != str_0 );
  VERIFY( L"cost" != str_0 );
  VERIFY( L"costa ricans" != str_0 );
  VERIFY( !(L"costa rica" != str_0) );
  VERIFY( !(str_0 != L"costa rica") );

  VERIFY( str_0 > L"costa marbella" ); //true cuz r>m
  VERIFY( str_0 > L"cost" );
  VERIFY( !(str_0 > L"costa ricans") );
  VERIFY( !(L"costa marbella" > str_0) );//false cuz m<r
  VERIFY( !(L"cost" > str_0) );
  VERIFY( L"costa ricans" > str_0 );
  VERIFY( !(L"costa rica" > str_0) );
  VERIFY( !(str_0 > L"costa rica") );

  VERIFY( !(str_0 < L"costa marbella") );//false cuz r>m
  VERIFY( !(str_0 < L"cost") );
  VERIFY( str_0 < L"costa ricans" );
  VERIFY( L"costa marbella" < str_0 );//true cuz m<r
  VERIFY( L"cost" < str_0 );
  VERIFY( !(L"costa ricans" < str_0) );
  VERIFY( !(L"costa rica" < str_0) );
  VERIFY( !(str_0 < L"costa rica") );

  VERIFY( str_0 >= L"costa marbella" );//true cuz r>m
  VERIFY( str_0 >= L"cost" );
  VERIFY( !(str_0 >= L"costa ricans") );
  VERIFY( !(L"costa marbella" >= str_0) );//false cuz m<r
  VERIFY( !(L"cost" >= str_0) );
  VERIFY( L"costa ricans" >= str_0 );
  VERIFY( L"costa rica" >= str_0 );
  VERIFY( str_0 >= L"costa rica" );

  VERIFY( !(str_0 <= L"costa marbella") );//false cuz r>m
  VERIFY( !(str_0 <= L"cost") );
  VERIFY( str_0 <= L"costa ricans" );
  VERIFY( L"costa marbella" <= str_0 );//true cuz m<r
  VERIFY( L"cost" <= str_0 );
  VERIFY( !(L"costa ricans" <= str_0) );
  VERIFY( L"costa rica" <= str_0 );
  VERIFY( str_0 <= L"costa rica" );
}

constexpr bool
test02()
{
  std::wstring_view 	str_0(L"costa rica");
  std::wstring_view 	str_1(L"costa marbella");
  std::wstring_view 	str_2(L"cost");
  std::wstring_view	str_3(L"costa ricans");
  std::wstring_view  str_4;

#undef VERIFY
#define VERIFY(x) if (!(x)) return false

  str_4 = str_0;
  //comparisons between string_view objects
  VERIFY( !(str_0 == str_1) );
  VERIFY( !(str_0 == str_2) );
  VERIFY( !(str_0 == str_3) );
  VERIFY( !(str_1 == str_0) );
  VERIFY( !(str_2 == str_0) );
  VERIFY( !(str_3 == str_0) );
  VERIFY( str_4 == str_0 );
  VERIFY( str_0 == str_4 );

  VERIFY( str_0 != str_1 );
  VERIFY( str_0 != str_2 );
  VERIFY( str_0 != str_3 );
  VERIFY( str_1 != str_0 );
  VERIFY( str_2 != str_0 );
  VERIFY( str_3 != str_0 );
  VERIFY( !(str_0 != str_4) );
  VERIFY( !(str_4 != str_0) );

  VERIFY( str_0 > str_1 ); //true cuz r>m
  VERIFY( str_0 > str_2 );
  VERIFY( !(str_0 > str_3) );
  VERIFY( !(str_1 > str_0) ); //false cuz m<r
  VERIFY( !(str_2 > str_0) );
  VERIFY( str_3 > str_0 );
  VERIFY( !(str_0 > str_4) );
  VERIFY( !(str_4 > str_0) );

  VERIFY( !(str_0 < str_1) ); //false cuz r>m
  VERIFY( !(str_0 < str_2) );
  VERIFY( str_0 < str_3 );
  VERIFY( str_1 < str_0 ); //true cuz m<r
  VERIFY( str_2 < str_0 );
  VERIFY( !(str_3 < str_0) );
  VERIFY( !(str_0 < str_4) );
  VERIFY( !(str_4 < str_0) );

  VERIFY( str_0 >= str_1 ); //true cuz r>m
  VERIFY( str_0 >= str_2 );
  VERIFY( !(str_0 >= str_3) );
  VERIFY( !(str_1 >= str_0) );//false cuz m<r
  VERIFY( !(str_2 >= str_0) );
  VERIFY( str_3 >= str_0 );
  VERIFY( str_0 >= str_4 );
  VERIFY( str_4 >= str_0 );

  VERIFY( !(str_0 <= str_1) );//false cuz r>m
  VERIFY( !(str_0 <= str_2) );
  VERIFY( str_0 <= str_3 );
  VERIFY( str_1 <= str_0 );//true cuz m<r
  VERIFY( str_2 <= str_0 );
  VERIFY( !(str_3 <= str_0) );
  VERIFY( str_0 <= str_4 );
  VERIFY( str_4 <= str_0 );

  //comparisons between string_view object and string_view literal
  VERIFY( !(str_0 == L"costa marbella") );
  VERIFY( !(str_0 == L"cost") );
  VERIFY( !(str_0 == L"costa ricans") );
  VERIFY( !(L"costa marbella" == str_0) );
  VERIFY( !(L"cost" == str_0) );
  VERIFY( !(L"costa ricans" == str_0) );
  VERIFY( L"costa rica" == str_0 );
  VERIFY( str_0 == L"costa rica" );

  VERIFY( str_0 != L"costa marbella" );
  VERIFY( str_0 != L"cost" );
  VERIFY( str_0 != L"costa ricans" );
  VERIFY( L"costa marbella" != str_0 );
  VERIFY( L"cost" != str_0 );
  VERIFY( L"costa ricans" != str_0 );
  VERIFY( !(L"costa rica" != str_0) );
  VERIFY( !(str_0 != L"costa rica") );

  VERIFY( str_0 > L"costa marbella" ); //true cuz r>m
  VERIFY( str_0 > L"cost" );
  VERIFY( !(str_0 > L"costa ricans") );
  VERIFY( !(L"costa marbella" > str_0) );//false cuz m<r
  VERIFY( !(L"cost" > str_0) );
  VERIFY( L"costa ricans" > str_0 );
  VERIFY( !(L"costa rica" > str_0) );
  VERIFY( !(str_0 > L"costa rica") );

  VERIFY( !(str_0 < L"costa marbella") );//false cuz r>m
  VERIFY( !(str_0 < L"cost") );
  VERIFY( str_0 < L"costa ricans" );
  VERIFY( L"costa marbella" < str_0 );//true cuz m<r
  VERIFY( L"cost" < str_0 );
  VERIFY( !(L"costa ricans" < str_0) );
  VERIFY( !(L"costa rica" < str_0) );
  VERIFY( !(str_0 < L"costa rica") );

  VERIFY( str_0 >= L"costa marbella" );//true cuz r>m
  VERIFY( str_0 >= L"cost" );
  VERIFY( !(str_0 >= L"costa ricans") );
  VERIFY( !(L"costa marbella" >= str_0) );//false cuz m<r
  VERIFY( !(L"cost" >= str_0) );
  VERIFY( L"costa ricans" >= str_0 );
  VERIFY( L"costa rica" >= str_0 );
  VERIFY( str_0 >= L"costa rica" );

  VERIFY( !(str_0 <= L"costa marbella") );//false cuz r>m
  VERIFY( !(str_0 <= L"cost") );
  VERIFY( str_0 <= L"costa ricans" );
  VERIFY( L"costa marbella" <= str_0 );//true cuz m<r
  VERIFY( L"cost" <= str_0 );
  VERIFY( !(L"costa ricans" <= str_0) );
  VERIFY( L"costa rica" <= str_0 );
  VERIFY( str_0 <= L"costa rica" );

  return true;
}

int
main()
{
  test01();
  static_assert( test02() );
}
