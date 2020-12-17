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

#ifndef RUST_AST_LOWER_ITEM
#define RUST_AST_LOWER_ITEM

#include "rust-diagnostics.h"

#include "rust-ast-lower-base.h"
#include "rust-ast-lower-type.h"
#include "rust-ast-lower-stmt.h"
#include "rust-ast-lower-pattern.h"

namespace Rust {
namespace HIR {

class ASTLoweringItem : public ASTLoweringBase
{
public:
  static HIR::Item *translate (AST::Item *item)
  {
    ASTLoweringItem resolver;
    item->accept_vis (resolver);
    return resolver.translated;
  }

  virtual ~ASTLoweringItem () {}

  void visit (AST::Function &function)
  {
    // ignore for now and leave empty
    std::vector<std::unique_ptr<HIR::GenericParam> > generic_params;
    std::vector<HIR::Attribute> outer_attrs;
    std::vector<std::unique_ptr<HIR::WhereClauseItem> > where_clause_items;
    HIR::WhereClause where_clause (std::move (where_clause_items));
    HIR::FunctionQualifiers qualifiers (
      HIR::FunctionQualifiers::AsyncConstStatus::NONE, false);
    HIR::Visibility vis = HIR::Visibility::create_public ();

    // need
    Identifier function_name = function.get_function_name ();
    Location locus = function.get_locus ();

    std::unique_ptr<HIR::Type> return_type
      = function.has_return_type () ? std::unique_ptr<HIR::Type> (
	  ASTLoweringType::translate (function.get_return_type ().get ()))
				    : nullptr;

    std::vector<HIR::FunctionParam> function_params;
    for (auto &param : function.get_function_params ())
      {
	auto translated_pattern = std::unique_ptr<HIR::Pattern> (
	  ASTLoweringPattern::translate (param.get_pattern ().get ()));
	auto translated_type = std::unique_ptr<HIR::Type> (
	  ASTLoweringType::translate (param.get_type ().get ()));

	function_params.push_back (
	  HIR::FunctionParam (std::move (translated_pattern),
			      std::move (translated_type), param.get_locus ()));
      }

    std::unique_ptr<HIR::BlockExpr> function_body
      = std::unique_ptr<HIR::BlockExpr> (
	translate (function.get_definition ().get ()));

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, function.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    translated
      = new HIR::Function (mapping, std::move (function_name),
			   std::move (qualifiers), std::move (generic_params),
			   std::move (function_params), std::move (return_type),
			   std::move (where_clause), std::move (function_body),
			   std::move (vis), std::move (outer_attrs), locus);

    mappings->insert_defid_mapping (mapping.get_defid (), translated);
    mappings->insert_hir_item (mapping.get_crate_num (), mapping.get_hirid (),
			       translated);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       function.get_locus ());
  }

  // Helpers

  HIR::BlockExpr *translate (AST::BlockExpr *function_body)
  {
    std::vector<std::unique_ptr<HIR::Stmt> > block_stmts;
    std::unique_ptr<HIR::ExprWithoutBlock> block_expr;
    std::vector<HIR::Attribute> inner_attribs;
    std::vector<HIR::Attribute> outer_attribs;

    function_body->iterate_stmts ([&] (AST::Stmt *s) mutable -> bool {
      auto translated_stmt = ASTLoweringStmt::translate (s);
      block_stmts.push_back (std::unique_ptr<HIR::Stmt> (translated_stmt));
      return true;
    });

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, function_body->get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    return new HIR::BlockExpr (mapping, std::move (block_stmts),
			       std::move (block_expr),
			       std::move (inner_attribs),
			       std::move (outer_attribs),
			       function_body->get_locus ());
  }

private:
  ASTLoweringItem () : translated (nullptr) {}

  HIR::Item *translated;
};

} // namespace HIR
} // namespace Rust

#endif // RUST_AST_LOWER_ITEM
