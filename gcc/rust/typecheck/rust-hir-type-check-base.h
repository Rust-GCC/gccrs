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

#ifndef RUST_HIR_TYPE_CHECK_BASE
#define RUST_HIR_TYPE_CHECK_BASE

#include "rust-diagnostics.h"
#include "rust-hir-type-check.h"
#include "rust-name-resolver.h"
#include "rust-hir-visitor.h"
#include "rust-hir-map.h"

namespace Rust {
namespace Resolver {

// base class to allow derivatives to overload as needed
class TypeCheckBase : public HIR::HIRVisitor
{
public:
  virtual ~TypeCheckBase () {}

  // visitor impl
  // rust-ast.h
  //  virtual void visit(AttrInput& attr_input);
  //  virtual void visit(TokenTree& token_tree);
  //  virtual void visit(MacroMatch& macro_match);
  virtual void visit (HIR::Token &tok) {}
  virtual void visit (HIR::DelimTokenTree &delim_tok_tree) {}
  virtual void visit (HIR::AttrInputMetaItemContainer &input) {}
  //  virtual void visit(MetaItem& meta_item) {}
  //  void vsit(Stmt& stmt) {}
  //  virtual void visit(Expr& expr) {}
  virtual void visit (HIR::IdentifierExpr &ident_expr) {}
  //  virtual void visit(Pattern& pattern) {}
  //  virtual void visit(Type& type) {}
  //  virtual void visit(TypeParamBound& type_param_bound) {}
  virtual void visit (HIR::Lifetime &lifetime) {}
  //  virtual void visit(GenericParam& generic_param) {}
  virtual void visit (HIR::LifetimeParam &lifetime_param) {}
  //  virtual void visit(TraitItem& trait_item) {}
  //  virtual void visit(InherentImplItem& inherent_impl_item) {}
  //  virtual void visit(TraitImplItem& trait_impl_item) {}

  // rust-path.h
  virtual void visit (HIR::PathInExpression &path) {}
  virtual void visit (HIR::TypePathSegment &segment) {}
  virtual void visit (HIR::TypePathSegmentGeneric &segment) {}
  virtual void visit (HIR::TypePathSegmentFunction &segment) {}
  virtual void visit (HIR::TypePath &path) {}
  virtual void visit (HIR::QualifiedPathInExpression &path) {}
  virtual void visit (HIR::QualifiedPathInType &path) {}

