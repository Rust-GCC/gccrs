// { dg-do compile }
// { dg-options "-std=c++98" }

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

// libstdc++/56111
// In C++03, we try to keep the macro "complex" as long as it does not
// conflict (std::complex).
#include <bits/c++config.h>
#if _GLIBCXX_HAVE_COMPLEX_H
# include <complex.h>
#endif

int main()
{
#if _GLIBCXX_HAVE_COMPLEX_H
  double complex x = .5;
  double complex y = cacos (x);
  (void)y;
#endif
}
