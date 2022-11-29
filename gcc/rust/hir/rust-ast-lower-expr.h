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

#ifndef RUST_AST_LOWER_EXPR
#define RUST_AST_LOWER_EXPR

#include "rust-diagnostics.h"
#include "rust-ast-lower-base.h"
#include "rust-ast-lower-block.h"
#include "rust-ast-lower-struct-field-expr.h"
#include "rust-ast-lower-pattern.h"

namespace Rust {
namespace HIR {

class ASTLowerPathInExpression : public ASTLoweringBase
{
  using Rust::HIR::ASTLoweringBase::visit;

public:
  static HIR::PathInExpression *translate (AST::PathInExpression *expr)
  {
    ASTLowerPathInExpression compiler;
    expr->accept_vis (compiler);
    rust_assert (compiler.translated);
    return compiler.translated;
  }

  void visit (AST::PathInExpression &expr) override;

private:
  ASTLowerPathInExpression () : translated (nullptr) {}

  HIR::PathInExpression *translated;
};

class ASTLowerQualPathInExpression : public ASTLoweringBase
{
  using Rust::HIR::ASTLoweringBase::visit;

public:
  static HIR::QualifiedPathInExpression *
  translate (AST::QualifiedPathInExpression *expr)
  {
    ASTLowerQualPathInExpression compiler;
    expr->accept_vis (compiler);
    rust_assert (compiler.translated);
    return compiler.translated;
  }

  void visit (AST::QualifiedPathInExpression &expr) override;

private:
  ASTLowerQualPathInExpression () : translated (nullptr) {}

  HIR::QualifiedPathInExpression *translated;
};

class ASTLoweringExpr : public ASTLoweringBase
{
  using Rust::HIR::ASTLoweringBase::visit;

public:
  static HIR::Expr *translate (AST::Expr *expr, bool *terminated = nullptr)
  {
    ASTLoweringExpr resolver;
    expr->accept_vis (resolver);
    if (resolver.translated == nullptr)
      {
	rust_fatal_error (expr->get_locus (), "Failed to lower expr: [%s]",
			  expr->as_string ().c_str ());
	return nullptr;
      }

    resolver.mappings->insert_hir_expr (resolver.translated);
    resolver.mappings->insert_location (
      resolver.translated->get_mappings ().get_hirid (), expr->get_locus ());

    if (terminated != nullptr)
      *terminated = resolver.terminated;

    return resolver.translated;
  }

  void visit (AST::TupleIndexExpr &expr) override
  {
    HIR::Expr *tuple_expr
      = ASTLoweringExpr::translate (expr.get_tuple_expr ().get (), &terminated);

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::TupleIndexExpr (mapping,
				 std::unique_ptr<HIR::Expr> (tuple_expr),
				 expr.get_tuple_index (),
				 expr.get_outer_attrs (), expr.get_locus ());
  }

  void visit (AST::TupleExpr &expr) override
  {
    std::vector<std::unique_ptr<HIR::Expr> > tuple_elements;
    for (auto &e : expr.get_tuple_elems ())
      {
	HIR::Expr *t = ASTLoweringExpr::translate (e.get ());
	tuple_elements.push_back (std::unique_ptr<HIR::Expr> (t));
      }

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::TupleExpr (std::move (mapping), std::move (tuple_elements),
			    expr.get_inner_attrs (), expr.get_outer_attrs (),
			    expr.get_locus ());
  }

  void visit (AST::IfExpr &expr) override
  {
    translated = ASTLoweringIfBlock::translate (&expr, &terminated);
  }

  void visit (AST::IfExprConseqElse &expr) override
  {
    translated = ASTLoweringIfBlock::translate (&expr, &terminated);
  }

  void visit (AST::IfExprConseqIf &expr) override
  {
    translated = ASTLoweringIfBlock::translate (&expr, &terminated);
  }

  void visit (AST::BlockExpr &expr) override
  {
    translated = ASTLoweringBlock::translate (&expr, &terminated);
  }

  void visit (AST::UnsafeBlockExpr &expr) override
  {
    translated = ASTLoweringBlock::translate (&expr, &terminated);
  }

  void visit (AST::PathInExpression &expr) override
  {
    translated = ASTLowerPathInExpression::translate (&expr);
  }

