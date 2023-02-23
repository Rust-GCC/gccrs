// { dg-do compile { target { c++11 && { ! c++23 } } } }

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

#include <exception>

namespace std {
  class exception;
  class bad_exception;

  typedef void (*unexpected_handler)();
  unexpected_handler set_unexpected(unexpected_handler  f ) throw(); // { dg-warning "deprecated" { target c++11 } }
  unexpected_handler get_unexpected() noexcept;
  void unexpected();

  typedef void (*terminate_handler)();
  terminate_handler set_terminate(terminate_handler  f ) throw();
  terminate_handler get_terminate() noexcept;
  void terminate() throw();

  bool uncaught_exception() throw();
}
