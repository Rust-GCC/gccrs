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

#include "rust-session-manager.h"
#include "ordered-hash-map.h"
#include "feature/rust-feature.h"
#include "rust-feature-gate.h"
#include "rust-item.h"

namespace Rust {

FeatureGateVisitor::FeatureGateVisitor (
  const std::vector<const Feature *> &feature_lst)
  : AST::ASTVisitor (), feature_lst (&feature_lst)
{}

void
FeatureGateVisitor::visit (AST::Token &)
{}
void
FeatureGateVisitor::visit (AST::DelimTokenTree &)
{}
void
FeatureGateVisitor::visit (AST::AttrInputMetaItemContainer &)
{}
void
FeatureGateVisitor::visit (AST::IdentifierExpr &ident_expr)
{}
void
FeatureGateVisitor::visit (AST::Lifetime &)
{}
void
FeatureGateVisitor::visit (AST::LifetimeParam &)
{}
void
FeatureGateVisitor::visit (AST::ConstGenericParam &)
{}
void
FeatureGateVisitor::visit (AST::MacroInvocation &macro_invoc)
{}
void
FeatureGateVisitor::visit (AST::PathInExpression &path)
{}
void
FeatureGateVisitor::visit (AST::TypePathSegment &)
{}
void
FeatureGateVisitor::visit (AST::TypePathSegmentGeneric &segment)
{}
void
FeatureGateVisitor::visit (AST::TypePathSegmentFunction &segment)
{}
void
FeatureGateVisitor::visit (AST::TypePath &path)
{}
void
FeatureGateVisitor::visit (AST::QualifiedPathInExpression &path)
{}
void
FeatureGateVisitor::visit (AST::QualifiedPathInType &path)
{}

void
FeatureGateVisitor::visit (AST::LiteralExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::AttrInputLiteral &)
{}
void
FeatureGateVisitor::visit (AST::MetaItemLitExpr &)
{}
void
FeatureGateVisitor::visit (AST::MetaItemPathLit &)
{}
void
FeatureGateVisitor::visit (AST::BorrowExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::DereferenceExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ErrorPropagationExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::NegationExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ArithmeticOrLogicalExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ComparisonExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::LazyBooleanExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::TypeCastExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::AssignmentExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::CompoundAssignmentExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::GroupedExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ArrayElemsValues &elems)
{}
void
FeatureGateVisitor::visit (AST::ArrayElemsCopied &elems)
{}
void
FeatureGateVisitor::visit (AST::ArrayExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ArrayIndexExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::TupleExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::TupleIndexExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::StructExprStruct &expr)
{}
void
FeatureGateVisitor::visit (AST::StructExprFieldIdentifier &)
{}
void
FeatureGateVisitor::visit (AST::StructExprFieldIdentifierValue &field)
{}

void
FeatureGateVisitor::visit (AST::StructExprFieldIndexValue &field)
{}
void
FeatureGateVisitor::visit (AST::StructExprStructFields &expr)
{}
void
FeatureGateVisitor::visit (AST::StructExprStructBase &expr)
{}
void
FeatureGateVisitor::visit (AST::CallExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::MethodCallExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::FieldAccessExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ClosureExprInner &expr)
{}

void
FeatureGateVisitor::visit (AST::BlockExpr &expr)
{}

void
FeatureGateVisitor::visit (AST::ClosureExprInnerTyped &expr)
{}
void
FeatureGateVisitor::visit (AST::ContinueExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::BreakExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::RangeFromToExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::RangeFromExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::RangeToExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::RangeFullExpr &)
{}
void
FeatureGateVisitor::visit (AST::RangeFromToInclExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::RangeToInclExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ReturnExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::UnsafeBlockExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::LoopExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::WhileLoopExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::WhileLetLoopExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::ForLoopExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::IfExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::IfExprConseqElse &expr)
{}
void
FeatureGateVisitor::visit (AST::IfExprConseqIf &expr)
{}
void
FeatureGateVisitor::visit (AST::IfExprConseqIfLet &expr)
{}
void
FeatureGateVisitor::visit (AST::IfLetExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::IfLetExprConseqElse &expr)
{}
void
FeatureGateVisitor::visit (AST::IfLetExprConseqIf &expr)
{}
void
FeatureGateVisitor::visit (AST::IfLetExprConseqIfLet &expr)
{}
void
FeatureGateVisitor::visit (AST::MatchExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::AwaitExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::AsyncBlockExpr &expr)
{}
void
FeatureGateVisitor::visit (AST::TypeParam &param)
{
  for (const auto &bp : param.get_type_param_bounds ())
    {
      bp->accept_vis (*this);
    }
}
void
FeatureGateVisitor::visit (AST::LifetimeWhereClauseItem &)
{}
void
FeatureGateVisitor::visit (AST::TypeBoundWhereClauseItem &item)
{}
void
FeatureGateVisitor::visit (AST::Method &method)
{}
void
FeatureGateVisitor::visit (AST::Module &module)
{}
void
FeatureGateVisitor::visit (AST::ExternCrate &crate)
{}
void
FeatureGateVisitor::visit (AST::UseTreeGlob &)
{}
void
FeatureGateVisitor::visit (AST::UseTreeList &)
{}
void
FeatureGateVisitor::visit (AST::UseTreeRebind &)
{}
void
FeatureGateVisitor::visit (AST::UseDeclaration &use_decl)
{}

void
FeatureGateVisitor::visit (AST::Function &function)
{
  for (const auto &gp : function.get_generic_params ())
    {
      gp->accept_vis (*this);
    }
}
void
FeatureGateVisitor::visit (AST::TypeAlias &type_alias)
{}
void
FeatureGateVisitor::visit (AST::StructStruct &struct_item)
{}
void
FeatureGateVisitor::visit (AST::TupleStruct &tuple_struct)
{}
void
FeatureGateVisitor::visit (AST::EnumItem &item)
{}
void
FeatureGateVisitor::visit (AST::EnumItemTuple &item)
{}
void
FeatureGateVisitor::visit (AST::EnumItemStruct &item)
{}
void
FeatureGateVisitor::visit (AST::EnumItemDiscriminant &item)
{}
void
FeatureGateVisitor::visit (AST::Enum &enum_item)
{}
void
FeatureGateVisitor::visit (AST::Union &union_item)
{}
void
FeatureGateVisitor::visit (AST::ConstantItem &const_item)
{}
void
FeatureGateVisitor::visit (AST::StaticItem &static_item)
{}
void
FeatureGateVisitor::visit (AST::TraitItemFunc &item)
{}
void
FeatureGateVisitor::visit (AST::TraitItemMethod &item)
{}
void
FeatureGateVisitor::visit (AST::TraitItemConst &item)
{}
void
FeatureGateVisitor::visit (AST::TraitItemType &item)
{}
void
FeatureGateVisitor::visit (AST::Trait &trait)
{}
void
FeatureGateVisitor::visit (AST::InherentImpl &impl)
{}
void
FeatureGateVisitor::visit (AST::TraitImpl &impl)
{}
void
FeatureGateVisitor::visit (AST::ExternalStaticItem &item)
{}
void
FeatureGateVisitor::visit (AST::ExternalFunctionItem &item)
{}
void
FeatureGateVisitor::visit (AST::ExternBlock &block)
{}
void
FeatureGateVisitor::visit (AST::MacroMatchFragment &)
{}
void
FeatureGateVisitor::visit (AST::MacroMatchRepetition &)
{}
void
FeatureGateVisitor::visit (AST::MacroMatcher &)
{}
void
FeatureGateVisitor::visit (AST::MacroRulesDefinition &rules_def)
{}
void
FeatureGateVisitor::visit (AST::MetaItemPath &)
{}
void
FeatureGateVisitor::visit (AST::MetaItemSeq &)
{}
void
FeatureGateVisitor::visit (AST::MetaWord &)
{}
void
FeatureGateVisitor::visit (AST::MetaNameValueStr &)
{}
void
FeatureGateVisitor::visit (AST::MetaListPaths &)
{}
void
FeatureGateVisitor::visit (AST::MetaListNameValueStr &)
{}
void
FeatureGateVisitor::visit (AST::LiteralPattern &)
{}
void
FeatureGateVisitor::visit (AST::IdentifierPattern &pattern)
{}
void
FeatureGateVisitor::visit (AST::WildcardPattern &)
{}
void
FeatureGateVisitor::visit (AST::RangePatternBoundLiteral &)
{}
void
FeatureGateVisitor::visit (AST::RangePatternBoundPath &bound)
{}
void
FeatureGateVisitor::visit (AST::RangePatternBoundQualPath &bound)
{}
void
FeatureGateVisitor::visit (AST::RangePattern &pattern)
{}
void
FeatureGateVisitor::visit (AST::ReferencePattern &pattern)
{}
void
FeatureGateVisitor::visit (AST::StructPatternFieldTuplePat &field)
{}
void
FeatureGateVisitor::visit (AST::StructPatternFieldIdentPat &field)
{}
void
FeatureGateVisitor::visit (AST::StructPatternFieldIdent &field)
{}
void
FeatureGateVisitor::visit (AST::StructPattern &pattern)
{}
void
FeatureGateVisitor::visit (AST::TupleStructItemsNoRange &tuple_items)
{}
void
FeatureGateVisitor::visit (AST::TupleStructItemsRange &tuple_items)
{}
void
FeatureGateVisitor::visit (AST::TupleStructPattern &pattern)
{}
void
FeatureGateVisitor::visit (AST::TuplePatternItemsMultiple &tuple_items)
{}
void
FeatureGateVisitor::visit (AST::TuplePatternItemsRanged &tuple_items)
{}
void
FeatureGateVisitor::visit (AST::TuplePattern &pattern)
{}
void
FeatureGateVisitor::visit (AST::GroupedPattern &pattern)
{}
void
FeatureGateVisitor::visit (AST::SlicePattern &pattern)
{}

void
FeatureGateVisitor::visit (AST::EmptyStmt &)
{}
void
FeatureGateVisitor::visit (AST::LetStmt &stmt)
{}
void
FeatureGateVisitor::visit (AST::ExprStmtWithoutBlock &stmt)
{}
void
FeatureGateVisitor::visit (AST::ExprStmtWithBlock &stmt)
{}

void
FeatureGateVisitor::visit (AST::TraitBound &bound)
{
  bound.get_type_path ().accept_vis (*this);
}
void
FeatureGateVisitor::visit (AST::ImplTraitType &type)
{}
void
FeatureGateVisitor::visit (AST::TraitObjectType &type)
{}
void
FeatureGateVisitor::visit (AST::ParenthesisedType &type)
{}
void
FeatureGateVisitor::visit (AST::ImplTraitTypeOneBound &type)
{}
void
FeatureGateVisitor::visit (AST::TraitObjectTypeOneBound &type)
{}
void
FeatureGateVisitor::visit (AST::TupleType &type)
{}
void
FeatureGateVisitor::visit (AST::NeverType &)
{}
void
FeatureGateVisitor::visit (AST::RawPointerType &type)
{}
void
FeatureGateVisitor::visit (AST::ReferenceType &type)
{}
void
FeatureGateVisitor::visit (AST::ArrayType &type)
{}
void
FeatureGateVisitor::visit (AST::SliceType &type)
{}
void
FeatureGateVisitor::visit (AST::InferredType &)
{}
void
FeatureGateVisitor::visit (AST::BareFunctionType &type)
{}

} // namespace Rust