  void visit (AST::QualifiedPathInExpression &expr) override
  {
    translated = ASTLowerQualPathInExpression::translate (&expr);
  }

  void visit (AST::ReturnExpr &expr) override
  {
    terminated = true;
    HIR::Expr *return_expr
      = expr.has_returned_expr ()
	  ? ASTLoweringExpr::translate (expr.get_returned_expr ().get ())
	  : nullptr;

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::ReturnExpr (mapping, expr.get_locus (),
				      std::unique_ptr<HIR::Expr> (return_expr));
  }

  void visit (AST::CallExpr &expr) override
  {
    HIR::Expr *func
      = ASTLoweringExpr::translate (expr.get_function_expr ().get ());

    auto const &in_params = expr.get_params ();
    std::vector<std::unique_ptr<HIR::Expr> > params;
    for (auto &param : in_params)
      {
	auto trans = ASTLoweringExpr::translate (param.get ());
	params.push_back (std::unique_ptr<HIR::Expr> (trans));
      }

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (
      crate_num, UNKNOWN_NODEID /* this can map back to the AST*/,
      mappings->get_next_hir_id (crate_num), UNKNOWN_LOCAL_DEFID);

    translated = new HIR::CallExpr (mapping, std::unique_ptr<HIR::Expr> (func),
				    std::move (params), expr.get_outer_attrs (),
				    expr.get_locus ());
  }

  void visit (AST::MethodCallExpr &expr) override
  {
    HIR::PathExprSegment method_path
      = lower_path_expr_seg (expr.get_method_name ());

    HIR::Expr *receiver
      = ASTLoweringExpr::translate (expr.get_receiver_expr ().get ());

    auto const &in_params = expr.get_params ();
    std::vector<std::unique_ptr<HIR::Expr> > params;
    for (auto &param : in_params)
      {
	auto trans = ASTLoweringExpr::translate (param.get ());
	params.push_back (std::unique_ptr<HIR::Expr> (trans));
      }

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::MethodCallExpr (mapping, std::unique_ptr<HIR::Expr> (receiver),
				 method_path, std::move (params),
				 expr.get_outer_attrs (), expr.get_locus ());
  }

