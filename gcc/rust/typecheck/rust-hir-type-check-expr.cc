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

#include "rust-hir-full.h"
#include "rust-tyty-call.h"
#include "rust-hir-type-check-struct-field.h"
#include "rust-hir-path-probe.h"
#include "rust-substitution-mapper.h"
#include "rust-hir-trait-resolve.h"
#include "rust-hir-type-bounds.h"
#include "rust-hir-dot-operator.h"
#include "rust-hir-type-check-pattern.h"
#include "rust-hir-type-check-expr.h"
#include "rust-hir-type-check-stmt.h"

namespace Rust {
namespace Resolver {

TypeCheckExpr::TypeCheckExpr () : TypeCheckBase (), infered (nullptr) {}

// Perform type checking on expr. Also runs type unification algorithm.
// Returns the unified type of expr
TyTy::BaseType *
TypeCheckExpr::Resolve (HIR::Expr *expr)
{
  TypeCheckExpr resolver;
  expr->accept_vis (resolver);

  if (resolver.infered == nullptr)
    {
      // FIXME
      // this is an internal error message for debugging and should be removed
      // at some point
      rust_error_at (expr->get_locus (), "failed to type resolve expression");
      return new TyTy::ErrorType (expr->get_mappings ().get_hirid ());
    }

  auto ref = expr->get_mappings ().get_hirid ();
  resolver.infered->set_ref (ref);
  resolver.context->insert_type (expr->get_mappings (), resolver.infered);

  return resolver.infered;
}

void
TypeCheckExpr::visit (HIR::TupleIndexExpr &expr)
{
  auto resolved = TypeCheckExpr::Resolve (expr.get_tuple_expr ().get ());
  if (resolved->get_kind () == TyTy::TypeKind::ERROR)
    {
      rust_error_at (expr.get_tuple_expr ()->get_locus (),
		     "failed to resolve TupleIndexExpr receiver");
      return;
    }

  // FIXME does this require autoderef here?
  if (resolved->get_kind () == TyTy::TypeKind::REF)
    {
      TyTy::ReferenceType *r = static_cast<TyTy::ReferenceType *> (resolved);
      resolved = r->get_base ();
    }

  bool is_valid_type = resolved->get_kind () == TyTy::TypeKind::ADT
		       || resolved->get_kind () == TyTy::TypeKind::TUPLE;
  if (!is_valid_type)
    {
      rust_error_at (expr.get_tuple_expr ()->get_locus (),
		     "Expected Tuple or ADT got: %s",
		     resolved->as_string ().c_str ());
      return;
    }

  if (resolved->get_kind () == TyTy::TypeKind::TUPLE)
    {
      TyTy::TupleType *tuple = static_cast<TyTy::TupleType *> (resolved);
      TupleIndex index = expr.get_tuple_index ();
      if ((size_t) index >= tuple->num_fields ())
	{
	  rust_error_at (expr.get_locus (), "unknown field at index %i", index);
	  return;
	}

      auto field_tyty = tuple->get_field ((size_t) index);
      if (field_tyty == nullptr)
	{
	  rust_error_at (expr.get_locus (),
			 "failed to lookup field type at index %i", index);
	  return;
	}

      infered = field_tyty;
      return;
    }

  TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (resolved);
  rust_assert (!adt->is_enum ());
  rust_assert (adt->number_of_variants () == 1);

  TyTy::VariantDef *variant = adt->get_variants ().at (0);
  TupleIndex index = expr.get_tuple_index ();
  if ((size_t) index >= variant->num_fields ())
    {
      rust_error_at (expr.get_locus (), "unknown field at index %i", index);
      return;
    }

  auto field_tyty = variant->get_field_at_index ((size_t) index);
  if (field_tyty == nullptr)
    {
      rust_error_at (expr.get_locus (),
		     "failed to lookup field type at index %i", index);
      return;
    }

  infered = field_tyty->get_field_type ();
}

void
TypeCheckExpr::visit (HIR::TupleExpr &expr)
{
  if (expr.is_unit ())
    {
      auto unit_node_id = resolver->get_unit_type_node_id ();
      if (!context->lookup_builtin (unit_node_id, &infered))
	{
	  rust_error_at (expr.get_locus (),
			 "failed to lookup builtin unit type");
	}
      return;
    }

  std::vector<TyTy::TyVar> fields;
  for (auto &elem : expr.get_tuple_elems ())
    {
      auto field_ty = TypeCheckExpr::Resolve (elem.get ());
      fields.push_back (TyTy::TyVar (field_ty->get_ref ()));
    }
  infered = new TyTy::TupleType (expr.get_mappings ().get_hirid (),
				 expr.get_locus (), fields);
}

void
TypeCheckExpr::visit (HIR::ReturnExpr &expr)
{
  auto fn_return_tyty = context->peek_return_type ();
  rust_assert (fn_return_tyty != nullptr);

  TyTy::BaseType *expr_ty
    = expr.has_return_expr ()
	? TypeCheckExpr::Resolve (expr.get_expr ())
	: TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());

  infered = fn_return_tyty->unify (expr_ty);
  fn_return_tyty->append_reference (expr_ty->get_ref ());
  for (auto &ref : infered->get_combined_refs ())
    fn_return_tyty->append_reference (ref);

  infered = new TyTy::NeverType (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::CallExpr &expr)
{
  TyTy::BaseType *function_tyty = TypeCheckExpr::Resolve (expr.get_fnexpr ());

  bool valid_tyty = function_tyty->get_kind () == TyTy::TypeKind::ADT
		    || function_tyty->get_kind () == TyTy::TypeKind::FNDEF
		    || function_tyty->get_kind () == TyTy::TypeKind::FNPTR;
  if (!valid_tyty)
    {
      rust_error_at (expr.get_locus (),
		     "Failed to resolve expression of function call");
      return;
    }

  TyTy::VariantDef &variant = TyTy::VariantDef::get_error_node ();
  if (function_tyty->get_kind () == TyTy::TypeKind::ADT)
    {
      TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (function_tyty);
      if (adt->is_enum ())
	{
	  // lookup variant id
	  HirId variant_id;
	  bool ok = context->lookup_variant_definition (
	    expr.get_fnexpr ()->get_mappings ().get_hirid (), &variant_id);
	  rust_assert (ok);

	  TyTy::VariantDef *lookup_variant = nullptr;
	  ok = adt->lookup_variant_by_id (variant_id, &lookup_variant);
	  rust_assert (ok);

	  variant = *lookup_variant;
	}
      else
	{
	  rust_assert (adt->number_of_variants () == 1);
	  variant = *adt->get_variants ().at (0);
	}
    }

  infered = TyTy::TypeCheckCallExpr::go (function_tyty, expr, variant, context);
}

void
TypeCheckExpr::visit (HIR::AssignmentExpr &expr)
{
  infered = TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());

  auto lhs = TypeCheckExpr::Resolve (expr.get_lhs ());
  auto rhs = TypeCheckExpr::Resolve (expr.get_rhs ());

