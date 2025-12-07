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

#ifndef RUST_NODE_ID_VISITOR_H
#define RUST_NODE_ID_VISITOR_H

#include "rust-ast-visitor.h"
#include "rust-expand-visitor.h"

namespace Rust {

// assigns node ids to instances of HasNodeId
class NodeIdVisitor : public AST::DefaultASTVisitor
{
public:
  NodeIdVisitor () {}

  static void go (AST::Crate &crate)
  {
    NodeIdVisitor visitor;
    visitor.visit (crate);
  }

  static void go (AST::Item &item)
  {
    NodeIdVisitor visitor;
    item.accept_vis (visitor);
  }

  static void go (AST::SimplePath &path)
  {
    NodeIdVisitor visitor;
    visitor.visit (path);
  }

  static void go (AST::GenericArg &arg)
  {
    NodeIdVisitor visitor;
    arg.accept_vis (visitor);
  }

  static void go (AST::Fragment &frag)
  {
    for (auto &node : frag.get_nodes ())
      go (node);
  }

  static void go (AST::SingleASTNode &node)
  {
    NodeIdVisitor visitor;
    node.accept_vis (visitor);
  }

#define X(type) \
  void visit (AST::type &node) override \
  { \
    handle (node); \
    if (!has_derive (node)) \
      AST::DefaultASTVisitor::visit (node); \
  }

  void visit (AST::UseDeclaration &node) override
  {
    handle (node);
    // skip handling of import path
  }

  X(Crate)
  X(Token)
  X(DelimTokenTree)
  X(AttrInputMetaItemContainer)
  X(IdentifierExpr)
  X(Lifetime)
  X(LifetimeParam)
  X(ConstGenericParam)
  X(PathInExpression)
  X(TypePathSegment)
  X(TypePathSegmentGeneric)
  X(TypePathSegmentFunction)
  X(TypePath)
  X(QualifiedPathInExpression)
  X(QualifiedPathInType)
  X(LiteralExpr)
  X(AttrInputLiteral)
  X(AttrInputMacro)
  X(MetaItemLitExpr)
  X(MetaItemPathExpr)
  X(BorrowExpr)
  X(DereferenceExpr)
  X(ErrorPropagationExpr)
  X(NegationExpr)
  X(ArithmeticOrLogicalExpr)
  X(ComparisonExpr)
  X(LazyBooleanExpr)
  X(TypeCastExpr)
  X(AssignmentExpr)
  X(CompoundAssignmentExpr)
  X(GroupedExpr)
  X(ArrayElemsValues)
  X(ArrayElemsCopied)
  X(ArrayExpr)
  X(ArrayIndexExpr)
  X(TupleExpr)
  X(TupleIndexExpr)
  X(StructExprStruct)
  X(StructExprFieldIdentifier)
  X(StructExprFieldIdentifierValue)
  X(StructExprFieldIndexValue)
  X(StructExprStructFields)
  X(StructExprStructBase)
  X(CallExpr)
  X(MethodCallExpr)
  X(FieldAccessExpr)
  X(ClosureExprInner)
  X(BlockExpr)
  X(AnonConst)
  X(ConstBlock)
  X(ClosureExprInnerTyped)
  X(ContinueExpr)
  X(BreakExpr)
  X(RangeFromToExpr)
  X(RangeFromExpr)
  X(RangeToExpr)
  X(RangeFullExpr)
  X(RangeFromToInclExpr)
  X(RangeToInclExpr)
  X(ReturnExpr)
  X(TryExpr)
  X(BoxExpr)
  X(UnsafeBlockExpr)
  X(LoopExpr)
  X(WhileLoopExpr)
  X(WhileLetLoopExpr)
  X(ForLoopExpr)
  X(IfExpr)
  X(IfExprConseqElse)
  X(IfLetExpr)
  X(IfLetExprConseqElse)
  X(MatchExpr)
  X(AwaitExpr)
  X(AsyncBlockExpr)

