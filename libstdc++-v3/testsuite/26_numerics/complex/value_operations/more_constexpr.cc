// { dg-options "-std=gnu++2a" }
// { dg-do compile { target c++2a } }

// Copyright (C) 2018-2023 Free Software Foundation, Inc.
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

#include <complex>
#include <testsuite_common_types.h>

namespace __gnu_test
{
  struct constexpr_functions
  {
    template<typename _Ttesttype>
      void
      operator()()
      {
	struct _Concept
	{
	  void __constraint()
	  { 
	    typedef typename _Ttesttype::_ComplexT _ComplexT;
	    constexpr _ComplexT cc = { 1.1 };
	    constexpr _Ttesttype a(cc);
	    constexpr auto v1 [[maybe_unused]] = norm(a);
	    constexpr auto v2 [[maybe_unused]] = conj(a);
	  }
	};

	_Concept c;
	c.__constraint();
      }
  };
}

int main()
{
  __gnu_test::constexpr_functions test;

  test.operator()<std::complex<float>>();
  test.operator()<std::complex<double>>();
  test.operator()<std::complex<long double>>();

  return 0;
}
