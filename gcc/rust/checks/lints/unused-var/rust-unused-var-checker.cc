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

#include "rust-unused-var-checker.h"
#include "rust-hir-item.h"

#include "options.h"

namespace Rust {
namespace Analysis {
UnusedVarChecker::UnusedVarChecker ()
  : nr_context (
    Resolver2_0::ImmutableNameResolutionContext::get ().resolver ()),
    mappings (Analysis::Mappings::get ()),
    unused_var_context (*UnusedVarContext::get ())
{}
void
UnusedVarChecker::go (HIR::Crate &crate)
{
  UnusedVarCollector ().go (crate);
  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}
void
UnusedVarChecker::visit (HIR::LetStmt &stmt)
{
  HIR::Pattern &pattern = stmt.get_pattern ();
  check_variable (pattern);
  walk (stmt);
}
void
UnusedVarChecker::visit_function_param (HIR::FunctionParam &param)
{
  check_variable (param.get_param_name ());
}

void
UnusedVarChecker::visit (HIR::ConstantItem &item)
{
  std::string var_name = item.get_identifier ().as_string ();
  bool starts_with_under_score = var_name.compare (0, 1, "_") == 0;
  auto id = item.get_mappings ().get_hirid ();
  if (!unused_var_context.is_variable_used (id) && !starts_with_under_score)
    rust_warning_at (item.get_locus (), OPT_Wunused_variable,
		     "unused name '%s'",
		     item.get_identifier ().as_string ().c_str ());
}

void
UnusedVarChecker::visit (HIR::StaticItem &item)
{
  std::string var_name = item.get_identifier ().as_string ();
  bool starts_with_under_score = var_name.compare (0, 1, "_") == 0;
  auto id = item.get_mappings ().get_hirid ();
  if (!unused_var_context.is_variable_used (id) && !starts_with_under_score)
    rust_warning_at (item.get_locus (), OPT_Wunused_variable,
		     "unused name '%s'",
		     item.get_identifier ().as_string ().c_str ());
}

void
UnusedVarChecker::visit (HIR::TraitItemFunc &item)
{
  // TODO: check trait item functions if they are not derived.
}

void
UnusedVarChecker::check_variable (HIR::Pattern &pattern)
{
  switch (pattern.get_pattern_type ())
    {
    case HIR::Pattern::PatternType::IDENTIFIER:
      check_variable_identifier (
	static_cast<HIR::IdentifierPattern &> (pattern));
      break;
    case HIR::Pattern::PatternType::TUPLE:
      check_variable_tuple (static_cast<HIR::TuplePattern &> (pattern));
      break;
    default:
      break;
    }
}
void
UnusedVarChecker::check_variable_identifier (HIR::IdentifierPattern &pattern)
{
  std::string var_name = pattern.get_identifier ().as_string ();
  bool starts_with_under_score = var_name.compare (0, 1, "_") == 0;
  auto id = pattern.get_mappings ().get_hirid ();
  if (!unused_var_context.is_variable_used (id) && var_name != "self"
      && !starts_with_under_score)
    rust_warning_at (pattern.get_locus (), OPT_Wunused_variable,
		     "unused name '%s'",
		     pattern.get_identifier ().as_string ().c_str ());
}
void
UnusedVarChecker::check_variable_tuple (HIR::TuplePattern &pattern)
{
  switch (pattern.get_items ().get_item_type ())
    {
    case HIR::TuplePatternItems::ItemType::NO_REST:
      {
	auto items
	  = static_cast<HIR::TuplePatternItemsNoRest &> (pattern.get_items ());
	for (auto &item : items.get_patterns ())
	  check_variable (*item);
      }
      break;
    default:
      break;
    }
}
} // namespace Analysis
} // namespace Rust