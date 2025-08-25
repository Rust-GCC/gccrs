// Copyright (C) 2025 Free Software Foundation, Inc.

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

#include "rust-unused-var-collector.h"
#include "rust-hir-full-decls.h"
#include "rust-hir-item.h"
#include "rust-hir-path.h"
#include "rust-immutable-name-resolution-context.h"

namespace Rust {
namespace Analysis {
UnusedVarCollector::UnusedVarCollector (UnusedVarContext &context)
  : nr_context (
    Resolver2_0::ImmutableNameResolutionContext::get ().resolver ()),
    mappings (Analysis::Mappings::get ()), unused_var_context (context)
{}
void
UnusedVarCollector::go (HIR::Crate &crate)
{
  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}
void
UnusedVarCollector::visit (HIR::LetStmt &stmt)
{
  HIR::Pattern &pattern = stmt.get_pattern ();
  collect_variable (pattern);
  walk (stmt);
}

void
UnusedVarCollector::visit (HIR::ConstantItem &item)
{
  unused_var_context.add_variable (item.get_mappings ().get_hirid ());
  walk (item);
}

void
UnusedVarCollector::visit (HIR::StaticItem &item)
{
  unused_var_context.add_variable (item.get_mappings ().get_hirid ());
  walk (item);
}

void
UnusedVarCollector::visit_function_param (HIR::FunctionParam &param)
{
  collect_variable (param.get_param_name ());
}

void
UnusedVarCollector::visit_closure_param (HIR::ClosureParam &param)
{
  collect_variable (param.get_pattern ());
}

void
UnusedVarCollector::collect_variable (HIR::Pattern &pattern)
{
  switch (pattern.get_pattern_type ())
    {
    case HIR::Pattern::PatternType::IDENTIFIER:
      {
	auto &identifier = static_cast<HIR::IdentifierPattern &> (pattern);
	auto id = identifier.get_mappings ().get_hirid ();
	unused_var_context.add_variable (id);
      }
      break;
    case HIR::Pattern::PatternType::TUPLE:
      {
	collect_variable_tuple (static_cast<HIR::TuplePattern &> (pattern));
      }
      break;
    default:
      break;
    }
}
void
UnusedVarCollector::collect_variable_tuple (HIR::TuplePattern &pattern)
{
  switch (pattern.get_items ().get_item_type ())
    {
    case HIR::TuplePatternItems::ItemType::NO_REST:
      {
	auto &items
	  = static_cast<HIR::TuplePatternItemsNoRest &> (pattern.get_items ());
	for (auto &sub : items.get_patterns ())
	  collect_variable (*sub);
      }
      break;
    default:
      break;
    }
}

void
UnusedVarCollector::visit (HIR::PathInExpression &expr)
{
  mark_path_used (expr);
}

void
UnusedVarCollector::visit (HIR::QualifiedPathInExpression &expr)
{
  mark_path_used (expr);
}

void
UnusedVarCollector::visit (HIR::StructExprFieldIdentifier &ident)
{
  mark_path_used (ident);
}
} // namespace Analysis
} // namespace Rust
