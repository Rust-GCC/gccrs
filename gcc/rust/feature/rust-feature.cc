// Copyright (C) 2020-2022 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "rust-session-manager.h"
#include "ordered-hash-map.h"
#include "rust-feature.h"

namespace Rust {

Feature feature_table[] = {

  DECLARE_ACTIVE_FEATURE ("associated_type_bounds", "1.34.0", "0.0.0", 52662, 0,
			  ""),
  DECLARE_ACTIVE_FEATURE ("intrinsics", "1.0.0", "0.0.0", 0, 0, ""),
  DECLARE_ACTIVE_FEATURE ("rustc_attrs", "1.0.0", "0.0.0", 0, 0, ""),

};

uint32_t Feature::NUM = 0;
std::map<std::string, const Feature *> Feature::feature_hash_map;

std::map<std::string, const Feature *> &
Feature::load_hash_map ()
{
  if (NUM != feature_hash_map.size ())
    {
      for (uint32_t i = 0; i < NUM; i++)
	{
	  feature_hash_map.insert ({feature_table[i].name, &feature_table[i]});
	}
    }
  return feature_hash_map;
}

} // namespace Rust