  coercion_site (expr.get_mappings ().get_hirid (), lhs, rhs,
		 expr.get_locus ());
}

void
TypeCheckExpr::visit (HIR::CompoundAssignmentExpr &expr)
{
  infered = TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());

  auto lhs = TypeCheckExpr::Resolve (expr.get_left_expr ().get ());
  auto rhs = TypeCheckExpr::Resolve (expr.get_right_expr ().get ());

  // we dont care about the result of the unify from a compound assignment
  // since this is a unit-type expr
  auto result = lhs->unify (rhs);
  if (result->get_kind () == TyTy::TypeKind::ERROR)
    return;

  auto lang_item_type
    = Analysis::RustLangItem::CompoundAssignmentOperatorToLangItem (
      expr.get_expr_type ());
  bool operator_overloaded
    = resolve_operator_overload (lang_item_type, expr, lhs, rhs);
  if (operator_overloaded)
    return;

  bool valid_lhs = validate_arithmetic_type (lhs, expr.get_expr_type ());
  bool valid_rhs = validate_arithmetic_type (rhs, expr.get_expr_type ());
  bool valid = valid_lhs && valid_rhs;
  if (!valid)
    {
      rust_error_at (expr.get_locus (),
		     "cannot apply this operator to types %s and %s",
		     lhs->as_string ().c_str (), rhs->as_string ().c_str ());
      return;
    }
}

void
TypeCheckExpr::visit (HIR::LiteralExpr &expr)
{
  infered = resolve_literal (expr.get_mappings (), expr.get_literal (),
			     expr.get_locus ());
}

void
TypeCheckExpr::visit (HIR::ArithmeticOrLogicalExpr &expr)
{
  auto lhs = TypeCheckExpr::Resolve (expr.get_lhs ());
  auto rhs = TypeCheckExpr::Resolve (expr.get_rhs ());

  auto lang_item_type
    = Analysis::RustLangItem::OperatorToLangItem (expr.get_expr_type ());
  bool operator_overloaded
    = resolve_operator_overload (lang_item_type, expr, lhs, rhs);
  if (operator_overloaded)
    return;

  bool valid_lhs = validate_arithmetic_type (lhs, expr.get_expr_type ());
  bool valid_rhs = validate_arithmetic_type (rhs, expr.get_expr_type ());
  bool valid = valid_lhs && valid_rhs;
  if (!valid)
    {
      rust_error_at (expr.get_locus (),
		     "cannot apply this operator to types %s and %s",
		     lhs->as_string ().c_str (), rhs->as_string ().c_str ());
      return;
    }

  switch (expr.get_expr_type ())
    {
    case ArithmeticOrLogicalOperator::LEFT_SHIFT:
      case ArithmeticOrLogicalOperator::RIGHT_SHIFT: {
	TyTy::TyWithLocation from (rhs, expr.get_rhs ()->get_locus ());
	TyTy::TyWithLocation to (lhs, expr.get_lhs ()->get_locus ());
	infered = cast_site (expr.get_mappings ().get_hirid (), from, to,
			     expr.get_locus ());
      }
      break;

    default:
      infered = lhs->unify (rhs);
      break;
    }
}

void
TypeCheckExpr::visit (HIR::ComparisonExpr &expr)
{
  auto lhs = TypeCheckExpr::Resolve (expr.get_lhs ());
  auto rhs = TypeCheckExpr::Resolve (expr.get_rhs ());

  auto result = lhs->unify (rhs);
  if (result == nullptr || result->get_kind () == TyTy::TypeKind::ERROR)
    return;

  bool ok = context->lookup_builtin ("bool", &infered);
  rust_assert (ok);
}

void
TypeCheckExpr::visit (HIR::LazyBooleanExpr &expr)
{
  auto lhs = TypeCheckExpr::Resolve (expr.get_lhs ());
  auto rhs = TypeCheckExpr::Resolve (expr.get_rhs ());

  // we expect the lhs and rhs must be bools at this point
  TyTy::BoolType elhs (expr.get_mappings ().get_hirid ());
  lhs = elhs.unify (lhs);
  if (lhs->get_kind () == TyTy::TypeKind::ERROR)
    return;

  TyTy::BoolType rlhs (expr.get_mappings ().get_hirid ());
  rhs = elhs.unify (rhs);
  if (lhs->get_kind () == TyTy::TypeKind::ERROR)
    return;

  infered = lhs->unify (rhs);
}

void
TypeCheckExpr::visit (HIR::NegationExpr &expr)
{
  auto negated_expr_ty = TypeCheckExpr::Resolve (expr.get_expr ().get ());

  // check for operator overload
  auto lang_item_type = Analysis::RustLangItem::NegationOperatorToLangItem (
    expr.get_expr_type ());
  bool operator_overloaded
    = resolve_operator_overload (lang_item_type, expr, negated_expr_ty,
				 nullptr);
  if (operator_overloaded)
    return;

  // https://doc.rust-lang.org/reference/expressions/operator-expr.html#negation-operators
  switch (expr.get_expr_type ())
    {
      case NegationOperator::NEGATE: {
	bool valid
	  = (negated_expr_ty->get_kind () == TyTy::TypeKind::INT)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::UINT)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::FLOAT)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::ISIZE)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::USIZE)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::INFER
		&& (((TyTy::InferType *) negated_expr_ty)->get_infer_kind ()
		    == TyTy::InferType::INTEGRAL))
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::INFER
		&& (((TyTy::InferType *) negated_expr_ty)->get_infer_kind ()
		    == TyTy::InferType::FLOAT));
	if (!valid)
	  {
	    rust_error_at (expr.get_locus (), "cannot apply unary - to %s",
			   negated_expr_ty->as_string ().c_str ());
	    return;
	  }
      }
      break;

      case NegationOperator::NOT: {
	bool valid
	  = (negated_expr_ty->get_kind () == TyTy::TypeKind::BOOL)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::INT)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::UINT)
	    || (negated_expr_ty->get_kind () == TyTy::TypeKind::INFER
		&& (((TyTy::InferType *) negated_expr_ty)->get_infer_kind ()
		    == TyTy::InferType::INTEGRAL));
	if (!valid)
	  {
	    rust_error_at (expr.get_locus (), "cannot apply unary %<!%> to %s",
			   negated_expr_ty->as_string ().c_str ());
	    return;
	  }
      }
      break;
    }

  infered = negated_expr_ty->clone ();
  infered->append_reference (negated_expr_ty->get_ref ());
}

void
TypeCheckExpr::visit (HIR::IfExpr &expr)
{
  TypeCheckExpr::Resolve (expr.get_if_condition ());
  TypeCheckExpr::Resolve (expr.get_if_block ());

  infered = TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::IfExprConseqElse &expr)
{
  TypeCheckExpr::Resolve (expr.get_if_condition ());
  auto if_blk_resolved = TypeCheckExpr::Resolve (expr.get_if_block ());
  auto else_blk_resolved = TypeCheckExpr::Resolve (expr.get_else_block ());

  if (if_blk_resolved->get_kind () == TyTy::NEVER)
    infered = else_blk_resolved;
  else if (else_blk_resolved->get_kind () == TyTy::NEVER)
    infered = if_blk_resolved;
  else
    infered = if_blk_resolved->unify (else_blk_resolved);
}

