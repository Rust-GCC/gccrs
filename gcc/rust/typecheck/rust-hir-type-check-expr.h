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

#ifndef RUST_HIR_TYPE_CHECK_EXPR
#define RUST_HIR_TYPE_CHECK_EXPR

#include "rust-hir-type-check-base.h"
#include "rust-hir-full.h"
#include "rust-system.h"
#include "rust-tyty.h"
#include "rust-tyty-call.h"
#include "rust-hir-type-check-struct-field.h"
#include "rust-hir-path-probe.h"
#include "rust-substitution-mapper.h"
#include "rust-hir-const-fold.h"
#include "rust-hir-trait-resolve.h"
#include "rust-hir-type-bounds.h"
#include "rust-hir-dot-operator.h"

namespace Rust {
namespace Resolver {

class TypeCheckExpr : public TypeCheckBase
{
  using Rust::Resolver::TypeCheckBase::visit;

public:
  /* Perform type checking on expr. Also runs type unification algorithm.
     Returns the unified type of expr.
     `inside_loop` acts as a context for BreakExpr, etc. May change later. */
  static TyTy::BaseType *Resolve (HIR::Expr *expr, bool inside_loop)
  {
    TypeCheckExpr resolver (inside_loop);
    expr->accept_vis (resolver);

    if (resolver.infered == nullptr)
      {
	rust_error_at (expr->get_locus (), "failed to type resolve expression");
	return new TyTy::ErrorType (expr->get_mappings ().get_hirid ());
      }

    auto ref = expr->get_mappings ().get_hirid ();
    resolver.infered->set_ref (ref);
    resolver.context->insert_type (expr->get_mappings (), resolver.infered);

    return resolver.infered;
  }

  void visit (HIR::TupleIndexExpr &expr) override
  {
    auto resolved
      = TypeCheckExpr::Resolve (expr.get_tuple_expr ().get (), inside_loop);
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
	    rust_error_at (expr.get_locus (), "unknown field at index %i",
			   index);
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
    TupleIndex index = expr.get_tuple_index ();
    if ((size_t) index >= adt->num_fields ())
      {
	rust_error_at (expr.get_locus (), "unknown field at index %i", index);
	return;
      }

    auto field_tyty = adt->get_field ((size_t) index);
    if (field_tyty == nullptr)
      {
	rust_error_at (expr.get_locus (),
		       "failed to lookup field type at index %i", index);
	return;
      }

    infered = field_tyty->get_field_type ();
  }

  void visit (HIR::TupleExpr &expr) override
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
	auto field_ty = TypeCheckExpr::Resolve (elem.get (), false);
	fields.push_back (TyTy::TyVar (field_ty->get_ref ()));
      }
    infered = new TyTy::TupleType (expr.get_mappings ().get_hirid (), fields);
  }

  void visit (HIR::ReturnExpr &expr) override
  {
    auto fn_return_tyty = context->peek_return_type ();
    rust_assert (fn_return_tyty != nullptr);

    TyTy::BaseType *expr_ty;
    if (expr.has_return_expr ())
      expr_ty = TypeCheckExpr::Resolve (expr.get_expr (), false);
    else
      expr_ty = new TyTy::TupleType (expr.get_mappings ().get_hirid ());

    if (expr_ty == nullptr)
      {
	rust_error_at (expr.get_locus (),
		       "failed to resolve type for ReturnExpr");
	return;
      }

    infered = fn_return_tyty->unify (expr_ty);
    fn_return_tyty->append_reference (expr_ty->get_ref ());
    for (auto &ref : infered->get_combined_refs ())
      fn_return_tyty->append_reference (ref);

    infered = new TyTy::NeverType (expr.get_mappings ().get_hirid ());
  }

  void visit (HIR::CallExpr &expr) override
  {
    TyTy::BaseType *function_tyty
      = TypeCheckExpr::Resolve (expr.get_fnexpr (), false);

    bool valid_tyty = function_tyty->get_kind () == TyTy::TypeKind::ADT
		      || function_tyty->get_kind () == TyTy::TypeKind::FNDEF
		      || function_tyty->get_kind () == TyTy::TypeKind::FNPTR;
    if (!valid_tyty)
      {
	rust_error_at (expr.get_locus (),
		       "Failed to resolve expression of function call");
	return;
      }

    infered = TyTy::TypeCheckCallExpr::go (function_tyty, expr, context);
    if (infered == nullptr)
      {
	rust_error_at (expr.get_locus (), "failed to lookup type to CallExpr");
	return;
      }

    infered->set_ref (expr.get_mappings ().get_hirid ());
  }

