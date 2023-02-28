// 2002-01-08 bkoz

// Copyright (C) 2002-2023 Free Software Foundation, Inc.
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


// 27.6.1.5 - Template class basic_iostream
// NB: This file is for testing iostream with NO OTHER INCLUDES.

#include <iostream>

// libstdc++/3647
void test07()
{
  // Should not block.
  std::cout << std::cin.rdbuf()->in_avail() << std::endl;
}

int main() 
{
  test07();
  return 0;
}
