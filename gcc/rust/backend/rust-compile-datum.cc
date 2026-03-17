// Copyright (C) 2020-2026 Free Software Foundation, Inc.

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

#include "rust-compile-datum.h"
#include "rust-backend.h"
#include "rust-compile-context.h"
#include "rust-compile-drop.h"
#include "rust-compile-type.h"
#include "rust-tyty.h"
namespace Rust {
namespace Compile {

Datum
Datum::to_lvalue (Context *ctx, HirId expr_id)
{
  if (kind == DatumKind::Lvalue)
    return *this;
  assert (is_valid () && "invalid datum");
  DropContext *drop_ctx = DropContext::get (ctx);
  tree compiled_type = TyTyResolveCompile::compile (ctx, ty);
  assert_node (compiled_type, "something wrong with compiled_type");
  tree pstatement = NULL_TREE;
  tree fndecl = ctx->peek_fn ().fndecl;
  tree bind_tree = ctx->peek_enclosing_scope ();
  LocalVariable tmp
    = Backend::temporary_variable (fndecl, bind_tree, compiled_type, val, false,
				   locus, &pstatement);
  assert (pstatement != NULL_TREE);
  ctx->add_statement (pstatement);
  val = Backend::var_expression (tmp, locus);
  if (drop_ctx->needs_drop (ty))
    {
      DropPath tmp_path;
      tmp_path.root = expr_id;
      tmp_path.var_name = "tmp_expr_" + std::to_string (expr_id);
      DropPlace *place = drop_ctx->make_drop_place (val, tmp_path, ty);
      assert (place != nullptr);
      tree drop_expr = drop_ctx->drop (place);
      assert_node (drop_expr, "drop expr is not valid");
      ctx->add_cleanup (drop_expr);
      ctx->add_statement (drop_ctx->init (place));
      drop_place = place;
    }
  kind = DatumKind::Lvalue;
  return *this;
}

Datum
Datum::to_rvalue (Context *ctx)
{
  if (kind == DatumKind::Rvalue)
    return *this;
  assert (is_valid () && "invalid datum");
  DropContext *drop_ctx = DropContext::get (ctx);
  tree compiled_type = TyTyResolveCompile::compile (ctx, ty);
  assert_node (compiled_type, "something wrong with compiled type");
  tree pstatement = NULL_TREE;
  tree fndecl = ctx->peek_fn ().fndecl;
  tree bind_tree = ctx->peek_enclosing_scope ();
  LocalVariable scratch
    = Backend::temporary_variable (fndecl, bind_tree, compiled_type, NULL_TREE,
				   false, locus, &pstatement);
  assert (pstatement != NULL_TREE);
  ctx->add_statement (pstatement);
  tree scratch_expr = Backend::var_expression (scratch, locus);
  tree copy = Backend::assignment_statement (scratch_expr, val, locus);
  ctx->add_statement (copy);
  if (needs_drop (drop_ctx) && drop_place)
    ctx->add_statement (drop_ctx->uninit (drop_place));
  return Datum::new_rvalue (scratch_expr, ty, locus);
}

void
Datum::store_to (Context *ctx, tree dest_ptr)
{
  tree assignment = Backend::assignment_statement (dest_ptr, val, locus);
  ctx->add_statement (assignment);
  post_store (ctx);
}

void
Datum::post_store (Context *ctx)
{
  if (kind == DatumKind::Lvalue && ty != nullptr && drop_place != nullptr)
    {
      DropContext *drop_ctx = DropContext::get (ctx);
      if (drop_ctx->needs_drop (ty))
	ctx->add_statement (drop_ctx->uninit (drop_place));
    }
}

Datum
Datum::get_field (Context *ctx, size_t field_index, location_t field_locus)
{
  tree field_expr
    = Backend::struct_field_expression (val, field_index, field_locus);
  TyTy::BaseType *field_ty = nullptr;
  assert (is_valid () && "invalid datum in get_field");
  TyTy::BaseType *resolved = ty->destructure ();
  switch (resolved->get_kind ())
    {
    case TyTy::TypeKind::ADT:
      {
	TyTy::ADTType *adt = resolved->as<TyTy::ADTType> ();
	TyTy::VariantDef *variant = adt->get_variants ().at (0);
	field_ty = variant->get_field_at_index (field_index)->get_field_type ();
	break;
      }
    case TyTy::TypeKind::TUPLE:
      {
	TyTy::TupleType *tuple = resolved->as<TyTy::TupleType> ();
	field_ty = tuple->get_field (field_index);
	break;
      }
    default:
      todo_die ("handle all remaining cases explicitly");
      break;
    }
  DropPlace *field_dp = nullptr;
  if (drop_place != nullptr && !drop_place->is_leaf ())
    {
      DropPath child_path = drop_place->path.child (field_index);
      for (auto *child : drop_place->children)
	if (child->path == child_path)
	  field_dp = child;
    }
  return Datum::new_lvalue (field_expr, field_ty, field_dp, field_locus);
}

void
Datum::add_clean_if_rvalue (Context *ctx, HirId expr_id)
{
  if (kind == DatumKind::Lvalue)
    return;
  DropContext *drop_ctx = DropContext::get (ctx);
  if (!needs_drop (drop_ctx))
    return;
  to_lvalue (ctx, expr_id);
}
} // namespace Compile
} // namespace Rust
