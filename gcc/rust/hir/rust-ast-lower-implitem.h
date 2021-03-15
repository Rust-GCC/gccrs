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

#ifndef RUST_AST_LOWER_IMPLITEM_H
#define RUST_AST_LOWER_IMPLITEM_H

#include "rust-diagnostics.h"
#include "rust-ast-lower-type.h"
#include "rust-ast-lower-stmt.h"
#include "rust-ast-lower-expr.h"
#include "rust-ast-lower-pattern.h"
#include "rust-ast-lower-block.h"

namespace Rust {
namespace HIR {

class ASTLowerImplItem : public ASTLoweringBase
{
  using Rust::HIR::ASTLoweringBase::visit;

public:
  static HIR::InherentImplItem *translate (AST::InherentImplItem *item)
  {
    ASTLowerImplItem resolver;
    item->accept_vis (resolver);
    rust_assert (resolver.translated != nullptr);
    return resolver.translated;
  }

  HIR::SelfParam lower_self (AST::SelfParam &self)
  {
    HIR::Type *type = self.has_type ()
			? ASTLoweringType::translate (self.get_type ().get ())
			: nullptr;

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, self.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    return HIR::SelfParam (mapping, std::unique_ptr<HIR::Type> (type),
			   self.get_is_mut (), self.get_locus ());
  }

  void visit (AST::ConstantItem &constant) override
  {
    std::vector<HIR::Attribute> outer_attrs;
    HIR::Visibility vis = HIR::Visibility::create_public ();

    HIR::Type *type = ASTLoweringType::translate (constant.get_type ().get ());
    HIR::Expr *expr = ASTLoweringExpr::translate (constant.get_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, constant.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    translated = new HIR::ConstantItem (mapping, constant.get_identifier (),
					vis, std::unique_ptr<HIR::Type> (type),
					std::unique_ptr<HIR::Expr> (expr),
					outer_attrs, constant.get_locus ());

    mappings->insert_hir_implitem (mapping.get_crate_num (),
				   mapping.get_hirid (), translated);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       constant.get_locus ());
  }

  void visit (AST::Function &function) override
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

	auto crate_num = mappings->get_current_crate ();
	Analysis::NodeMapping mapping (crate_num, param.get_node_id (),
				       mappings->get_next_hir_id (crate_num),
				       UNKNOWN_LOCAL_DEFID);

	auto hir_param
	  = HIR::FunctionParam (mapping, std::move (translated_pattern),
				std::move (translated_type),
				param.get_locus ());
	function_params.push_back (hir_param);
      }

    bool terminated = false;
    std::unique_ptr<HIR::BlockExpr> function_body
      = std::unique_ptr<HIR::BlockExpr> (
	ASTLoweringBlock::translate (function.get_definition ().get (),
				     &terminated));

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, function.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    mappings->insert_location (crate_num,
			       function_body->get_mappings ().get_hirid (),
			       function.get_locus ());

    auto fn
      = new HIR::Function (mapping, std::move (function_name),
			   std::move (qualifiers), std::move (generic_params),
			   std::move (function_params), std::move (return_type),
			   std::move (where_clause), std::move (function_body),
			   std::move (vis), std::move (outer_attrs), locus);

    mappings->insert_hir_implitem (mapping.get_crate_num (),
				   mapping.get_hirid (), fn);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       function.get_locus ());

    // add the mappings for the function params at the end
    for (auto &param : fn->get_function_params ())
      {
	mappings->insert_hir_param (mapping.get_crate_num (),
				    param.get_mappings ().get_hirid (), &param);
	mappings->insert_location (crate_num, mapping.get_hirid (),
				   param.get_locus ());
      }

    translated = fn;
  }

  void visit (AST::Method &method) override
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
    Identifier method_name = method.get_method_name ();
    Location locus = method.get_locus ();

    HIR::SelfParam self_param = lower_self (method.get_self_param ());

    std::unique_ptr<HIR::Type> return_type
      = method.has_return_type () ? std::unique_ptr<HIR::Type> (
	  ASTLoweringType::translate (method.get_return_type ().get ()))
				  : nullptr;

    std::vector<HIR::FunctionParam> function_params;
    for (auto &param : method.get_function_params ())
      {
	auto translated_pattern = std::unique_ptr<HIR::Pattern> (
	  ASTLoweringPattern::translate (param.get_pattern ().get ()));
	auto translated_type = std::unique_ptr<HIR::Type> (
	  ASTLoweringType::translate (param.get_type ().get ()));

	auto crate_num = mappings->get_current_crate ();
	Analysis::NodeMapping mapping (crate_num, param.get_node_id (),
				       mappings->get_next_hir_id (crate_num),
				       UNKNOWN_LOCAL_DEFID);

	auto hir_param
	  = HIR::FunctionParam (mapping, std::move (translated_pattern),
				std::move (translated_type),
				param.get_locus ());
	function_params.push_back (hir_param);
      }

    bool terminated = false;
    std::unique_ptr<HIR::BlockExpr> method_body
      = std::unique_ptr<HIR::BlockExpr> (
	ASTLoweringBlock::translate (method.get_definition ().get (),
				     &terminated));

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, method.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));
    auto mth
      = new HIR::Method (mapping, std::move (method_name),
			 std::move (qualifiers), std::move (generic_params),
			 std::move (self_param), std::move (function_params),
			 std::move (return_type), std::move (where_clause),
			 std::move (method_body), std::move (vis),
			 std::move (outer_attrs), locus);

    mappings->insert_hir_implitem (mapping.get_crate_num (),
				   mapping.get_hirid (), mth);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       method.get_locus ());

    // insert mappings for self
    mappings->insert_hir_self_param (crate_num,
				     self_param.get_mappings ().get_hirid (),
				     &self_param);
    mappings->insert_location (crate_num,
			       self_param.get_mappings ().get_hirid (),
			       self_param.get_locus ());

    // add the mappings for the function params at the end
    for (auto &param : mth->get_function_params ())
      {
	mappings->insert_hir_param (mapping.get_crate_num (),
				    param.get_mappings ().get_hirid (), &param);
	mappings->insert_location (crate_num, mapping.get_hirid (),
				   param.get_locus ());
      }

    translated = mth;
  }

private:
  ASTLowerImplItem () : translated (nullptr) {}

  HIR::InherentImplItem *translated;
};

} // namespace HIR

} // namespace Rust

#endif // RUST_AST_LOWER_IMPLITEM_H