void
TypeCheckExpr::visit (HIR::IfExprConseqIf &expr)
{
  TypeCheckExpr::Resolve (expr.get_if_condition ());
  auto if_blk_resolved = TypeCheckExpr::Resolve (expr.get_if_block ());
  auto else_blk_resolved = TypeCheckExpr::Resolve (expr.get_conseq_if_expr ());

  if (if_blk_resolved->get_kind () == TyTy::NEVER)
    infered = else_blk_resolved;
  else if (else_blk_resolved->get_kind () == TyTy::NEVER)
    infered = if_blk_resolved;
  else
    infered = if_blk_resolved->unify (else_blk_resolved);
}

void
TypeCheckExpr::visit (HIR::IfLetExpr &expr)
{
  // this needs to perform a least upper bound coercion on the blocks and then
  // unify the scruintee and arms
  TyTy::BaseType *scrutinee_tyty
    = TypeCheckExpr::Resolve (expr.get_scrutinee_expr ().get ());

  for (auto &pattern : expr.get_patterns ())
    {
      TyTy::BaseType *kase_arm_ty
	= TypeCheckPattern::Resolve (pattern.get (), scrutinee_tyty);

      TyTy::BaseType *checked_kase = scrutinee_tyty->unify (kase_arm_ty);
      if (checked_kase->get_kind () == TyTy::TypeKind::ERROR)
	return;
    }

  TypeCheckExpr::Resolve (expr.get_if_block ());

  infered = TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::UnsafeBlockExpr &expr)
{
  infered = TypeCheckExpr::Resolve (expr.get_block_expr ().get ());
}

void
TypeCheckExpr::visit (HIR::BlockExpr &expr)
{
  for (auto &s : expr.get_statements ())
    {
      if (!s->is_item ())
	continue;

      TypeCheckStmt::Resolve (s.get ());
    }

  for (auto &s : expr.get_statements ())
    {
      if (s->is_item ())
	continue;

      auto resolved = TypeCheckStmt::Resolve (s.get ());
      if (resolved == nullptr)
	{
	  rust_error_at (s->get_locus (), "failure to resolve type");
	  return;
	}

      if (s->is_unit_check_needed () && !resolved->is_unit ())
	{
	  auto unit
	    = TyTy::TupleType::get_unit_type (s->get_mappings ().get_hirid ());
	  resolved = unit->unify (resolved);
	}
    }

  if (expr.has_expr ())
    infered = TypeCheckExpr::Resolve (expr.get_final_expr ().get ())->clone ();
  else if (expr.is_tail_reachable ())
    infered
      = TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());
  else
    infered = new TyTy::NeverType (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::RangeFromToExpr &expr)
{
  auto lang_item_type = Analysis::RustLangItem::ItemType::RANGE;

  DefId respective_lang_item_id = UNKNOWN_DEFID;
  bool lang_item_defined
    = mappings->lookup_lang_item (lang_item_type, &respective_lang_item_id);

  // we need to have it maybe
  if (!lang_item_defined)
    {
      rust_internal_error_at (
	expr.get_locus (), "unable to find relevant lang item: %s",
	Analysis::RustLangItem::ToString (lang_item_type).c_str ());
      return;
    }

  // look it up and it _must_ be a struct definition
  HIR::Item *item = mappings->lookup_defid (respective_lang_item_id);
  rust_assert (item != nullptr);

  TyTy::BaseType *item_type = nullptr;
  bool ok
    = context->lookup_type (item->get_mappings ().get_hirid (), &item_type);
  rust_assert (ok);
  rust_assert (item_type->get_kind () == TyTy::TypeKind::ADT);
  TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (item_type);

  // this is a single generic item lets assert that
  rust_assert (adt->get_num_substitutions () == 1);

  // resolve the range expressions and these types must unify then we use that
  // type to substitute into the ADT
  TyTy::BaseType *from_ty
    = TypeCheckExpr::Resolve (expr.get_from_expr ().get ());
  TyTy::BaseType *to_ty = TypeCheckExpr::Resolve (expr.get_to_expr ().get ());
  TyTy::BaseType *unified = from_ty->unify (to_ty);

  // substitute it in
  std::vector<TyTy::SubstitutionArg> subst_mappings;
  const TyTy::SubstitutionParamMapping *param_ref = &adt->get_substs ().at (0);
  subst_mappings.push_back (TyTy::SubstitutionArg (param_ref, unified));

  TyTy::SubstitutionArgumentMappings subst (subst_mappings, expr.get_locus ());
  infered = SubstMapperInternal::Resolve (adt, subst);
}

void
TypeCheckExpr::visit (HIR::RangeFromExpr &expr)
{
  auto lang_item_type = Analysis::RustLangItem::ItemType::RANGE_FROM;

  DefId respective_lang_item_id = UNKNOWN_DEFID;
  bool lang_item_defined
    = mappings->lookup_lang_item (lang_item_type, &respective_lang_item_id);

  // we need to have it maybe
  if (!lang_item_defined)
    {
      rust_internal_error_at (
	expr.get_locus (), "unable to find relevant lang item: %s",
	Analysis::RustLangItem::ToString (lang_item_type).c_str ());
      return;
    }

  // look it up and it _must_ be a struct definition
  HIR::Item *item = mappings->lookup_defid (respective_lang_item_id);
  rust_assert (item != nullptr);

  TyTy::BaseType *item_type = nullptr;
  bool ok
    = context->lookup_type (item->get_mappings ().get_hirid (), &item_type);
  rust_assert (ok);
  rust_assert (item_type->get_kind () == TyTy::TypeKind::ADT);
  TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (item_type);

  // this is a single generic item lets assert that
  rust_assert (adt->get_num_substitutions () == 1);

  // resolve the range expressions and these types must unify then we use that
  // type to substitute into the ADT
  TyTy::BaseType *from_ty
    = TypeCheckExpr::Resolve (expr.get_from_expr ().get ());

  // substitute it in
  std::vector<TyTy::SubstitutionArg> subst_mappings;
  const TyTy::SubstitutionParamMapping *param_ref = &adt->get_substs ().at (0);
  subst_mappings.push_back (TyTy::SubstitutionArg (param_ref, from_ty));

  TyTy::SubstitutionArgumentMappings subst (subst_mappings, expr.get_locus ());
  infered = SubstMapperInternal::Resolve (adt, subst);
}

