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

#include "rust-hir-type-check.h"
#include "rust-hir-full.h"
#include "rust-hir-type-check-toplevel.h"
#include "rust-hir-type-check-item.h"
#include "rust-hir-type-check-expr.h"
#include "rust-hir-type-check-struct-field.h"
#include "rust-hir-inherent-impl-overlap.h"
#include "rust-hir-const-fold.h"

extern bool
saw_errors (void);

namespace Rust {
namespace Resolver {

void
TypeResolution::Resolve (HIR::Crate &crate)
{
  for (auto it = crate.items.begin (); it != crate.items.end (); it++)
    TypeCheckTopLevel::Resolve (it->get ());

  if (saw_errors ())
    return;

  OverlappingImplItemPass::go ();
  if (saw_errors ())
    return;

  for (auto it = crate.items.begin (); it != crate.items.end (); it++)
    TypeCheckItem::Resolve (it->get ());

  if (saw_errors ())
    return;

  auto mappings = Analysis::Mappings::get ();
  auto context = TypeCheckContext::get ();

  // default inference variables if possible
  context->iterate ([&] (HirId id, TyTy::BaseType *ty) mutable -> bool {
    if (ty->get_kind () == TyTy::TypeKind::ERROR)
      {
	rust_error_at (mappings->lookup_location (id),
		       "failure in type resolution for %u", id);
	return false;
      }

    // nothing to do
    if (ty->get_kind () != TyTy::TypeKind::INFER)
      return true;

    TyTy::InferType *infer_var = (TyTy::InferType *) ty;
    TyTy::BaseType *default_type;
    bool ok = infer_var->default_type (&default_type);
    if (!ok)
      {
	rust_error_at (mappings->lookup_location (id),
		       "type annotations needed");
	return true;
      }
    else
      {
	auto result = ty->unify (default_type);
	result->set_ref (id);
	context->insert_type (
	  Analysis::NodeMapping (mappings->get_current_crate (), 0, id,
				 UNKNOWN_LOCAL_DEFID),
	  result);
      }

    return true;
  });
}

// RUST_HIR_TYPE_CHECK_EXPR
void
TypeCheckExpr::visit (HIR::BlockExpr &expr)
{
  expr.iterate_stmts ([&] (HIR::Stmt *s) mutable -> bool {
    auto resolved = TypeCheckStmt::Resolve (s, inside_loop);
    if (resolved == nullptr)
      {
	rust_error_at (s->get_locus (), "failure to resolve type");
	return false;
      }

    if (s->is_unit_check_needed () && !resolved->is_unit ())
      {
	auto unit = new TyTy::TupleType (s->get_mappings ().get_hirid ());
	resolved = unit->unify (resolved);
      }

    return true;
  });

  if (expr.has_expr ())
    infered
      = TypeCheckExpr::Resolve (expr.get_final_expr ().get (), inside_loop)
	  ->clone ();
  else if (expr.is_tail_reachable ())
    infered = new TyTy::TupleType (expr.get_mappings ().get_hirid ());
  else
    infered = new TyTy::NeverType (expr.get_mappings ().get_hirid ());
}

// RUST_HIR_TYPE_CHECK_STRUCT_FIELD

void
TypeCheckStructExpr::visit (HIR::StructExprStructFields &struct_expr)
{
  TyTy::BaseType *struct_path_ty
    = TypeCheckExpr::Resolve (&struct_expr.get_struct_name (), false);
  if (struct_path_ty->get_kind () != TyTy::TypeKind::ADT)
    {
      rust_error_at (struct_expr.get_struct_name ().get_locus (),
		     "expected an ADT type for constructor");
      return;
    }

  struct_path_resolved = static_cast<TyTy::ADTType *> (struct_path_ty);
  TyTy::ADTType *struct_def = struct_path_resolved;
  if (struct_expr.has_struct_base ())
    {
      TyTy::BaseType *base_resolved
	= TypeCheckExpr::Resolve (struct_expr.struct_base->base_struct.get (),
				  false);
      struct_def
	= (TyTy::ADTType *) struct_path_resolved->unify (base_resolved);
      if (struct_def == nullptr)
	{
	  rust_fatal_error (struct_expr.struct_base->base_struct->get_locus (),
			    "incompatible types for base struct reference");
	  return;
	}
    }

  std::vector<TyTy::StructFieldType *> infered_fields;
  bool ok = true;

  for (auto &field : struct_expr.get_fields ())
    {
      resolved_field_value_expr = nullptr;
      field->accept_vis (*this);
      if (resolved_field_value_expr == nullptr)
	{
	  rust_fatal_error (field->get_locus (),
			    "failed to resolve type for field");
	  ok = false;
	  break;
	}

      context->insert_type (field->get_mappings (), resolved_field_value_expr);
    }

  // something failed setting up the fields
  if (!ok)
    {
      rust_error_at (struct_expr.get_locus (),
		     "constructor type resolution failure");
      return;
    }

  // check the arguments are all assigned and fix up the ordering
  if (fields_assigned.size () != struct_path_resolved->num_fields ())
    {
      if (struct_def->is_union ())
	{
	  if (fields_assigned.size () != 1 || struct_expr.has_struct_base ())
	    {
	      rust_error_at (
		struct_expr.get_locus (),
		"union must have exactly one field variant assigned");
	      return;
	    }
	}
      else if (!struct_expr.has_struct_base ())
	{
	  rust_error_at (struct_expr.get_locus (),
			 "constructor is missing fields");
	  return;
	}
      else
	{
	  // we have a struct base to assign the missing fields from.
	  // the missing fields can be implicit FieldAccessExprs for the value
	  std::set<std::string> missing_fields;
	  struct_path_resolved->iterate_fields (
	    [&] (TyTy::StructFieldType *field) mutable -> bool {
	      auto it = fields_assigned.find (field->get_name ());
	      if (it == fields_assigned.end ())
		missing_fields.insert (field->get_name ());
	      return true;
	    });

	  // we can generate FieldAccessExpr or TupleAccessExpr for the values
	  // of the missing fields.
	  for (auto &missing : missing_fields)
	    {
	      HIR::Expr *receiver
		= struct_expr.struct_base->base_struct->clone_expr_impl ();

	      HIR::StructExprField *implicit_field = nullptr;

	      AST::AttrVec outer_attribs;
	      auto crate_num = mappings->get_current_crate ();
	      Analysis::NodeMapping mapping (
		crate_num,
		struct_expr.struct_base->base_struct->get_mappings ()
		  .get_nodeid (),
		mappings->get_next_hir_id (crate_num), UNKNOWN_LOCAL_DEFID);

	      HIR::Expr *field_value = new HIR::FieldAccessExpr (
		mapping, std::unique_ptr<HIR::Expr> (receiver), missing,
		std::move (outer_attribs),
		struct_expr.struct_base->base_struct->get_locus ());

	      implicit_field = new HIR::StructExprFieldIdentifierValue (
		mapping, missing, std::unique_ptr<HIR::Expr> (field_value),
		struct_expr.struct_base->base_struct->get_locus ());

	      size_t field_index;
	      bool ok = struct_path_resolved->get_field (missing, &field_index);
	      rust_assert (ok);

	      adtFieldIndexToField[field_index] = implicit_field;
	      struct_expr.get_fields ().push_back (
		std::unique_ptr<HIR::StructExprField> (implicit_field));
	    }
	}
    }

  if (struct_def->is_union ())
    {
      // There is exactly one field in this constructor, we need to
      // figure out the field index to make sure we initialize the
      // right union field.
      for (size_t i = 0; i < adtFieldIndexToField.size (); i++)
	{
	  if (adtFieldIndexToField[i])
	    {
	      struct_expr.union_index = i;
	      break;
	    }
	}
      rust_assert (struct_expr.union_index != -1);
    }
  else
    {
      // everything is ok, now we need to ensure all field values are ordered
      // correctly. The GIMPLE backend uses a simple algorithm that assumes each
      // assigned field in the constructor is in the same order as the field in
      // the type
      for (auto &field : struct_expr.get_fields ())
	field.release ();

      std::vector<std::unique_ptr<HIR::StructExprField> > ordered_fields;
      for (size_t i = 0; i < adtFieldIndexToField.size (); i++)
	{
	  ordered_fields.push_back (
	    std::unique_ptr<HIR::StructExprField> (adtFieldIndexToField[i]));
	}
      struct_expr.set_fields_as_owner (std::move (ordered_fields));
    }

  resolved = struct_def;
}

void
TypeCheckStructExpr::visit (HIR::StructExprFieldIdentifierValue &field)
{
  auto it = fields_assigned.find (field.field_name);
  if (it != fields_assigned.end ())
    {
      rust_fatal_error (field.get_locus (), "used more than once");
      return;
    }

  size_t field_index;
  TyTy::StructFieldType *field_type
    = struct_path_resolved->get_field (field.field_name, &field_index);
  if (field_type == nullptr)
    {
      rust_error_at (field.get_locus (), "unknown field");
      return;
    }

  TyTy::BaseType *value = TypeCheckExpr::Resolve (field.get_value (), false);
  resolved_field_value_expr = field_type->get_field_type ()->unify (value);
  if (resolved_field_value_expr != nullptr)
    {
      fields_assigned.insert (field.field_name);
      adtFieldIndexToField[field_index] = &field;
    }
}

void
TypeCheckStructExpr::visit (HIR::StructExprFieldIndexValue &field)
{
  std::string field_name (std::to_string (field.get_tuple_index ()));
  auto it = fields_assigned.find (field_name);
  if (it != fields_assigned.end ())
    {
      rust_fatal_error (field.get_locus (), "used more than once");
      return;
    }

  size_t field_index;

  TyTy::StructFieldType *field_type
    = struct_path_resolved->get_field (field_name, &field_index);
  if (field_type == nullptr)
    {
      rust_error_at (field.get_locus (), "unknown field");
      return;
    }

  TyTy::BaseType *value = TypeCheckExpr::Resolve (field.get_value (), false);
  resolved_field_value_expr = field_type->get_field_type ()->unify (value);
  if (resolved_field_value_expr != nullptr)
    {
      fields_assigned.insert (field_name);
      adtFieldIndexToField[field_index] = &field;
    }
}

void
TypeCheckStructExpr::visit (HIR::StructExprFieldIdentifier &field)
{
  auto it = fields_assigned.find (field.get_field_name ());
  if (it != fields_assigned.end ())
    {
      rust_fatal_error (field.get_locus (), "used more than once");
      return;
    }

  size_t field_index;
  TyTy::StructFieldType *field_type
    = struct_path_resolved->get_field (field.get_field_name (), &field_index);
  if (field_type == nullptr)
    {
      rust_error_at (field.get_locus (), "unknown field");
      return;
    }

  // we can make the field look like an identifier expr to take advantage of
  // existing code to figure out the type
  HIR::IdentifierExpr expr (field.get_mappings (), field.get_field_name (),
			    field.get_locus ());
  TyTy::BaseType *value = TypeCheckExpr::Resolve (&expr, false);

  resolved_field_value_expr = field_type->get_field_type ()->unify (value);
  if (resolved_field_value_expr != nullptr)

    {
      fields_assigned.insert (field.field_name);
      adtFieldIndexToField[field_index] = &field;
    }
}

// rust-hir-type-check-type.h

void
TypeCheckType::visit (HIR::ArrayType &type)
{
  auto capacity_type = TypeCheckExpr::Resolve (type.get_size_expr (), false);
  if (capacity_type->get_kind () == TyTy::TypeKind::ERROR)
    return;

  TyTy::USizeType *expected_ty
    = new TyTy::USizeType (type.get_size_expr ()->get_mappings ().get_hirid ());
  context->insert_type (type.get_size_expr ()->get_mappings (), expected_ty);

  auto unified = expected_ty->unify (capacity_type);
  if (unified->get_kind () == TyTy::TypeKind::ERROR)
    return;

  auto capacity = ConstFold::ConstFoldExpr::fold (type.get_size_expr ());
  if (capacity == nullptr)
    return;

  TyTy::BaseType *base = TypeCheckType::Resolve (type.get_element_type ());
  translated = new TyTy::ArrayType (type.get_mappings ().get_hirid (), capacity,
				    TyTy::TyVar (base->get_ref ()));
}

// rust-hir-trait-ref.h

TraitItemReference::TraitItemReference (
  std::string identifier, bool optional, TraitItemType type,
  HIR::TraitItem *hir_trait_item, TyTy::BaseType *self,
  std::vector<TyTy::SubstitutionParamMapping> substitutions, Location locus)
  : identifier (identifier), optional_flag (optional), type (type),
    hir_trait_item (hir_trait_item),
    inherited_substitutions (std::move (substitutions)), locus (locus),
    self (self), context (TypeCheckContext::get ())
{}

TraitItemReference::TraitItemReference (TraitItemReference const &other)
  : identifier (other.identifier), optional_flag (other.optional_flag),
    type (other.type), hir_trait_item (other.hir_trait_item),
    locus (other.locus), self (other.self), context (TypeCheckContext::get ())
{
  inherited_substitutions.clear ();
  inherited_substitutions.reserve (other.inherited_substitutions.size ());
  for (size_t i = 0; i < other.inherited_substitutions.size (); i++)
    inherited_substitutions.push_back (
      other.inherited_substitutions.at (i).clone ());
}

TraitItemReference &
TraitItemReference::operator= (TraitItemReference const &other)
{
  identifier = other.identifier;
  optional_flag = other.optional_flag;
  type = other.type;
  hir_trait_item = other.hir_trait_item;
  self = other.self;
  locus = other.locus;
  context = other.context;

  inherited_substitutions.clear ();
  inherited_substitutions.reserve (other.inherited_substitutions.size ());
  for (size_t i = 0; i < other.inherited_substitutions.size (); i++)
    inherited_substitutions.push_back (
      other.inherited_substitutions.at (i).clone ());

  return *this;
}

TyTy::BaseType *
TraitItemReference::get_type_from_typealias (/*const*/
					     HIR::TraitItemType &type) const
{
  TyTy::TyVar var (get_mappings ().get_hirid ());
  return var.get_tyty ();
}

TyTy::BaseType *
TraitItemReference::get_type_from_constant (
  /*const*/ HIR::TraitItemConst &constant) const
{
  TyTy::BaseType *type = TypeCheckType::Resolve (constant.get_type ().get ());
  if (constant.has_expr ())
    {
      TyTy::BaseType *expr
	= TypeCheckExpr::Resolve (constant.get_expr ().get (), false);

      return type->unify (expr);
    }
  return type;
}

TyTy::BaseType *
TraitItemReference::get_type_from_fn (/*const*/ HIR::TraitItemFunc &fn) const
{
  std::vector<TyTy::SubstitutionParamMapping> substitutions
    = inherited_substitutions;

  HIR::TraitFunctionDecl &function = fn.get_decl ();
  if (function.has_generics ())
    {
      for (auto &generic_param : function.get_generic_params ())
	{
	  switch (generic_param.get ()->get_kind ())
	    {
	    case HIR::GenericParam::GenericKind::LIFETIME:
	      // Skipping Lifetime completely until better handling.
	      break;

	      case HIR::GenericParam::GenericKind::TYPE: {
		auto param_type
		  = TypeResolveGenericParam::Resolve (generic_param.get ());
		context->insert_type (generic_param->get_mappings (),
				      param_type);

		substitutions.push_back (TyTy::SubstitutionParamMapping (
		  static_cast<HIR::TypeParam &> (*generic_param), param_type));
	      }
	      break;
	    }
	}
    }

  TyTy::BaseType *ret_type = nullptr;
  if (!function.has_return_type ())
    ret_type = new TyTy::TupleType (fn.get_mappings ().get_hirid ());
  else
    {
      auto resolved
	= TypeCheckType::Resolve (function.get_return_type ().get ());
      if (resolved->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (fn.get_locus (), "failed to resolve return type");
	  return get_error ();
	}

      ret_type = resolved->clone ();
      ret_type->set_ref (
	function.get_return_type ()->get_mappings ().get_hirid ());
    }

  std::vector<std::pair<HIR::Pattern *, TyTy::BaseType *> > params;
  if (function.is_method ())
    {
      // add the synthetic self param at the front, this is a placeholder
      // for compilation to know parameter names. The types are ignored
      // but we reuse the HIR identifier pattern which requires it
      HIR::SelfParam &self_param = function.get_self ();
      HIR::IdentifierPattern *self_pattern
	= new HIR::IdentifierPattern ("self", self_param.get_locus (),
				      self_param.is_ref (),
				      self_param.is_mut (),
				      std::unique_ptr<HIR::Pattern> (nullptr));
      // might have a specified type
      TyTy::BaseType *self_type = nullptr;
      if (self_param.has_type ())
	{
	  std::unique_ptr<HIR::Type> &specified_type = self_param.get_type ();
	  self_type = TypeCheckType::Resolve (specified_type.get ());
	}
      else
	{
	  switch (self_param.get_self_kind ())
	    {
	    case HIR::SelfParam::IMM:
	    case HIR::SelfParam::MUT:
	      self_type = self->clone ();
	      break;

	    case HIR::SelfParam::IMM_REF:
	      self_type = new TyTy::ReferenceType (
		self_param.get_mappings ().get_hirid (),
		TyTy::TyVar (self->get_ref ()), false);
	      break;

	    case HIR::SelfParam::MUT_REF:
	      self_type = new TyTy::ReferenceType (
		self_param.get_mappings ().get_hirid (),
		TyTy::TyVar (self->get_ref ()), true);
	      break;

	    default:
	      gcc_unreachable ();
	      return nullptr;
	    }
	}

      context->insert_type (self_param.get_mappings (), self_type);
      params.push_back (
	std::pair<HIR::Pattern *, TyTy::BaseType *> (self_pattern, self_type));
    }

  for (auto &param : function.get_function_params ())
    {
      // get the name as well required for later on
      auto param_tyty = TypeCheckType::Resolve (param.get_type ());
      params.push_back (
	std::pair<HIR::Pattern *, TyTy::BaseType *> (param.get_param_name (),
						     param_tyty));

      context->insert_type (param.get_mappings (), param_tyty);
    }

  auto resolved
    = new TyTy::FnType (fn.get_mappings ().get_hirid (),
			fn.get_mappings ().get_defid (),
			function.get_function_name (),
			function.is_method () ? FNTYPE_IS_METHOD_FLAG
					      : FNTYPE_DEFAULT_FLAGS,
			ABI::RUST, std::move (params), ret_type, substitutions);

  context->insert_type (fn.get_mappings (), resolved);
  return resolved;
}

} // namespace Resolver
} // namespace Rust
