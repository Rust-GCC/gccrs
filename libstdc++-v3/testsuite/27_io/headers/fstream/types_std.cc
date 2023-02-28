// { dg-do compile }
// { dg-require-c-std "" }

// Copyright (C) 2007-2023 Free Software Foundation, Inc.
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

#include <fstream>

namespace gnu
{
  typedef std::filebuf t1;
  typedef std::wfilebuf t2;

  typedef std::ifstream t3;
  typedef std::wifstream t4;

  typedef std::ofstream t5;
  typedef std::wofstream t6;

  typedef std::fstream t7;
  typedef std::wfstream t8;
}
