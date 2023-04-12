// Copyright (C) 2020-2023 Free Software Foundation, Inc.

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

#include "rust-ast-visitor.h"
#include "rust-ast.h"

namespace Rust {
// Visitor used to maybe_strip attributes.
class CfgStrip : public AST::ASTVisitor
{
private:
public:
  CfgStrip () {}

  /* Run the AttrVisitor on an entire crate */
  void go (AST::Crate &crate);

  void maybe_strip_struct_fields (std::vector<AST::StructField> &fields);
  void maybe_strip_tuple_fields (std::vector<AST::TupleField> &fields);
  void maybe_strip_function_params (std::vector<AST::FunctionParam> &params);
  void maybe_strip_generic_args (AST::GenericArgs &args);
  void maybe_strip_qualified_path_type (AST::QualifiedPathType &path_type);
  void maybe_strip_closure_params (std::vector<AST::ClosureParam> &params);
  void maybe_strip_self_param (AST::SelfParam &self_param);
  void maybe_strip_where_clause (AST::WhereClause &where_clause);
  void maybe_strip_trait_function_decl (AST::TraitFunctionDecl &decl);
  void maybe_strip_trait_method_decl (AST::TraitMethodDecl &decl);

  /**
   * maybe_strip a set of values, erasing them if they are marked for strip.
   *
   * @param values Iterable reference over values to replace or erase
   */
  template <typename T> void maybe_strip_pointer_allow_strip (T &values)
  {
    for (auto it = values.begin (); it != values.end ();)
      {
	auto &value = *it;

	// mark for stripping if required
	value->accept_vis (*this);
	if (value->is_marked_for_strip ())
	  it = values.erase (it);
	else
	  ++it;
      }
  }

  void visit (AST::Token &) override;
  void visit (AST::DelimTokenTree &) override;
  void visit (AST::AttrInputMetaItemContainer &) override;
  void visit (AST::IdentifierExpr &ident_expr) override;
  void visit (AST::Lifetime &) override;
  void visit (AST::LifetimeParam &) override;
  void visit (AST::ConstGenericParam &) override;

  void visit (AST::MacroInvocation &macro_invoc) override;

  void visit (AST::PathInExpression &path) override;
  void visit (AST::TypePathSegment &) override;
  void visit (AST::TypePathSegmentGeneric &segment) override;
  void visit (AST::TypePathSegmentFunction &segment) override;
  void visit (AST::TypePath &path) override;
  void visit (AST::QualifiedPathInExpression &path) override;
  void visit (AST::QualifiedPathInType &path) override;

  void visit (AST::LiteralExpr &expr) override;
  void visit (AST::AttrInputLiteral &) override;
  void visit (AST::MetaItemLitExpr &) override;
  void visit (AST::MetaItemPathLit &) override;
  void visit (AST::BorrowExpr &expr) override;
  void visit (AST::DereferenceExpr &expr) override;
  void visit (AST::ErrorPropagationExpr &expr) override;
  void visit (AST::NegationExpr &expr) override;
  void visit (AST::ArithmeticOrLogicalExpr &expr) override;
  void visit (AST::ComparisonExpr &expr) override;
  void visit (AST::LazyBooleanExpr &expr) override;
  void visit (AST::TypeCastExpr &expr) override;
  void visit (AST::AssignmentExpr &expr) override;
  void visit (AST::CompoundAssignmentExpr &expr) override;
  void visit (AST::GroupedExpr &expr) override;
  void visit (AST::ArrayElemsValues &elems) override;
  void visit (AST::ArrayElemsCopied &elems) override;
  void visit (AST::ArrayExpr &expr) override;
  void visit (AST::ArrayIndexExpr &expr) override;
  void visit (AST::TupleExpr &expr) override;
  void visit (AST::TupleIndexExpr &expr) override;
  void visit (AST::StructExprStruct &expr) override;
  void visit (AST::StructExprFieldIdentifier &) override;
  void visit (AST::StructExprFieldIdentifierValue &field) override;

  void visit (AST::StructExprFieldIndexValue &field) override;
  void visit (AST::StructExprStructFields &expr) override;
  void visit (AST::StructExprStructBase &expr) override;
  void visit (AST::CallExpr &expr) override;
  void visit (AST::MethodCallExpr &expr) override;
  void visit (AST::FieldAccessExpr &expr) override;
  void visit (AST::ClosureExprInner &expr) override;

  void visit (AST::BlockExpr &expr) override;

