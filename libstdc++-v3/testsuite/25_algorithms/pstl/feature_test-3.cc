// Copyright (C) 2020-2023 Free Software Foundation, Inc.
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

// { dg-do preprocess { target c++17 } }
// { dg-require-effective-target tbb_backend }

#include <execution>

#ifndef __cpp_lib_execution
# error "Feature-test macro for execution policies is missing in <execution>"
#elif __cpp_lib_execution != 201902L
# error "Feature-test macro for parallel algorithms has wrong value in <execution>"
#endif

// Neither SD-6 nor C++20 requires this macro to be defined in <execution>.
#ifndef __cpp_lib_parallel_algorithm
# error "Feature-test macro for parallel algorithms missing in <execution>"
#elif __cpp_lib_parallel_algorithm != 201603L
# error "Feature-test macro for parallel algorithms has wrong value in <execution>"
#endif
