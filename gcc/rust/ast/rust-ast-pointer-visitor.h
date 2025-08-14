// Copyright (C) 2025 Free Software Foundation, Inc.

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

#ifndef RUST_AST_POINTER_VISITOR_H
#define RUST_AST_POINTER_VISITOR_H

#include "rust-ast-visitor.h"
#include "rust-ast.h"
#include "rust-item.h"

namespace Rust {
namespace AST {

/**
 * Regular AST visitor which may reseat pointers when necessary.
 */
class PointerVisitor : public DefaultASTVisitor
{
public:
  using DefaultASTVisitor::visit;

  virtual void reseat (std::unique_ptr<AST::Expr> &ptr) {}
  virtual void reseat (std::unique_ptr<AST::Stmt> &ptr) {}
  virtual void reseat (std::unique_ptr<AST::Item> &ptr) {}
  virtual void reseat (std::unique_ptr<AST::AssociatedItem> &ptr) {}
  virtual void reseat (std::unique_ptr<AST::ExternalItem> &ptr) {}
  virtual void reseat (std::unique_ptr<AST::Type> &ptr) {}
  virtual void reseat (std::unique_ptr<AST::TypeNoBounds> &ptr) {}
  virtual void reseat (std::unique_ptr<AST::Pattern> &ptr) {}

  virtual void visit (AST::Crate &crate) override;
  virtual void visit (AST::ConstGenericParam &const_param) override;
  virtual void visit (AST::GenericArgsBinding &binding) override;
  virtual void visit (AST::QualifiedPathType &path) override;
  virtual void visit (AST::TypePathFunction &tpf) override;
  virtual void visit (AST::MetaItemPathExpr &meta_item) override;
  virtual void visit (AST::BorrowExpr &expr) override;
  virtual void visit (AST::DereferenceExpr &expr) override;
  virtual void visit (AST::ErrorPropagationExpr &expr) override;
  virtual void visit (AST::NegationExpr &expr) override;
  virtual void visit (AST::ArithmeticOrLogicalExpr &expr) override;
  virtual void visit (AST::ComparisonExpr &expr) override;
  virtual void visit (AST::LazyBooleanExpr &expr) override;
  virtual void visit (AST::TypeCastExpr &expr) override;
  virtual void visit (AST::AssignmentExpr &expr) override;
  virtual void visit (AST::CompoundAssignmentExpr &expr) override;
  virtual void visit (AST::GroupedExpr &expr) override;
  virtual void visit (AST::ArrayElemsValues &elems) override;
  virtual void visit (AST::ArrayElemsCopied &elems) override;
  virtual void visit (AST::ArrayIndexExpr &expr) override;
  virtual void visit (AST::TupleExpr &expr) override;
  virtual void visit (AST::TupleIndexExpr &expr) override;
  virtual void visit (AST::StructExprFieldIdentifierValue &field) override;
  virtual void visit (AST::StructExprFieldIndexValue &field) override;
  virtual void visit (AST::CallExpr &expr) override;
  virtual void visit (AST::MethodCallExpr &expr) override;
  virtual void visit (AST::FieldAccessExpr &expr) override;
  virtual void visit (AST::ClosureExprInner &expr) override;
  virtual void visit (AST::BlockExpr &expr) override;
  virtual void visit (AST::AnonConst &expr) override;
  virtual void visit (AST::ClosureExprInnerTyped &expr) override;
  virtual void visit (AST::BreakExpr &expr) override;
  virtual void visit (AST::RangeFromToExpr &expr) override;
  virtual void visit (AST::RangeFromExpr &expr) override;
  virtual void visit (AST::RangeToExpr &expr) override;
  virtual void visit (AST::RangeFromToInclExpr &expr) override;
  virtual void visit (AST::RangeToInclExpr &expr) override;
  virtual void visit (AST::ReturnExpr &expr) override;
  virtual void visit (AST::BoxExpr &expr) override;
  virtual void visit (AST::WhileLoopExpr &expr) override;
  virtual void visit (AST::WhileLetLoopExpr &expr) override;
  virtual void visit (AST::ForLoopExpr &expr) override;
  virtual void visit (AST::IfExpr &expr) override;
  virtual void visit (AST::IfExprConseqElse &expr) override;
  virtual void visit (AST::IfLetExpr &expr) override;
  virtual void visit (AST::IfLetExprConseqElse &expr) override;
  virtual void visit (AST::MatchExpr &expr) override;
  virtual void visit (AST::MatchArm &expr) override;
  virtual void visit (AST::MatchCase &expr) override;
  virtual void visit (AST::AwaitExpr &expr) override;
  virtual void visit (AST::InlineAsm &expr) override;
  virtual void visit (AST::LlvmInlineAsm &expr) override;
  virtual void visit (AST::TypeParam &param) override;
  virtual void visit (AST::TypeBoundWhereClauseItem &item) override;
  virtual void visit (AST::Module &module) override;
  virtual void visit (AST::Function &function) override;
  virtual void visit (AST::TypeAlias &type_alias) override;
  virtual void visit (AST::StructField &base) override;
  virtual void visit (AST::TupleField &base) override;
  virtual void visit (AST::StructBase &base) override;
  virtual void visit (AST::EnumItemDiscriminant &item) override;
  virtual void visit (AST::ConstantItem &const_item) override;
  virtual void visit (AST::StaticItem &static_item) override;
  virtual void visit (AST::TraitItemConst &item) override;
  virtual void visit (AST::Trait &trait) override;
  virtual void visit (AST::InherentImpl &impl) override;
  virtual void visit (AST::TraitImpl &impl) override;
  virtual void visit (AST::ExternalStaticItem &item) override;
  virtual void visit (AST::ExternBlock &block) override;
  virtual void visit (AST::IdentifierPattern &pattern) override;
  virtual void visit (AST::ReferencePattern &pattern) override;
  virtual void visit (AST::StructPatternFieldTuplePat &field) override;
  virtual void visit (AST::StructPatternFieldIdentPat &field) override;
  virtual void visit (AST::TupleStructItemsNoRange &tuple_items) override;
  virtual void visit (AST::TupleStructItemsRange &tuple_items) override;
  virtual void visit (AST::TuplePatternItemsMultiple &tuple_items) override;
  virtual void visit (AST::TuplePatternItemsRanged &tuple_items) override;
  virtual void visit (AST::GroupedPattern &pattern) override;
  virtual void visit (AST::SlicePatternItemsNoRest &items) override;
  virtual void visit (AST::SlicePatternItemsHasRest &items) override;
  virtual void visit (AST::AltPattern &pattern) override;
  virtual void visit (AST::LetStmt &stmt) override;
  virtual void visit (AST::ExprStmt &stmt) override;
  virtual void visit (AST::ParenthesisedType &type) override;
  virtual void visit (AST::TupleType &type) override;
  virtual void visit (AST::RawPointerType &type) override;
  virtual void visit (AST::ReferenceType &type) override;
  virtual void visit (AST::ArrayType &type) override;
  virtual void visit (AST::SliceType &type) override;
  virtual void visit (AST::BareFunctionType &type) override;
  virtual void visit (AST::SelfParam &self) override;
  virtual void visit (AST::FunctionParam &param) override;
  virtual void visit (AST::ClosureParam &param) override;
  virtual void visit (AST::VariadicParam &param) override;
  virtual void visit (AST::OffsetOf &fmt) override;
  virtual void visit (AST::MaybeNamedParam &param) override;
};

} // namespace AST
} // namespace Rust

#endif
