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
#include "rust-ast-lower-implitem.h"
#include "rust-ast-lower-stmt.h"
#include "rust-ast-lower-expr.h"
#include "rust-ast-lower-pattern.h"
#include "rust-ast-lower-block.h"

namespace Rust {
namespace HIR {

class ASTLoweringItem : public ASTLoweringBase
{
public:
  static HIR::Item *translate (AST::Item *item)
  {
    ASTLoweringItem resolver;
    item->accept_vis (resolver);

    // this is useful for debugging
    // if (resolver.translated == nullptr)
    //   {
    //     rust_fatal_error (item->get_locus_slow (), "failed to lower: %s",
    //     		  item->as_string ().c_str ());
    //     return nullptr;
    //   }

    return resolver.translated;
  }

  void visit (AST::TupleStruct &struct_decl)
  {
    std::vector<std::unique_ptr<HIR::GenericParam> > generic_params;
    if (struct_decl.has_generics ())
      {
	generic_params
	  = lower_generic_params (struct_decl.get_generic_params ());
      }

    std::vector<std::unique_ptr<HIR::WhereClauseItem> > where_clause_items;
    HIR::WhereClause where_clause (std::move (where_clause_items));
    HIR::Visibility vis = HIR::Visibility::create_public ();
    std::vector<HIR::Attribute> outer_attrs;

    std::vector<HIR::TupleField> fields;
    struct_decl.iterate ([&] (AST::TupleField &field) mutable -> bool {
      std::vector<HIR::Attribute> outer_attrs;
      HIR::Visibility vis = HIR::Visibility::create_public ();
      HIR::Type *type
	= ASTLoweringType::translate (field.get_field_type ().get ());

      auto crate_num = mappings->get_current_crate ();
      Analysis::NodeMapping mapping (crate_num, field.get_node_id (),
				     mappings->get_next_hir_id (crate_num),
				     mappings->get_next_localdef_id (
				       crate_num));

      // FIXME
      // AST::TupleField is missing Location info
      Location field_locus;
      HIR::TupleField translated_field (mapping,
					std::unique_ptr<HIR::Type> (type), vis,
					field_locus, outer_attrs);
      fields.push_back (std::move (translated_field));
      return true;
    });

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, struct_decl.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    translated = new HIR::TupleStruct (mapping, std::move (fields),
				       struct_decl.get_identifier (),
				       std::move (generic_params),
				       std::move (where_clause), vis,
				       std::move (outer_attrs),
				       struct_decl.get_locus ());

    mappings->insert_defid_mapping (mapping.get_defid (), translated);
    mappings->insert_hir_item (mapping.get_crate_num (), mapping.get_hirid (),
			       translated);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       struct_decl.get_locus ());
  }

  void visit (AST::StructStruct &struct_decl)
  {
    std::vector<std::unique_ptr<HIR::GenericParam> > generic_params;
    if (struct_decl.has_generics ())
      {
	generic_params
	  = lower_generic_params (struct_decl.get_generic_params ());
      }

    std::vector<std::unique_ptr<HIR::WhereClauseItem> > where_clause_items;
    HIR::WhereClause where_clause (std::move (where_clause_items));
    HIR::Visibility vis = HIR::Visibility::create_public ();
    std::vector<HIR::Attribute> outer_attrs;

    bool is_unit = struct_decl.is_unit_struct ();
    std::vector<HIR::StructField> fields;
    struct_decl.iterate ([&] (AST::StructField &field) mutable -> bool {
      std::vector<HIR::Attribute> outer_attrs;
      HIR::Visibility vis = HIR::Visibility::create_public ();
      HIR::Type *type
	= ASTLoweringType::translate (field.get_field_type ().get ());

      auto crate_num = mappings->get_current_crate ();
      Analysis::NodeMapping mapping (crate_num, field.get_node_id (),
				     mappings->get_next_hir_id (crate_num),
				     mappings->get_next_localdef_id (
				       crate_num));

      // FIXME
      // AST::StructField is missing Location info
      Location field_locus;
      HIR::StructField translated_field (mapping, field.get_field_name (),
					 std::unique_ptr<HIR::Type> (type), vis,
					 field_locus, outer_attrs);
      fields.push_back (std::move (translated_field));
      return true;
    });

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, struct_decl.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    translated = new HIR::StructStruct (mapping, std::move (fields),
					struct_decl.get_identifier (),
					std::move (generic_params),
					std::move (where_clause), is_unit, vis,
					std::move (outer_attrs),
					struct_decl.get_locus ());

    mappings->insert_defid_mapping (mapping.get_defid (), translated);
    mappings->insert_hir_item (mapping.get_crate_num (), mapping.get_hirid (),
			       translated);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       struct_decl.get_locus ());
  }