void
TypeCheckExpr::visit (HIR::RangeToExpr &expr)
{
  auto lang_item_type = Analysis::RustLangItem::ItemType::RANGE_TO;

  DefId respective_lang_item_id = UNKNOWN_DEFID;
  bool lang_item_defined
    = mappings->lookup_lang_item (lang_item_type, &respective_lang_item_id);

  // we need to have it maybe
  if (!lang_item_defined)
    {
      rust_internal_error_at (
	expr.get_locus (), "unable to find relevant lang item: %s",
	Analysis::RustLangItem::ToString (lang_item_type).c_str ());
      return;
    }

  // look it up and it _must_ be a struct definition
  HIR::Item *item = mappings->lookup_defid (respective_lang_item_id);
  rust_assert (item != nullptr);

  TyTy::BaseType *item_type = nullptr;
  bool ok
    = context->lookup_type (item->get_mappings ().get_hirid (), &item_type);
  rust_assert (ok);
  rust_assert (item_type->get_kind () == TyTy::TypeKind::ADT);
  TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (item_type);

  // this is a single generic item lets assert that
  rust_assert (adt->get_num_substitutions () == 1);

  // resolve the range expressions and these types must unify then we use that
  // type to substitute into the ADT
  TyTy::BaseType *from_ty = TypeCheckExpr::Resolve (expr.get_to_expr ().get ());

  // substitute it in
  std::vector<TyTy::SubstitutionArg> subst_mappings;
  const TyTy::SubstitutionParamMapping *param_ref = &adt->get_substs ().at (0);
  subst_mappings.push_back (TyTy::SubstitutionArg (param_ref, from_ty));

  TyTy::SubstitutionArgumentMappings subst (subst_mappings, expr.get_locus ());
  infered = SubstMapperInternal::Resolve (adt, subst);
}

void
TypeCheckExpr::visit (HIR::RangeFullExpr &expr)
{
  auto lang_item_type = Analysis::RustLangItem::ItemType::RANGE_FULL;

  DefId respective_lang_item_id = UNKNOWN_DEFID;
  bool lang_item_defined
    = mappings->lookup_lang_item (lang_item_type, &respective_lang_item_id);

  // we need to have it maybe
  if (!lang_item_defined)
    {
      rust_internal_error_at (
	expr.get_locus (), "unable to find relevant lang item: %s",
	Analysis::RustLangItem::ToString (lang_item_type).c_str ());
      return;
    }

  // look it up and it _must_ be a struct definition
  HIR::Item *item = mappings->lookup_defid (respective_lang_item_id);
  rust_assert (item != nullptr);

  TyTy::BaseType *item_type = nullptr;
  bool ok
    = context->lookup_type (item->get_mappings ().get_hirid (), &item_type);
  rust_assert (ok);
  rust_assert (item_type->is_unit ());

  infered = item_type;
}

void
TypeCheckExpr::visit (HIR::RangeFromToInclExpr &expr)
{
  auto lang_item_type = Analysis::RustLangItem::ItemType::RANGE_INCLUSIVE;

  DefId respective_lang_item_id = UNKNOWN_DEFID;
  bool lang_item_defined
    = mappings->lookup_lang_item (lang_item_type, &respective_lang_item_id);

  // we need to have it maybe
  if (!lang_item_defined)
    {
      rust_internal_error_at (
	expr.get_locus (), "unable to find relevant lang item: %s",
	Analysis::RustLangItem::ToString (lang_item_type).c_str ());
      return;
    }

  // look it up and it _must_ be a struct definition
  HIR::Item *item = mappings->lookup_defid (respective_lang_item_id);
  rust_assert (item != nullptr);

  TyTy::BaseType *item_type = nullptr;
  bool ok
    = context->lookup_type (item->get_mappings ().get_hirid (), &item_type);
  rust_assert (ok);
  rust_assert (item_type->get_kind () == TyTy::TypeKind::ADT);
  TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (item_type);

  // this is a single generic item lets assert that
  rust_assert (adt->get_num_substitutions () == 1);

  // resolve the range expressions and these types must unify then we use that
  // type to substitute into the ADT
  TyTy::BaseType *from_ty
    = TypeCheckExpr::Resolve (expr.get_from_expr ().get ());
  TyTy::BaseType *to_ty = TypeCheckExpr::Resolve (expr.get_to_expr ().get ());
  TyTy::BaseType *unified = from_ty->unify (to_ty);

  // substitute it in
  std::vector<TyTy::SubstitutionArg> subst_mappings;
  const TyTy::SubstitutionParamMapping *param_ref = &adt->get_substs ().at (0);
  subst_mappings.push_back (TyTy::SubstitutionArg (param_ref, unified));

  TyTy::SubstitutionArgumentMappings subst (subst_mappings, expr.get_locus ());
  infered = SubstMapperInternal::Resolve (adt, subst);
}

void
TypeCheckExpr::visit (HIR::ArrayIndexExpr &expr)
{
  auto array_expr_ty = TypeCheckExpr::Resolve (expr.get_array_expr ());
  if (array_expr_ty->get_kind () == TyTy::TypeKind::ERROR)
    return;

  auto index_expr_ty = TypeCheckExpr::Resolve (expr.get_index_expr ());
  if (index_expr_ty->get_kind () == TyTy::TypeKind::ERROR)
    return;

  // first attempt to use direct array index logic
  auto direct_array_expr_ty = array_expr_ty;
  if (direct_array_expr_ty->get_kind () == TyTy::TypeKind::REF)
    {
      // lets try and deref it since rust allows this
      auto ref = static_cast<TyTy::ReferenceType *> (direct_array_expr_ty);
      auto base = ref->get_base ();
      if (base->get_kind () == TyTy::TypeKind::ARRAY)
	direct_array_expr_ty = base;
    }

  TyTy::BaseType *size_ty;
  bool ok = context->lookup_builtin ("usize", &size_ty);
  rust_assert (ok);

  bool maybe_simple_array_access = index_expr_ty->can_eq (size_ty, false);
  if (maybe_simple_array_access
      && direct_array_expr_ty->get_kind () == TyTy::TypeKind::ARRAY)
    {
      auto resolved_index_expr = size_ty->unify (index_expr_ty);
      if (resolved_index_expr->get_kind () == TyTy::TypeKind::ERROR)
	return;

      TyTy::ArrayType *array_type
	= static_cast<TyTy::ArrayType *> (direct_array_expr_ty);
      infered = array_type->get_element_type ()->clone ();
      return;
    }

  // is this a case of core::ops::index?
  auto lang_item_type = Analysis::RustLangItem::ItemType::INDEX;
  bool operator_overloaded
    = resolve_operator_overload (lang_item_type, expr, array_expr_ty,
				 index_expr_ty);
  if (operator_overloaded)
    {
      // index and index mut always return a reference to the element
      TyTy::BaseType *resolved = infered;
      rust_assert (resolved->get_kind () == TyTy::TypeKind::REF);
      TyTy::ReferenceType *ref = static_cast<TyTy::ReferenceType *> (resolved);

      infered = ref->get_base ()->clone ();
      return;
    }

  // error[E0277]: the type `[{integer}]` cannot be indexed by `u32`
  RichLocation r (expr.get_locus ());
  r.add_range (expr.get_array_expr ()->get_locus ());
  r.add_range (expr.get_index_expr ()->get_locus ());
  rust_error_at (r, "the type %<%s%> cannot be indexed by %<%s%>",
		 array_expr_ty->get_name ().c_str (),
		 index_expr_ty->get_name ().c_str ());
}