  // rust-expr.h
  virtual void visit (HIR::LiteralExpr &expr) {}
  virtual void visit (HIR::AttrInputLiteral &attr_input) {}
  virtual void visit (HIR::MetaItemLitExpr &meta_item) {}
  virtual void visit (HIR::MetaItemPathLit &meta_item) {}
  virtual void visit (HIR::BorrowExpr &expr) {}
  virtual void visit (HIR::DereferenceExpr &expr) {}
  virtual void visit (HIR::ErrorPropagationExpr &expr) {}
  virtual void visit (HIR::NegationExpr &expr) {}
  virtual void visit (HIR::ArithmeticOrLogicalExpr &expr) {}
  virtual void visit (HIR::ComparisonExpr &expr) {}
  virtual void visit (HIR::LazyBooleanExpr &expr) {}
  virtual void visit (HIR::TypeCastExpr &expr) {}
  virtual void visit (HIR::AssignmentExpr &expr) {}
  virtual void visit (HIR::CompoundAssignmentExpr &expr) {}
  virtual void visit (HIR::GroupedExpr &expr) {}
  //  virtual void visit(ArrayElems& elems) {}
  virtual void visit (HIR::ArrayElemsValues &elems) {}
  virtual void visit (HIR::ArrayElemsCopied &elems) {}
  virtual void visit (HIR::ArrayExpr &expr) {}
  virtual void visit (HIR::ArrayIndexExpr &expr) {}
  virtual void visit (HIR::TupleExpr &expr) {}
  virtual void visit (HIR::TupleIndexExpr &expr) {}
  virtual void visit (HIR::StructExprStruct &expr) {}
  //  virtual void visit(StructExprField& field) {}
  virtual void visit (HIR::StructExprFieldIdentifier &field) {}
  virtual void visit (HIR::StructExprFieldIdentifierValue &field) {}
  virtual void visit (HIR::StructExprFieldIndexValue &field) {}
  virtual void visit (HIR::StructExprStructFields &expr) {}
  virtual void visit (HIR::StructExprStructBase &expr) {}
  virtual void visit (HIR::StructExprTuple &expr) {}
  virtual void visit (HIR::StructExprUnit &expr) {}
  //  virtual void visit(EnumExprField& field) {}
  virtual void visit (HIR::EnumExprFieldIdentifier &field) {}
  virtual void visit (HIR::EnumExprFieldIdentifierValue &field) {}
  virtual void visit (HIR::EnumExprFieldIndexValue &field) {}
  virtual void visit (HIR::EnumExprStruct &expr) {}
  virtual void visit (HIR::EnumExprTuple &expr) {}
  virtual void visit (HIR::EnumExprFieldless &expr) {}
  virtual void visit (HIR::CallExpr &expr) {}
  virtual void visit (HIR::MethodCallExpr &expr) {}
  virtual void visit (HIR::FieldAccessExpr &expr) {}
  virtual void visit (HIR::ClosureExprInner &expr) {}
  virtual void visit (HIR::BlockExpr &expr) {}
  virtual void visit (HIR::ClosureExprInnerTyped &expr) {}
  virtual void visit (HIR::ContinueExpr &expr) {}
  virtual void visit (HIR::BreakExpr &expr) {}
  virtual void visit (HIR::RangeFromToExpr &expr) {}
  virtual void visit (HIR::RangeFromExpr &expr) {}
  virtual void visit (HIR::RangeToExpr &expr) {}
  virtual void visit (HIR::RangeFullExpr &expr) {}
  virtual void visit (HIR::RangeFromToInclExpr &expr) {}
  virtual void visit (HIR::RangeToInclExpr &expr) {}
  virtual void visit (HIR::ReturnExpr &expr) {}
  virtual void visit (HIR::UnsafeBlockExpr &expr) {}
  virtual void visit (HIR::LoopExpr &expr) {}
  virtual void visit (HIR::WhileLoopExpr &expr) {}
  virtual void visit (HIR::WhileLetLoopExpr &expr) {}
  virtual void visit (HIR::ForLoopExpr &expr) {}
  virtual void visit (HIR::IfExpr &expr) {}
  virtual void visit (HIR::IfExprConseqElse &expr) {}
  virtual void visit (HIR::IfExprConseqIf &expr) {}
  virtual void visit (HIR::IfExprConseqIfLet &expr) {}
  virtual void visit (HIR::IfLetExpr &expr) {}
  virtual void visit (HIR::IfLetExprConseqElse &expr) {}
  virtual void visit (HIR::IfLetExprConseqIf &expr) {}
  virtual void visit (HIR::IfLetExprConseqIfLet &expr) {}
  //  virtual void visit(MatchCase& match_case) {}
  // virtual void visit (HIR::MatchCaseBlockExpr &match_case) {}
  // virtual void visit (HIR::MatchCaseExpr &match_case) {}
  virtual void visit (HIR::MatchExpr &expr) {}
  virtual void visit (HIR::AwaitExpr &expr) {}
  virtual void visit (HIR::AsyncBlockExpr &expr) {}

  // rust-item.h
  virtual void visit (HIR::TypeParam &param) {}
  //  virtual void visit(WhereClauseItem& item) {}
  virtual void visit (HIR::LifetimeWhereClauseItem &item) {}
  virtual void visit (HIR::TypeBoundWhereClauseItem &item) {}
  virtual void visit (HIR::Method &method) {}
  virtual void visit (HIR::ModuleBodied &module) {}
  virtual void visit (HIR::ModuleNoBody &module) {}
  virtual void visit (HIR::ExternCrate &crate) {}
  //  virtual void visit(UseTree& use_tree) {}
  virtual void visit (HIR::UseTreeGlob &use_tree) {}
  virtual void visit (HIR::UseTreeList &use_tree) {}
  virtual void visit (HIR::UseTreeRebind &use_tree) {}
  virtual void visit (HIR::UseDeclaration &use_decl) {}
  virtual void visit (HIR::Function &function) {}
  virtual void visit (HIR::TypeAlias &type_alias) {}
  virtual void visit (HIR::StructStruct &struct_item) {}
  virtual void visit (HIR::TupleStruct &tuple_struct) {}
  virtual void visit (HIR::EnumItem &item) {}
  virtual void visit (HIR::EnumItemTuple &item) {}
  virtual void visit (HIR::EnumItemStruct &item) {}
  virtual void visit (HIR::EnumItemDiscriminant &item) {}
  virtual void visit (HIR::Enum &enum_item) {}
  virtual void visit (HIR::Union &union_item) {}
  virtual void visit (HIR::ConstantItem &const_item) {}
  virtual void visit (HIR::StaticItem &static_item) {}
  virtual void visit (HIR::TraitItemFunc &item) {}
  virtual void visit (HIR::TraitItemMethod &item) {}
  virtual void visit (HIR::TraitItemConst &item) {}
  virtual void visit (HIR::TraitItemType &item) {}
  virtual void visit (HIR::Trait &trait) {}
  virtual void visit (HIR::InherentImpl &impl) {}
  virtual void visit (HIR::TraitImpl &impl) {}
  //  virtual void visit(ExternalItem& item) {}
  virtual void visit (HIR::ExternalStaticItem &item) {}
  virtual void visit (HIR::ExternalFunctionItem &item) {}
  virtual void visit (HIR::ExternBlock &block) {}

