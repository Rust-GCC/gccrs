// -*- C++ -*-

// Copyright (C) 2005-2023 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.


// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice
// and this permission notice appear in supporting documentation. None
// of the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

/**
 * @file sample_mean.hpp
 * Contains a function for calculating a sample mean
 */

#ifndef PB_DS_SAMPLE_MEAN_HPP
#define PB_DS_SAMPLE_MEAN_HPP

#include <list>
#include <numeric>
#include <math.h>
#include <iterator>

namespace __gnu_pbds
{
  namespace test
  {
    namespace detail
    {
#define PB_DS_VTYPE typename std::iterator_traits<It>::value_type

      template<typename It>
      PB_DS_VTYPE
      sample_mean(It b, It e)
      {
	const PB_DS_VTYPE total = std::accumulate(b, e, PB_DS_VTYPE(0));
	const size_t num = std::distance(b, e);
	return total / num;
      }

#undef PB_DS_VTYPE

    } // namespace detail
  } // namespace test
} // namespace __gnu_pbds

#endif

