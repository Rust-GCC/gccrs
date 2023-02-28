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
 * @file native_multimap.hpp
 * Contains an adapter to std::multimap
 */

#ifndef PB_DS_NATIVE_MULTIMAP_HPP
#define PB_DS_NATIVE_MULTIMAP_HPP

#include <map>
#include <string>
#include <ext/pb_ds/detail/type_utils.hpp>
#include <native_type/native_tree_tag.hpp>

namespace __gnu_pbds
{
  namespace test
  {
#define PB_DS_BASE_C_DEC \
    std::multimap<Key, Data, Less_Fn, \
      typename _Alloc::template rebind<std::pair<const Key, Data> >::other>

    template<typename Key, typename Data, class Less_Fn = std::less<Key>,
	     typename _Alloc = std::allocator<char> >
    class native_multimap : public PB_DS_BASE_C_DEC
    {
    private:
      typedef PB_DS_BASE_C_DEC base_type;

    public:
      typedef native_tree_tag container_category;

      typedef _Alloc allocator;

      typedef
      typename _Alloc::template rebind<
	std::pair<Key, Data> >::other::const_reference
      const_reference;

      typedef typename base_type::iterator iterator;
      typedef typename base_type::const_iterator const_iterator;

      native_multimap()  { }

      template<typename It>
      native_multimap(It f, It l) : base_type(f, l)
      { }

      inline void
      insert(const_reference r_val)
      {
        typedef std::pair<iterator, iterator> eq_range_t;
        eq_range_t f = base_type::equal_range(r_val.first);

        iterator it = f.first;
        while (it != f.second)
	  {
            if (it->second == r_val.second)
	      return;
            ++it;
	  }
        base_type::insert(r_val);
      }

      inline iterator
      find(const_reference r_val)
      {
        typedef std::pair<iterator, iterator> eq_range_t;
        eq_range_t f = base_type::equal_range(r_val.first);

        iterator it = f.first;
        while (it != f.second)
	  {
            if (it->second == r_val.second)
	      return it;
            ++it;
	  }

        return base_type::end();
      }

      inline const_iterator
      find(const_reference r_val) const
      {
        typedef std::pair<const_iterator, const_iterator> eq_range_t;
        eq_range_t f = base_type::equal_range(r_val.first);

        const_iterator it = f.first;
        while (it != f.second)
	  {
            if (it->second == r_val.second)
	      return it;
            ++it;
	  }
        return base_type::end();
      }

      static std::string
      name()
      { return std::string("n_mmap"); }

      static std::string
      desc()
      { return make_xml_tag("type", "value", "std_multimap"); }
    };

#undef PB_DS_BASE_C_DEC
} // namespace test

} // namespace __gnu_pbds

#endif // #ifndef PB_DS_NATIVE_MULTIMAP_HPP
