// { dg-do compile }

// 2006-02-04  Edward Smith-Rowland <3dw4rd@verizon.net>
//
// Copyright (C) 2006-2023 Free Software Foundation, Inc.
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

// 5.2.1.2 assoc_legendre

#include <tr1/cmath>

void
test01()
{

  float xf = 0.5F;
  double xd = 0.5;
  long double xl = 0.5L;
  unsigned int l = 2, m = 1;

  std::tr1::assoc_legendre(l, m, xf);
  std::tr1::assoc_legendref(l, m, xf);
  std::tr1::assoc_legendre(l, m, xd);
  std::tr1::assoc_legendre(l, m, xl);
  std::tr1::assoc_legendrel(l, m, xl);

  return;
}

