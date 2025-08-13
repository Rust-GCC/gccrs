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

#include "rust-ast-pointer-visitor.h"
#include "rust-ast-visitor.h"
#include "rust-ast-full.h"

namespace Rust {
namespace AST {

void
PointerVisitor::visit (AST::Crate &crate)
{
  for (auto &item : crate.items)
    reseat (item);

  DefaultASTVisitor::visit (crate);
}

void
PointerVisitor::visit (AST::ConstGenericParam &const_param)
{
  if (const_param.has_type ())
    reseat (const_param.get_type_ptr ());

  DefaultASTVisitor::visit (const_param);
}

void
PointerVisitor::visit (GenericArgsBinding &binding)
{
  reseat (binding.get_type_ptr ());
}

void
PointerVisitor::visit (AST::TypePathFunction &tpf)
{
  for (auto &input : tpf.get_params ())
    reseat (input);

  if (tpf.has_return_type ())
    reseat (tpf.get_return_type_ptr ());

  DefaultASTVisitor::visit (tpf);
}

void
PointerVisitor::visit (AST::QualifiedPathType &path)
{
  reseat (path.get_type_ptr ());

  DefaultASTVisitor::visit (path);
}

void
PointerVisitor::visit (AST::MetaItemPathExpr &meta_item)
{
  reseat (meta_item.get_expr_ptr ());

  DefaultASTVisitor::visit (meta_item);
}

void
PointerVisitor::visit (AST::BorrowExpr &expr)
{
  reseat (expr.get_borrowed_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::DereferenceExpr &expr)
{
  reseat (expr.get_dereferenced_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ErrorPropagationExpr &expr)
{
  reseat (expr.get_propagating_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::NegationExpr &expr)
{
  reseat (expr.get_negated_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ArithmeticOrLogicalExpr &expr)
{
  reseat (expr.get_left_expr_ptr ());
  reseat (expr.get_right_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ComparisonExpr &expr)
{
  reseat (expr.get_left_expr_ptr ());
  reseat (expr.get_right_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::LazyBooleanExpr &expr)
{
  reseat (expr.get_left_expr_ptr ());
  reseat (expr.get_right_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::TypeCastExpr &expr)
{
  reseat (expr.get_casted_expr_ptr ());
  reseat (expr.get_type_to_cast_to_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::AssignmentExpr &expr)
{
  reseat (expr.get_left_expr_ptr ());
  reseat (expr.get_right_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::CompoundAssignmentExpr &expr)
{
  reseat (expr.get_left_expr_ptr ());
  reseat (expr.get_right_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::GroupedExpr &expr)
{
  reseat (expr.get_expr_in_parens_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ArrayElemsValues &elems)
{
  for (auto &value : elems.get_values ())
    reseat (value);

  DefaultASTVisitor::visit (elems);
}

void
PointerVisitor::visit (AST::ArrayElemsCopied &elems)
{
  reseat (elems.get_elem_to_copy_ptr ());
  reseat (elems.get_num_copies_ptr ());

  DefaultASTVisitor::visit (elems);
}

void
PointerVisitor::visit (AST::ArrayIndexExpr &expr)
{
  reseat (expr.get_array_expr_ptr ());
  reseat (expr.get_index_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::TupleExpr &expr)
{
  for (auto &elem : expr.get_tuple_elems ())
    reseat (elem);

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::TupleIndexExpr &expr)
{
  reseat (expr.get_tuple_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::StructExprFieldIdentifierValue &field)
{
  reseat (field.get_value_ptr ());

  DefaultASTVisitor::visit (field);
}

void
PointerVisitor::visit (AST::StructExprFieldIndexValue &field)
{
  reseat (field.get_value_ptr ());

  DefaultASTVisitor::visit (field);
}

void
PointerVisitor::visit (AST::StructBase &base)
{
  reseat (base.get_base_struct_ptr ());

  DefaultASTVisitor::visit (base);
}

void
PointerVisitor::visit (AST::CallExpr &expr)
{
  reseat (expr.get_function_expr_ptr ());

  for (auto &param : expr.get_params ())
    reseat (param);

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::MethodCallExpr &expr)
{
  reseat (expr.get_receiver_expr_ptr ());

  for (auto &param : expr.get_params ())
    reseat (param);

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::FieldAccessExpr &expr)
{
  reseat (expr.get_receiver_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ClosureExprInner &expr)
{
  reseat (expr.get_definition_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::BlockExpr &expr)
{
  for (auto &stmt : expr.get_statements ())
    reseat (stmt);

  if (expr.has_tail_expr ())
    reseat (expr.get_tail_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::AnonConst &expr)
{
  if (!expr.is_deferred ())
    reseat (expr.get_inner_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ClosureExprInnerTyped &expr)
{
  reseat (expr.get_return_type_ptr ());

  // No need to reseat the definition since it is always a block

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ClosureParam &param)
{
  reseat (param.get_pattern_ptr ());

  if (param.has_type_given ())
    reseat (param.get_type_ptr ());

  DefaultASTVisitor::visit (param);
}

void
PointerVisitor::visit (AST::BreakExpr &expr)
{
  if (expr.has_break_expr ())
    reseat (expr.get_break_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::RangeFromToExpr &expr)
{
  reseat (expr.get_from_expr_ptr ());
  reseat (expr.get_to_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::RangeFromExpr &expr)
{
  reseat (expr.get_from_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::RangeToExpr &expr)
{
  reseat (expr.get_to_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::RangeFromToInclExpr &expr)
{
  reseat (expr.get_from_expr_ptr ());
  reseat (expr.get_to_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::RangeToInclExpr &expr)
{
  reseat (expr.get_to_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ReturnExpr &expr)
{
  if (expr.has_returned_expr ())
    reseat (expr.get_returned_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::BoxExpr &expr)
{
  reseat (expr.get_boxed_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::WhileLoopExpr &expr)
{
  reseat (expr.get_predicate_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::WhileLetLoopExpr &expr)
{
  for (auto &pattern : expr.get_patterns ())
    reseat (pattern);

  reseat (expr.get_scrutinee_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::ForLoopExpr &expr)
{
  reseat (expr.get_pattern_ptr ());
  reseat (expr.get_iterator_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::IfExpr &expr)
{
  reseat (expr.get_condition_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::IfExprConseqElse &expr)
{
  reseat (expr.get_condition_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::IfLetExpr &expr)
{
  for (auto &pattern : expr.get_patterns ())
    reseat (pattern);

  reseat (expr.get_value_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::IfLetExprConseqElse &expr)
{
  reseat (expr.get_value_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::MatchArm &arm)
{
  for (auto &pattern : arm.get_patterns ())
    reseat (pattern);
  if (arm.has_match_arm_guard ())
    reseat (arm.get_guard_expr_ptr ());

  DefaultASTVisitor::visit (arm);
}

void
PointerVisitor::visit (AST::MatchCase &arm)
{
  reseat (arm.get_expr_ptr ());

  DefaultASTVisitor::visit (arm);
}

void
PointerVisitor::visit (AST::MatchExpr &expr)
{
  reseat (expr.get_scrutinee_expr_ptr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::AwaitExpr &expr)
{
  reseat (expr.get_awaited_expr ());

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::InlineAsm &expr)
{
  using RegisterType = AST::InlineAsmOperand::RegisterType;

  for (auto &operand : expr.get_operands ())
    {
      switch (operand.get_register_type ())
	{
	case RegisterType::In:
	  {
	    reseat (operand.get_in ().expr);
	    break;
	  }
	case RegisterType::Out:
	  {
	    reseat (operand.get_out ().expr);
	    break;
	  }
	case RegisterType::InOut:
	  {
	    reseat (operand.get_in_out ().expr);
	    break;
	  }
	case RegisterType::SplitInOut:
	  {
	    auto split = operand.get_split_in_out ();
	    reseat (split.in_expr);
	    reseat (split.out_expr);
	    break;
	  }
	case RegisterType::Const:
	  {
	    reseat (operand.get_const ().anon_const.get_inner_expr_ptr ());
	    break;
	  }
	case RegisterType::Sym:
	  {
	    reseat (operand.get_sym ().expr);
	    break;
	  }
	case RegisterType::Label:
	  {
	    reseat (operand.get_label ().expr);
	    break;
	  }
	}
    }

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::LlvmInlineAsm &expr)
{
  for (auto &output : expr.get_outputs ())
    reseat (output.expr);

  for (auto &input : expr.get_inputs ())
    reseat (input.expr);

  DefaultASTVisitor::visit (expr);
}

void
PointerVisitor::visit (AST::TypeParam &param)
{
  if (param.has_type ())
    reseat (param.get_type_ptr ());

  DefaultASTVisitor::visit (param);
}

void
PointerVisitor::visit (AST::TypeBoundWhereClauseItem &item)
{
  reseat (item.get_type_ptr ());

  DefaultASTVisitor::visit (item);
}

void
PointerVisitor::visit (AST::FunctionParam &param)
{
  if (param.has_name ())
    reseat (param.get_pattern_ptr ());

  reseat (param.get_type_ptr ());

  DefaultASTVisitor::visit (param);
}

void
PointerVisitor::visit (AST::SelfParam &param)
{
  if (param.has_type ())
    reseat (param.get_type_ptr ());

  DefaultASTVisitor::visit (param);
}

void
PointerVisitor::visit (AST::Module &module)
{
  for (auto &item : module.get_items ())
    reseat (item);

  DefaultASTVisitor::visit (module);
}

void
PointerVisitor::visit (AST::Function &function)
{
  visit_function_params (function);

  if (function.has_return_type ())
    reseat (function.get_return_type_ptr ());

  DefaultASTVisitor::visit (function);
}

void
PointerVisitor::visit (AST::TypeAlias &type_alias)
{
  reseat (type_alias.get_type_aliased_ptr ());

  DefaultASTVisitor::visit (type_alias);
}

void
PointerVisitor::visit (AST::StructField &field)
{
  reseat (field.get_field_type_ptr ());

  DefaultASTVisitor::visit (field);
}

void
PointerVisitor::visit (AST::TupleField &field)
{
  reseat (field.get_field_type_ptr ());

  DefaultASTVisitor::visit (field);
}

void
PointerVisitor::visit (AST::EnumItemDiscriminant &item)
{
  reseat (item.get_expr_ptr ());

  DefaultASTVisitor::visit (item);
}

void
PointerVisitor::visit (AST::ConstantItem &const_item)
{
  reseat (const_item.get_type_ptr ());
  if (const_item.has_expr ())
    reseat (const_item.get_expr_ptr ());

  DefaultASTVisitor::visit (const_item);
}

void
PointerVisitor::visit (AST::StaticItem &static_item)
{
  reseat (static_item.get_type_ptr ());
  reseat (static_item.get_expr_ptr ());

  DefaultASTVisitor::visit (static_item);
}

void
PointerVisitor::visit (AST::TraitItemConst &item)
{
  reseat (item.get_type_ptr ());
  if (item.has_expr ())
    reseat (item.get_expr_ptr ());

  DefaultASTVisitor::visit (item);
}

void
PointerVisitor::visit (AST::Trait &trait)
{
  for (auto &item : trait.get_trait_items ())
    reseat (item);

  DefaultASTVisitor::visit (trait);
}

void
PointerVisitor::visit (AST::InherentImpl &impl)
{
  reseat (impl.get_type_ptr ());
  for (auto &item : impl.get_impl_items ())
    reseat (item);

  DefaultASTVisitor::visit (impl);
}

void
PointerVisitor::visit (AST::TraitImpl &impl)
{
  reseat (impl.get_type_ptr ());
  for (auto &item : impl.get_impl_items ())
    reseat (item);

  DefaultASTVisitor::visit (impl);
}

void
PointerVisitor::visit (AST::ExternalStaticItem &item)
{
  reseat (item.get_type_ptr ());

  DefaultASTVisitor::visit (item);
}

void
PointerVisitor::visit (AST::ExternBlock &block)
{
  for (auto &item : block.get_extern_items ())
    reseat (item);

  DefaultASTVisitor::visit (block);
}

void
PointerVisitor::visit (AST::IdentifierPattern &pattern)
{
  if (pattern.has_subpattern ())
    reseat (pattern.get_subpattern_ptr ());

  DefaultASTVisitor::visit (pattern);
}

void
PointerVisitor::visit (AST::ReferencePattern &pattern)
{
  reseat (pattern.get_referenced_pattern_ptr ());

  DefaultASTVisitor::visit (pattern);
}

void
PointerVisitor::visit (AST::StructPatternFieldTuplePat &field)
{
  reseat (field.get_index_pattern_ptr ());

  DefaultASTVisitor::visit (field);
}

void
PointerVisitor::visit (AST::StructPatternFieldIdentPat &field)
{
  reseat (field.get_ident_pattern_ptr ());

  DefaultASTVisitor::visit (field);
}

void
PointerVisitor::visit (AST::TupleStructItemsNoRange &tuple_items)
{
  for (auto &pattern : tuple_items.get_patterns ())
    reseat (pattern);

  DefaultASTVisitor::visit (tuple_items);
}

void
PointerVisitor::visit (AST::TupleStructItemsRange &tuple_items)
{
  for (auto &lower : tuple_items.get_lower_patterns ())
    reseat (lower);
  for (auto &upper : tuple_items.get_upper_patterns ())
    reseat (upper);

  DefaultASTVisitor::visit (tuple_items);
}

void
PointerVisitor::visit (AST::TuplePatternItemsMultiple &tuple_items)
{
  for (auto &pattern : tuple_items.get_patterns ())
    reseat (pattern);

  DefaultASTVisitor::visit (tuple_items);
}

void
PointerVisitor::visit (AST::TuplePatternItemsRanged &tuple_items)
{
  for (auto &lower : tuple_items.get_lower_patterns ())
    reseat (lower);
  for (auto &upper : tuple_items.get_upper_patterns ())
    reseat (upper);

  DefaultASTVisitor::visit (tuple_items);
}

void
PointerVisitor::visit (AST::GroupedPattern &pattern)
{
  reseat (pattern.get_pattern_in_parens_ptr ());

  DefaultASTVisitor::visit (pattern);
}

void
PointerVisitor::visit (AST::SlicePatternItemsNoRest &items)
{
  for (auto &item : items.get_patterns ())
    reseat (item);

  DefaultASTVisitor::visit (items);
}

void
PointerVisitor::visit (AST::SlicePatternItemsHasRest &items)
{
  for (auto &item : items.get_lower_patterns ())
    reseat (item);
  for (auto &item : items.get_upper_patterns ())
    reseat (item);

  DefaultASTVisitor::visit (items);
}

void
PointerVisitor::visit (AST::AltPattern &pattern)
{
  for (auto &alt : pattern.get_alts ())
    reseat (alt);

  DefaultASTVisitor::visit (pattern);
}

void
PointerVisitor::visit (AST::LetStmt &stmt)
{
  reseat (stmt.get_pattern_ptr ());
  if (stmt.has_type ())
    reseat (stmt.get_type_ptr ());
  if (stmt.has_init_expr ())
    reseat (stmt.get_init_expr_ptr ());

  DefaultASTVisitor::visit (stmt);
}

void
PointerVisitor::visit (AST::ExprStmt &stmt)
{
  reseat (stmt.get_expr_ptr ());

  DefaultASTVisitor::visit (stmt);
}

void
PointerVisitor::visit (AST::ParenthesisedType &type)
{
  reseat (type.get_type_in_parens ());

  DefaultASTVisitor::visit (type);
}

void
PointerVisitor::visit (AST::TupleType &type)
{
  for (auto &elem : type.get_elems ())
    reseat (elem);

  DefaultASTVisitor::visit (type);
}

void
PointerVisitor::visit (AST::RawPointerType &type)
{
  reseat (type.get_type_pointed_to_ptr ());

  DefaultASTVisitor::visit (type);
}

void
PointerVisitor::visit (AST::ReferenceType &type)
{
  reseat (type.get_type_referenced_ptr ());

  DefaultASTVisitor::visit (type);
}

void
PointerVisitor::visit (AST::ArrayType &type)
{
  reseat (type.get_elem_type_ptr ());

  DefaultASTVisitor::visit (type);
}

void
PointerVisitor::visit (AST::SliceType &type)
{
  reseat (type.get_elem_type_ptr ());

  DefaultASTVisitor::visit (type);
}

void
PointerVisitor::visit (AST::MaybeNamedParam &param)
{
  reseat (param.get_type_ptr ());

  DefaultASTVisitor::visit (param);
}

void
PointerVisitor::visit (AST::BareFunctionType &type)
{
  if (type.has_return_type ())
    reseat (type.get_return_type_ptr ());

  DefaultASTVisitor::visit (type);
}

void
PointerVisitor::visit (AST::OffsetOf &offset_of)
{
  reseat (offset_of.get_type_ptr ());

  DefaultASTVisitor::visit (offset_of);
}

void
PointerVisitor::visit (AST::VariadicParam &param)
{
  if (param.has_pattern ())
    reseat (param.get_pattern_ptr ());

  DefaultASTVisitor::visit (param);
}

} // namespace AST
} // namespace Rust