void
TypeCheckExpr::visit (HIR::ArrayExpr &expr)
{
  HIR::ArrayElems &elements = *expr.get_internal_elements ();

  HIR::Expr *capacity_expr = nullptr;
  TyTy::BaseType *element_type = nullptr;
  switch (elements.get_array_expr_type ())
    {
      case HIR::ArrayElems::ArrayExprType::COPIED: {
	HIR::ArrayElemsCopied &elems
	  = static_cast<HIR::ArrayElemsCopied &> (elements);
	element_type = TypeCheckExpr::Resolve (elems.get_elem_to_copy ());

	auto capacity_type
	  = TypeCheckExpr::Resolve (elems.get_num_copies_expr ());

	TyTy::BaseType *expected_ty = nullptr;
	bool ok = context->lookup_builtin ("usize", &expected_ty);
	rust_assert (ok);
	context->insert_type (elems.get_num_copies_expr ()->get_mappings (),
			      expected_ty);

	auto unified = expected_ty->unify (capacity_type);
	if (unified->get_kind () == TyTy::TypeKind::ERROR)
	  return;

	capacity_expr = elems.get_num_copies_expr ();
      }
      break;

      case HIR::ArrayElems::ArrayExprType::VALUES: {
	HIR::ArrayElemsValues &elems
	  = static_cast<HIR::ArrayElemsValues &> (elements);

	std::vector<TyTy::BaseType *> types;
	for (auto &elem : elems.get_values ())
	  {
	    types.push_back (TypeCheckExpr::Resolve (elem.get ()));
	  }

	element_type
	  = TyTy::TyVar::get_implicit_infer_var (expr.get_locus ()).get_tyty ();
	for (auto &type : types)
	  {
	    element_type = element_type->unify (type);
	  }

	auto crate_num = mappings->get_current_crate ();
	Analysis::NodeMapping mapping (crate_num, UNKNOWN_NODEID,
				       mappings->get_next_hir_id (crate_num),
				       UNKNOWN_LOCAL_DEFID);
	std::string capacity_str = std::to_string (elems.get_num_elements ());
	capacity_expr = new HIR::LiteralExpr (mapping, capacity_str,
					      HIR::Literal::LitType::INT,
					      PrimitiveCoreType::CORETYPE_USIZE,
					      Location (), {});

	// mark the type for this implicit node
	TyTy::BaseType *expected_ty = nullptr;
	bool ok = context->lookup_builtin ("usize", &expected_ty);
	rust_assert (ok);
	context->insert_type (mapping, expected_ty);
      }
      break;
    }

  infered = new TyTy::ArrayType (expr.get_mappings ().get_hirid (),
				 expr.get_locus (), *capacity_expr,
				 TyTy::TyVar (element_type->get_ref ()));
}

// empty struct
void
TypeCheckExpr::visit (HIR::StructExprStruct &struct_expr)
{
  TyTy::BaseType *struct_path_ty
    = TypeCheckExpr::Resolve (&struct_expr.get_struct_name ());
  if (struct_path_ty->get_kind () != TyTy::TypeKind::ADT)
    {
      rust_error_at (struct_expr.get_struct_name ().get_locus (),
		     "expected an ADT type for constructor");
      return;
    }

  infered = struct_path_ty;
}

void
TypeCheckExpr::visit (HIR::StructExprStructFields &struct_expr)
{
  infered = TypeCheckStructExpr::Resolve (&struct_expr);
}

void
TypeCheckExpr::visit (HIR::GroupedExpr &expr)
{
  infered = TypeCheckExpr::Resolve (expr.get_expr_in_parens ().get ());
}

void
TypeCheckExpr::visit (HIR::FieldAccessExpr &expr)
{
  auto struct_base = TypeCheckExpr::Resolve (expr.get_receiver_expr ().get ());

  // FIXME does this require autoderef here?
  if (struct_base->get_kind () == TyTy::TypeKind::REF)
    {
      TyTy::ReferenceType *r = static_cast<TyTy::ReferenceType *> (struct_base);
      struct_base = r->get_base ();
    }

  bool is_valid_type = struct_base->get_kind () == TyTy::TypeKind::ADT;
  if (!is_valid_type)
    {
      rust_error_at (expr.get_locus (),
		     "expected algebraic data type got: [%s]",
		     struct_base->as_string ().c_str ());
      return;
    }

  TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (struct_base);
  rust_assert (!adt->is_enum ());
  rust_assert (adt->number_of_variants () == 1);

  TyTy::VariantDef *vaiant = adt->get_variants ().at (0);

  TyTy::StructFieldType *lookup = nullptr;
  bool found = vaiant->lookup_field (expr.get_field_name (), &lookup, nullptr);
  if (!found)
    {
      rust_error_at (expr.get_locus (), "unknown field [%s] for type [%s]",
		     expr.get_field_name ().c_str (),
		     adt->as_string ().c_str ());
      return;
    }

  infered = lookup->get_field_type ();
}