  // rust-macro.h
  virtual void visit (HIR::MacroMatchFragment &match) {}
  virtual void visit (HIR::MacroMatchRepetition &match) {}
  virtual void visit (HIR::MacroMatcher &matcher) {}
  virtual void visit (HIR::MacroRulesDefinition &rules_def) {}
  virtual void visit (HIR::MacroInvocation &macro_invoc) {}
  virtual void visit (HIR::MetaItemPath &meta_item) {}
  virtual void visit (HIR::MetaItemSeq &meta_item) {}
  virtual void visit (HIR::MetaWord &meta_item) {}
  virtual void visit (HIR::MetaNameValueStr &meta_item) {}
  virtual void visit (HIR::MetaListPaths &meta_item) {}
  virtual void visit (HIR::MetaListNameValueStr &meta_item) {}

  // rust-pattern.h
  virtual void visit (HIR::LiteralPattern &pattern) {}
  virtual void visit (HIR::IdentifierPattern &pattern) {}
  virtual void visit (HIR::WildcardPattern &pattern) {}
  //  virtual void visit(RangePatternBound& bound) {}
  virtual void visit (HIR::RangePatternBoundLiteral &bound) {}
  virtual void visit (HIR::RangePatternBoundPath &bound) {}
  virtual void visit (HIR::RangePatternBoundQualPath &bound) {}
  virtual void visit (HIR::RangePattern &pattern) {}
  virtual void visit (HIR::ReferencePattern &pattern) {}
  //  virtual void visit(StructPatternField& field) {}
  virtual void visit (HIR::StructPatternFieldTuplePat &field) {}
  virtual void visit (HIR::StructPatternFieldIdentPat &field) {}
  virtual void visit (HIR::StructPatternFieldIdent &field) {}
  virtual void visit (HIR::StructPattern &pattern) {}
  //  virtual void visit(TupleStructItems& tuple_items) {}
  virtual void visit (HIR::TupleStructItemsNoRange &tuple_items) {}
  virtual void visit (HIR::TupleStructItemsRange &tuple_items) {}
  virtual void visit (HIR::TupleStructPattern &pattern) {}
  //  virtual void visit(TuplePatternItems& tuple_items) {}
  virtual void visit (HIR::TuplePatternItemsMultiple &tuple_items) {}
  virtual void visit (HIR::TuplePatternItemsRanged &tuple_items) {}
  virtual void visit (HIR::TuplePattern &pattern) {}
  virtual void visit (HIR::GroupedPattern &pattern) {}
  virtual void visit (HIR::SlicePattern &pattern) {}

  // rust-stmt.h
  virtual void visit (HIR::EmptyStmt &stmt) {}
  virtual void visit (HIR::LetStmt &stmt) {}
  virtual void visit (HIR::ExprStmtWithoutBlock &stmt) {}
  virtual void visit (HIR::ExprStmtWithBlock &stmt) {}

  // rust-type.h
  virtual void visit (HIR::TraitBound &bound) {}
  virtual void visit (HIR::ImplTraitType &type) {}
  virtual void visit (HIR::TraitObjectType &type) {}
  virtual void visit (HIR::ParenthesisedType &type) {}
  virtual void visit (HIR::ImplTraitTypeOneBound &type) {}
  virtual void visit (HIR::TraitObjectTypeOneBound &type) {}
  virtual void visit (HIR::TupleType &type) {}
  virtual void visit (HIR::NeverType &type) {}
  virtual void visit (HIR::RawPointerType &type) {}
  virtual void visit (HIR::ReferenceType &type) {}
  virtual void visit (HIR::ArrayType &type) {}
  virtual void visit (HIR::SliceType &type) {}
  virtual void visit (HIR::InferredType &type) {}
  virtual void visit (HIR::BareFunctionType &type) {}

protected:
  TypeCheckBase ()
    : mappings (Analysis::Mappings::get ()), resolver (Resolver::get ()),
      context (TypeCheckContext::get ())
  {}

  Analysis::Mappings *mappings;
  Resolver *resolver;
  TypeCheckContext *context;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_HIR_TYPE_CHECK_BASE
