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
 * @file size_policy_string_form.hpp
 * Transforms containers into string form.
 */

#ifndef PB_DS_SIZE_POLICY_STRING_FORM_HPP
#define PB_DS_SIZE_POLICY_STRING_FORM_HPP

#include <string>
#include <common_type/assoc/detail/size_policy_string_form.hpp>
#include <common_type/assoc/detail/trigger_policy_string_form.hpp>
#include <common_type/assoc/template_policy.hpp>
#include <io/xml.hpp>

namespace __gnu_pbds
{

  namespace test
  {

    namespace detail
    {

      template<typename Size_Policy>
      struct size_policy_string_form;

      template<typename _Alloc>
      struct size_policy_string_form<
	__gnu_pbds::test::hash_exponential_size_policy_t_<_Alloc> >
      {
	static std::string
        name()
	{ return ("exp_"); }

	static std::string
        desc()
	{
	  return (make_xml_tag("Size_Policy", "value", "hash_exponential_size_policy"));
	}
      };

      template<>
      struct size_policy_string_form<
	__gnu_pbds::test::hash_prime_size_policy_t_>
      {
	static std::string
        name()
	{ return ("prime_"); }

	static std::string
        desc()
	{
	  return (make_xml_tag("Size_Policy", "value", "hash_prime_size_policy"));
	}
      };

    } // namespace detail

  } // namespace test

} // namespace __gnu_pbds

#endif // #ifndef PB_DS_SIZE_POLICY_STRING_FORM_HPP