void
TypeCheckExpr::visit (HIR::MethodCallExpr &expr)
{
  auto receiver_tyty = TypeCheckExpr::Resolve (expr.get_receiver ().get ());
  if (receiver_tyty->get_kind () == TyTy::TypeKind::ERROR)
    {
      rust_error_at (expr.get_receiver ()->get_locus (),
		     "failed to resolve receiver in MethodCallExpr");
      return;
    }

  context->insert_receiver (expr.get_mappings ().get_hirid (), receiver_tyty);

  auto candidate
    = MethodResolver::Probe (receiver_tyty,
			     expr.get_method_name ().get_segment ());
  if (candidate.is_error ())
    {
      rust_error_at (
	expr.get_method_name ().get_locus (),
	"failed to resolve method for %<%s%>",
	expr.get_method_name ().get_segment ().as_string ().c_str ());
      return;
    }

  // Get the adjusted self
  Adjuster adj (receiver_tyty);
  TyTy::BaseType *adjusted_self = adj.adjust_type (candidate.adjustments);

  // store the adjustments for code-generation to know what to do which must be
  // stored onto the receiver to so as we don't trigger duplicate deref mappings
  // ICE when an argument is a method call
  HirId autoderef_mappings_id
    = expr.get_receiver ()->get_mappings ().get_hirid ();
  context->insert_autoderef_mappings (autoderef_mappings_id,
				      std::move (candidate.adjustments));

  PathProbeCandidate &resolved_candidate = candidate.candidate;
  TyTy::BaseType *lookup_tyty = candidate.candidate.ty;
  NodeId resolved_node_id
    = resolved_candidate.is_impl_candidate ()
	? resolved_candidate.item.impl.impl_item->get_impl_mappings ()
	    .get_nodeid ()
	: resolved_candidate.item.trait.item_ref->get_mappings ().get_nodeid ();

  if (lookup_tyty->get_kind () != TyTy::TypeKind::FNDEF)
    {
      RichLocation r (expr.get_method_name ().get_locus ());
      r.add_range (resolved_candidate.locus);
      rust_error_at (r, "associated impl item is not a method");
      return;
    }

  TyTy::BaseType *lookup = lookup_tyty;
  TyTy::FnType *fn = static_cast<TyTy::FnType *> (lookup);
  if (!fn->is_method ())
    {
      RichLocation r (expr.get_method_name ().get_locus ());
      r.add_range (resolved_candidate.locus);
      rust_error_at (r, "associated function is not a method");
      return;
    }

  auto root = receiver_tyty->get_root ();
  if (root->get_kind () == TyTy::TypeKind::ADT)
    {
      const TyTy::ADTType *adt = static_cast<const TyTy::ADTType *> (root);
      if (adt->has_substitutions () && fn->needs_substitution ())
	{
	  // consider the case where we have:
	  //
	  // struct Foo<X,Y>(X,Y);
	  //
	  // impl<T> Foo<T, i32> {
	  //   fn test<X>(self, a:X) -> (T,X) { (self.0, a) }
	  // }
	  //
	  // In this case we end up with an fn type of:
	  //
	  // fn <T,X> test(self:Foo<T,i32>, a:X) -> (T,X)
	  //
	  // This means the instance or self we are calling this method for
	  // will be substituted such that we can get the inherited type
	  // arguments but then need to use the turbo fish if available or
	  // infer the remaining arguments. Luckily rust does not allow for
	  // default types GenericParams on impl blocks since these must
	  // always be at the end of the list

	  auto s = fn->get_self_type ()->get_root ();
	  rust_assert (s->can_eq (adt, false));
	  rust_assert (s->get_kind () == TyTy::TypeKind::ADT);
	  const TyTy::ADTType *self_adt
	    = static_cast<const TyTy::ADTType *> (s);

	  // we need to grab the Self substitutions as the inherit type
	  // parameters for this
	  if (self_adt->needs_substitution ())
	    {
	      rust_assert (adt->was_substituted ());

	      TyTy::SubstitutionArgumentMappings used_args_in_prev_segment
		= GetUsedSubstArgs::From (adt);

	      TyTy::SubstitutionArgumentMappings inherit_type_args
		= self_adt->solve_mappings_from_receiver_for_self (
		  used_args_in_prev_segment);

	      // there may or may not be inherited type arguments
	      if (!inherit_type_args.is_error ())
		{
		  // need to apply the inherited type arguments to the
		  // function
		  lookup = fn->handle_substitions (inherit_type_args);
		}
	    }
	}
    }

  // apply any remaining generic arguments
  if (expr.get_method_name ().has_generic_args ())
    {
      HIR::GenericArgs &args = expr.get_method_name ().get_generic_args ();
      lookup
	= SubstMapper::Resolve (lookup, expr.get_method_name ().get_locus (),
				&args);
      if (lookup->get_kind () == TyTy::TypeKind::ERROR)
	return;
    }
  else if (lookup->needs_generic_substitutions ())
    {
      lookup = SubstMapper::InferSubst (lookup,
					expr.get_method_name ().get_locus ());
    }

  TyTy::BaseType *function_ret_tyty
    = TyTy::TypeCheckMethodCallExpr::go (lookup, expr, adjusted_self, context);
  if (function_ret_tyty == nullptr
      || function_ret_tyty->get_kind () == TyTy::TypeKind::ERROR)
    {
      rust_error_at (expr.get_locus (),
		     "failed to lookup type to MethodCallExpr");
      return;
    }

  // store the expected fntype
  context->insert_type (expr.get_method_name ().get_mappings (), lookup);

  // set up the resolved name on the path
  resolver->insert_resolved_name (expr.get_mappings ().get_nodeid (),
				  resolved_node_id);

  // return the result of the function back
  infered = function_ret_tyty;
}