  void visit (AST::StaticItem &var)
  {
    std::vector<HIR::Attribute> outer_attrs;
    HIR::Visibility vis = HIR::Visibility::create_public ();

    HIR::Type *type = ASTLoweringType::translate (var.get_type ().get ());
    HIR::Expr *expr = ASTLoweringExpr::translate (var.get_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, var.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    translated
      = new HIR::StaticItem (mapping, var.get_identifier (), var.is_mutable (),
			     std::unique_ptr<HIR::Type> (type),
			     std::unique_ptr<HIR::Expr> (expr), vis,
			     outer_attrs, var.get_locus ());

    mappings->insert_defid_mapping (mapping.get_defid (), translated);
    mappings->insert_hir_item (mapping.get_crate_num (), mapping.get_hirid (),
			       translated);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       var.get_locus ());
  }

  void visit (AST::ConstantItem &constant)
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

    mappings->insert_defid_mapping (mapping.get_defid (), translated);
    mappings->insert_hir_item (mapping.get_crate_num (), mapping.get_hirid (),
			       translated);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       constant.get_locus ());
  }

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

    mappings->insert_defid_mapping (mapping.get_defid (), translated);
    mappings->insert_hir_item (mapping.get_crate_num (), mapping.get_hirid (),
			       fn);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       function.get_locus ());

    // add the mappings for the function params at the end
    for (auto &param : fn->function_params)
      {
	mappings->insert_hir_param (mapping.get_crate_num (),
				    param.get_mappings ().get_hirid (), &param);
	mappings->insert_location (crate_num, mapping.get_hirid (),
				   param.get_locus ());
      }

    translated = fn;
  }

  void visit (AST::InherentImpl &impl_block)
  {
    std::vector<HIR::Attribute> inner_attrs;
    std::vector<HIR::Attribute> outer_attrs;
    std::vector<std::unique_ptr<HIR::GenericParam> > generic_params;
    std::vector<std::unique_ptr<HIR::WhereClauseItem> > where_clause_items;

    HIR::WhereClause where_clause (std::move (where_clause_items));
    HIR::Visibility vis = HIR::Visibility::create_public ();
    HIR::Type *trait_type
      = ASTLoweringType::translate (impl_block.get_type ().get ());

    std::vector<std::unique_ptr<HIR::InherentImplItem> > impl_items;
    for (auto &impl_item : impl_block.get_impl_items ())
      {
	HIR::InherentImplItem *lowered
	  = ASTLowerImplItem::translate (impl_item.get ());
	impl_items.push_back (std::unique_ptr<HIR::InherentImplItem> (lowered));
      }

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, impl_block.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   mappings->get_next_localdef_id (crate_num));

    translated
      = new HIR::InherentImpl (mapping, std::move (impl_items),
			       std::move (generic_params),
			       std::unique_ptr<HIR::Type> (trait_type),
			       where_clause, vis, std::move (inner_attrs),
			       std::move (outer_attrs),
			       impl_block.get_locus ());

    mappings->insert_defid_mapping (mapping.get_defid (), translated);
    mappings->insert_hir_item (mapping.get_crate_num (), mapping.get_hirid (),
			       translated);
    mappings->insert_location (crate_num, mapping.get_hirid (),
			       impl_block.get_locus ());
  }

private:
  std::vector<std::unique_ptr<HIR::GenericParam> > lower_generic_params (
    std::vector<std::unique_ptr<AST::GenericParam> > &params)
  {
    std::vector<std::unique_ptr<HIR::GenericParam> > lowered;
    for (auto &ast_param : params)
      {
	auto hir_param = ASTLowerGenericParam::translate (ast_param.get ());
	lowered.push_back (std::unique_ptr<HIR::GenericParam> (hir_param));
      }

    return lowered;
  }

  ASTLoweringItem () : translated (nullptr) {}

  HIR::Item *translated;
};

} // namespace HIR
} // namespace Rust

#endif // RUST_AST_LOWER_ITEM
