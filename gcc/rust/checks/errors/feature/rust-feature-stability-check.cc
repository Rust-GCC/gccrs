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

#include "rust-feature-stability-check.h"
#include "rust-attribute-values.h"
#include "rust-diagnostics.h"
#include "rust-feature-stability.h"
#include "rust-finalized-name-resolution-context.h"
#include "rust-hir-item.h"
#include "rust-hir-map.h"
#include "rust-hir.h"
#include "rust-item.h"

namespace Rust {
namespace HIR {

namespace {
tl::optional<std::pair<std::string, Stability>>
parse_stability_attribute (const std::vector<AST::Attribute> &attributes)
{
  for (const AST::Attribute &attr : attributes)
    {
      std::string attr_name = attr.get_path ().as_string ();

      Stability stability;

      if (attr_name == Values::Attributes::STABLE)
	stability = Stability::STABLE;
      else if (attr_name == Values::Attributes::UNSTABLE)
	stability = Stability::UNSTABLE;
      else if (attr_name == Values::Attributes::RUSTC_CONST_STABLE)
	stability = Stability::STABLE;
      else if (attr_name == Values::Attributes::RUSTC_CONST_UNSTABLE)
	stability = Stability::UNSTABLE;
      else
	continue;

      if (attr.empty_input ())
	continue;

      auto &attr_input = attr.get_attr_input ();
      if (attr_input.get_attr_input_type ()
	  != AST::AttrInput::AttrInputType::TOKEN_TREE)
	continue;

      std::unique_ptr<AST::AttrInputMetaItemContainer> meta_item (
	static_cast<const AST::DelimTokenTree &> (attr_input)
	  .parse_to_meta_item ());

      for (auto &item : meta_item->get_items ())
	{
	  if (item->is_key_value_pair ())
	    {
	      auto &pair = static_cast<const AST::MetaNameValueStr &> (*item);
	      if (pair.get_name ().as_string () == "feature")
		auto x = pair.get_value ();
	      return std::make_pair (pair.get_value (), stability);
	    }
	}
    }
  return tl::nullopt;
}
} // namespace

void
StabilityChecker::check_early (AST::Crate &crate)
{
  check_macros ();

  for (auto &item : crate.items)
    item->accept_vis (*this);
}

void
StabilityChecker::check_late (HIR::Crate &crate)
{
  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}

void
StabilityChecker::check_macros ()
{
  for (auto &pair : mappings.get_macro_invocations ())
    {
      NodeId invoc_nodeid = pair.first;
      AST::MacroRulesDefinition *def = pair.second;

      if (auto def_crate_num
	  = mappings.lookup_macro_def_crate (def->get_node_id ()))
	if (def_crate_num.value () == mappings.crate.get_current_crate ())
	  continue;

      location_t invoc_locus
	= mappings.get_macro_invocation_location (invoc_nodeid);

      if (auto lib_feature
	  = parse_stability_attribute (def->get_outer_attrs ()))
	if (lib_feature->second == Stability::UNSTABLE)
	  if (active_features.valid_lib_features.find (lib_feature->first)
	      == active_features.valid_lib_features.end ())
	    stability_error (invoc_locus, lib_feature->first.c_str ());
    }
}

void
StabilityChecker::visit (AST::UseTreeRebind &use_tree)
{
  if (auto nslookup = name_ctx.lookup (
	use_tree.get_path ().get_final_segment ().get_node_id (),
	Resolver2_0::Namespace::Types, Resolver2_0::Namespace::Values,
	Resolver2_0::Namespace::Macros))
    {
      if (nslookup.value ().ns == Resolver2_0::Namespace::Macros)
	check_stability_macro (use_tree.get_locus (), nslookup->id);
      else
	check_stability (use_tree.get_locus (), nslookup->id);
    }
}

void
StabilityChecker::visit (AST::UseTreeList &use_tree_list)
{
  for (auto &use_tree : use_tree_list.get_trees ())
    use_tree->accept_vis (*this);
}

void
StabilityChecker::visit (AST::ExternCrate &crate)
{
  if (auto extern_crate_num
      = mappings.crate.lookup_crate_name (crate.get_referenced_crate ()))
    {
      auto &extern_crate = mappings.crate.get_ast_crate (*extern_crate_num);

      if (auto lib_feature
	  = parse_stability_attribute (extern_crate.get_inner_attrs ()))
	if (lib_feature->second == Stability::UNSTABLE)
	  if (active_features.valid_lib_features.find (lib_feature->first)
	      == active_features.valid_lib_features.end ())
	    stability_error (crate.get_locus (), lib_feature->first.c_str ());
    }
}

void
StabilityChecker::visit (HIR::PathInExpression &expr)
{
  if (auto nslookup = name_ctx.lookup (expr.get_mappings ().get_nodeid (),
				       Resolver2_0::Namespace::Values,
				       Resolver2_0::Namespace::Types))
    check_stability (expr.get_locus (), nslookup->id);
}

void
StabilityChecker::visit (HIR::MethodCallExpr &expr)
{
  if (auto nodeid = name_ctx.lookup (expr.get_mappings ().get_nodeid (),
				     Resolver2_0::Namespace::Values))
    check_stability (expr.get_locus (), *nodeid);
}

void
StabilityChecker::visit (HIR::TypePath &expr)
{
  if (auto nodeid = name_ctx.lookup (expr.get_mappings ().get_nodeid (),
				     Resolver2_0::Namespace::Types))
    check_stability (expr.get_locus (), *nodeid);
}

void
StabilityChecker::visit (HIR::QualifiedPathInExpression &expr)
{
  auto &ty = expr.get_path_type ().get_type ();
  if (auto nodeid = name_ctx.lookup (ty.get_mappings ().get_nodeid (),
				     Resolver2_0::Namespace::Types))
    check_stability (ty.get_locus (), *nodeid);

  if (auto nslookup = name_ctx.lookup (expr.get_mappings ().get_nodeid (),
				       Resolver2_0::Namespace::Values,
				       Resolver2_0::Namespace::Types))
    check_stability (expr.get_locus (), nslookup->id);
}

void
StabilityChecker::visit (HIR::QualifiedPathInType &expr)
{
  auto &path = expr.get_path_type ();

  auto &ty = path.get_type ();
  if (auto nodeid = name_ctx.lookup (ty.get_mappings ().get_nodeid (),
				     Resolver2_0::Namespace::Types))
    check_stability (ty.get_locus (), *nodeid);

  auto &trait_path = path.get_trait ();
  if (auto nodeid = name_ctx.lookup (trait_path.get_mappings ().get_nodeid (),
				     Resolver2_0::Namespace::Types))
    if (auto hirid = mappings.lookup_node_to_hir (*nodeid))
      if (auto item = mappings.hir.items.lookup (*hirid))
	if (item.value ()->get_item_kind () == HIR::Item::ItemKind::Trait)
	  {
	    auto trait = static_cast<HIR::Trait *> (*item);
	    std::string segment_name = expr.get_associated_segment ()
					 .get_ident_segment ()
					 .to_string ();

	    for (auto &trait_item : trait->get_trait_items ())
	      {
		if (trait_item->get_item_kind ()
		    == HIR::TraitItem::TraitItemKind::TYPE)
		  {
		    auto &type_item
		      = static_cast<HIR::TraitItemType &> (*trait_item);
		    if (type_item.get_name () == segment_name)
		      {
			check_stability (
			  expr.get_locus (),
			  type_item.get_mappings ().get_nodeid ());
			break;
		      }
		  }
	      }
	  }
}

tl::optional<std::pair<CrateNum, const AST::AttrVec &>>
StabilityChecker::get_crate_num_and_outer_attrs (HirId hir_id)
{
  if (auto item = mappings.hir.items.lookup (hir_id))
    return std::pair<CrateNum, const AST::AttrVec &> (
      item.value ()->get_mappings ().get_crate_num (),
      item.value ()->get_outer_attrs ());

  if (auto impl_item = mappings.lookup_hir_implitem (hir_id))
    switch (impl_item->first->get_impl_item_type ())
      {
      case HIR::ImplItem::ImplItemType::FUNCTION:
	return std::pair<CrateNum, const AST::AttrVec &> (
	  impl_item->first->get_impl_mappings ().get_crate_num (),
	  static_cast<HIR::Function *> (impl_item->first)->get_outer_attrs ());

      case HIR::ImplItem::ImplItemType::TYPE_ALIAS:
	return std::pair<CrateNum, const AST::AttrVec &> (
	  impl_item->first->get_impl_mappings ().get_crate_num (),
	  static_cast<HIR::TypeAlias *> (impl_item->first)->get_outer_attrs ());

      case HIR::ImplItem::ImplItemType::CONSTANT:
	return std::pair<CrateNum, const AST::AttrVec &> (
	  impl_item->first->get_impl_mappings ().get_crate_num (),
	  static_cast<HIR::ConstantItem *> (impl_item->first)
	    ->get_outer_attrs ());
      }

  if (auto trait_item = mappings.hir.trait_items.lookup (hir_id))
    return std::pair<CrateNum, const AST::AttrVec &> (
      trait_item.value ()->get_mappings ().get_crate_num (),
      trait_item.value ()->get_outer_attrs ());

  if (auto extern_item = mappings.lookup_hir_extern_item (hir_id))
    return std::pair<CrateNum, const AST::AttrVec &> (
      extern_item->first->get_mappings ().get_crate_num (),
      extern_item.value ().first->get_outer_attrs ());

  return tl::nullopt;
}

void
StabilityChecker::stability_error (location_t locus, const char *feature_name)
{
  rust_error_at (locus, ErrorCode::E0658, "use of unstable library feature %qs",
		 feature_name);

  rust_inform (locus,
	       "add %<#![feature(%s)]%> to the crate "
	       "attributes to enable",
	       feature_name);
}

void
StabilityChecker::check_stability (location_t locus, NodeId nodeid)
{
  if (auto hirid = mappings.lookup_node_to_hir (nodeid))
    if (auto pair = get_crate_num_and_outer_attrs (*hirid))
      if (pair->first != mappings.crate.get_current_crate ())
	if (auto lib_feature = parse_stability_attribute (pair->second))
	  if (lib_feature->second == Stability::UNSTABLE)
	    if (active_features.valid_lib_features.find (lib_feature->first)
		== active_features.valid_lib_features.end ())
	      stability_error (locus, lib_feature->first.c_str ());
}

void
StabilityChecker::check_stability_macro (location_t locus, NodeId nodeid)
{
  if (auto def = mappings.lookup_macro_def (nodeid))
    if (auto def_crate_num
	= mappings.lookup_macro_def_crate (def.value ()->get_node_id ()))
      if (def_crate_num.value () != mappings.crate.get_current_crate ())
	if (auto lib_feature
	    = parse_stability_attribute (def.value ()->get_outer_attrs ()))
	  if (lib_feature->second == Stability::UNSTABLE)
	    if (active_features.valid_lib_features.find (lib_feature->first)
		== active_features.valid_lib_features.end ())
	      stability_error (locus, lib_feature->first.c_str ());
}

} // namespace HIR
} // namespace Rust