void
TypeCheckExpr::visit (HIR::LoopExpr &expr)
{
  context->push_new_loop_context (expr.get_mappings ().get_hirid (),
				  expr.get_locus ());
  TyTy::BaseType *block_expr
    = TypeCheckExpr::Resolve (expr.get_loop_block ().get ());
  if (!block_expr->is_unit ())
    {
      rust_error_at (expr.get_loop_block ()->get_locus (),
		     "expected %<()%> got %s",
		     block_expr->as_string ().c_str ());
      return;
    }

  TyTy::BaseType *loop_context_type = context->pop_loop_context ();

  bool loop_context_type_infered
    = (loop_context_type->get_kind () != TyTy::TypeKind::INFER)
      || ((loop_context_type->get_kind () == TyTy::TypeKind::INFER)
	  && (((TyTy::InferType *) loop_context_type)->get_infer_kind ()
	      != TyTy::InferType::GENERAL));

  infered
    = loop_context_type_infered
	? loop_context_type
	: TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::WhileLoopExpr &expr)
{
  context->push_new_while_loop_context (expr.get_mappings ().get_hirid ());

  TypeCheckExpr::Resolve (expr.get_predicate_expr ().get ());
  TyTy::BaseType *block_expr
    = TypeCheckExpr::Resolve (expr.get_loop_block ().get ());

  if (!block_expr->is_unit ())
    {
      rust_error_at (expr.get_loop_block ()->get_locus (),
		     "expected %<()%> got %s",
		     block_expr->as_string ().c_str ());
      return;
    }

  context->pop_loop_context ();
  infered = TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::BreakExpr &expr)
{
  if (!context->have_loop_context ())
    {
      rust_error_at (expr.get_locus (), "cannot %<break%> outside of a loop");
      return;
    }

  if (expr.has_break_expr ())
    {
      TyTy::BaseType *break_expr_tyty
	= TypeCheckExpr::Resolve (expr.get_expr ().get ());

      TyTy::BaseType *loop_context = context->peek_loop_context ();
      if (loop_context->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (expr.get_locus (),
			 "can only break with a value inside %<loop%>");
	  return;
	}

      TyTy::BaseType *unified_ty = loop_context->unify (break_expr_tyty);
      context->swap_head_loop_context (unified_ty);
    }

  infered = new TyTy::NeverType (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::ContinueExpr &expr)
{
  if (!context->have_loop_context ())
    {
      rust_error_at (expr.get_locus (),
		     "cannot %<continue%> outside of a loop");
      return;
    }

  infered = new TyTy::NeverType (expr.get_mappings ().get_hirid ());
}

void
TypeCheckExpr::visit (HIR::BorrowExpr &expr)
{
  TyTy::BaseType *resolved_base
    = TypeCheckExpr::Resolve (expr.get_expr ().get ());

  // In Rust this is valid because of DST's
  //
  // fn test() {
  //     let a:&str = "TEST 1";
  //     let b:&str = &"TEST 2";
  // }
  if (resolved_base->get_kind () == TyTy::TypeKind::REF)
    {
      const TyTy::ReferenceType *ref
	= static_cast<const TyTy::ReferenceType *> (resolved_base);

      // this might end up being a more generic is_dyn object check but lets
      // double check dyn traits type-layout first
      if (ref->is_dyn_str_type ())
	{
	  infered = resolved_base;
	  return;
	}
    }

  if (expr.get_is_double_borrow ())
    {
      // FIXME double_reference
      gcc_unreachable ();
    }

  infered = new TyTy::ReferenceType (expr.get_mappings ().get_hirid (),
				     TyTy::TyVar (resolved_base->get_ref ()),
				     expr.get_mut ());
}

void
TypeCheckExpr::visit (HIR::DereferenceExpr &expr)
{
  TyTy::BaseType *resolved_base
    = TypeCheckExpr::Resolve (expr.get_expr ().get ());

  auto lang_item_type = Analysis::RustLangItem::ItemType::DEREF;
  bool operator_overloaded
    = resolve_operator_overload (lang_item_type, expr, resolved_base, nullptr);
  if (operator_overloaded)
    {
      // operator overloaded deref always refurns a reference type lets assert
      // this
      rust_assert (infered->get_kind () == TyTy::TypeKind::REF);
      resolved_base = infered;
    }

  bool is_valid_type = resolved_base->get_kind () == TyTy::TypeKind::REF
		       || resolved_base->get_kind () == TyTy::TypeKind::POINTER;
  if (!is_valid_type)
    {
      rust_error_at (expr.get_locus (), "expected reference type got %s",
		     resolved_base->as_string ().c_str ());
      return;
    }

  if (resolved_base->get_kind () == TyTy::TypeKind::REF)
    {
      TyTy::ReferenceType *ref_base
	= static_cast<TyTy::ReferenceType *> (resolved_base);
      infered = ref_base->get_base ()->clone ();
    }
  else
    {
      TyTy::PointerType *ref_base
	= static_cast<TyTy::PointerType *> (resolved_base);
      infered = ref_base->get_base ()->clone ();
    }
}

void
TypeCheckExpr::visit (HIR::TypeCastExpr &expr)
{
  TyTy::BaseType *expr_to_convert
    = TypeCheckExpr::Resolve (expr.get_casted_expr ().get ());
  TyTy::BaseType *tyty_to_convert_to
    = TypeCheckType::Resolve (expr.get_type_to_convert_to ().get ());

  TyTy::TyWithLocation from (expr_to_convert,
			     expr.get_casted_expr ()->get_locus ());
  TyTy::TyWithLocation to (tyty_to_convert_to,
			   expr.get_type_to_convert_to ()->get_locus ());
  infered = cast_site (expr.get_mappings ().get_hirid (), from, to,
		       expr.get_locus ());
}

void
TypeCheckExpr::visit (HIR::MatchExpr &expr)
{
  // this needs to perform a least upper bound coercion on the blocks and then
  // unify the scruintee and arms
  TyTy::BaseType *scrutinee_tyty
    = TypeCheckExpr::Resolve (expr.get_scrutinee_expr ().get ());

  std::vector<TyTy::BaseType *> kase_block_tys;
  for (auto &kase : expr.get_match_cases ())
    {
      // lets check the arms
      HIR::MatchArm &kase_arm = kase.get_arm ();
      for (auto &pattern : kase_arm.get_patterns ())
	{
	  TyTy::BaseType *kase_arm_ty
	    = TypeCheckPattern::Resolve (pattern.get (), scrutinee_tyty);

	  TyTy::BaseType *checked_kase = scrutinee_tyty->unify (kase_arm_ty);
	  if (checked_kase->get_kind () == TyTy::TypeKind::ERROR)
	    return;
	}

      // check the kase type
      TyTy::BaseType *kase_block_ty
	= TypeCheckExpr::Resolve (kase.get_expr ().get ());
      kase_block_tys.push_back (kase_block_ty);
    }

  if (kase_block_tys.size () == 0)
    {
      infered
	= TyTy::TupleType::get_unit_type (expr.get_mappings ().get_hirid ());
      return;
    }

  infered = kase_block_tys.at (0);
  for (size_t i = 1; i < kase_block_tys.size (); i++)
    {
      TyTy::BaseType *kase_ty = kase_block_tys.at (i);
      infered = infered->unify (kase_ty);
      if (infered->get_kind () == TyTy::TypeKind::ERROR)
	return;
    }
}

bool
TypeCheckExpr::resolve_operator_overload (
  Analysis::RustLangItem::ItemType lang_item_type, HIR::OperatorExprMeta expr,
  TyTy::BaseType *lhs, TyTy::BaseType *rhs)
{
  // look up lang item for arithmetic type
  std::string associated_item_name
    = Analysis::RustLangItem::ToString (lang_item_type);
  DefId respective_lang_item_id = UNKNOWN_DEFID;
  bool lang_item_defined
    = mappings->lookup_lang_item (lang_item_type, &respective_lang_item_id);

  // probe for the lang-item
  if (!lang_item_defined)
    return false;

  auto segment = HIR::PathIdentSegment (associated_item_name);
  auto candidate
    = MethodResolver::Probe (lhs, HIR::PathIdentSegment (associated_item_name));

  bool have_implementation_for_lang_item = !candidate.is_error ();
  if (!have_implementation_for_lang_item)
    return false;

  // Get the adjusted self
  Adjuster adj (lhs);
  TyTy::BaseType *adjusted_self = adj.adjust_type (candidate.adjustments);

  // is this the case we are recursive
  // handle the case where we are within the impl block for this lang_item
  // otherwise we end up with a recursive operator overload such as the i32
  // operator overload trait
  TypeCheckContextItem &fn_context = context->peek_context ();
  if (fn_context.get_type () == TypeCheckContextItem::ItemType::IMPL_ITEM)
    {
      auto &impl_item = fn_context.get_impl_item ();
      HIR::ImplBlock *parent = impl_item.first;
      HIR::Function *fn = impl_item.second;

      if (parent->has_trait_ref ()
	  && fn->get_function_name ().compare (associated_item_name) == 0)
	{
	  TraitReference *trait_reference
	    = TraitResolver::Lookup (*parent->get_trait_ref ().get ());
	  if (!trait_reference->is_error ())
	    {
	      TyTy::BaseType *lookup = nullptr;
	      bool ok = context->lookup_type (fn->get_mappings ().get_hirid (),
					      &lookup);
	      rust_assert (ok);
	      rust_assert (lookup->get_kind () == TyTy::TypeKind::FNDEF);

	      TyTy::FnType *fntype = static_cast<TyTy::FnType *> (lookup);
	      rust_assert (fntype->is_method ());

	      bool is_lang_item_impl
		= trait_reference->get_mappings ().get_defid ()
		  == respective_lang_item_id;
	      bool self_is_lang_item_self
		= fntype->get_self_type ()->is_equal (*adjusted_self);
	      bool recursive_operator_overload
		= is_lang_item_impl && self_is_lang_item_self;

	      if (recursive_operator_overload)
		return false;
	    }
	}
    }

  // store the adjustments for code-generation to know what to do
  context->insert_autoderef_mappings (expr.get_lvalue_mappings ().get_hirid (),
				      std::move (candidate.adjustments));

  // now its just like a method-call-expr
  context->insert_receiver (expr.get_mappings ().get_hirid (), lhs);

  PathProbeCandidate &resolved_candidate = candidate.candidate;
  TyTy::BaseType *lookup_tyty = candidate.candidate.ty;
  NodeId resolved_node_id
    = resolved_candidate.is_impl_candidate ()
	? resolved_candidate.item.impl.impl_item->get_impl_mappings ()
	    .get_nodeid ()
	: resolved_candidate.item.trait.item_ref->get_mappings ().get_nodeid ();

  rust_assert (lookup_tyty->get_kind () == TyTy::TypeKind::FNDEF);
  TyTy::BaseType *lookup = lookup_tyty;
  TyTy::FnType *fn = static_cast<TyTy::FnType *> (lookup);
  rust_assert (fn->is_method ());

  auto root = lhs->get_root ();
  if (root->get_kind () == TyTy::TypeKind::ADT)
    {
      const TyTy::ADTType *adt = static_cast<const TyTy::ADTType *> (root);
      if (adt->has_substitutions () && fn->needs_substitution ())
	{
	  // consider the case where we have:
	  //
	  // struct Foo<X,Y>(X,Y);
	  //
	  // impl<T> Foo<T, i32> {
	  //   fn test<X>(self, a:X) -> (T,X) { (self.0, a) }
	  // }
	  //
	  // In this case we end up with an fn type of:
	  //
	  // fn <T,X> test(self:Foo<T,i32>, a:X) -> (T,X)
	  //
	  // This means the instance or self we are calling this method for
	  // will be substituted such that we can get the inherited type
	  // arguments but then need to use the turbo fish if available or
	  // infer the remaining arguments. Luckily rust does not allow for
	  // default types GenericParams on impl blocks since these must
	  // always be at the end of the list

	  auto s = fn->get_self_type ()->get_root ();
	  rust_assert (s->can_eq (adt, false));
	  rust_assert (s->get_kind () == TyTy::TypeKind::ADT);
	  const TyTy::ADTType *self_adt
	    = static_cast<const TyTy::ADTType *> (s);

	  // we need to grab the Self substitutions as the inherit type
	  // parameters for this
	  if (self_adt->needs_substitution ())
	    {
	      rust_assert (adt->was_substituted ());

	      TyTy::SubstitutionArgumentMappings used_args_in_prev_segment
		= GetUsedSubstArgs::From (adt);

	      TyTy::SubstitutionArgumentMappings inherit_type_args
		= self_adt->solve_mappings_from_receiver_for_self (
		  used_args_in_prev_segment);

	      // there may or may not be inherited type arguments
	      if (!inherit_type_args.is_error ())
		{
		  // need to apply the inherited type arguments to the
		  // function
		  lookup = fn->handle_substitions (inherit_type_args);
		}
	    }
	}
    }

  // handle generics
  if (lookup->needs_generic_substitutions ())
    lookup = SubstMapper::InferSubst (lookup, expr.get_locus ());

  // type check the arguments if required
  TyTy::FnType *type = static_cast<TyTy::FnType *> (lookup);
  rust_assert (type->num_params () > 0);
  auto fnparam = type->param_at (0);
  fnparam.second->unify (adjusted_self); // typecheck the self
  if (rhs == nullptr)
    {
      rust_assert (type->num_params () == 1);
    }
  else
    {
      rust_assert (type->num_params () == 2);
      auto fnparam = type->param_at (1);
      fnparam.second->unify (rhs); // typecheck the rhs
    }

  rust_assert (lookup->get_kind () == TyTy::TypeKind::FNDEF);
  fn = static_cast<TyTy::FnType *> (lookup);
  fn->monomorphize ();

  // get the return type
  TyTy::BaseType *function_ret_tyty
    = type->get_return_type ()->monomorphized_clone ();

  // store the expected fntype
  context->insert_operator_overload (expr.get_mappings ().get_hirid (), type);

  // set up the resolved name on the path
  resolver->insert_resolved_name (expr.get_mappings ().get_nodeid (),
				  resolved_node_id);

  // return the result of the function back
  infered = function_ret_tyty;

  return true;
}

bool
TypeCheckExpr::validate_arithmetic_type (
  const TyTy::BaseType *tyty, HIR::ArithmeticOrLogicalExpr::ExprType expr_type)
{
  const TyTy::BaseType *type = tyty->destructure ();

  // https://doc.rust-lang.org/reference/expressions/operator-expr.html#arithmetic-and-logical-binary-operators
  // this will change later when traits are added
  switch (expr_type)
    {
    case ArithmeticOrLogicalOperator::ADD:
    case ArithmeticOrLogicalOperator::SUBTRACT:
    case ArithmeticOrLogicalOperator::MULTIPLY:
    case ArithmeticOrLogicalOperator::DIVIDE:
    case ArithmeticOrLogicalOperator::MODULUS:
      return (type->get_kind () == TyTy::TypeKind::INT)
	     || (type->get_kind () == TyTy::TypeKind::UINT)
	     || (type->get_kind () == TyTy::TypeKind::FLOAT)
	     || (type->get_kind () == TyTy::TypeKind::USIZE)
	     || (type->get_kind () == TyTy::TypeKind::ISIZE)
	     || (type->get_kind () == TyTy::TypeKind::INFER
		 && (((const TyTy::InferType *) type)->get_infer_kind ()
		     == TyTy::InferType::INTEGRAL))
	     || (type->get_kind () == TyTy::TypeKind::INFER
		 && (((const TyTy::InferType *) type)->get_infer_kind ()
		     == TyTy::InferType::FLOAT));

      // integers or bools
    case ArithmeticOrLogicalOperator::BITWISE_AND:
    case ArithmeticOrLogicalOperator::BITWISE_OR:
    case ArithmeticOrLogicalOperator::BITWISE_XOR:
      return (type->get_kind () == TyTy::TypeKind::INT)
	     || (type->get_kind () == TyTy::TypeKind::UINT)
	     || (type->get_kind () == TyTy::TypeKind::USIZE)
	     || (type->get_kind () == TyTy::TypeKind::ISIZE)
	     || (type->get_kind () == TyTy::TypeKind::BOOL)
	     || (type->get_kind () == TyTy::TypeKind::INFER
		 && (((const TyTy::InferType *) type)->get_infer_kind ()
		     == TyTy::InferType::INTEGRAL));

      // integers only
    case ArithmeticOrLogicalOperator::LEFT_SHIFT:
    case ArithmeticOrLogicalOperator::RIGHT_SHIFT:
      return (type->get_kind () == TyTy::TypeKind::INT)
	     || (type->get_kind () == TyTy::TypeKind::UINT)
	     || (type->get_kind () == TyTy::TypeKind::USIZE)
	     || (type->get_kind () == TyTy::TypeKind::ISIZE)
	     || (type->get_kind () == TyTy::TypeKind::INFER
		 && (((const TyTy::InferType *) type)->get_infer_kind ()
		     == TyTy::InferType::INTEGRAL));
    }

  gcc_unreachable ();
  return false;
}

} // namespace Resolver
} // namespace Rust
