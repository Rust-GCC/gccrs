// Copyright (C) 2020-2024 Free Software Foundation, Inc.

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

#include "rust-hir-full-decls.h"
#include "rust-hir-visitor.h"
#include "rust-hir-full.h"
#include <memory>

namespace Rust {
namespace HIR {

void
DefaultHIRVisitor::walk (Lifetime &)
{}

void
DefaultHIRVisitor::walk (LifetimeParam &lifetime_param)
{
  Lifetime lifetime = lifetime_param.get_lifetime ();
  lifetime.accept_vis (*this);
  for (Lifetime &lifetime_bound : lifetime_param.get_lifetime_bounds ())
    lifetime_bound.accept_vis (*this);
}

void
DefaultHIRVisitor::walk (PathInExpression &path_in_expr)
{}

void
DefaultHIRVisitor::walk (TypePathSegment &)
{}

void
DefaultHIRVisitor::walk (TypePathSegmentGeneric &segment_generic)
{}

void
DefaultHIRVisitor::walk (TypePath &type_path)
{
  for (const std::unique_ptr<TypePathSegment> &segment :
       type_path.get_segments ())
    {
      segment->accept_vis (*this);
    }
}

void
DefaultHIRVisitor::walk (QualifiedPathInExpression &path_in_expr)
{}

void
DefaultHIRVisitor::walk (QualifiedPathInType &)
{}

void
DefaultHIRVisitor::walk (LiteralExpr &)
{}

void
DefaultHIRVisitor::walk (BorrowExpr &expr)
{
  expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (DereferenceExpr &expr)
{
  expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ErrorPropagationExpr &expr)
{
  expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (NegationExpr &expr)
{
  expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ArithmeticOrLogicalExpr &expr)
{
  expr.get_lhs ().accept_vis (*this);
  expr.get_rhs ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ComparisonExpr &expr)
{
  expr.get_lhs ().accept_vis (*this);
  expr.get_rhs ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (LazyBooleanExpr &expr)
{
  expr.get_lhs ().accept_vis (*this);
  expr.get_rhs ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TypeCastExpr &expr)
{
  expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (AssignmentExpr &expr)
{
  expr.get_lhs ().accept_vis (*this);
  expr.get_rhs ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (CompoundAssignmentExpr &expr)
{
  expr.get_lhs ().accept_vis (*this);
  expr.get_rhs ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (GroupedExpr &expr)
{
  expr.get_expr_in_parens ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ArrayElemsValues &elems)
{
  for (auto &elem : elems.get_values ())
    elem->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ArrayElemsCopied &elems)
{
  elems.get_elem_to_copy ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ArrayExpr &expr)
{
  expr.get_internal_elements ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ArrayIndexExpr &expr)
{
  expr.get_array_expr ().accept_vis (*this);
  expr.get_index_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TupleExpr &expr)
{
  for (auto &elem : expr.get_tuple_elems ())
    elem->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TupleIndexExpr &expr)
{
  expr.get_tuple_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (StructExprStruct &)
{}

void
DefaultHIRVisitor::walk (StructExprFieldIdentifier &)
{}

void
DefaultHIRVisitor::walk (StructExprFieldIdentifierValue &field)
{
  field.get_value ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (StructExprFieldIndexValue &field)
{
  field.get_value ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (StructExprStructFields &expr)
{
  for (auto &field : expr.get_fields ())
    field->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (StructExprStructBase &)
{}

void
DefaultHIRVisitor::walk (CallExpr &expr)
{}

void
DefaultHIRVisitor::walk (MethodCallExpr &expr)
{
  expr.get_receiver ().accept_vis (*this);

  for (auto &arg : expr.get_arguments ())
    arg->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (FieldAccessExpr &expr)
{
  expr.get_receiver_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ClosureExpr &expr)
{
  expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (BlockExpr &expr)
{
  for (auto &stmt : expr.get_statements ())
    stmt->accept_vis (*this);

  if (expr.has_expr ())
    expr.get_final_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (AnonConst &expr)
{
  expr.get_inner_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ConstBlock &expr)
{
  expr.get_const_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ContinueExpr &)
{}

void
DefaultHIRVisitor::walk (BreakExpr &expr)
{
  if (expr.has_break_expr ())
    expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (RangeFromToExpr &expr)
{
  expr.get_from_expr ().accept_vis (*this);
  expr.get_to_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (RangeFromExpr &expr)
{
  expr.get_from_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (RangeToExpr &expr)
{
  expr.get_to_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (RangeFullExpr &)
{}

void
DefaultHIRVisitor::walk (RangeFromToInclExpr &expr)
{
  expr.get_from_expr ().accept_vis (*this);
  expr.get_to_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (RangeToInclExpr &expr)
{
  expr.get_to_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ReturnExpr &expr)
{
  if (expr.has_return_expr ())
    expr.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (UnsafeBlockExpr &expr)
{
  expr.get_block_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (LoopExpr &expr)
{
  expr.get_loop_block ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (WhileLoopExpr &expr)
{
  expr.get_predicate_expr ().accept_vis (*this);
  expr.get_loop_block ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (WhileLetLoopExpr &expr)
{
  expr.get_cond ().accept_vis (*this);
  expr.get_loop_block ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (IfExpr &expr)
{
  expr.get_if_condition ().accept_vis (*this);
  expr.get_if_block ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (IfExprConseqElse &expr)
{
  expr.get_if_condition ().accept_vis (*this);
  expr.get_if_block ().accept_vis (*this);
  expr.get_else_block ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (MatchExpr &expr)
{
  expr.get_scrutinee_expr ().accept_vis (*this);

  for (auto &match_arm : expr.get_match_cases ())
    match_arm.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (AwaitExpr &)
{
  // TODO: Visit expression
}

void
DefaultHIRVisitor::walk (AsyncBlockExpr &)
{
  // TODO: Visit block expression
}

void
DefaultHIRVisitor::walk (InlineAsm &expr)
{}

void
DefaultHIRVisitor::walk (LlvmInlineAsm &expr)
{}

void
DefaultHIRVisitor::walk (TypeParam &)
{}

void
DefaultHIRVisitor::walk (ConstGenericParam &)
{}

void
DefaultHIRVisitor::walk (LifetimeWhereClauseItem &)
{}

void
DefaultHIRVisitor::walk (TypeBoundWhereClauseItem &)
{}

void
DefaultHIRVisitor::walk (Module &module)
{
  for (auto &item : module.get_items ())
    item->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ExternCrate &)
{}

void
DefaultHIRVisitor::walk (UseTreeGlob &)
{}

void
DefaultHIRVisitor::walk (UseTreeList &)
{}

void
DefaultHIRVisitor::walk (UseTreeRebind &)
{}

void
DefaultHIRVisitor::walk (UseDeclaration &)
{}

void
DefaultHIRVisitor::walk (Function &function)
{
  function.get_definition ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TypeAlias &)
{}

void
DefaultHIRVisitor::walk (StructStruct &)
{}

void
DefaultHIRVisitor::walk (TupleStruct &)
{}

void
DefaultHIRVisitor::walk (EnumItem &)
{}

void
DefaultHIRVisitor::walk (EnumItemTuple &)
{}

void
DefaultHIRVisitor::walk (EnumItemStruct &)
{}

void
DefaultHIRVisitor::walk (EnumItemDiscriminant &)
{}

void
DefaultHIRVisitor::walk (Enum &)
{}

void
DefaultHIRVisitor::walk (Union &)
{}

void
DefaultHIRVisitor::walk (ConstantItem &const_item)
{
  const_item.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (StaticItem &static_item)
{
  static_item.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TraitItemFunc &item)
{
  if (item.has_definition ())
    item.get_block_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TraitItemConst &item)
{
  if (item.has_expr ())
    item.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TraitItemType &)
{}

void
DefaultHIRVisitor::walk (Trait &trait)
{
  for (auto &item : trait.get_trait_items ())
    item->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ImplBlock &impl)
{
  for (auto &item : impl.get_impl_items ())
    item->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ExternalStaticItem &)
{}

void
DefaultHIRVisitor::walk (ExternalFunctionItem &)
{}

void
DefaultHIRVisitor::walk (ExternalTypeItem &)
{}

void
DefaultHIRVisitor::walk (ExternBlock &block)
{
  // FIXME: Do we need to do this?
  for (auto &item : block.get_extern_items ())
    item->accept_vis (*this);
}

void
DefaultHIRVisitor::walk (LiteralPattern &)
{}

void
DefaultHIRVisitor::walk (IdentifierPattern &)
{}

void
DefaultHIRVisitor::walk (WildcardPattern &)
{}

void
DefaultHIRVisitor::walk (RangePatternBoundLiteral &)
{}

void
DefaultHIRVisitor::walk (RangePatternBoundPath &)
{}

void
DefaultHIRVisitor::walk (RangePatternBoundQualPath &)
{}

void
DefaultHIRVisitor::walk (RangePattern &)
{}

void
DefaultHIRVisitor::walk (ReferencePattern &)
{}

void
DefaultHIRVisitor::walk (StructPatternFieldTuplePat &)
{}

void
DefaultHIRVisitor::walk (StructPatternFieldIdentPat &)
{}

void
DefaultHIRVisitor::walk (StructPatternFieldIdent &)
{}

void
DefaultHIRVisitor::walk (StructPattern &)
{}

void
DefaultHIRVisitor::walk (TupleStructItemsNoRange &)
{}

void
DefaultHIRVisitor::walk (TupleStructItemsRange &)
{}

void
DefaultHIRVisitor::walk (TupleStructPattern &)
{}

void
DefaultHIRVisitor::walk (TuplePatternItemsMultiple &)
{}

void
DefaultHIRVisitor::walk (TuplePatternItemsRanged &)
{}

void
DefaultHIRVisitor::walk (TuplePattern &)
{}

void
DefaultHIRVisitor::walk (SlicePattern &)
{}

void
DefaultHIRVisitor::walk (AltPattern &)
{}

void
DefaultHIRVisitor::walk (EmptyStmt &)
{}

void
DefaultHIRVisitor::walk (LetStmt &stmt)
{
  if (stmt.has_init_expr ())
    stmt.get_init_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (ExprStmt &stmt)
{
  stmt.get_expr ().accept_vis (*this);
}

void
DefaultHIRVisitor::walk (TraitBound &)
{}

void
DefaultHIRVisitor::walk (ImplTraitType &)
{}

void
DefaultHIRVisitor::walk (TraitObjectType &)
{}

void
DefaultHIRVisitor::walk (ParenthesisedType &)
{}

void
DefaultHIRVisitor::walk (TupleType &)
{}

void
DefaultHIRVisitor::walk (NeverType &)
{}

void
DefaultHIRVisitor::walk (RawPointerType &)
{}

void
DefaultHIRVisitor::walk (ReferenceType &)
{}

void
DefaultHIRVisitor::walk (ArrayType &)
{}

void
DefaultHIRVisitor::walk (SliceType &)
{}

void
DefaultHIRVisitor::walk (InferredType &)
{}

void
DefaultHIRVisitor::walk (BareFunctionType &)
{}

} // namespace HIR
} // namespace Rust