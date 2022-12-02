// Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#include "rust-unsafe-checker.h"
#include "rust-hir.h"
#include "rust-hir-expr.h"
#include "rust-hir-stmt.h"
#include "rust-hir-item.h"

namespace Rust {
namespace HIR {

UnsafeChecker::UnsafeChecker ()
  : context (*Resolver::TypeCheckContext::get ()),
    resolver (*Resolver::Resolver::get ()),
    mappings (*Analysis::Mappings::get ())
{}

void
UnsafeChecker::go (HIR::Crate &crate)
{
  for (auto &item : crate.items)
    item->accept_vis (*this);
}

static void
check_static_mut (HIR::Item *maybe_static, Location locus)
{
  if (maybe_static->get_hir_kind () == Node::BaseKind::VIS_ITEM)
    {
      auto item = static_cast<Item *> (maybe_static);
      if (item->get_item_kind () == Item::ItemKind::Static)
	{
	  auto static_item = static_cast<StaticItem *> (item);
	  if (static_item->is_mut ())
	    rust_error_at (
	      locus, "use of mutable static requires unsafe function or block");
	}
    }
}

static void
check_extern_static (HIR::ExternalItem *maybe_static, Location locus)
{
  if (maybe_static->get_extern_kind () == ExternalItem::ExternKind::Static)
    rust_error_at (locus,
		   "use of extern static requires unsafe function or block");
}

void
UnsafeChecker::check_use_of_static (HirId node_id, Location locus)
{
  if (unsafe_context.is_in_context ())
    return;

  auto maybe_static_mut = mappings.lookup_hir_item (node_id);

  HirId extern_block;
  auto maybe_extern_static
    = mappings.lookup_hir_extern_item (node_id, &extern_block);

  if (maybe_static_mut)
    check_static_mut (maybe_static_mut, locus);

  if (maybe_extern_static)
    check_extern_static (static_cast<ExternalItem *> (maybe_extern_static),
			 locus);
}

static void
check_unsafe_call (HIR::Function *fn, Location locus, const std::string &kind)
{
  if (fn->get_qualifiers ().is_unsafe ())
    rust_error_at (locus, "call to unsafe %s requires unsafe function or block",
		   kind.c_str ());
}

static bool
is_safe_intrinsic (const std::string &fn_name)
{
  static const std::unordered_set<std::string> safe_intrinsics = {
    "abort",
    "size_of",
    "min_align_of",
    "needs_drop",
    "caller_location",
    "add_with_overflow",
    "sub_with_overflow",
    "mul_with_overflow",
    "wrapping_add",
    "wrapping_sub",
    "wrapping_mul",
    "saturating_add",
    "saturating_sub",
    "rotate_left",
    "rotate_right",
    "ctpop",
    "ctlz",
    "cttz",
    "bswap",
    "bitreverse",
    "discriminant_value",
    "type_id",
    "likely",
    "unlikely",
    "ptr_guaranteed_eq",
    "ptr_guaranteed_ne",
    "minnumf32",
    "minnumf64",
    "maxnumf32",
    "rustc_peek",
    "maxnumf64",
    "type_name",
    "forget",
    "black_box",
    "variant_count",
  };

  return safe_intrinsics.find (fn_name) != safe_intrinsics.end ();
}

static void
check_extern_call (HIR::ExternalItem *maybe_fn, HIR::ExternBlock *parent_block,
		   Location locus)
{
  // We have multiple operations to perform here
  //     1. Is the item an actual function we're calling
  //     2. Is the block it's defined in an FFI block or an `extern crate` block
  //
  // It is not unsafe to call into other crates, so items defined in an `extern
  // crate` must be callable without being in an unsafe context. On the other
  // hand, any function defined in a block with a specific ABI (even `extern
  // "Rust"` blocks) is unsafe to call

  if (maybe_fn->get_extern_kind () != ExternalItem::ExternKind::Function)
    return;

  // Some intrinsics are safe to call
  if (parent_block->get_abi () == Rust::ABI::INTRINSIC
      && is_safe_intrinsic (maybe_fn->get_item_name ()))
    return;

  rust_error_at (locus,
		 "call to extern function requires unsafe function or block");
}

void
UnsafeChecker::check_function_call (HirId node_id, Location locus)
{
  if (unsafe_context.is_in_context ())
    return;

  HirId parent_extern_block;
  auto maybe_fn = mappings.lookup_hir_item (node_id);
  auto maybe_extern
    = mappings.lookup_hir_extern_item (node_id, &parent_extern_block);

  if (maybe_fn && maybe_fn->get_item_kind () == Item::ItemKind::Function)
    check_unsafe_call (static_cast<Function *> (maybe_fn), locus, "function");

  if (maybe_extern)
    check_extern_call (static_cast<ExternalItem *> (maybe_extern),
		       mappings.lookup_hir_extern_block (parent_extern_block),
		       locus);
}

void
UnsafeChecker::visit (Lifetime &lifetime)
{}

void
UnsafeChecker::visit (LifetimeParam &lifetime_param)
{}

void
UnsafeChecker::visit (PathInExpression &path)
{
  NodeId ast_node_id = path.get_mappings ().get_nodeid ();
  NodeId ref_node_id;
  HirId definition_id;

  if (!resolver.lookup_resolved_name (ast_node_id, &ref_node_id))
    return;

  rust_assert (mappings.lookup_node_to_hir (ref_node_id, &definition_id));

  check_use_of_static (definition_id, path.get_locus ());
}

void
UnsafeChecker::visit (TypePathSegment &segment)
{}

void
UnsafeChecker::visit (TypePathSegmentGeneric &segment)
{}

void
UnsafeChecker::visit (TypePathSegmentFunction &segment)
{}

void
UnsafeChecker::visit (TypePath &path)
{}

void
UnsafeChecker::visit (QualifiedPathInExpression &path)
{}

void
UnsafeChecker::visit (QualifiedPathInType &path)
{}

void
UnsafeChecker::visit (LiteralExpr &expr)
{}

void
UnsafeChecker::visit (BorrowExpr &expr)
{
  expr.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (DereferenceExpr &expr)
{
  TyTy::BaseType *to_deref_type;
  auto to_deref = expr.get_expr ()->get_mappings ().get_hirid ();

  rust_assert (context.lookup_type (to_deref, &to_deref_type));

  if (to_deref_type->get_kind () == TyTy::TypeKind::POINTER
      && !unsafe_context.is_in_context ())
    rust_error_at (expr.get_locus (), "dereference of raw pointer requires "
				      "unsafe function or block");
}

void
UnsafeChecker::visit (ErrorPropagationExpr &expr)
{
  expr.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (NegationExpr &expr)
{
  expr.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ArithmeticOrLogicalExpr &expr)
{
  expr.get_lhs ()->accept_vis (*this);
  expr.get_rhs ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ComparisonExpr &expr)
{
  expr.get_lhs ()->accept_vis (*this);
  expr.get_rhs ()->accept_vis (*this);
}

void
UnsafeChecker::visit (LazyBooleanExpr &expr)
{
  expr.get_lhs ()->accept_vis (*this);
  expr.get_rhs ()->accept_vis (*this);
}

void
UnsafeChecker::visit (TypeCastExpr &expr)
{
  expr.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (AssignmentExpr &expr)
{
  expr.get_lhs ()->accept_vis (*this);
  expr.get_rhs ()->accept_vis (*this);
}

void
UnsafeChecker::visit (CompoundAssignmentExpr &expr)
{
  expr.get_left_expr ()->accept_vis (*this);
  expr.get_right_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (GroupedExpr &expr)
{
  expr.get_expr_in_parens ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ArrayElemsValues &elems)
{
  for (auto &elem : elems.get_values ())
    elem->accept_vis (*this);
}

void
UnsafeChecker::visit (ArrayElemsCopied &elems)
{
  elems.get_elem_to_copy ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ArrayExpr &expr)
{
  expr.get_internal_elements ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ArrayIndexExpr &expr)
{
  expr.get_array_expr ()->accept_vis (*this);
  expr.get_index_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (TupleExpr &expr)
{
  for (auto &elem : expr.get_tuple_elems ())
    elem->accept_vis (*this);
}

void
UnsafeChecker::visit (TupleIndexExpr &expr)
{
  expr.get_tuple_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (StructExprStruct &expr)
{}

void
UnsafeChecker::visit (StructExprFieldIdentifier &field)
{}

void
UnsafeChecker::visit (StructExprFieldIdentifierValue &field)
{
  field.get_value ()->accept_vis (*this);
}

void
UnsafeChecker::visit (StructExprFieldIndexValue &field)
{
  field.get_value ()->accept_vis (*this);
}

void
UnsafeChecker::visit (StructExprStructFields &expr)
{
  for (auto &field : expr.get_fields ())
    field->accept_vis (*this);
}

void
UnsafeChecker::visit (StructExprStructBase &expr)
{}

void
UnsafeChecker::visit (CallExpr &expr)
{
  auto fn = expr.get_fnexpr ();
  if (!fn)
    return;

  NodeId ast_node_id = fn->get_mappings ().get_nodeid ();
  NodeId ref_node_id;
  HirId definition_id;

  // There are no unsafe types, and functions are defined in the name resolver.
  // If we can't find the name, then we're dealing with a type and should return
  // early.
  if (!resolver.lookup_resolved_name (ast_node_id, &ref_node_id))
    return;

  rust_assert (mappings.lookup_node_to_hir (ref_node_id, &definition_id));

  // At this point we have the function's HIR Id. There are two checks we
  // must perform:
  //     1. The function is an unsafe one
  //     2. The function is an extern one
  check_function_call (definition_id, expr.get_locus ());

  if (expr.has_params ())
    for (auto &arg : expr.get_arguments ())
      arg->accept_vis (*this);
}

void
UnsafeChecker::visit (MethodCallExpr &expr)
{
  TyTy::BaseType *method_type;
  context.lookup_type (expr.get_method_name ().get_mappings ().get_hirid (),
		       &method_type);

  auto fn = *static_cast<TyTy::FnType *> (method_type);
  auto method = mappings.lookup_hir_implitem (fn.get_ref (), nullptr);

  if (!unsafe_context.is_in_context () && method)
    check_unsafe_call (static_cast<Function *> (method), expr.get_locus (),
		       "method");

  expr.get_receiver ()->accept_vis (*this);

  for (auto &arg : expr.get_arguments ())
    arg->accept_vis (*this);
}

void
UnsafeChecker::visit (FieldAccessExpr &expr)
{
  expr.get_receiver_expr ()->accept_vis (*this);

  if (unsafe_context.is_in_context ())
    return;

  TyTy::BaseType *receiver_ty;
  auto ok = context.lookup_type (
    expr.get_receiver_expr ()->get_mappings ().get_hirid (), &receiver_ty);
  rust_assert (ok);

  if (receiver_ty->get_kind () == TyTy::TypeKind::ADT)
    {
      auto maybe_union = static_cast<TyTy::ADTType *> (receiver_ty);
      if (maybe_union->is_union ())
	rust_error_at (
	  expr.get_locus (),
	  "access to union field requires unsafe function or block");
    }
}

void
UnsafeChecker::visit (ClosureExprInner &expr)
{}

void
UnsafeChecker::visit (BlockExpr &expr)
{
  for (auto &stmt : expr.get_statements ())
    stmt->accept_vis (*this);

  if (expr.has_expr ())
    expr.get_final_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ClosureExprInnerTyped &expr)
{}

void
UnsafeChecker::visit (ContinueExpr &expr)
{}

void
UnsafeChecker::visit (BreakExpr &expr)
{
  if (expr.has_break_expr ())
    expr.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (RangeFromToExpr &expr)
{
  expr.get_from_expr ()->accept_vis (*this);
  expr.get_to_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (RangeFromExpr &expr)
{
  expr.get_from_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (RangeToExpr &expr)
{
  expr.get_to_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (RangeFullExpr &expr)
{}

void
UnsafeChecker::visit (RangeFromToInclExpr &expr)
{
  expr.get_from_expr ()->accept_vis (*this);
  expr.get_to_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (RangeToInclExpr &expr)
{
  expr.get_to_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ReturnExpr &expr)
{
  if (expr.has_return_expr ())
    expr.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (UnsafeBlockExpr &expr)
{
  unsafe_context.enter (expr.get_mappings ().get_hirid ());

  expr.get_block_expr ()->accept_vis (*this);

  unsafe_context.exit ();
}

void
UnsafeChecker::visit (LoopExpr &expr)
{
  expr.get_loop_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (WhileLoopExpr &expr)
{
  expr.get_predicate_expr ()->accept_vis (*this);
  expr.get_loop_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (WhileLetLoopExpr &expr)
{
  expr.get_cond ()->accept_vis (*this);
  expr.get_loop_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ForLoopExpr &expr)
{
  expr.get_iterator_expr ()->accept_vis (*this);
  expr.get_loop_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (IfExpr &expr)
{
  expr.get_if_condition ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (IfExprConseqElse &expr)
{
  expr.get_if_condition ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);
  expr.get_else_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (IfExprConseqIf &expr)
{
  expr.get_if_condition ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);
  expr.get_conseq_if_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (IfExprConseqIfLet &expr)
{
  expr.get_if_condition ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);

  // TODO: Visit conseq if let expression
}

void
UnsafeChecker::visit (IfLetExpr &expr)
{
  expr.get_scrutinee_expr ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (IfLetExprConseqElse &expr)
{
  expr.get_scrutinee_expr ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);

  // TODO: Visit else expression
}

void
UnsafeChecker::visit (IfLetExprConseqIf &expr)
{
  expr.get_scrutinee_expr ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);
}

void
UnsafeChecker::visit (IfLetExprConseqIfLet &expr)
{
  expr.get_scrutinee_expr ()->accept_vis (*this);
  expr.get_if_block ()->accept_vis (*this);

  // TODO: Visit conseq if let expression
}

void
UnsafeChecker::visit (MatchExpr &expr)
{
  expr.get_scrutinee_expr ()->accept_vis (*this);

  for (auto &match_arm : expr.get_match_cases ())
    match_arm.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (AwaitExpr &expr)
{
  // TODO: Visit expression
}

void
UnsafeChecker::visit (AsyncBlockExpr &expr)
{
  // TODO: Visit block expression
}

void
UnsafeChecker::visit (TypeParam &param)
{}

void
UnsafeChecker::visit (ConstGenericParam &param)
{}

void
UnsafeChecker::visit (LifetimeWhereClauseItem &item)
{}

void
UnsafeChecker::visit (TypeBoundWhereClauseItem &item)
{}

void
UnsafeChecker::visit (Module &module)
{
  for (auto &item : module.get_items ())
    item->accept_vis (*this);
}

void
UnsafeChecker::visit (ExternCrate &crate)
{}

void
UnsafeChecker::visit (UseTreeGlob &use_tree)
{}

void
UnsafeChecker::visit (UseTreeList &use_tree)
{}

void
UnsafeChecker::visit (UseTreeRebind &use_tree)
{}

void
UnsafeChecker::visit (UseDeclaration &use_decl)
{}

void
UnsafeChecker::visit (Function &function)
{
  auto is_unsafe_fn = function.get_qualifiers ().is_unsafe ();

  if (is_unsafe_fn)
    unsafe_context.enter (function.get_mappings ().get_hirid ());

  function.get_definition ()->accept_vis (*this);

  if (is_unsafe_fn)
    unsafe_context.exit ();
}

void
UnsafeChecker::visit (TypeAlias &type_alias)
{
  // FIXME: What do we need to do to handle type aliasing? Is it possible to
  // have unsafe types? Type aliases on unsafe functions?
}

void
UnsafeChecker::visit (StructStruct &struct_item)
{}

void
UnsafeChecker::visit (TupleStruct &tuple_struct)
{}

void
UnsafeChecker::visit (EnumItem &item)
{}

void
UnsafeChecker::visit (EnumItemTuple &item)
{}

void
UnsafeChecker::visit (EnumItemStruct &item)
{}

void
UnsafeChecker::visit (EnumItemDiscriminant &item)
{}

void
UnsafeChecker::visit (Enum &enum_item)
{}

void
UnsafeChecker::visit (Union &union_item)
{}

void
UnsafeChecker::visit (ConstantItem &const_item)
{
  const_item.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (StaticItem &static_item)
{
  static_item.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (TraitItemFunc &item)
{
  if (item.has_block_defined ())
    item.get_block_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (TraitItemConst &item)
{
  if (item.has_expr ())
    item.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (TraitItemType &item)
{}

void
UnsafeChecker::visit (Trait &trait)
{
  // FIXME: Handle unsafe traits
  for (auto &item : trait.get_trait_items ())
    item->accept_vis (*this);
}

void
UnsafeChecker::visit (ImplBlock &impl)
{
  // FIXME: Handle unsafe impls
  for (auto &item : impl.get_impl_items ())
    item->accept_vis (*this);
}

void
UnsafeChecker::visit (ExternalStaticItem &item)
{}

void
UnsafeChecker::visit (ExternalFunctionItem &item)
{}

void
UnsafeChecker::visit (ExternBlock &block)
{
  // FIXME: Do we need to do this?
  for (auto &item : block.get_extern_items ())
    item->accept_vis (*this);
}

void
UnsafeChecker::visit (LiteralPattern &pattern)
{}

void
UnsafeChecker::visit (IdentifierPattern &pattern)
{}

void
UnsafeChecker::visit (WildcardPattern &pattern)
{}

void
UnsafeChecker::visit (RangePatternBoundLiteral &bound)
{}

void
UnsafeChecker::visit (RangePatternBoundPath &bound)
{}

void
UnsafeChecker::visit (RangePatternBoundQualPath &bound)
{}

void
UnsafeChecker::visit (RangePattern &pattern)
{}

void
UnsafeChecker::visit (ReferencePattern &pattern)
{}

void
UnsafeChecker::visit (StructPatternFieldTuplePat &field)
{}

void
UnsafeChecker::visit (StructPatternFieldIdentPat &field)
{}

void
UnsafeChecker::visit (StructPatternFieldIdent &field)
{}

void
UnsafeChecker::visit (StructPattern &pattern)
{}

void
UnsafeChecker::visit (TupleStructItemsNoRange &tuple_items)
{}

void
UnsafeChecker::visit (TupleStructItemsRange &tuple_items)
{}

void
UnsafeChecker::visit (TupleStructPattern &pattern)
{}

void
UnsafeChecker::visit (TuplePatternItemsMultiple &tuple_items)
{}

void
UnsafeChecker::visit (TuplePatternItemsRanged &tuple_items)
{}

void
UnsafeChecker::visit (TuplePattern &pattern)
{}

void
UnsafeChecker::visit (GroupedPattern &pattern)
{}

void
UnsafeChecker::visit (SlicePattern &pattern)
{}

void
UnsafeChecker::visit (EmptyStmt &stmt)
{}

void
UnsafeChecker::visit (LetStmt &stmt)
{
  if (stmt.has_init_expr ())
    stmt.get_init_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ExprStmtWithoutBlock &stmt)
{
  stmt.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (ExprStmtWithBlock &stmt)
{
  stmt.get_expr ()->accept_vis (*this);
}

void
UnsafeChecker::visit (TraitBound &bound)
{}

void
UnsafeChecker::visit (ImplTraitType &type)
{}

void
UnsafeChecker::visit (TraitObjectType &type)
{}

void
UnsafeChecker::visit (ParenthesisedType &type)
{}

void
UnsafeChecker::visit (ImplTraitTypeOneBound &type)
{}

void
UnsafeChecker::visit (TupleType &type)
{}

void
UnsafeChecker::visit (NeverType &type)
{}

void
UnsafeChecker::visit (RawPointerType &type)
{}

void
UnsafeChecker::visit (ReferenceType &type)
{}

void
UnsafeChecker::visit (ArrayType &type)
{}

void
UnsafeChecker::visit (SliceType &type)
{}

void
UnsafeChecker::visit (InferredType &type)
{}

void
UnsafeChecker::visit (BareFunctionType &type)
{}

} // namespace HIR
} // namespace Rust
