// { dg-require-iconv "UCS-2BE" }
// { dg-require-iconv "UCS-4BE" }

// 2000-08-23 Benjamin Kosnik <bkoz@cygnus.com>

// Copyright (C) 2000-2023 Free Software Foundation, Inc.
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

// 22.2.1.5 - Template class codecvt [lib.locale.codecvt]

#include <locale>
#include <testsuite_hooks.h>
#include <ext/codecvt_specializations.h>

// Partial specialization using encoding_state.
// codecvt<unicode_t, wchar_t, encoding_state>
void test01()
{
  using namespace std;
  typedef codecvt_base::result			result;
  typedef unsigned short       			int_type;
  typedef wchar_t				ext_type;
  typedef __gnu_cxx::encoding_state	       		state_type;
  typedef codecvt<int_type, ext_type, state_type>	unicode_codecvt;
  typedef char_traits<int_type>			int_traits;
  typedef char_traits<ext_type>			ext_traits;

  int 			size = 23;
  char  e_lit_base[96] __attribute__((aligned(__alignof__(ext_type)))) =
  {
    char(0x00), char(0x00), char(0x00), char(0x62), char(0x00), char(0x00),
    char(0x00), char(0x6c), char(0x00), char(0x00), char(0x00), char(0x61),
    char(0x00), char(0x00), char(0x00), char(0x63), char(0x00), char(0x00),
    char(0x00), char(0x6b), char(0x00), char(0x00), char(0x00), char(0x20),
    char(0x00), char(0x00), char(0x00), char(0x70), char(0x00), char(0x00),
    char(0x00), char(0x65), char(0x00), char(0x00), char(0x00), char(0x61),
    char(0x00), char(0x00), char(0x00), char(0x72), char(0x00), char(0x00),
    char(0x00), char(0x6c), char(0x00), char(0x00), char(0x00), char(0x20),
    char(0x00), char(0x00), char(0x00), char(0x6a), char(0x00), char(0x00),
    char(0x00), char(0x61), char(0x00), char(0x00), char(0x00), char(0x73),
    char(0x00), char(0x00), char(0x00), char(0x6d), char(0x00), char(0x00),
    char(0x00), char(0x69), char(0x00), char(0x00), char(0x00), char(0x6e),
    char(0x00), char(0x00), char(0x00), char(0x65), char(0x00), char(0x00),
    char(0x00), char(0x20), char(0x00), char(0x00), char(0x00), char(0x74),
    char(0x00), char(0x00), char(0x00), char(0x65), char(0x00), char(0x00),
    char(0x00), char(0x61), char(0x00), char(0x00), char(0x00), char(0xa0)     
  };
  const ext_type* 	e_lit = reinterpret_cast<ext_type*>(e_lit_base);

  char  i_lit_base[48] __attribute__((aligned(__alignof__(int_type)))) = 
  { 
    char(0x00), char(0x62), char(0x00), char(0x6c), char(0x00), char(0x61),
    char(0x00), char(0x63), char(0x00), char(0x6b), char(0x00), char(0x20),
    char(0x00), char(0x70), char(0x00), char(0x65), char(0x00), char(0x61),
    char(0x00), char(0x72), char(0x00), char(0x6c), char(0x00), char(0x20),
    char(0x00), char(0x6a), char(0x00), char(0x61), char(0x00), char(0x73),
    char(0x00), char(0x6d), char(0x00), char(0x69), char(0x00), char(0x6e),
    char(0x00), char(0x65), char(0x00), char(0x20), char(0x00), char(0x74),
    char(0x00), char(0x65), char(0x00), char(0x61), char(0x00), char(0xa0) 
  };
  const int_type* 	i_lit = reinterpret_cast<int_type*>(i_lit_base);

  const ext_type*       efrom_next;
  const int_type*       ifrom_next;
  ext_type* 		e_arr = new ext_type[size + 1];
  ext_type*		eto_next;
  int_type* 		i_arr = new int_type[size + 1];
  int_type*		ito_next;

  // construct a locale object with the specialized facet.
  locale 		loc(locale::classic(), new unicode_codecvt);
  // sanity check the constructed locale has the specialized facet.
  VERIFY( has_facet<unicode_codecvt>(loc) );
  const unicode_codecvt& cvt = use_facet<unicode_codecvt>(loc); 

  // in
  //  unicode_codecvt::state_type state01("UCS-2BE", "UCS-4BE", 0xfeff, 0);
  unicode_codecvt::state_type state01("UCS-2BE", "UCS-4BE", 0, 0);
  result r1 = cvt.in(state01, e_lit, e_lit + size, efrom_next, 
		     i_arr, i_arr + size + 1, ito_next);
  VERIFY( r1 == codecvt_base::ok );
  VERIFY( !int_traits::compare(i_arr, i_lit, size) ); 
  VERIFY( efrom_next == e_lit + size );
  VERIFY( ito_next == i_arr + size );

  // out
  unicode_codecvt::state_type state02("UCS-2BE", "UCS-4BE", 0, 0);
  result r2 = cvt.out(state02, i_lit, i_lit + size, ifrom_next, 
		       e_arr, e_arr + size, eto_next);   
  VERIFY( r2 == codecvt_base::ok ); // XXX?
  VERIFY( !ext_traits::compare(e_arr, e_lit, size) ); 
  VERIFY( ifrom_next == i_lit + size );
  VERIFY( eto_next == e_arr + size );

  // unshift
  ext_traits::copy(e_arr, e_lit, size);
  unicode_codecvt::state_type state03("UCS-2BE", "UCS-4BE", 0, 0);
  result r3 = cvt.unshift(state03, e_arr, e_arr + size, eto_next);
  VERIFY( r3 == codecvt_base::noconv );
  VERIFY( !ext_traits::compare(e_arr, e_lit, size) ); 
  VERIFY( eto_next == e_arr );

  int i = cvt.encoding();
  VERIFY( i == 0 );

  VERIFY( !cvt.always_noconv() );

  unicode_codecvt::state_type state04("UCS-2BE", "UCS-4BE", 0, 0);
  int j = cvt.length(state03, e_lit, e_lit + size, 5);
  VERIFY( j == 5 );

  int k = cvt.max_length();
  VERIFY( k == 1 );

  delete [] e_arr;
  delete [] i_arr;
}

int main ()
{
  test01();
  return 0;
}