  void visit (AST::AssignmentExpr &expr) override
  {
    HIR::Expr *lhs = ASTLoweringExpr::translate (expr.get_left_expr ().get ());
    HIR::Expr *rhs = ASTLoweringExpr::translate (expr.get_right_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::AssignmentExpr (mapping, std::unique_ptr<HIR::Expr> (lhs),
				 std::unique_ptr<HIR::Expr> (rhs),
				 expr.get_locus ());
  }

  void visit (AST::IdentifierExpr &expr) override
  {
    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping1 (crate_num, expr.get_node_id (),
				    mappings->get_next_hir_id (crate_num),
				    UNKNOWN_LOCAL_DEFID);
    Analysis::NodeMapping mapping2 (mapping1);

    HIR::PathIdentSegment ident_seg (expr.get_ident ());
    HIR::PathExprSegment seg (mapping1, ident_seg, expr.get_locus (),
			      HIR::GenericArgs::create_empty ());
    translated = new HIR::PathInExpression (mapping2, {seg}, expr.get_locus (),
					    false, expr.get_outer_attrs ());
  }

  void visit (AST::ArrayExpr &expr) override
  {
    expr.get_array_elems ()->accept_vis (*this);
    rust_assert (translated_array_elems != nullptr);
    HIR::ArrayElems *elems = translated_array_elems;

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::ArrayExpr (mapping, std::unique_ptr<HIR::ArrayElems> (elems),
			    expr.get_inner_attrs (), expr.get_outer_attrs (),
			    expr.get_locus ());
  }

  void visit (AST::ArrayIndexExpr &expr) override
  {
    HIR::Expr *array_expr
      = ASTLoweringExpr::translate (expr.get_array_expr ().get ());
    HIR::Expr *array_index_expr
      = ASTLoweringExpr::translate (expr.get_index_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::ArrayIndexExpr (mapping,
				 std::unique_ptr<HIR::Expr> (array_expr),
				 std::unique_ptr<HIR::Expr> (array_index_expr),
				 expr.get_outer_attrs (), expr.get_locus ());
  }

  void visit (AST::ArrayElemsValues &elems) override
  {
    std::vector<std::unique_ptr<HIR::Expr> > elements;
    for (auto &elem : elems.get_values ())
      {
	HIR::Expr *translated_elem = ASTLoweringExpr::translate (elem.get ());
	elements.push_back (std::unique_ptr<HIR::Expr> (translated_elem));
      }

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (mappings->get_current_crate (),
				   elems.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated_array_elems
      = new HIR::ArrayElemsValues (mapping, std::move (elements));
  }

  void visit (AST::ArrayElemsCopied &elems) override
  {
    HIR::Expr *element
      = ASTLoweringExpr::translate (elems.get_elem_to_copy ().get ());
    HIR::Expr *num_copies
      = ASTLoweringExpr::translate (elems.get_num_copies ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (mappings->get_current_crate (),
				   elems.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated_array_elems
      = new HIR::ArrayElemsCopied (mapping,
				   std::unique_ptr<HIR::Expr> (element),
				   std::unique_ptr<HIR::Expr> (num_copies));
  }

  void visit (AST::LiteralExpr &expr) override
  {
    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    HIR::Literal l = lower_literal (expr.get_literal ());
    translated
      = new HIR::LiteralExpr (mapping, std::move (l), expr.get_locus (),
			      expr.get_outer_attrs ());
  }

  void visit (AST::ArithmeticOrLogicalExpr &expr) override
  {
    HIR::Expr *lhs = ASTLoweringExpr::translate (expr.get_left_expr ().get ());
    rust_assert (lhs != nullptr);
    HIR::Expr *rhs = ASTLoweringExpr::translate (expr.get_right_expr ().get ());
    rust_assert (rhs != nullptr);

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::ArithmeticOrLogicalExpr (mapping,
					  std::unique_ptr<HIR::Expr> (lhs),
					  std::unique_ptr<HIR::Expr> (rhs),
					  expr.get_expr_type (),
					  expr.get_locus ());
  }

  void visit (AST::ComparisonExpr &expr) override
  {
    HIR::Expr *lhs = ASTLoweringExpr::translate (expr.get_left_expr ().get ());
    rust_assert (lhs != nullptr);
    HIR::Expr *rhs = ASTLoweringExpr::translate (expr.get_right_expr ().get ());
    rust_assert (rhs != nullptr);

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::ComparisonExpr (mapping, std::unique_ptr<HIR::Expr> (lhs),
				 std::unique_ptr<HIR::Expr> (rhs),
				 expr.get_expr_type (), expr.get_locus ());
  }

  void visit (AST::LazyBooleanExpr &expr) override
  {
    HIR::Expr *lhs = ASTLoweringExpr::translate (expr.get_left_expr ().get ());
    rust_assert (lhs != nullptr);
    HIR::Expr *rhs = ASTLoweringExpr::translate (expr.get_right_expr ().get ());
    rust_assert (rhs != nullptr);

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::LazyBooleanExpr (mapping, std::unique_ptr<HIR::Expr> (lhs),
				  std::unique_ptr<HIR::Expr> (rhs),
				  expr.get_expr_type (), expr.get_locus ());
  }

  void visit (AST::NegationExpr &expr) override
  {
    HIR::Expr *negated_value
      = ASTLoweringExpr::translate (expr.get_negated_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);
    translated
      = new HIR::NegationExpr (mapping,
			       std::unique_ptr<HIR::Expr> (negated_value),
			       expr.get_expr_type (), expr.get_outer_attrs (),
			       expr.get_locus ());
  }

  void visit (AST::TypeCastExpr &expr) override
  {
    HIR::Expr *expr_to_cast_to
      = ASTLoweringExpr::translate (expr.get_casted_expr ().get ());
    HIR::Type *type_to_cast_to
      = lower_type_no_bounds (expr.get_type_to_cast_to ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::TypeCastExpr (mapping,
			       std::unique_ptr<HIR::Expr> (expr_to_cast_to),
			       std::unique_ptr<HIR::Type> (type_to_cast_to),
			       expr.get_locus ());
  }

  void visit (AST::CompoundAssignmentExpr &expr) override
  {
    ArithmeticOrLogicalOperator op;
    switch (expr.get_expr_type ())
      {
      case CompoundAssignmentOperator::ADD:
	op = ArithmeticOrLogicalOperator::ADD;
	break;
      case CompoundAssignmentOperator::SUBTRACT:
	op = ArithmeticOrLogicalOperator::SUBTRACT;
	break;
      case CompoundAssignmentOperator::MULTIPLY:
	op = ArithmeticOrLogicalOperator::MULTIPLY;
	break;
      case CompoundAssignmentOperator::DIVIDE:
	op = ArithmeticOrLogicalOperator::DIVIDE;
	break;
      case CompoundAssignmentOperator::MODULUS:
	op = ArithmeticOrLogicalOperator::MODULUS;
	break;
      case CompoundAssignmentOperator::BITWISE_AND:
	op = ArithmeticOrLogicalOperator::BITWISE_AND;
	break;
      case CompoundAssignmentOperator::BITWISE_OR:
	op = ArithmeticOrLogicalOperator::BITWISE_OR;
	break;
      case CompoundAssignmentOperator::BITWISE_XOR:
	op = ArithmeticOrLogicalOperator::BITWISE_XOR;
	break;
      case CompoundAssignmentOperator::LEFT_SHIFT:
	op = ArithmeticOrLogicalOperator::LEFT_SHIFT;
	break;
      case CompoundAssignmentOperator::RIGHT_SHIFT:
	op = ArithmeticOrLogicalOperator::RIGHT_SHIFT;
	break;
      default:
	gcc_unreachable ();
      }

    HIR::Expr *asignee_expr
      = ASTLoweringExpr::translate (expr.get_left_expr ().get ());
    HIR::Expr *value
      = ASTLoweringExpr::translate (expr.get_right_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::CompoundAssignmentExpr (
      mapping, std::unique_ptr<HIR::Expr> (asignee_expr),
      std::unique_ptr<HIR::Expr> (value), op, expr.get_locus ());
  }

  void visit (AST::StructExprStruct &struct_expr) override
  {
    HIR::PathInExpression *path
      = ASTLowerPathInExpression::translate (&struct_expr.get_struct_name ());
    HIR::PathInExpression copied_path (*path);
    delete path;

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, struct_expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::StructExprStruct (mapping, copied_path,
					    struct_expr.get_inner_attrs (),
					    struct_expr.get_outer_attrs (),
					    struct_expr.get_locus ());
  }

  void visit (AST::StructExprStructFields &struct_expr) override
  {
    // bit of a hack for now
    HIR::PathInExpression *path
      = ASTLowerPathInExpression::translate (&struct_expr.get_struct_name ());
    HIR::PathInExpression copied_path (*path);
    delete path;

    HIR::StructBase *base = nullptr;
    if (struct_expr.has_struct_base ())
      {
	HIR::Expr *translated_base = ASTLoweringExpr::translate (
	  struct_expr.get_struct_base ().get_base_struct ().get ());
	base
	  = new HIR::StructBase (std::unique_ptr<HIR::Expr> (translated_base));
      }

    auto const &in_fields = struct_expr.get_fields ();
    std::vector<std::unique_ptr<HIR::StructExprField> > fields;
    for (auto &field : in_fields)
      {
	HIR::StructExprField *translated
	  = ASTLowerStructExprField::translate (field.get ());
	fields.push_back (std::unique_ptr<HIR::StructExprField> (translated));
      }

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, struct_expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::StructExprStructFields (
      mapping, copied_path, std::move (fields), struct_expr.get_locus (), base,
      struct_expr.get_inner_attrs (), struct_expr.get_outer_attrs ());
  }

  void visit (AST::GroupedExpr &expr) override
  {
    HIR::Expr *paren_expr
      = ASTLoweringExpr::translate (expr.get_expr_in_parens ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::GroupedExpr (mapping, std::unique_ptr<HIR::Expr> (paren_expr),
			      expr.get_inner_attrs (), expr.get_outer_attrs (),
			      expr.get_locus ());
  }

  void visit (AST::FieldAccessExpr &expr) override
  {
    HIR::Expr *receiver
      = ASTLoweringExpr::translate (expr.get_receiver_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);
    translated
      = new HIR::FieldAccessExpr (mapping,
				  std::unique_ptr<HIR::Expr> (receiver),
				  expr.get_field_name (),
				  expr.get_outer_attrs (), expr.get_locus ());
  }

  void visit (AST::LoopExpr &expr) override
  {
    translated = ASTLoweringExprWithBlock::translate (&expr, &terminated);
  }

  void visit (AST::WhileLoopExpr &expr) override
  {
    translated = ASTLoweringExprWithBlock::translate (&expr, &terminated);
  }

  void visit (AST::ForLoopExpr &expr) override
  {
    translated = ASTLoweringExprWithBlock::translate (&expr, &terminated);
  }

  void visit (AST::BreakExpr &expr) override
  {
    HIR::Lifetime break_label = lower_lifetime (expr.get_label ());
    HIR::Expr *break_expr
      = expr.has_break_expr ()
	  ? ASTLoweringExpr::translate (expr.get_break_expr ().get ())
	  : nullptr;

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::BreakExpr (mapping, expr.get_locus (),
				     std ::move (break_label),
				     std::unique_ptr<HIR::Expr> (break_expr),
				     expr.get_outer_attrs ());
  }

  void visit (AST::ContinueExpr &expr) override
  {
    HIR::Lifetime break_label = lower_lifetime (expr.get_label ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::ContinueExpr (mapping, expr.get_locus (),
					std ::move (break_label),
					expr.get_outer_attrs ());
  }

  void visit (AST::BorrowExpr &expr) override
  {
    HIR::Expr *borrow_lvalue
      = ASTLoweringExpr::translate (expr.get_borrowed_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::BorrowExpr (
      mapping, std::unique_ptr<HIR::Expr> (borrow_lvalue),
      expr.get_is_mut () ? Mutability::Mut : Mutability::Imm,
      expr.get_is_double_borrow (), expr.get_outer_attrs (), expr.get_locus ());
  }

  void visit (AST::DereferenceExpr &expr) override
  {
    HIR::Expr *dref_lvalue
      = ASTLoweringExpr::translate (expr.get_dereferenced_expr ().get ());

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated
      = new HIR::DereferenceExpr (mapping,
				  std::unique_ptr<HIR::Expr> (dref_lvalue),
				  expr.get_outer_attrs (), expr.get_locus ());
  }

  void visit (AST::MatchExpr &expr) override
  {
    translated = ASTLoweringExprWithBlock::translate (&expr, &terminated);
  }

  void visit (AST::RangeFromToExpr &expr) override
  {
    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    HIR::Expr *range_from
      = ASTLoweringExpr::translate (expr.get_from_expr ().get ());
    HIR::Expr *range_to
      = ASTLoweringExpr::translate (expr.get_to_expr ().get ());

    translated
      = new HIR::RangeFromToExpr (mapping,
				  std::unique_ptr<HIR::Expr> (range_from),
				  std::unique_ptr<HIR::Expr> (range_to),
				  expr.get_locus ());
  }

  void visit (AST::RangeFromExpr &expr) override
  {
    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    HIR::Expr *range_from
      = ASTLoweringExpr::translate (expr.get_from_expr ().get ());

    translated
      = new HIR::RangeFromExpr (mapping,
				std::unique_ptr<HIR::Expr> (range_from),
				expr.get_locus ());
  }

  void visit (AST::RangeToExpr &expr) override
  {
    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    HIR::Expr *range_to
      = ASTLoweringExpr::translate (expr.get_to_expr ().get ());

    translated
      = new HIR::RangeToExpr (mapping, std::unique_ptr<HIR::Expr> (range_to),
			      expr.get_locus ());
  }

  void visit (AST::RangeFullExpr &expr) override
  {
    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    translated = new HIR::RangeFullExpr (mapping, expr.get_locus ());
  }

  void visit (AST::RangeFromToInclExpr &expr) override
  {
    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, expr.get_node_id (),
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);

    HIR::Expr *range_from
      = ASTLoweringExpr::translate (expr.get_from_expr ().get ());
    HIR::Expr *range_to
      = ASTLoweringExpr::translate (expr.get_to_expr ().get ());

    translated
      = new HIR::RangeFromToInclExpr (mapping,
				      std::unique_ptr<HIR::Expr> (range_from),
				      std::unique_ptr<HIR::Expr> (range_to),
				      expr.get_locus ());
  }

private:
  ASTLoweringExpr ()
    : ASTLoweringBase (), translated (nullptr),
      translated_array_elems (nullptr), terminated (false)
  {}

  HIR::Expr *translated;
  HIR::ArrayElems *translated_array_elems;
  bool terminated;
};

} // namespace HIR
} // namespace Rust

#endif // RUST_AST_LOWER_EXPR
