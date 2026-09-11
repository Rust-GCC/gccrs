// Copyright (C) 2026 Free Software Foundation, Inc.

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

#include "rust-hir-map.h"
#include "optional.h"

namespace Rust {
namespace Analysis {

template <typename Item, bool insert_location>
tl::optional<Item *>
PtrMapping<Item, insert_location>::lookup (HirId id)
{
  auto it = storage.find (id);
  if (it == storage.cend ())
    return tl::nullopt;
  return it->second;
}

template <typename Item, bool insert_location>
void
PtrMapping<Item, insert_location>::insert (Item *item)
{
  auto hir_id = item->get_mappings ().get_hirid ();
  rust_assert (!lookup (hir_id).has_value ());

  storage.insert ({hir_id, item});

  Analysis::Mappings::get ().insert_node_to_hir (
    item->get_mappings ().get_nodeid (), hir_id);
  if constexpr (insert_location)
    {
      Analysis::Mappings::get ().insert_location (hir_id, item->get_locus ());
    }
}
} // namespace Analysis
} // namespace Rust
