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

  auto resolver = Resolver::Resolver::get ();
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
    if (ok)
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

  // scan the ribs to ensure the decls are all setup correctly
  resolver->iterate_name_ribs ([&] (Rib *r) -> bool {
    r->iterate_decls ([&] (NodeId decl_node_id, Location locus) -> bool {
      Definition def;
      if (!resolver->lookup_definition (decl_node_id, &def))
	{
	  rust_error_at (locus, "failed to lookup decl def");
	  return true;
	}

      HirId hir_node = UNKNOWN_HIRID;
      if (!mappings->lookup_node_to_hir (mappings->get_current_crate (),
					 def.parent, &hir_node))
	{
	  rust_error_at (locus, "failed to lookup type hir node id");
	  return true;
	}

      // lookup the ty
      TyTy::BaseType *ty = nullptr;
      bool ok = context->lookup_type (hir_node, &ty);
      if (!ok)
	{
	  rust_error_at (locus, "failed to lookup type for decl node_id: %u",
			 decl_node_id);
	  return true;
	}

      if (!ty->is_concrete ())
	rust_error_at (locus, "unable to determine type");

      return true;
    });
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
	rust_error_at (s->get_locus_slow (), "failure to resolve type");
	return false;
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
	  rust_fatal_error (
	    struct_expr.struct_base->base_struct->get_locus_slow (),
	    "incompatible types for base struct reference");
	  return;
	}
    }

  std::vector<TyTy::StructFieldType *> infered_fields;
  bool ok = true;
  struct_expr.iterate ([&] (HIR::StructExprField *field) mutable -> bool {
    resolved_field_value_expr = nullptr;
    field->accept_vis (*this);
    if (resolved_field_value_expr == nullptr)
      {
	rust_fatal_error (field->get_locus (),
			  "failed to resolve type for field");
	ok = false;
	return false;
      }

    context->insert_type (field->get_mappings (), resolved_field_value_expr);
    return true;
  });

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
      if (!struct_expr.has_struct_base ())
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

	      std::vector<HIR::Attribute> outer_attribs;
	      auto crate_num = mappings->get_current_crate ();
	      Analysis::NodeMapping mapping (
		crate_num,
		struct_expr.struct_base->base_struct->get_mappings ()
		  .get_nodeid (),
		mappings->get_next_hir_id (crate_num), UNKNOWN_LOCAL_DEFID);

	      HIR::Expr *field_value = new HIR::FieldAccessExpr (
		mapping, std::unique_ptr<HIR::Expr> (receiver), missing,
		std::move (outer_attribs),
		struct_expr.struct_base->base_struct->get_locus_slow ());

	      implicit_field = new HIR::StructExprFieldIdentifierValue (
		mapping, missing, std::unique_ptr<HIR::Expr> (field_value),
		struct_expr.struct_base->base_struct->get_locus_slow ());

	      size_t field_index;
	      bool ok = struct_path_resolved->get_field (missing, &field_index);
	      rust_assert (ok);

	      adtFieldIndexToField[field_index] = implicit_field;
	      struct_expr.get_fields ().push_back (
		std::unique_ptr<HIR::StructExprField> (implicit_field));
	    }
	}
    }

  // everything is ok, now we need to ensure all field values are ordered
  // correctly. The GIMPLE backend uses a simple algorithm that assumes each
  // assigned field in the constructor is in the same order as the field in the
  // type

  std::vector<std::unique_ptr<HIR::StructExprField> > expr_fields
    = struct_expr.get_fields_as_owner ();
  for (auto &f : expr_fields)
    f.release ();

  std::vector<std::unique_ptr<HIR::StructExprField> > ordered_fields;
  for (size_t i = 0; i < adtFieldIndexToField.size (); i++)
    {
      ordered_fields.push_back (
	std::unique_ptr<HIR::StructExprField> (adtFieldIndexToField[i]));
    }
  struct_expr.set_fields_as_owner (std::move (ordered_fields));

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

} // namespace Resolver
} // namespace Rust
