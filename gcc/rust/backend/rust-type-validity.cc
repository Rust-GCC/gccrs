// Copyright (C) 2026 Free Software Foundation, Inc.

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

#include "rust-type-validity.h"
#include "rust-compile-context.h"
#include "rust-compile-expr.h"
#include "rust-constexpr.h"
#include "rust-hir-item.h"
#include "rust-system.h"
#include "rust-tree.h"

namespace Rust {

namespace {

struct RangeInclusive
{
  wide_int start;
  wide_int end;

  bool contains (wide_int val)
  {
    if (wi::geu_p (end, start))
      return wi::geu_p (val, start) && wi::leu_p (val, end);
    return false;
  }
};

struct Scalar
{
  int bits;
  RangeInclusive valid_range;

  struct RangeExclusive
  {
    wide_int start;
    wide_int end;
  };

  // https://github.com/rust-lang/rust/blob/e1884a8e3c3e813aada8254edfa120e85bf5ffca/compiler/rustc_target/src/abi/mod.rs#L628
  RangeExclusive valid_range_exclusive () const
  {
    if (bits == 0)
      return {wi::zero (1), wi::zero (1)};

    wide_int w_start = this->valid_range.start;
    wide_int w_end = this->valid_range.end;

    wide_int w_exclusive_end = wi::add (w_end, 1);

    return {w_start, w_exclusive_end};
  }
};

struct Vector
{
  Scalar element;
  unsigned long count;
};

tl::optional<Scalar>
make_scalar (TyTy::TypeKind t_kind, tree t_tree)
{
  bool is_start_1 = false;
  switch (t_kind)
    {
    case TyTy::TypeKind::INT:
    case TyTy::TypeKind::UINT:
    case TyTy::TypeKind::ISIZE:
    case TyTy::TypeKind::USIZE:
    case TyTy::TypeKind::FLOAT:
    case TyTy::TypeKind::POINTER:
    case TyTy::TypeKind::BOOL:
      break;
    case TyTy::TypeKind::REF:
    case TyTy::TypeKind::FNPTR:
      is_start_1 = true;
      break;
    default:
      return tl::nullopt;
    }

  int bits;
  wide_int start;
  wide_int end;
  if (t_kind == TyTy::TypeKind::BOOL)
    {
      // TYPE_PRECISION(boolean_type_node) returns 1 but rustc uses i8 for
      // booleans
      bits = 8;
      start = wi::uhwi (0, bits);
      end = wi::uhwi (1, bits);
    }
  else
    {
      bits = TYPE_PRECISION (t_tree);
      start = is_start_1 ? wi::uhwi (1, bits) : wi::zero (bits);
      end = wi::max_value (bits, UNSIGNED);
    }
  return Scalar{bits, {start, end}};
}

std::pair<tl::optional<std::string>, tl::optional<std::string>>
check_layout_attrs (TyTy::ADTType *adt)
{
  tl::optional<std::string> start = tl::nullopt;
  tl::optional<std::string> end = tl::nullopt;
  if (auto item = Analysis::Mappings::get ().lookup_defid (adt->get_id ()))
    {
      for (const auto &attr : item.value ()->get_outer_attrs ())
	{
	  std::string path = attr.get_path ().as_string ();
	  bool is_start = path == "rustc_layout_scalar_valid_range_start";
	  bool is_end = path == "rustc_layout_scalar_valid_range_end";
	  if ((is_start || is_end) && attr.has_attr_input ())
	    {
	      auto input
		= static_cast<AST::DelimTokenTree &> (attr.get_attr_input ());
	      auto token_stream = input.to_token_stream ();
	      if (token_stream.size () >= 3)
		{
		  auto &val_token = token_stream.at (1);

		  if (is_start)
		    start = val_token->as_string ();
		  else
		    end = val_token->as_string ();
		}
	    }
	}
    }

  return std::make_pair (start, end);
}

tl::optional<Scalar>
try_make_scalar (TyTy::BaseType *t, tree t_tree)
{
  if (auto adt = t->try_as<TyTy::ADTType> ())
    {
      if (adt->is_struct_struct () || adt->is_tuple_struct ())
	{
	  auto &fields = adt->get_variants ().front ()->get_fields ();
	  if (fields.size () == 1)
	    {
	      if (auto s = try_make_scalar (fields.front ()->get_field_type (),
					    TREE_TYPE (TYPE_FIELDS (t_tree))))
		{
		  auto range = check_layout_attrs (adt);
		  if (range.first.has_value ())
		    {
		      mpz_t mpz_val;
		      mpz_init (mpz_val);
		      mpz_set_str (mpz_val, range.first.value ().c_str (), 10);
		      s->valid_range.start
			= wi::from_mpz (build_nonstandard_integer_type (s->bits,
									1),
					mpz_val, true);
		      mpz_clear (mpz_val);
		    }
		  if (range.second.has_value ())
		    {
		      mpz_t mpz_val;
		      mpz_init (mpz_val);
		      mpz_set_str (mpz_val, range.second.value ().c_str (), 10);
		      s->valid_range.end
			= wi::from_mpz (build_nonstandard_integer_type (s->bits,
									1),
					mpz_val, true);
		      mpz_clear (mpz_val);
		    }

		  return s;
		}
	    }
	}
    }

  return make_scalar (t->get_kind (), t_tree);
}

tl::optional<std::pair<Scalar, Scalar>>
make_scalar_pair (TyTy::BaseType *t, tree t_tree)
{
  if (!RS_DST_FLAG_P (t_tree))
    return tl::nullopt;

  rust_assert (t->is<TyTy::ReferenceType> () || t->is<TyTy::PointerType> ());

  Scalar data_ptr;
  Scalar meta;

  if (t->is<TyTy::ReferenceType> ())
    {
      auto ref_ty = static_cast<TyTy::ReferenceType *> (t);
      data_ptr = make_scalar (TyTy::TypeKind::REF, ptr_type_node).value ();
      if (ref_ty->is_dyn_slice_type () || ref_ty->is_dyn_str_type ())
	meta = make_scalar (TyTy::TypeKind::USIZE, size_type_node).value ();
      else if (ref_ty->is_dyn_obj_type () || ref_ty->is_dyn_adt_type ())
	meta = make_scalar (TyTy::TypeKind::REF, ptr_type_node).value ();
      else
	return tl::nullopt;
    }
  else
    {
      auto ptr_ty = static_cast<TyTy::PointerType *> (t);
      data_ptr = make_scalar (TyTy::TypeKind::POINTER, ptr_type_node).value ();
      if (ptr_ty->is_dyn_slice_type () || ptr_ty->is_dyn_str_type ())
	meta = make_scalar (TyTy::TypeKind::USIZE, size_type_node).value ();
      else if (ptr_ty->is_dyn_obj_type () || ptr_ty->is_dyn_adt_type ())
	meta = make_scalar (TyTy::TypeKind::REF, ptr_type_node).value ();
      else
	return tl::nullopt;
    }

  return std::make_pair (data_ptr, meta);
}

tl::optional<Vector>
make_vector (TyTy::BaseType *t, tree t_tree)
{
  if (auto adt_ty = t->try_as<TyTy::ADTType> ())
    {
      if (adt_ty->get_repr_options ().repr_kind
	  == TyTy::ADTType::ReprKind::SIMD)
	{
	  rust_assert (VECTOR_TYPE_P (t_tree));
	  rust_assert (adt_ty->is_struct_struct ()
		       || adt_ty->is_tuple_struct ());
	  auto &fields = adt_ty->get_variants ().front ()->get_fields ();

	  if (fields.empty ())
	    return Vector{Scalar{0, {wi::zero (1), wi::zero (1)}}, 0};

	  auto first = fields.front ()->get_field_type ();
	  if (auto element
	      = make_scalar (first->get_kind (), TREE_TYPE (t_tree)))
	    {
	      return Vector{element.value (), fields.size ()};
	    }
	}
    }
  return tl::nullopt;
}

} // namespace

// https://github.com/rust-lang/rust/blob/e1884a8e3c3e813aada8254edfa120e85bf5ffca/compiler/rustc_target/src/abi/mod.rs#L1129
bool
TypeValidity::might_permit_raw_init (TyTy::BaseType *type, tree type_tree,
				     RawInitKind init_kind)
{
  auto scalar_allows_raw_init = [init_kind] (Scalar &s) {
    if (init_kind == RawInitKind::ZeroInit)
      {
	RangeInclusive &range = s.valid_range;
	return range.contains (wi::zero (s.bits))
	       || wi::gtu_p (range.start, range.end); // wrap-around allows 0
      }
    else
      {
	Scalar::RangeExclusive range = s.valid_range_exclusive ();
	return range.start == range.end;
      }
  };

  bool valid = true;
  bool is_aggregate = false;
  if (is_uninhabited (type))
    valid = false;
  else if (auto pair = make_scalar_pair (type, type_tree))
    valid = scalar_allows_raw_init (pair->first)
	    && scalar_allows_raw_init (pair->second);
  else if (auto e = make_vector (type, type_tree))
    valid = e->count == 0 || scalar_allows_raw_init (e->element);
  else if (auto s = try_make_scalar (type, type_tree))
    valid = scalar_allows_raw_init (*s);
  else
    is_aggregate = true;

  if (!valid)
    return false;

  if (is_aggregate)
    {
      if (auto adt_ty = type->try_as<TyTy::ADTType> ())
	{
	  if (adt_ty->is_tuple_struct () || adt_ty->is_struct_struct ())
	    {
	      tree tree_iter = TYPE_FIELDS (type_tree);
	      for (auto field : adt_ty->get_variants ().front ()->get_fields ())
		if (!check_and_step_field (field->get_field_type (), tree_iter,
					   init_kind))
		  return false;
	    }
	  else if (adt_ty->is_enum ())
	    {
	      if (adt_ty->number_of_variants () == 1)
		{
		  auto variant = adt_ty->get_variants ().front ();
		  if (variant->is_dataless_variant ())
		    return true;

		  tree disc = TYPE_FIELDS (type_tree);
		  tree payload_type = TREE_TYPE (DECL_CHAIN (disc));
		  tree variant_record_type
		    = TREE_TYPE (TYPE_FIELDS (payload_type));
		  tree tree_iter = TYPE_FIELDS (variant_record_type);

		  for (auto field : variant->get_fields ())
		    if (!check_and_step_field (field->get_field_type (),
					       tree_iter, init_kind))
		      return false;
		}
	      else
		{
		  if (init_kind == RawInitKind::Uninit)
		    return false;

		  for (auto variant : adt_ty->get_variants ())
		    {
		      HIR::Expr &discrim_expr = variant->get_discriminant ();
		      tree discrim_expr_node = Compile::CompileExpr::Compile (
			discrim_expr, Compile::Context::get ());
		      tree folded_discrim_expr
			= Compile::fold_expr (discrim_expr_node);
		      if (folded_discrim_expr == NULL_TREE
			  || TREE_CODE (folded_discrim_expr) != INTEGER_CST)
			continue;

		      size_t disc_wi
			= (size_t) wi::to_wide (folded_discrim_expr).to_uhwi ();
		      if (disc_wi == 0)
			return true;
		    }

		  return false;
		}
	    }
	}
      else if (auto tuple_ty = type->try_as<TyTy::TupleType> ())
	{
	  tree tree_iter = TYPE_FIELDS (type_tree);
	  for (auto field : tuple_ty->get_fields ())
	    if (!check_and_step_field (field.get_tyty (), tree_iter, init_kind))
	      return false;
	}
    }

  // For now, we are conservative and do not check multi-variant enums and
  // arrays
  return true;
}

/**
 * Check if a type is uninhabited - either the Never type or a variant-less enum
 */
bool
TypeValidity::is_uninhabited (TyTy::BaseType *t)
{
  rust_assert (t);

  if (t->is<TyTy::NeverType> ())
    return true;

  else if (auto arr = t->try_as<TyTy::ArrayType> ())
    {
      if (is_uninhabited (arr->get_element_type ()))
	{
	  if (arr->get_capacity ()->get_kind () == TyTy::TypeKind::CONST)
	    {
	      auto cap_val = static_cast<TyTy::ConstValueType *> (
		arr->get_capacity ()->as_const_type ());
	      size_t cap_wi
		= (size_t) wi::to_wide (cap_val->get_value ()).to_uhwi ();
	      return cap_wi != 0;
	    }
	}
    }

  else if (auto tuple = t->try_as<TyTy::TupleType> ())
    {
      for (auto field : tuple->get_fields ())
	if (is_uninhabited (field.get_tyty ()))
	  return true;
    }

  else if (auto adt = t->try_as<TyTy::ADTType> ())
    {
      if (adt->is_enum ())
	{
	  if (adt->number_of_variants () == 0)
	    return true;

	  for (auto variant : adt->get_variants ())
	    {
	      bool variant_is_uninhabited = false;
	      for (auto field : variant->get_fields ())
		{
		  if (is_uninhabited (field->get_field_type ()))
		    {
		      variant_is_uninhabited = true;
		      break;
		    }
		}
	      if (!variant_is_uninhabited)
		return false;
	    }

	  // all variants are uninhabited
	  return true;
	}
      if (adt->is_struct_struct () || adt->is_tuple_struct ())
	for (auto field : adt->get_variants ().front ()->get_fields ())
	  if (is_uninhabited (field->get_field_type ()))
	    return true;
    }

  return false;
}

bool
TypeValidity::check_and_step_field (TyTy::BaseType *field_type, tree &tree_iter,
				    RawInitKind init_kind)
{
  rust_assert (tree_iter != NULL_TREE);
  tree field_tree = TREE_TYPE (tree_iter);
  tree_iter = DECL_CHAIN (tree_iter);
  return might_permit_raw_init (field_type, field_tree, init_kind);
}

} // namespace Rust
