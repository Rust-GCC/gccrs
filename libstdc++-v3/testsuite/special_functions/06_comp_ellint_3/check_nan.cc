// { dg-do run { target c++11 } }
// { dg-require-c-std "" }
// { dg-options "-D__STDCPP_WANT_MATH_SPEC_FUNCS__" }
// { dg-add-options ieee }

// Copyright (C) 2016-2023 Free Software Foundation, Inc.
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

// 8.1.6 comp_ellint_3

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  float kf = std::numeric_limits<float>::quiet_NaN();
  double kd = std::numeric_limits<double>::quiet_NaN();
  long double kl = std::numeric_limits<long double>::quiet_NaN();

  float nuf = 0.0F;
  double nud = 0.0;
  long double nul = 0.0L;

  float a = std::comp_ellint_3(kf, nuf);
  float b = std::comp_ellint_3f(kf, nuf);
  double c = std::comp_ellint_3(kd, nud);
  long double d = std::comp_ellint_3(kl, nul);
  long double e = std::comp_ellint_3l(kl, nul);

  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

void
test02()
{
  float kf = 1.0F;
  double kd = 1.0;
  long double kl = 1.0L;

  float nuf = std::numeric_limits<float>::quiet_NaN();
  double nud = std::numeric_limits<double>::quiet_NaN();
  long double nul = std::numeric_limits<long double>::quiet_NaN();

  float a = std::comp_ellint_3(kf, nuf);
  float b = std::comp_ellint_3f(kf, nuf);
  double c = std::comp_ellint_3(kd, nud);
  long double d = std::comp_ellint_3(kl, nul);
  long double e = std::comp_ellint_3l(kl, nul);

  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

int
main()
{
  test01();
  test02();
  return 0;
}