  X(InlineAsm)
  X(LlvmInlineAsm)

  X(TypeParam)
  X(LifetimeWhereClauseItem)
  X(TypeBoundWhereClauseItem)
  X(Module)
  X(ExternCrate)
  X(UseTreeGlob)
  X(UseTreeList)
  X(UseTreeRebind)
  // X(UseDeclaration)
  X(Function)
  X(TypeAlias)
  X(StructStruct)
  X(TupleStruct)
  X(EnumItem)
  X(EnumItemTuple)
  X(EnumItemStruct)
  X(EnumItemDiscriminant)
  X(Enum)
  X(Union)
  X(ConstantItem)
  X(StaticItem)
  X(TraitItemType)
  X(Trait)
  X(InherentImpl)
  X(TraitImpl)
  X(ExternalTypeItem)
  X(ExternalStaticItem)
  X(ExternBlock)
  X(MacroMatchFragment)
  X(MacroMatchRepetition)
  X(MacroMatcher)
  X(MacroRulesDefinition)
  X(MacroInvocation)
  X(MetaItemPath)
  X(MetaItemSeq)
  X(MetaWord)
  X(MetaNameValueStr)
  X(MetaListPaths)
  X(MetaListNameValueStr)
  X(LiteralPattern)
  X(IdentifierPattern)
  X(WildcardPattern)
  X(RestPattern)
  X(RangePatternBoundLiteral)
  X(RangePatternBoundPath)
  X(RangePatternBoundQualPath)
  X(RangePattern)
  X(ReferencePattern)
  X(StructPatternFieldTuplePat)
  X(StructPatternFieldIdentPat)
  X(StructPatternFieldIdent)
  X(StructPattern)
  X(TupleStructItemsNoRest)
  X(TupleStructItemsHasRest)
  X(TupleStructPattern)
  X(TuplePatternItemsNoRest)
  X(TuplePatternItemsHasRest)
  X(TuplePattern)
  X(GroupedPattern)
  X(SlicePatternItemsNoRest)
  X(SlicePatternItemsHasRest)
  X(SlicePattern)
  X(AltPattern)
  X(EmptyStmt)
  X(LetStmt)
  X(ExprStmt)
  X(TraitBound)
  X(ImplTraitType)
  X(TraitObjectType)
  X(ParenthesisedType)
  X(ImplTraitTypeOneBound)
  X(TraitObjectTypeOneBound)
  X(TupleType)
  X(NeverType)
  X(RawPointerType)
  X(ReferenceType)
  X(ArrayType)
  X(SliceType)
  X(InferredType)
  X(BareFunctionType)
  X(SelfParam)
  X(FunctionParam)
  X(VariadicParam)
  X(FormatArgs)
  X(OffsetOf)

  X(GenericArgsBinding)
  X(PathExprSegment)
  X(GenericArgs)
  X(QualifiedPathType)
  X(TypePathFunction)
  X(PathIdentSegment)
  X(SimplePath)
  X(SimplePathSegment)
  X(StructBase)
  X(ClosureParam)
  X(LoopLabel)
  X(MatchCase)
  X(MatchArm)
  X(Visibility)
  X(FunctionQualifiers)
  X(WhereClause)
  X(StructField)
  X(TupleField)
  X(MacroRule)
  X(MacroInvocData)
  X(MacroTranscriber)
  X(StructPatternElements)
  X(MaybeNamedParam)

private:
  template <typename T, typename = typename std::enable_if<std::is_class<T>::value && !std::is_base_of<HasNodeId, T>::value>::type>
  void handle (T&) {}

  void handle (HasNodeId &node) { node.assign_node_id (); }

  bool has_derive (const AST::Item &node)
  {
    for (auto &attr : node.get_outer_attrs ())
      {
        if (attr.is_derive ())
	  return true;
      }
    return false;
  }

  template <typename T> bool has_derive (const T &)
  {
    return false;
  }
};

} // namespace Rust

#endif // RUST_NODE_ID_VISITOR_H