  void visit (HIR::MethodCallExpr &expr) override
  {
    auto receiver_tyty
      = TypeCheckExpr::Resolve (expr.get_receiver ().get (), false);
    if (receiver_tyty->get_kind () == TyTy::TypeKind::ERROR)
      {
	rust_error_at (expr.get_receiver ()->get_locus (),
		       "failed to resolve receiver in MethodCallExpr");
	return;
      }

    context->insert_receiver (expr.get_mappings ().get_hirid (), receiver_tyty);

    // in order to probe of the correct type paths we need the root type, which
    // strips any references
    TyTy::BaseType *root = receiver_tyty->get_root ();

    // https://doc.rust-lang.org/reference/expressions/method-call-expr.html
    // method resolution is complex in rust once we start handling generics and
    // traits. For now we only support looking up the valid name in impl blocks
    // which is simple. There will need to be adjustments to ensure we can turn
    // the receiver into borrowed references etc

    bool receiver_is_type_param = root->get_kind () == TyTy::TypeKind::PARAM;
    bool receiver_is_dyn = root->get_kind () == TyTy::TypeKind::DYNAMIC;

    bool receiver_is_generic = receiver_is_type_param || receiver_is_dyn;
    bool probe_bounds = true;
    bool probe_impls = !receiver_is_generic;
    bool ignore_mandatory_trait_items = !receiver_is_generic;

    auto candidates
      = PathProbeType::Probe (root, expr.get_method_name ().get_segment (),
			      probe_impls, probe_bounds,
			      ignore_mandatory_trait_items);
    if (candidates.empty ())
      {
	rust_error_at (expr.get_locus (),
		       "failed to resolve the PathExprSegment to any item");
	return;
      }

    std::vector<Adjustment> adjustments;
    PathProbeCandidate *resolved_candidate
      = MethodResolution::Select (candidates, receiver_tyty, adjustments);
    if (resolved_candidate == nullptr)
      {
	if (candidates.size () > 1)
	  {
	    // not sure if this is the correct error here
	    ReportMultipleCandidateError::Report (
	      candidates, expr.get_method_name ().get_segment (),
	      expr.get_method_name ().get_locus ());
	  }
	else
	  {
	    rust_error_at (expr.get_locus (), "failed to resolve method");
	  }
	return;
      }

    // store the adjustments for code-generation to know what to do
    context->insert_autoderef_mappings (expr.get_mappings ().get_hirid (),
					std::move (adjustments));

    TyTy::BaseType *lookup_tyty = resolved_candidate->ty;
    NodeId resolved_node_id
      = resolved_candidate->is_impl_candidate ()
	  ? resolved_candidate->item.impl.impl_item->get_impl_mappings ()
	      .get_nodeid ()
	  : resolved_candidate->item.trait.item_ref->get_mappings ()
	      .get_nodeid ();

    if (lookup_tyty->get_kind () != TyTy::TypeKind::FNDEF)
      {
	RichLocation r (expr.get_method_name ().get_locus ());
	r.add_range (resolved_candidate->locus);
	rust_error_at (r, "associated impl item is not a method");
	return;
      }

    TyTy::BaseType *lookup = lookup_tyty;
    TyTy::FnType *fn = static_cast<TyTy::FnType *> (lookup);
    if (!fn->is_method ())
      {
	RichLocation r (expr.get_method_name ().get_locus ());
	r.add_range (resolved_candidate->locus);
	rust_error_at (r, "associated function is not a method");
	return;
      }

    if (root->get_kind () == TyTy::TypeKind::ADT)
      {
	TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (root);
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
	    TyTy::ADTType *self_adt = static_cast<TyTy::ADTType *> (s);

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

    if (!receiver_is_type_param)
      {
	// apply any remaining generic arguments
	if (expr.get_method_name ().has_generic_args ())
	  {
	    HIR::GenericArgs &args
	      = expr.get_method_name ().get_generic_args ();
	    lookup = SubstMapper::Resolve (lookup,
					   expr.get_method_name ().get_locus (),
					   &args);
	    if (lookup->get_kind () == TyTy::TypeKind::ERROR)
	      return;
	  }
	else if (lookup->needs_generic_substitutions ())
	  {
	    lookup
	      = SubstMapper::InferSubst (lookup,
					 expr.get_method_name ().get_locus ());
	  }
      }

    TyTy::BaseType *function_ret_tyty
      = TyTy::TypeCheckMethodCallExpr::go (lookup, expr, context);
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

  void visit (HIR::AssignmentExpr &expr) override
  {
    infered = new TyTy::TupleType (expr.get_mappings ().get_hirid ());

    auto lhs = TypeCheckExpr::Resolve (expr.get_lhs (), false);
    auto rhs = TypeCheckExpr::Resolve (expr.get_rhs (), false);

    auto result = lhs->coerce (rhs);
    if (result->get_kind () == TyTy::TypeKind::ERROR)
      return;

    // in the case of declare first for an ADT Type:
    //
    // let a;
    // a = Foo{..}
    // let b = a.field;
    //
    // The lhs will have a TyTy of INFER and so when the declaration is
    // referenced it will still have an unknown type so we will fail to resolve
    // FieldAccessExpr

    NodeId ast_node_id = expr.get_lhs ()->get_mappings ().get_nodeid ();
    NodeId ref_node_id;
    if (!resolver->lookup_resolved_name (ast_node_id, &ref_node_id))
      return;

    Definition def;
    if (!resolver->lookup_definition (ref_node_id, &def))
      {
	rust_error_at (expr.get_locus (),
		       "assignment infer - unknown reference");
	return;
      }

    HirId ref;
    if (!mappings->lookup_node_to_hir (expr.get_mappings ().get_crate_num (),
				       def.parent, &ref))
      {
	rust_error_at (expr.get_locus (),
		       "assignment infer - reverse lookup failure");
	return;
      }

    context->insert_type (
      Analysis::NodeMapping (expr.get_lhs ()->get_mappings ().get_crate_num (),
			     ref_node_id, ref, UNKNOWN_LOCAL_DEFID),
      result->clone ());
  }

  void visit (HIR::IdentifierExpr &expr) override
  {
    NodeId ast_node_id = expr.get_mappings ().get_nodeid ();

    // then lookup the reference_node_id
    NodeId ref_node_id = UNKNOWN_NODEID;
    if (resolver->lookup_resolved_name (ast_node_id, &ref_node_id))
      {
	// these ref_node_ids will resolve to a pattern declaration but we are
	// interested in the definition that this refers to get the parent id
	Definition def;
	if (!resolver->lookup_definition (ref_node_id, &def))
	  {
	    rust_error_at (expr.get_locus (),
			   "unknown reference for resolved name");
	    return;
	  }
	ref_node_id = def.parent;
      }
    else if (!resolver->lookup_resolved_type (ast_node_id, &ref_node_id))
      {
	rust_error_at (expr.get_locus (),
		       "Failed to lookup type reference for node: %s",
		       expr.as_string ().c_str ());
	return;
      }

    if (ref_node_id == UNKNOWN_NODEID)
      {
	rust_error_at (expr.get_locus (), "unresolved node: %s",
		       expr.as_string ().c_str ());
	return;
      }

    // node back to HIR
    HirId ref;
    if (!mappings->lookup_node_to_hir (expr.get_mappings ().get_crate_num (),
				       ref_node_id, &ref))
      {
	rust_error_at (expr.get_locus (), "123 reverse lookup failure");
	return;
      }

    // the base reference for this name _must_ have a type set
    TyTy::BaseType *lookup;
    if (!context->lookup_type (ref, &lookup))
      {
	rust_error_at (mappings->lookup_location (ref),
		       "Failed to resolve IdentifierExpr type: %s",
		       expr.as_string ().c_str ());
	return;
      }

    infered = lookup->clone ();
  }

  void visit (HIR::LiteralExpr &expr) override
  {
    switch (expr.get_lit_type ())
      {
	case HIR::Literal::LitType::INT: {
	  bool ok = false;

	  switch (expr.get_literal ()->get_type_hint ())
	    {
	    case CORETYPE_I8:
	      ok = context->lookup_builtin ("i8", &infered);
	      break;
	    case CORETYPE_I16:
	      ok = context->lookup_builtin ("i16", &infered);
	      break;
	    case CORETYPE_I32:
	      ok = context->lookup_builtin ("i32", &infered);
	      break;
	    case CORETYPE_I64:
	      ok = context->lookup_builtin ("i64", &infered);
	      break;
	    case CORETYPE_I128:
	      ok = context->lookup_builtin ("i128", &infered);
	      break;

	    case CORETYPE_U8:
	      ok = context->lookup_builtin ("u8", &infered);
	      break;
	    case CORETYPE_U16:
	      ok = context->lookup_builtin ("u16", &infered);
	      break;
	    case CORETYPE_U32:
	      ok = context->lookup_builtin ("u32", &infered);
	      break;
	    case CORETYPE_U64:
	      ok = context->lookup_builtin ("u64", &infered);
	      break;
	    case CORETYPE_U128:
	      ok = context->lookup_builtin ("u128", &infered);
	      break;

	    case CORETYPE_F32:
	      expr.get_literal ()->set_lit_type (HIR::Literal::LitType::FLOAT);
	      ok = context->lookup_builtin ("f32", &infered);
	      break;
	    case CORETYPE_F64:
	      expr.get_literal ()->set_lit_type (HIR::Literal::LitType::FLOAT);
	      ok = context->lookup_builtin ("f64", &infered);
	      break;

	    default:
	      ok = true;
	      infered = new TyTy::InferType (
		expr.get_mappings ().get_hirid (),
		TyTy::InferType::InferTypeKind::INTEGRAL);
	      break;
	    }
	  rust_assert (ok);
	}
	break;

	case HIR::Literal::LitType::FLOAT: {
	  bool ok = false;

	  switch (expr.get_literal ()->get_type_hint ())
	    {
	    case CORETYPE_F32:
	      ok = context->lookup_builtin ("f32", &infered);
	      break;
	    case CORETYPE_F64:
	      ok = context->lookup_builtin ("f64", &infered);
	      break;

	    default:
	      ok = true;
	      infered
		= new TyTy::InferType (expr.get_mappings ().get_hirid (),
				       TyTy::InferType::InferTypeKind::FLOAT);
	      break;
	    }
	  rust_assert (ok);
	}
	break;

	case HIR::Literal::LitType::BOOL: {
	  auto ok = context->lookup_builtin ("bool", &infered);
	  rust_assert (ok);
	}
	break;

	case HIR::Literal::LitType::CHAR: {
	  auto ok = context->lookup_builtin ("char", &infered);
	  rust_assert (ok);
	}
	break;

	case HIR::Literal::LitType::BYTE: {
	  auto ok = context->lookup_builtin ("u8", &infered);
	  rust_assert (ok);
	}
	break;

	case HIR::Literal::LitType::STRING: {
	  TyTy::BaseType *base = nullptr;
	  auto ok = context->lookup_builtin ("str", &base);
	  rust_assert (ok);

	  infered
	    = new TyTy::ReferenceType (expr.get_mappings ().get_hirid (),
				       TyTy::TyVar (base->get_ref ()), false);
	}
	break;

	case HIR::Literal::LitType::BYTE_STRING: {
	  /* This is an arraytype of u8 reference (&[u8;size]). It isn't in
	     UTF-8, but really just a byte array. Code to construct the array
	     reference copied from ArrayElemsValues and ArrayType. */
	  TyTy::BaseType *u8;
	  auto ok = context->lookup_builtin ("u8", &u8);
	  rust_assert (ok);

	  auto crate_num = mappings->get_current_crate ();
	  Analysis::NodeMapping capacity_mapping (crate_num, UNKNOWN_NODEID,
						  mappings->get_next_hir_id (
						    crate_num),
						  UNKNOWN_LOCAL_DEFID);

	  /* Capacity is the size of the string (number of chars).
	     It is a constant, but for fold it to get a Bexpression.  */
	  std::string capacity_str
	    = std::to_string (expr.get_literal ()->as_string ().size ());
	  HIR::LiteralExpr literal_capacity (capacity_mapping, capacity_str,
					     HIR::Literal::LitType::INT,
					     PrimitiveCoreType::CORETYPE_USIZE,
					     expr.get_locus ());

	  // mark the type for this implicit node
	  context->insert_type (capacity_mapping,
				new TyTy::USizeType (
				  capacity_mapping.get_hirid ()));

	  Bexpression *capacity
	    = ConstFold::ConstFoldExpr::fold (&literal_capacity);

	  Analysis::NodeMapping array_mapping (crate_num, UNKNOWN_NODEID,
					       mappings->get_next_hir_id (
						 crate_num),
					       UNKNOWN_LOCAL_DEFID);

	  TyTy::ArrayType *array
	    = new TyTy::ArrayType (array_mapping.get_hirid (), capacity,
				   TyTy::TyVar (u8->get_ref ()));
	  context->insert_type (array_mapping, array);

	  infered
	    = new TyTy::ReferenceType (expr.get_mappings ().get_hirid (),
				       TyTy::TyVar (array->get_ref ()), false);
	}
	break;

      default:
	gcc_unreachable ();
	break;
      }

    infered = infered->clone ();
  }

  void visit (HIR::ArithmeticOrLogicalExpr &expr) override
  {
    auto lhs = TypeCheckExpr::Resolve (expr.get_lhs (), false);
    auto rhs = TypeCheckExpr::Resolve (expr.get_rhs (), false);

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

    infered = lhs->unify (rhs);
    infered->append_reference (lhs->get_ref ());
    infered->append_reference (rhs->get_ref ());
  }

  void visit (HIR::ComparisonExpr &expr) override
  {
    auto lhs = TypeCheckExpr::Resolve (expr.get_lhs (), false);
    auto rhs = TypeCheckExpr::Resolve (expr.get_rhs (), false);

    auto result = lhs->unify (rhs);
    if (result == nullptr || result->get_kind () == TyTy::TypeKind::ERROR)
      return;

    bool ok = context->lookup_builtin ("bool", &infered);
    rust_assert (ok);
  }

  void visit (HIR::LazyBooleanExpr &expr) override
  {
    auto lhs = TypeCheckExpr::Resolve (expr.get_lhs (), false);
    auto rhs = TypeCheckExpr::Resolve (expr.get_rhs (), false);

    // we expect the lhs and rhs must be bools at this point
    TyTy::BoolType elhs (expr.get_mappings ().get_hirid ());
    lhs = elhs.unify (lhs);
    if (lhs == nullptr || lhs->get_kind () == TyTy::TypeKind::ERROR)
      return;

    TyTy::BoolType rlhs (expr.get_mappings ().get_hirid ());
    rhs = elhs.unify (rhs);
    if (lhs == nullptr || lhs->get_kind () == TyTy::TypeKind::ERROR)
      return;

    infered = lhs->unify (rhs);
    infered->append_reference (lhs->get_ref ());
    infered->append_reference (rhs->get_ref ());
  }

  void visit (HIR::NegationExpr &expr) override
  {
    auto negated_expr_ty
      = TypeCheckExpr::Resolve (expr.get_expr ().get (), false);

    // https://doc.rust-lang.org/reference/expressions/operator-expr.html#negation-operators
    switch (expr.get_expr_type ())
      {
	case NegationOperator::NEGATE: {
	  bool valid
	    = (negated_expr_ty->get_kind () == TyTy::TypeKind::INT)
	      || (negated_expr_ty->get_kind () == TyTy::TypeKind::UINT)
	      || (negated_expr_ty->get_kind () == TyTy::TypeKind::FLOAT)
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
	      rust_error_at (expr.get_locus (),
			     "cannot apply unary %<!%> to %s",
			     negated_expr_ty->as_string ().c_str ());
	      return;
	    }
	}
	break;
      }

    infered = negated_expr_ty->clone ();
    infered->append_reference (negated_expr_ty->get_ref ());
  }

  void visit (HIR::IfExpr &expr) override
  {
    TypeCheckExpr::Resolve (expr.get_if_condition (), false);
    TypeCheckExpr::Resolve (expr.get_if_block (), inside_loop);

    infered = new TyTy::TupleType (expr.get_mappings ().get_hirid ());
  }

  void visit (HIR::IfExprConseqElse &expr) override
  {
    TypeCheckExpr::Resolve (expr.get_if_condition (), false);
    auto if_blk_resolved
      = TypeCheckExpr::Resolve (expr.get_if_block (), inside_loop);
    auto else_blk_resolved
      = TypeCheckExpr::Resolve (expr.get_else_block (), inside_loop);

    if (if_blk_resolved->get_kind () == TyTy::NEVER)
      infered = else_blk_resolved;
    else if (else_blk_resolved->get_kind () == TyTy::NEVER)
      infered = if_blk_resolved;
    else
      infered = if_blk_resolved->unify (else_blk_resolved);
  }

  void visit (HIR::IfExprConseqIf &expr) override
  {
    TypeCheckExpr::Resolve (expr.get_if_condition (), false);
    auto if_blk_resolved
      = TypeCheckExpr::Resolve (expr.get_if_block (), inside_loop);
    auto else_blk_resolved
      = TypeCheckExpr::Resolve (expr.get_conseq_if_expr (), inside_loop);

    if (if_blk_resolved->get_kind () == TyTy::NEVER)
      infered = else_blk_resolved;
    else if (else_blk_resolved->get_kind () == TyTy::NEVER)
      infered = if_blk_resolved;
    else
      infered = if_blk_resolved->unify (else_blk_resolved);
  }

  void visit (HIR::BlockExpr &expr) override;

  void visit (HIR::UnsafeBlockExpr &expr) override
  {
    infered
      = TypeCheckExpr::Resolve (expr.get_block_expr ().get (), inside_loop);
  }

  void visit (HIR::ArrayIndexExpr &expr) override
  {
    TyTy::BaseType *size_ty;
    if (!context->lookup_builtin ("usize", &size_ty))
      {
	rust_error_at (
	  expr.get_locus (),
	  "Failure looking up size type for index in ArrayIndexExpr");
	return;
      }

    auto resolved_index_expr
      = size_ty->unify (TypeCheckExpr::Resolve (expr.get_index_expr (), false));
    if (resolved_index_expr == nullptr)
      {
	rust_error_at (expr.get_index_expr ()->get_locus (),
		       "Type Resolver failure in Index for ArrayIndexExpr");
	return;
      }
    context->insert_type (expr.get_index_expr ()->get_mappings (),
			  resolved_index_expr);

    // resolve the array reference
    expr.get_array_expr ()->accept_vis (*this);
    if (infered == nullptr)
      {
	rust_error_at (expr.get_index_expr ()->get_locus (),
		       "failed to resolve array reference expression");
	return;
      }
    else if (infered->get_kind () != TyTy::TypeKind::ARRAY)
      {
	rust_error_at (expr.get_index_expr ()->get_locus (),
		       "expected an ArrayType got [%s]",
		       infered->as_string ().c_str ());
	infered = nullptr;
	return;
      }

    TyTy::ArrayType *array_type = static_cast<TyTy::ArrayType *> (infered);
    infered = array_type->get_element_type ()->clone ();
  }

  void visit (HIR::ArrayExpr &expr) override
  {
    HIR::ArrayElems *elements = expr.get_internal_elements ();
    root_array_expr_locus = expr.get_locus ();

    elements->accept_vis (*this);
    if (infered_array_elems == nullptr)
      return;
    if (folded_array_capacity == nullptr)
      return;

    infered
      = new TyTy::ArrayType (expr.get_mappings ().get_hirid (),
			     folded_array_capacity,
			     TyTy::TyVar (infered_array_elems->get_ref ()));
  }

  void visit (HIR::ArrayElemsValues &elems) override
  {
    std::vector<TyTy::BaseType *> types;

    for (auto &elem : elems.get_values ())
      {
	types.push_back (TypeCheckExpr::Resolve (elem.get (), false));
      }

    infered_array_elems
      = TyTy::TyVar::get_implicit_infer_var (root_array_expr_locus).get_tyty ();

    for (auto &type : types)
      {
	infered_array_elems = infered_array_elems->unify (type);
      }
    for (auto &elem : types)
      {
	infered_array_elems->append_reference (elem->get_ref ());
      }

    auto crate_num = mappings->get_current_crate ();
    Analysis::NodeMapping mapping (crate_num, UNKNOWN_NODEID,
				   mappings->get_next_hir_id (crate_num),
				   UNKNOWN_LOCAL_DEFID);
    std::string capacity_str = std::to_string (elems.get_num_elements ());
    HIR::LiteralExpr implicit_literal_capacity (
      mapping, capacity_str, HIR::Literal::LitType::INT,
      PrimitiveCoreType::CORETYPE_USIZE, Location ());

    // mark the type for this implicit node
    context->insert_type (mapping, new TyTy::USizeType (mapping.get_hirid ()));

    folded_array_capacity
      = ConstFold::ConstFoldExpr::fold (&implicit_literal_capacity);
  }

  void visit (HIR::ArrayElemsCopied &elems) override
  {
    auto capacity_type
      = TypeCheckExpr::Resolve (elems.get_num_copies_expr (), false);

    TyTy::USizeType *expected_ty = new TyTy::USizeType (
      elems.get_num_copies_expr ()->get_mappings ().get_hirid ());
    context->insert_type (elems.get_num_copies_expr ()->get_mappings (),
			  expected_ty);

    auto unified = expected_ty->unify (capacity_type);
    if (unified->get_kind () == TyTy::TypeKind::ERROR)
      return;

    folded_array_capacity
      = ConstFold::ConstFoldExpr::fold (elems.get_num_copies_expr ());

    infered_array_elems
      = TypeCheckExpr::Resolve (elems.get_elem_to_copy (), false);
  }

  // empty struct
  void visit (HIR::StructExprStruct &struct_expr) override
  {
    TyTy::BaseType *struct_path_ty
      = TypeCheckExpr::Resolve (&struct_expr.get_struct_name (), false);
    if (struct_path_ty->get_kind () != TyTy::TypeKind::ADT)
      {
	rust_error_at (struct_expr.get_struct_name ().get_locus (),
		       "expected an ADT type for constructor");
	return;
      }

    infered = struct_path_ty;
  }

  void visit (HIR::StructExprStructFields &struct_expr) override
  {
    infered = TypeCheckStructExpr::Resolve (&struct_expr);
  }

  void visit (HIR::GroupedExpr &expr) override
  {
    infered = TypeCheckExpr::Resolve (expr.get_expr_in_parens ().get (), false);
  }

  void visit (HIR::FieldAccessExpr &expr) override
  {
    auto struct_base
      = TypeCheckExpr::Resolve (expr.get_receiver_expr ().get (), false);

    // FIXME does this require autoderef here?
    if (struct_base->get_kind () == TyTy::TypeKind::REF)
      {
	TyTy::ReferenceType *r
	  = static_cast<TyTy::ReferenceType *> (struct_base);
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
    auto resolved = adt->get_field (expr.get_field_name ());
    if (resolved == nullptr)
      {
	rust_error_at (expr.get_locus (), "unknown field [%s] for type [%s]",
		       expr.get_field_name ().c_str (),
		       adt->as_string ().c_str ());
	return;
      }

    infered = resolved->get_field_type ();
  }

  void visit (HIR::QualifiedPathInExpression &expr) override;

  void visit (HIR::PathInExpression &expr) override;

  void visit (HIR::LoopExpr &expr) override
  {
    context->push_new_loop_context (expr.get_mappings ().get_hirid ());
    TyTy::BaseType *block_expr
      = TypeCheckExpr::Resolve (expr.get_loop_block ().get (), true);
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

    infered = loop_context_type_infered
		? loop_context_type
		: new TyTy::TupleType (expr.get_mappings ().get_hirid ());
  }

  void visit (HIR::WhileLoopExpr &expr) override
  {
    context->push_new_while_loop_context (expr.get_mappings ().get_hirid ());

    TypeCheckExpr::Resolve (expr.get_predicate_expr ().get (), false);
    TyTy::BaseType *block_expr
      = TypeCheckExpr::Resolve (expr.get_loop_block ().get (), true);

    if (!block_expr->is_unit ())
      {
	rust_error_at (expr.get_loop_block ()->get_locus (),
		       "expected %<()%> got %s",
		       block_expr->as_string ().c_str ());
	return;
      }

    context->pop_loop_context ();
    infered = new TyTy::TupleType (expr.get_mappings ().get_hirid ());
  }

  void visit (HIR::BreakExpr &expr) override
  {
    if (!inside_loop)
      {
	rust_error_at (expr.get_locus (), "cannot %<break%> outside of a loop");
	return;
      }

    if (expr.has_break_expr ())
      {
	TyTy::BaseType *break_expr_tyty
	  = TypeCheckExpr::Resolve (expr.get_expr ().get (), false);

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

  void visit (HIR::ContinueExpr &expr) override
  {
    if (!inside_loop)
      {
	rust_error_at (expr.get_locus (),
		       "cannot %<continue%> outside of a loop");
	return;
      }

    infered = new TyTy::NeverType (expr.get_mappings ().get_hirid ());
  }

  void visit (HIR::BorrowExpr &expr) override
  {
    TyTy::BaseType *resolved_base
      = TypeCheckExpr::Resolve (expr.get_expr ().get (), false);

    // FIXME double_reference

    infered = new TyTy::ReferenceType (expr.get_mappings ().get_hirid (),
				       TyTy::TyVar (resolved_base->get_ref ()),
				       expr.get_is_mut ());
  }

  void visit (HIR::DereferenceExpr &expr) override
  {
    TyTy::BaseType *resolved_base
      = TypeCheckExpr::Resolve (expr.get_expr ().get (), false);

    bool is_valid_type
      = resolved_base->get_kind () == TyTy::TypeKind::REF
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

  void visit (HIR::TypeCastExpr &expr) override
  {
    TyTy::BaseType *expr_to_convert
      = TypeCheckExpr::Resolve (expr.get_casted_expr ().get (), false);
    TyTy::BaseType *tyty_to_convert_to
      = TypeCheckType::Resolve (expr.get_type_to_convert_to ().get ());

    infered = expr_to_convert->cast (tyty_to_convert_to);
  }

private:
  TypeCheckExpr (bool inside_loop)
    : TypeCheckBase (), infered (nullptr), infered_array_elems (nullptr),
      folded_array_capacity (nullptr), inside_loop (inside_loop)
  {}

  // Beware: currently returns Tyty::ErrorType or nullptr in case of error.
  TyTy::BaseType *resolve_root_path (HIR::PathInExpression &expr,
				     size_t *offset,
				     NodeId *root_resolved_node_id);

  void resolve_segments (NodeId root_resolved_node_id,
			 std::vector<HIR::PathExprSegment> &segments,
			 size_t offset, TyTy::BaseType *tyseg,
			 const Analysis::NodeMapping &expr_mappings,
			 Location expr_locus);

  bool
  validate_arithmetic_type (TyTy::BaseType *type,
			    HIR::ArithmeticOrLogicalExpr::ExprType expr_type)
  {
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
		   && (((TyTy::InferType *) type)->get_infer_kind ()
		       == TyTy::InferType::INTEGRAL))
	       || (type->get_kind () == TyTy::TypeKind::INFER
		   && (((TyTy::InferType *) type)->get_infer_kind ()
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
		   && (((TyTy::InferType *) type)->get_infer_kind ()
		       == TyTy::InferType::INTEGRAL));

	// integers only
      case ArithmeticOrLogicalOperator::LEFT_SHIFT:
      case ArithmeticOrLogicalOperator::RIGHT_SHIFT:
	return (type->get_kind () == TyTy::TypeKind::INT)
	       || (type->get_kind () == TyTy::TypeKind::UINT)
	       || (type->get_kind () == TyTy::TypeKind::USIZE)
	       || (type->get_kind () == TyTy::TypeKind::ISIZE)
	       || (type->get_kind () == TyTy::TypeKind::INFER
		   && (((TyTy::InferType *) type)->get_infer_kind ()
		       == TyTy::InferType::INTEGRAL));
      }
    gcc_unreachable ();
  }

  /* The return value of TypeCheckExpr::Resolve */
  TyTy::BaseType *infered;

  /* The return value of visit(ArrayElemsValues&) and visit(ArrayElemsCopied&)
     Stores the type of array elements, if `expr` is ArrayExpr. */
  TyTy::BaseType *infered_array_elems;
  Bexpression *folded_array_capacity;
  Location root_array_expr_locus;

  bool inside_loop;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_HIR_TYPE_CHECK_EXPR
