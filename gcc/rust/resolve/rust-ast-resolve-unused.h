// Copyright (C) 2020 Free Software Foundation, Inc.

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

#ifndef RUST_AST_RESOLVE_UNUSED_H
#define RUST_AST_RESOLVE_UNUSED_H

#include "rust-ast-resolve-base.h"

namespace Rust {
namespace Resolver {

class ScanUnused : public ResolverBase
{
public:
  static void Scan (Rib *r)
  {
    r->iterate_decls ([&] (NodeId decl_node_id, Location locus) -> bool {
      if (!r->have_references_for_node (decl_node_id))
	{
	  rust_warning_at (locus, 0, "unused name");
	}
      return true;
    });
  }
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_AST_RESOLVE_UNUSED_H
