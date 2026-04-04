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

#include "rust-compile-drop.h"
#include "fold-const.h"
#include "optional.h"
#include "rust-backend.h"
#include "rust-compile-context.h"
#include "rust-compile-implitem.h"
#include "rust-gcc.h"
#include "rust-hir-item.h"
#include "rust-hir-trait-reference.h"
#include "rust-hir.h"
#include "rust-location.h"
#include "rust-mapping-common.h"
#include "rust-system.h"
#include "rust-tyty-util.h"
#include "rust-tyty.h"
#include "tree-core.h"
#include "tree.h"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
namespace Rust {
namespace Compile {

DropContext::DropContext (Context *compile_ctx)
  : compile_ctx (compile_ctx), tyctx (compile_ctx->get_tyctx ()),
    mappings (compile_ctx->get_mappings ())
{}

bool
DropContext::has_drop_impl (const TyTy::BaseType *ty, HirId *mapping)
{
  // TODO : handle manual drop
  auto opt_Drop = mappings.lookup_lang_item_node (LangItem::Kind::DROP);
  if (!opt_Drop.has_value ())
    return false;
  NodeId dropNodeId = opt_Drop.value ();
  auto opt_drop = mappings.lookup_node_to_hir (dropNodeId);
  if (!opt_drop.has_value ())
    return false;
  HirId dropHirId = opt_drop.value ();
  TyTy::BaseType *orig = nullptr;
  if (!tyctx->lookup_type (ty->get_ref (), &orig))
    return false;
  orig = orig->destructure ();
  std::vector<std::pair<TyTy::BaseType *, HirId>> impl_map;
  if (!tyctx->peek_associated_impl_mapping_for_self (dropHirId, &impl_map))
    return false;
  if (orig->get_kind () != TyTy::TypeKind::ADT)
    return false;
  auto orig_adt = orig->as<TyTy::ADTType> ();
  for (auto &item : impl_map)
    {
      TyTy::BaseType *impl_ty = item.first->destructure ();
      auto kind = impl_ty->get_kind ();
      assert (kind == TyTy::TypeKind::ADT && "we apply drop to ADT types only");
      if (orig_adt->get_id () == impl_ty->as<TyTy::ADTType> ()->get_id ())
	{
	  *mapping = item.second;
	  return true;
	}
    }
  return false;
}

bool
DropContext::has_drop_impl (const TyTy::BaseType *ty)
{
  HirId mapping = UNKNOWN_HIRID;
  return has_drop_impl (ty, &mapping);
}

bool
DropContext::needs_drop (const TyTy::BaseType *tyty)
{
  TyTy::BaseType const *ty = tyty->destructure ();
  if (has_drop_impl (ty))
    return true;
  switch (ty->get_kind ())
    {
    case TyTy::ADT:
      {
	auto adt_type = ty->as<const TyTy::ADTType> ();
	for (auto variant : adt_type->get_variants ())
	  for (auto field : variant->get_fields ())
	    if (needs_drop (field->get_field_type ()))
	      return true;
	return false;
      }
    case TyTy::TUPLE:
      {
	auto tuple_type = ty->as<const TyTy::TupleType> ();
	for (auto field : tuple_type->get_fields ())
	  if (needs_drop (field.get_tyty ()))
	    return true;
	return false;
      }
    default:
      // TODO : handle other types
      return false;
    }
}

tree
DropContext::build_drop (const TyTy::BaseType *ty)
{
  HirId impl_mapping = UNKNOWN_HIRID;
  bool has_impl_drop = has_drop_impl (ty, &impl_mapping);
  assert (has_impl_drop);
  HIR::ImplBlock *impl_block
    = mappings.lookup_hir_impl_block (impl_mapping).value ();
  assert (impl_block->get_impl_items ().size () == 1);
  HIR::ImplItem *function_item = impl_block->get_impl_items ()[0].get ();
  tree compiled_drop
    = CompileInherentImplItem::Compile (function_item, compile_ctx);
  assert_node (compiled_drop, "something went wrong when compiling drop");
  return compiled_drop;
}

DropPlace *
DropContext::make_drop_place (tree var_expr, DropPath var_path,
			      const TyTy::BaseType *ty)
{
  DropPlace *result = nullptr;
  switch (ty->get_kind ())
    {
    case TyTy::TypeKind::ADT:
      if (ty->as<const TyTy::ADTType> ()->is_union ())
	result = make_drop_place_union (var_expr, var_path,
					ty->as<const TyTy::ADTType> ());
      else
	result = make_drop_place_struct (var_expr, var_path,
					 ty->as<const TyTy::ADTType> ());
      break;
    case TyTy::TypeKind::TUPLE:
      result = make_drop_place_tuple (var_expr, var_path,
				      ty->as<const TyTy::TupleType> ());
      break;
    default:
      todo_die ("please handle this");
      return nullptr;
    }
  return result;
}

DropPlace *
DropContext::make_drop_place_struct (tree var_expr, DropPath var_path,
				     const TyTy::ADTType *var_ty)
{
  std::vector<DropPlace *> children;
  if (!has_drop_impl (var_ty))
    {
      assert (var_ty->number_of_variants () == 1);
      TyTy::VariantDef *variant = var_ty->get_variants ()[0];
      size_t field_index = 0;
      for (auto field : variant->get_fields ())
	{
	  TyTy::BaseType *field_ty = field->get_field_type ()->destructure ();
	  if (needs_drop (field_ty))
	    {
	      tree field_expr
		= Backend::struct_field_expression (var_expr, field_index,
						    UNDEF_LOCATION);
	      DropPath field_path
		= var_path.child (field_index, field->get_name ());
	      DropPlace *child
		= make_drop_place (field_expr, field_path, field_ty);
	      assert (child != nullptr && "child is nullptr");
	      children.push_back (child);
	    }
	  field_index++;
	}
    }
  return finish_make_drop_place (var_path, var_expr, NULL_TREE, var_ty,
				 std::move (children));
}

DropPlace *
DropContext::make_drop_place_union (tree var_expr, DropPath var_path,
				    const TyTy::ADTType *var_ty)
{
  if (!has_drop_impl (var_ty))
    return nullptr;
  return finish_make_drop_place (var_path, var_expr, NULL_TREE, var_ty);
}

DropPlace *
DropContext::make_drop_place_tuple (tree var_expr, DropPath var_path,
				    const TyTy::TupleType *var_ty)
{
  std::vector<DropPlace *> children;
  size_t field_index = 0;
  for (auto field : var_ty->get_fields ())
    {
      TyTy::BaseType *field_ty = field.get_tyty ()->destructure ();
      if (needs_drop (field_ty))
	{
	  tree field_expr
	    = Backend::struct_field_expression (var_expr, field_index,
						UNDEF_LOCATION);
	  DropPath field_path
	    = var_path.child (field_index, std::to_string (field_index));
	  DropPlace *child = make_drop_place (field_expr, field_path, field_ty);
	  assert (child != nullptr && "child is not valid");
	  children.push_back (child);
	}
      field_index++;
    }
  return finish_make_drop_place (var_path, var_expr, NULL_TREE, var_ty,
				 std::move (children));
}

void
DropContext::setup_drop_flag (DropPlace *drop_place)
{
  if (drop_place->is_leaf ())
    {
      tree fndecl = compile_ctx->peek_fn ().fndecl;
      tree bind_tree = compile_ctx->peek_enclosing_scope ();
      std::string flag_name = drop_place->path.flag_name ();
      tree decl = build_decl (UNDEF_LOCATION, VAR_DECL,
			      Backend::get_identifier_node (flag_name),
			      boolean_type_node);
      DECL_CONTEXT (decl) = fndecl;
      tree block_tree = BIND_EXPR_BLOCK (bind_tree);
      DECL_CHAIN (decl) = BLOCK_VARS (block_tree);
      BLOCK_VARS (block_tree) = decl;
      BIND_EXPR_VARS (bind_tree) = BLOCK_VARS (block_tree);
      rust_preserve_from_gc (decl);
      Bvariable *flag_var = new Bvariable (decl);
      tree flag_expr = Backend::var_expression (flag_var, UNDEF_LOCATION);
      tree init_stmt
	= Backend::assignment_statement (flag_expr, boolean_false_node,
					 UNDEF_LOCATION);
      compile_ctx->add_statement (init_stmt);
      drop_place->drop_flag = flag_expr;
    }
}

tree
DropContext::init (DropPlace *place)
{
  assert (place != nullptr && "don't call init when place is nullptr");
  std::vector<tree> stmts;
  mark_drop_flags (place, boolean_true_node, stmts);
  return Backend::statement_list (stmts);
}
tree
DropContext::uninit (DropPlace *place)
{
  assert (place != nullptr && "don't call uninit when place is nullptr");
  std::vector<tree> stmts;
  mark_drop_flags (place, boolean_false_node, stmts);
  return Backend::statement_list (stmts);
}

tree
DropContext::drop (DropPlace *place)
{
  assert (place != nullptr && "don't call drop when place is nullptr");
  if (place->is_leaf ())
    {
      assert_node (place->type, "place->type in drop");
      tree drop_call = call_drop_in_place (place->ty, place->type);
      assert_node (drop_call, "drop_call in drop");
      if (place->drop_flag != NULL_TREE)
	{
	  tree clear_flag = Backend::assignment_statement (place->drop_flag,
							   boolean_false_node,
							   UNDEF_LOCATION);
	  tree body = Backend::compound_statement (drop_call, clear_flag);
	  drop_call = Backend::if_statement (NULL_TREE, place->drop_flag, body,
					     NULL_TREE, UNDEF_LOCATION);
	}
      return drop_call;
    }
  std::vector<tree> stmts;
  for (auto child : place->children)
    {
      tree child_drop = drop (child);
      assert (child_drop != NULL_TREE);
      stmts.push_back (child_drop);
    }
  assert (!stmts.empty ());
  return Backend::statement_list (stmts);
}

void
DropContext::dump_all_drop_places ()
{
  for (auto &pair : drop_places)
    pair.second->dump ();
}

DropContext *
DropContext::get (Context *compile_ctx)
{
  static DropContext *instance;
  if (instance == nullptr)
    instance = new DropContext (compile_ctx);
  return instance;
}

DropPlace *
DropContext::lookup_drop_place (DropPath path)
{
  auto it = drop_places.find (path);
  if (it != drop_places.end ())
    return it->second.get ();
  return nullptr;
}

void
DropContext::mark_drop_flags (DropPlace *place, tree val,
			      std::vector<tree> &stmts)
{
  if (place->is_leaf ())
    {
      assert_node (place->drop_flag, "drop_flag must be set for leaf place");
      tree drop_flag_expr
	= Backend::assignment_statement (place->drop_flag, val, UNDEF_LOCATION);
      stmts.push_back (drop_flag_expr);
    }
  else
    {
      for (auto &child : place->children)
	mark_drop_flags (child, val, stmts);
    }
};

tree
DropContext::build_drop_in_place (const TyTy::BaseType *ty, tree self)
{
  tree res = NULL_TREE;
  switch (ty->get_kind ())
    {
    case TyTy::TypeKind::ADT:
      if (ty->as<const TyTy::ADTType> ()->is_union ())
	res = build_drop_in_place_union (ty->as<const TyTy::ADTType> (), self);
      else
	res = build_drop_in_place_struct (ty->as<const TyTy::ADTType> (), self);
      break;
    case TyTy::TypeKind::TUPLE:
      res = build_drop_in_place_tuple (ty->as<const TyTy::TupleType> (), self);
      break;
    default:
      todo_die ("check build_drop_in_place and return NULL_TREE explicitly");
    }
  return res;
}

tree
DropContext::build_drop_in_place_struct (const TyTy::ADTType *ty, tree self)
{
  std::vector<tree> stmts;
  if (has_drop_impl (ty))
    {
      tree drop_call = call_drop (ty, self);
      assert_node (drop_call, "drop_call");
      stmts.push_back (drop_call);
    }
  rust_assert (ty->number_of_variants () == 1);
  TyTy::VariantDef *variant = ty->get_variants ()[0];
  for (size_t i = 0; i < variant->num_fields (); ++i)
    {
      TyTy::StructFieldType *field = variant->get_field_at_index (i);
      TyTy::BaseType *field_ty = field->get_field_type ()->destructure ();
      if (needs_drop (field_ty))
	{
	  tree field_expr
	    = Backend::struct_field_expression (self, i, UNDEF_LOCATION);
	  assert_node (field_expr, "field_expr");
	  tree field_drop = call_drop_in_place (field_ty, field_expr);
	  stmts.push_back (field_drop);
	}
    }
  return Backend::statement_list (stmts);
}

tree
DropContext::build_drop_in_place_union (const TyTy::ADTType *ty, tree self)
{
  if (!has_drop_impl (ty))
    return NULL_TREE;
  tree drop_call = call_drop (ty, self);
  assert_node (drop_call, "drop_call");
  return drop_call;
}

tree
DropContext::build_drop_in_place_tuple (const TyTy::TupleType *ty, tree self)
{
  std::vector<tree> stmts;
  for (size_t i = 0; i < ty->num_fields (); ++i)
    {
      TyTy::BaseType *field_ty = ty->get_field (i)->destructure ();
      if (needs_drop (field_ty))
	{
	  tree field_expr
	    = Backend::struct_field_expression (self, i, UNDEF_LOCATION);
	  tree field_drop = call_drop_in_place (field_ty, field_expr);
	  assert_node (field_drop, "field_drop");
	  stmts.push_back (field_drop);
	}
    }
  return Backend::statement_list (stmts);
}
tree
DropContext::call_drop (const TyTy::BaseType *ty, tree self)
{
  assert (has_drop_impl (ty) && "ty does not implement Drop");
  tree drop_fn = build_drop (ty);
  assert_node (drop_fn, "drop_fn");
  tree self_ref = build_fold_addr_expr_loc (UNDEF_LOCATION, self);
  return Backend::call_expression (drop_fn, {self_ref}, nullptr,
				   UNDEF_LOCATION);
}
tree
DropContext::call_drop_in_place (const TyTy::BaseType *ty, tree self)
{
  assert (needs_drop (ty->destructure ()) && "ty does not need drop");
  return build_drop_in_place (ty->destructure (), self);
}
} // namespace Compile
} // namespace Rust
