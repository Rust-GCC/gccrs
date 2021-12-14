// Copyright (C) 2020-2021 Free Software Foundation, Inc.

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

#include "rust-compile.h"
#include "rust-compile-item.h"
#include "rust-compile-expr.h"
#include "rust-compile-struct-field-expr.h"
#include "rust-hir-trait-resolve.h"
#include "rust-hir-path-probe.h"
#include "rust-hir-type-bounds.h"
#include "rust-hir-dot-operator.h"

namespace Rust {
namespace Compile {

CompileCrate::CompileCrate (HIR::Crate &crate, Context *ctx)
  : crate (crate), ctx (ctx)
{}

CompileCrate::~CompileCrate () {}

void
CompileCrate::Compile (HIR::Crate &crate, Context *ctx)

{
  CompileCrate c (crate, ctx);
  c.go ();
}

void
CompileCrate::go ()
{
  for (auto &item : crate.items)
    CompileItem::compile (item.get (), ctx, false);

  for (auto &item : crate.items)
    CompileItem::compile (item.get (), ctx, true);
}

// rust-compile-block.h

void
CompileBlock::visit (HIR::BlockExpr &expr)
{
  fncontext fnctx = ctx->peek_fn ();
  tree fndecl = fnctx.fndecl;
  Location start_location = expr.get_locus ();
  Location end_location = expr.get_closing_locus ();
  auto body_mappings = expr.get_mappings ();

  Resolver::Rib *rib = nullptr;
  if (!ctx->get_resolver ()->find_name_rib (body_mappings.get_nodeid (), &rib))
    {
      rust_fatal_error (expr.get_locus (), "failed to setup locals per block");
      return;
    }

  std::vector<Bvariable *> locals;
  bool ok = compile_locals_for_block (*rib, fndecl, locals);
  rust_assert (ok);

  tree enclosing_scope = ctx->peek_enclosing_scope ();
  tree new_block = ctx->get_backend ()->block (fndecl, enclosing_scope, locals,
					       start_location, end_location);
  ctx->push_block (new_block);

  for (auto &s : expr.get_statements ())
    {
      auto compiled_expr = CompileStmt::Compile (s.get (), ctx);
      if (compiled_expr != nullptr)
	{
	  tree compiled_stmt
	    = ctx->get_backend ()->expression_statement (fnctx.fndecl,
							 compiled_expr);
	  ctx->add_statement (compiled_stmt);
	}
    }

  if (expr.has_expr ())
    {
      // the previous passes will ensure this is a valid return or
      // a valid trailing expression
      tree compiled_expr = CompileExpr::Compile (expr.expr.get (), ctx);
      if (compiled_expr != nullptr)
	{
	  if (result == nullptr)
	    {
	      tree final_stmt
		= ctx->get_backend ()->expression_statement (fnctx.fndecl,
							     compiled_expr);
	      ctx->add_statement (final_stmt);
	    }
	  else
	    {
	      tree result_reference = ctx->get_backend ()->var_expression (
		result, expr.get_final_expr ()->get_locus ());

	      tree assignment
		= ctx->get_backend ()->assignment_statement (fnctx.fndecl,
							     result_reference,
							     compiled_expr,
							     expr.get_locus ());
	      ctx->add_statement (assignment);
	    }
	}
    }

  ctx->pop_block ();
  translated = new_block;
}

void
CompileConditionalBlocks::visit (HIR::IfExpr &expr)
{
  fncontext fnctx = ctx->peek_fn ();
  tree fndecl = fnctx.fndecl;
  tree condition_expr = CompileExpr::Compile (expr.get_if_condition (), ctx);
  tree then_block = CompileBlock::compile (expr.get_if_block (), ctx, result);

  translated
    = ctx->get_backend ()->if_statement (fndecl, condition_expr, then_block,
					 NULL, expr.get_locus ());
}

void
CompileConditionalBlocks::visit (HIR::IfExprConseqElse &expr)
{
  fncontext fnctx = ctx->peek_fn ();
  tree fndecl = fnctx.fndecl;
  tree condition_expr = CompileExpr::Compile (expr.get_if_condition (), ctx);
  tree then_block = CompileBlock::compile (expr.get_if_block (), ctx, result);
  tree else_block = CompileBlock::compile (expr.get_else_block (), ctx, result);

  translated
    = ctx->get_backend ()->if_statement (fndecl, condition_expr, then_block,
					 else_block, expr.get_locus ());
}

void
CompileConditionalBlocks::visit (HIR::IfExprConseqIf &expr)
{
  fncontext fnctx = ctx->peek_fn ();
  tree fndecl = fnctx.fndecl;
  tree condition_expr = CompileExpr::Compile (expr.get_if_condition (), ctx);
  tree then_block = CompileBlock::compile (expr.get_if_block (), ctx, result);

  // else block
  std::vector<Bvariable *> locals;
  Location start_location = expr.get_conseq_if_expr ()->get_locus ();
  Location end_location = expr.get_conseq_if_expr ()->get_locus (); // FIXME
  tree enclosing_scope = ctx->peek_enclosing_scope ();
  tree else_block = ctx->get_backend ()->block (fndecl, enclosing_scope, locals,
						start_location, end_location);
  ctx->push_block (else_block);

  tree else_stmt_decl
    = CompileConditionalBlocks::compile (expr.get_conseq_if_expr (), ctx,
					 result);
  ctx->add_statement (else_stmt_decl);

  ctx->pop_block ();

  translated
    = ctx->get_backend ()->if_statement (fndecl, condition_expr, then_block,
					 else_block, expr.get_locus ());
}

// rust-compile-struct-field-expr.h

void
CompileStructExprField::visit (HIR::StructExprFieldIdentifierValue &field)
{
  translated = CompileExpr::Compile (field.get_value (), ctx);
}

void
CompileStructExprField::visit (HIR::StructExprFieldIndexValue &field)
{
  translated = CompileExpr::Compile (field.get_value (), ctx);
}

void
CompileStructExprField::visit (HIR::StructExprFieldIdentifier &field)
{
  // we can make the field look like an identifier expr to take advantage of
  // existing code
  HIR::IdentifierExpr expr (field.get_mappings (), field.get_field_name (),
			    field.get_locus ());
  translated = CompileExpr::Compile (&expr, ctx);
}

// Shared methods in compilation

void
HIRCompileBase::compile_function_body (
  tree fndecl, std::unique_ptr<HIR::BlockExpr> &function_body,
  bool has_return_type)
{
  for (auto &s : function_body->get_statements ())
    {
      auto compiled_expr = CompileStmt::Compile (s.get (), ctx);
      if (compiled_expr != nullptr)
	{
	  tree compiled_stmt
	    = ctx->get_backend ()->expression_statement (fndecl, compiled_expr);
	  ctx->add_statement (compiled_stmt);
	}
    }

  if (function_body->has_expr ())
    {
      // the previous passes will ensure this is a valid return
      // or a valid trailing expression
      tree compiled_expr
	= CompileExpr::Compile (function_body->expr.get (), ctx);

      if (compiled_expr != nullptr)
	{
	  if (has_return_type)
	    {
	      std::vector<tree> retstmts;
	      retstmts.push_back (compiled_expr);

	      auto ret = ctx->get_backend ()->return_statement (
		fndecl, retstmts,
		function_body->get_final_expr ()->get_locus ());
	      ctx->add_statement (ret);
	    }
	  else
	    {
	      tree final_stmt
		= ctx->get_backend ()->expression_statement (fndecl,
							     compiled_expr);
	      ctx->add_statement (final_stmt);
	    }
	}
    }
}

bool
HIRCompileBase::compile_locals_for_block (Resolver::Rib &rib, tree fndecl,
					  std::vector<Bvariable *> &locals)
{
  rib.iterate_decls ([&] (NodeId n, Location) mutable -> bool {
    Resolver::Definition d;
    bool ok = ctx->get_resolver ()->lookup_definition (n, &d);
    rust_assert (ok);

    HIR::Stmt *decl = nullptr;
    ok = ctx->get_mappings ()->resolve_nodeid_to_stmt (d.parent, &decl);
    rust_assert (ok);

    // if its a function we extract this out side of this fn context
    // and it is not a local to this function
    bool is_item = ctx->get_mappings ()->lookup_hir_item (
		     decl->get_mappings ().get_crate_num (),
		     decl->get_mappings ().get_hirid ())
		   != nullptr;
    if (is_item)
      {
	HIR::Item *item = static_cast<HIR::Item *> (decl);
	CompileItem::compile (item, ctx, true);
	return true;
      }

    Bvariable *compiled = CompileVarDecl::compile (fndecl, decl, ctx);
    locals.push_back (compiled);

    return true;
  });

  return true;
}

tree
HIRCompileBase::coercion_site (tree compiled_ref, TyTy::BaseType *actual,
			       TyTy::BaseType *expected, Location locus)
{
  auto root_actual_kind = actual->get_root ()->get_kind ();
  auto root_expected_kind = expected->get_root ()->get_kind ();

  if (root_expected_kind == TyTy::TypeKind::DYNAMIC
      && root_actual_kind != TyTy::TypeKind::DYNAMIC)
    {
      const TyTy::DynamicObjectType *dyn
	= static_cast<const TyTy::DynamicObjectType *> (expected->get_root ());
      return coerce_to_dyn_object (compiled_ref, actual, expected, dyn, locus);
    }

  return compiled_ref;
}

tree
HIRCompileBase::coerce_to_dyn_object (tree compiled_ref,
				      const TyTy::BaseType *actual,
				      const TyTy::BaseType *expected,
				      const TyTy::DynamicObjectType *ty,
				      Location locus)
{
  tree dynamic_object = TyTyResolveCompile::compile (ctx, ty);

  //' this assumes ordering and current the structure is
  // __trait_object_ptr
  // [list of function ptrs]

  auto root = actual->get_root ();
  std::vector<std::pair<Resolver::TraitReference *, HIR::ImplBlock *>>
    probed_bounds_for_receiver = Resolver::TypeBoundsProbe::Probe (root);

  std::vector<tree> vals;
  vals.push_back (compiled_ref);
  for (auto &bound : ty->get_object_items ())
    {
      const Resolver::TraitItemReference *item = bound.first;
      const TyTy::TypeBoundPredicate *predicate = bound.second;

      auto address = compute_address_for_trait_item (item, predicate,
						     probed_bounds_for_receiver,
						     actual, root, locus);
      vals.push_back (address);
    }

  tree constructed_trait_object
    = ctx->get_backend ()->constructor_expression (dynamic_object, vals, -1,
						   locus);

  fncontext fnctx = ctx->peek_fn ();
  tree enclosing_scope = ctx->peek_enclosing_scope ();
  bool is_address_taken = false;
  tree ret_var_stmt = NULL_TREE;

  Bvariable *dyn_tmp = ctx->get_backend ()->temporary_variable (
    fnctx.fndecl, enclosing_scope, dynamic_object, constructed_trait_object,
    is_address_taken, locus, &ret_var_stmt);
  ctx->add_statement (ret_var_stmt);

  // FIXME this needs to be more generic to apply any covariance

  auto e = expected;
  std::vector<Resolver::Adjustment> adjustments;
  while (e->get_kind () == TyTy::TypeKind::REF)
    {
      auto r = static_cast<const TyTy::ReferenceType *> (e);
      e = r->get_base ();

      if (r->is_mutable ())
	adjustments.push_back (
	  Resolver::Adjustment (Resolver::Adjustment::AdjustmentType::MUT_REF,
				e));
      else
	adjustments.push_back (
	  Resolver::Adjustment (Resolver::Adjustment::AdjustmentType::IMM_REF,
				e));
    }

  auto resulting_dyn_object_ref
    = ctx->get_backend ()->var_expression (dyn_tmp, locus);
  for (auto it = adjustments.rbegin (); it != adjustments.rend (); it++)
    {
      bool ok
	= it->get_type () == Resolver::Adjustment::AdjustmentType::IMM_REF
	  || it->get_type () == Resolver::Adjustment::AdjustmentType::MUT_REF;
      rust_assert (ok);

      resulting_dyn_object_ref
	= ctx->get_backend ()->address_expression (resulting_dyn_object_ref,
						   locus);
    }
  return resulting_dyn_object_ref;
}

tree
HIRCompileBase::compute_address_for_trait_item (
  const Resolver::TraitItemReference *ref,
  const TyTy::TypeBoundPredicate *predicate,
  std::vector<std::pair<Resolver::TraitReference *, HIR::ImplBlock *>>
    &receiver_bounds,
  const TyTy::BaseType *receiver, const TyTy::BaseType *root, Location locus)
{
  // There are two cases here one where its an item which has an implementation
  // within a trait-impl-block. Then there is the case where there is a default
  // implementation for this within the trait.
  //
  // The awkward part here is that this might be a generic trait and we need to
  // figure out the correct monomorphized type for this so we can resolve the
  // address of the function , this is stored as part of the
  // type-bound-predicate
  //
  // Algo:
  // check if there is an impl-item for this trait-item-ref first
  // else assert that the trait-item-ref has an implementation

  TyTy::TypeBoundPredicateItem predicate_item
    = predicate->lookup_associated_item (ref->get_identifier ());
  rust_assert (!predicate_item.is_error ());

  // this is the expected end type
  TyTy::BaseType *trait_item_type = predicate_item.get_tyty_for_receiver (root);
  rust_assert (trait_item_type->get_kind () == TyTy::TypeKind::FNDEF);
  TyTy::FnType *trait_item_fntype
    = static_cast<TyTy::FnType *> (trait_item_type);

  // find impl-block for this trait-item-ref
  HIR::ImplBlock *associated_impl_block = nullptr;
  const Resolver::TraitReference *predicate_trait_ref = predicate->get ();
  for (auto &item : receiver_bounds)
    {
      Resolver::TraitReference *trait_ref = item.first;
      HIR::ImplBlock *impl_block = item.second;
      if (predicate_trait_ref->is_equal (*trait_ref))
	{
	  associated_impl_block = impl_block;
	  break;
	}
    }

  // FIXME this probably should just return error_mark_node but this helps
  // debug for now since we are wrongly returning early on type-resolution
  // failures, until we take advantage of more error types and error_mark_node
  rust_assert (associated_impl_block != nullptr);

  // lookup self for the associated impl
  std::unique_ptr<HIR::Type> &self_type_path
    = associated_impl_block->get_type ();
  TyTy::BaseType *self = nullptr;
  bool ok = ctx->get_tyctx ()->lookup_type (
    self_type_path->get_mappings ().get_hirid (), &self);
  rust_assert (ok);

  // lookup the predicate item from the self
  TyTy::TypeBoundPredicate *self_bound = nullptr;
  for (auto &bound : self->get_specified_bounds ())
    {
      const Resolver::TraitReference *bound_ref = bound.get ();
      const Resolver::TraitReference *specified_ref = predicate->get ();
      if (bound_ref->is_equal (*specified_ref))
	{
	  self_bound = &bound;
	  break;
	}
    }
  rust_assert (self_bound != nullptr);

  // lookup the associated item from the associated impl block
  TyTy::TypeBoundPredicateItem associated_self_item
    = self_bound->lookup_associated_item (ref->get_identifier ());
  rust_assert (!associated_self_item.is_error ());

  // apply any generic arguments from this predicate
  TyTy::BaseType *mono1 = associated_self_item.get_tyty_for_receiver (self);
  TyTy::BaseType *mono2 = nullptr;
  if (predicate->has_generic_args ())
    {
      mono2 = associated_self_item.get_tyty_for_receiver (
	self, predicate->get_generic_args ());
    }
  else
    {
      mono2 = associated_self_item.get_tyty_for_receiver (self);
    }
  rust_assert (mono1 != nullptr);
  rust_assert (mono1->get_kind () == TyTy::TypeKind::FNDEF);
  TyTy::FnType *assocated_item_ty1 = static_cast<TyTy::FnType *> (mono1);

  rust_assert (mono2 != nullptr);
  rust_assert (mono2->get_kind () == TyTy::TypeKind::FNDEF);
  TyTy::FnType *assocated_item_ty2 = static_cast<TyTy::FnType *> (mono2);

  // Lookup the impl-block for the associated impl_item if it exists
  HIR::Function *associated_function = nullptr;
  for (auto &impl_item : associated_impl_block->get_impl_items ())
    {
      bool is_function = impl_item->get_impl_item_type ()
			 == HIR::ImplItem::ImplItemType::FUNCTION;
      if (!is_function)
	continue;

      HIR::Function *fn = static_cast<HIR::Function *> (impl_item.get ());
      bool found_associated_item
	= fn->get_function_name ().compare (ref->get_identifier ()) == 0;
      if (found_associated_item)
	associated_function = fn;
    }

  // we found an impl_item for this
  if (associated_function != nullptr)
    {
      // lookup the associated type for this item
      TyTy::BaseType *lookup = nullptr;
      bool ok = ctx->get_tyctx ()->lookup_type (
	associated_function->get_mappings ().get_hirid (), &lookup);
      rust_assert (ok);
      rust_assert (lookup->get_kind () == TyTy::TypeKind::FNDEF);
      TyTy::FnType *lookup_fntype = static_cast<TyTy::FnType *> (lookup);

      if (lookup_fntype->needs_substitution ())
	{
	  TyTy::SubstitutionArgumentMappings mappings
	    = assocated_item_ty1->solve_missing_mappings_from_this (
	      *assocated_item_ty2, *lookup_fntype);
	  lookup_fntype = lookup_fntype->handle_substitions (mappings);
	}

      return CompileInherentImplItem::Compile (root, associated_function, ctx,
					       true, lookup_fntype, true,
					       locus);
    }

  // we can only compile trait-items with a body
  bool trait_item_has_definition = ref->is_optional ();
  rust_assert (trait_item_has_definition);

  HIR::TraitItem *trait_item = ref->get_hir_trait_item ();
  return CompileTraitItem::Compile (root, trait_item, ctx, trait_item_fntype,
				    true, locus);
}

} // namespace Compile
} // namespace Rust
