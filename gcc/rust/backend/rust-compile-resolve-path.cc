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

#include "rust-linemap.h"
#include "rust-backend.h"
#include "rust-compile-resolve-path.h"
#include "rust-compile-item.h"

namespace Rust {
namespace Compile {

void
ResolvePath::visit (HIR::PathInExpression &expr)
{
  // need to look up the reference for this identifier
  NodeId ref_node_id;
  if (!ctx->get_resolver ()->lookup_resolved_name (
	expr.get_mappings ().get_nodeid (), &ref_node_id))
    {
      rust_fatal_error (expr.get_locus (), "failed to look up resolved name");
      return;
    }

  printf ("PATHIN have ast node id %u ref %u for expr [%s]\n",
	  expr.get_mappings ().get_nodeid (), ref_node_id,
	  expr.as_string ().c_str ());

  HirId ref;
  if (!ctx->get_mappings ()->lookup_node_to_hir (
	expr.get_mappings ().get_crate_num (), ref_node_id, &ref))
    {
      rust_fatal_error (expr.get_locus (), "reverse lookup failure");
      return;
    }

  // assumes paths are functions for now
  Bfunction *fn;
  if (!ctx->lookup_function_decl (ref, &fn))
    {
      printf (
	"path failed to lookup function attempting to forward resolve!\n");

      // this might fail because its a forward decl so we can attempt to
      // resolve it now
      HIR::Item *resolved_item = ctx->get_mappings ()->lookup_hir_item (
	expr.get_mappings ().get_crate_num (), ref);
      if (resolved_item == nullptr)
	{
	  rust_fatal_error (expr.get_locus (), "failed to lookup forward decl");
	  return;
	}

      CompileItem::compile (resolved_item, ctx);
      if (!ctx->lookup_function_decl (ref, &fn))
	{
	  rust_fatal_error (expr.get_locus (), "forward decl was not compiled");
	  return;
	}
    }

  resolved
    = ctx->get_backend ()->function_code_expression (fn, expr.get_locus ());
}

} // namespace Compile
} // namespace Rust
