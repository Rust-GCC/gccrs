// 2003-05-27 Brendan Kehoe  <brendan@zen.org>

// Copyright (C) 2003-2023 Free Software Foundation, Inc.
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

// $22.2.6.3/3
// The number of digits required after the decimal point (if any) is exactly
// the value returned by frac_digits().

#include <locale>
#include <sstream>

class dublin : public std::moneypunct<char> {
public:
  int do_frac_digits() const { return 3; }
};

int main()
{
  std::istringstream liffey;
  std::string coins;

  std::locale eire(std::locale::classic(), new dublin);
  liffey.imbue(eire);

  const std::money_get<char>& greed
    = std::use_facet<std::money_get<char> >(liffey.getloc());

  typedef std::istreambuf_iterator<char> iterator_type;
  iterator_type end;

  std::ios_base::iostate err01 = std::ios_base::goodbit;

  int fails = 0;

  // Feed it 1 digit too many, which should fail.
  liffey.str("12.3456");
  greed.get(liffey, end, false, liffey, err01, coins);
  if (! (err01 & std::ios_base::failbit ))
    fails |= 0x01;

  err01 = std::ios_base::goodbit;

  // Feed it exactly what it wants, which should succeed.
  liffey.str("12.345");
  greed.get(liffey, end, false, liffey, err01, coins);
  if ( err01 & std::ios_base::failbit )
    fails |= 0x02;

  err01 = std::ios_base::goodbit;

  // Feed it 1 digit too few, which should fail.
  liffey.str("12.34");
  greed.get(liffey, end, false, liffey, err01, coins);
  if (! ( err01 & std::ios_base::failbit ))
    fails |= 0x04;

  err01 = std::ios_base::goodbit;

  // Feed it only a decimal-point, which should fail.
  liffey.str("12.");
  greed.get(liffey, end, false, liffey, err01, coins);
  if (! (err01 & std::ios_base::failbit ))
    fails |= 0x08;

  err01 = std::ios_base::goodbit;

  // Feed it no decimal-point at all, which should succeed.
  liffey.str("12");
  greed.get(liffey, end, false, liffey, err01, coins);
  if ( err01 & std::ios_base::failbit )
    fails |= 0x10;

  return fails;
}