  void visit (AST::ClosureExprInnerTyped &expr) override;
  void visit (AST::ContinueExpr &expr) override;
  void visit (AST::BreakExpr &expr) override;
  void visit (AST::RangeFromToExpr &expr) override;
  void visit (AST::RangeFromExpr &expr) override;
  void visit (AST::RangeToExpr &expr) override;
  void visit (AST::RangeFullExpr &) override;
  void visit (AST::RangeFromToInclExpr &expr) override;
  void visit (AST::RangeToInclExpr &expr) override;
  void visit (AST::ReturnExpr &expr) override;
  void visit (AST::UnsafeBlockExpr &expr) override;
  void visit (AST::LoopExpr &expr) override;
  void visit (AST::WhileLoopExpr &expr) override;
  void visit (AST::WhileLetLoopExpr &expr) override;
  void visit (AST::ForLoopExpr &expr) override;
  void visit (AST::IfExpr &expr) override;
  void visit (AST::IfExprConseqElse &expr) override;
  void visit (AST::IfLetExpr &expr) override;
  void visit (AST::IfLetExprConseqElse &expr) override;
  void visit (AST::MatchExpr &expr) override;
  void visit (AST::AwaitExpr &expr) override;
  void visit (AST::AsyncBlockExpr &expr) override;
  void visit (AST::TypeParam &param) override;
  void visit (AST::LifetimeWhereClauseItem &) override;
  void visit (AST::TypeBoundWhereClauseItem &item) override;
  void visit (AST::Method &method) override;
  void visit (AST::Module &module) override;
  void visit (AST::ExternCrate &crate) override;
  void visit (AST::UseTreeGlob &) override;
  void visit (AST::UseTreeList &) override;
  void visit (AST::UseTreeRebind &) override;
  void visit (AST::UseDeclaration &use_decl) override;
  void visit (AST::Function &function) override;
  void visit (AST::TypeAlias &type_alias) override;
  void visit (AST::StructStruct &struct_item) override;
  void visit (AST::TupleStruct &tuple_struct) override;
  void visit (AST::EnumItem &item) override;
  void visit (AST::EnumItemTuple &item) override;
  void visit (AST::EnumItemStruct &item) override;
  void visit (AST::EnumItemDiscriminant &item) override;
  void visit (AST::Enum &enum_item) override;
  void visit (AST::Union &union_item) override;
  void visit (AST::ConstantItem &const_item) override;
  void visit (AST::StaticItem &static_item) override;
  void visit (AST::TraitItemFunc &item) override;
  void visit (AST::TraitItemMethod &item) override;
  void visit (AST::TraitItemConst &item) override;
  void visit (AST::TraitItemType &item) override;
  void visit (AST::Trait &trait) override;
  void visit (AST::InherentImpl &impl) override;
  void visit (AST::TraitImpl &impl) override;
  void visit (AST::ExternalTypeItem &item) override;
  void visit (AST::ExternalStaticItem &item) override;
  void visit (AST::ExternalFunctionItem &item) override;
  void visit (AST::ExternBlock &block) override;

  // I don't think it would be possible to strip macros without expansion
  void visit (AST::MacroMatchFragment &) override;
  void visit (AST::MacroMatchRepetition &) override;
  void visit (AST::MacroMatcher &) override;
  void visit (AST::MacroRulesDefinition &rules_def) override;
  void visit (AST::MetaItemPath &) override;
  void visit (AST::MetaItemSeq &) override;
  void visit (AST::MetaWord &) override;
  void visit (AST::MetaNameValueStr &) override;
  void visit (AST::MetaListPaths &) override;
  void visit (AST::MetaListNameValueStr &) override;
  void visit (AST::LiteralPattern &) override;
  void visit (AST::IdentifierPattern &pattern) override;
  void visit (AST::WildcardPattern &) override;
  void visit (AST::RestPattern &) override;
  void visit (AST::RangePatternBoundLiteral &) override;
  void visit (AST::RangePatternBoundPath &bound) override;
  void visit (AST::RangePatternBoundQualPath &bound) override;
  void visit (AST::RangePattern &pattern) override;
  void visit (AST::ReferencePattern &pattern) override;
  void visit (AST::StructPatternFieldTuplePat &field) override;
  void visit (AST::StructPatternFieldIdentPat &field) override;
  void visit (AST::StructPatternFieldIdent &field) override;
  void visit (AST::StructPattern &pattern) override;
  void visit (AST::TupleStructItemsNoRange &tuple_items) override;
  void visit (AST::TupleStructItemsRange &tuple_items) override;
  void visit (AST::TupleStructPattern &pattern) override;
  void visit (AST::TuplePatternItemsMultiple &tuple_items) override;
  void visit (AST::TuplePatternItemsRanged &tuple_items) override;
  void visit (AST::TuplePattern &pattern) override;
  void visit (AST::GroupedPattern &pattern) override;
  void visit (AST::SlicePattern &pattern) override;
  void visit (AST::AltPattern &pattern) override;

  void visit (AST::EmptyStmt &) override;
  void visit (AST::LetStmt &stmt) override;
  void visit (AST::ExprStmtWithoutBlock &stmt) override;
  void visit (AST::ExprStmtWithBlock &stmt) override;

  void visit (AST::TraitBound &bound) override;
  void visit (AST::ImplTraitType &type) override;
  void visit (AST::TraitObjectType &type) override;
  void visit (AST::ParenthesisedType &type) override;
  void visit (AST::ImplTraitTypeOneBound &type) override;
  void visit (AST::TraitObjectTypeOneBound &type) override;
  void visit (AST::TupleType &type) override;
  void visit (AST::NeverType &) override;
  void visit (AST::RawPointerType &type) override;
  void visit (AST::ReferenceType &type) override;
  void visit (AST::ArrayType &type) override;
  void visit (AST::SliceType &type) override;
  void visit (AST::InferredType &) override;
  void visit (AST::BareFunctionType &type) override;
};
} // namespace Rust
