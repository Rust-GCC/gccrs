// Copyright (C) 2020-2025 Free Software Foundation, Inc.

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

#include "rust-readonly-check2.h"
#include "rust-hir-expr-abstract.h"
#include "rust-hir-node.h"
#include "rust-hir-path.h"
#include "rust-hir-map.h"
#include "rust-hir-pattern.h"
#include "rust-mapping-common.h"
#include "rust-system.h"

namespace Rust {
namespace HIR {

static std::map<HirId, int> assignment_map = {};

ReadonlyChecker::ReadonlyChecker ()
  : resolver (*Resolver::Resolver::get ()),
    mappings (Analysis::Mappings::get ()), in_mutable_context (false)
{}

void
ReadonlyChecker::go (HIR::Crate &crate)
{
  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}

void
ReadonlyChecker::visit (AssignmentExpr &expr)
{
  Expr &lhs = expr.get_lhs ();
  in_mutable_context = true;
  lhs.accept_vis (*this);
  in_mutable_context = false;
}

void
ReadonlyChecker::visit (PathInExpression &expr)
{
  if (!in_mutable_context)
    return;

  NodeId id = expr.get_mappings ().get_nodeid ();
  NodeId def_id;
  if (!resolver.lookup_resolved_name (id, &def_id))
    return;

  auto hir_id = mappings.lookup_node_to_hir (def_id);
  if (!hir_id)
    return;

  if (assignment_map.find (*hir_id) == assignment_map.end ())
    return;

  assignment_map[*hir_id]++;

  // Check if the local variable is mutable.
  auto maybe_pattern = mappings.lookup_hir_pattern (*hir_id);
  if (maybe_pattern
      && maybe_pattern.value ()->get_pattern_type ()
	   == HIR::Pattern::PatternType::IDENTIFIER)
    {
      auto ident_pattern
	= static_cast<HIR::IdentifierPattern *> (*maybe_pattern);
      if (!ident_pattern->is_mut () && assignment_map[*hir_id] > 1)
	rust_error_at (expr.get_locus (),
		       "cannot assign to immutable variable");
    }

  // Check if the static variable is mutable.
  auto maybe_static_item = mappings.lookup_hir_item (*hir_id);
  if (maybe_static_item
      && maybe_static_item.value ()->get_item_kind ()
	   == HIR::Item::ItemKind::Static)
    {
      auto static_item = static_cast<HIR::StaticItem *> (*maybe_static_item);
      if (!static_item->is_mut () && assignment_map[*hir_id] > 1)
	rust_error_at (expr.get_locus (),
		       "cannot assign to mutable static variable");
    }
}

// Check if the variables are initialized in the let statement.
// If the pattern is a tuple, we need to check each element.
void
ReadonlyChecker::visit (LetStmt &stmt)
{
  HIR::Pattern &pattern = stmt.get_pattern ();
  HirId pattern_id = pattern.get_mappings ().get_hirid ();
  assignment_map.insert ({pattern_id, 0});
  if (stmt.has_init_expr ())
    {
      assignment_map[pattern_id]++;
    }
}

void
ReadonlyChecker::visit (FieldAccessExpr &expr)
{
  if (in_mutable_context)
    {
      // TODO: Add check for field access
    }
}

void
ReadonlyChecker::visit (TupleIndexExpr &expr)
{
  if (in_mutable_context)
    {
      // TODO: Add check for tuple index
    }
}

void
ReadonlyChecker::visit (ArrayIndexExpr &expr)
{
  if (in_mutable_context)
    {
      // TODO: Add check for array index
    }
}

void
ReadonlyChecker::visit (TupleExpr &expr)
{
  if (in_mutable_context)
    {
      // TODO: Add check for tuple expression
    }
}
} // namespace HIR
} // namespace Rust
