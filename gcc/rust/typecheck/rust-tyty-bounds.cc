// Copyright (C) 2021-2026 Free Software Foundation, Inc.

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

#include "rust-hir-full-decls.h"
#include "rust-hir-type-bounds.h"
#include "rust-hir-trait-resolve.h"
#include "rust-substitution-mapper.h"
#include "rust-hir-trait-resolve.h"
#include "rust-type-util.h"
#include "rust-tyty.h"

namespace Rust {
namespace Resolver {

TypeBoundsProbe::TypeBoundsProbe (TyTy::BaseType *receiver,
				  const HIR::Trait *specified_trait)
  : TypeCheckBase (), receiver (receiver), specified_trait (specified_trait)
{}

std::vector<std::pair<TraitReference *, HIR::ImplBlock *>>
TypeBoundsProbe::Probe (TyTy::BaseType *receiver,
			const HIR::Trait *specified_trait)
{
  TypeBoundsProbe probe (receiver, specified_trait);
  probe.scan ();
  return probe.trait_references;
}

bool
TypeBoundsProbe::is_bound_satisfied_for_type (TyTy::BaseType *receiver,
					      TraitReference *ref)
{
  // FIXME: consult item predicates before probing impls.

  std::vector<std::pair<TraitReference *, HIR::ImplBlock *>> bounds
    = Probe (receiver, ref->get_hir_trait_ref ());
  for (auto &bound : bounds)
    {
      const TraitReference *b = bound.first;
      if (b->is_equal (*ref))
	return true;
    }

  return false;
}

bool
TypeBoundsProbe::process_impl_block (
  HirId id, HIR::ImplBlock *impl,
  std::vector<std::pair<HIR::TypePath *, HIR::ImplBlock *>>
    &possible_trait_paths)
{
  // we are filtering for trait-impl-blocks
  if (!impl->has_trait_ref ())
    return true;

  // can be recursive trait resolution
  HIR::Trait *t = TraitResolver::ResolveHirItem (impl->get_trait_ref ());
  if (t == nullptr)
    return true;

  HirId impl_ty_id = impl->get_type ().get_mappings ().get_hirid ();
  TyTy::BaseType *impl_type = nullptr;
  if (!query_type (impl_ty_id, &impl_type))
    return true;

  if (!types_compatable (TyTy::TyWithLocation (receiver),
			 TyTy::TyWithLocation (impl_type), impl->get_locus (),
			 false /*emit_errors*/, false /*check-bounds*/))
    return true;

  possible_trait_paths.emplace_back (&impl->get_trait_ref (), impl);
  return true;
}

void
TypeBoundsProbe::scan ()
{
  std::vector<std::pair<HIR::TypePath *, HIR::ImplBlock *>>
    possible_trait_paths;
  auto process_impl = [&] (HirId id, HIR::ImplBlock *impl) mutable -> bool {
    return process_impl_block (id, impl, possible_trait_paths);
  };

  if (specified_trait == nullptr)
    mappings.iterate_trait_impl_blocks (process_impl);
  else
    mappings.iterate_trait_impl_blocks (
      specified_trait->get_mappings ().get_defid (), process_impl);

  for (auto &path : possible_trait_paths)
    {
      HIR::TypePath *trait_path = path.first;
      TraitReference *trait_ref = TraitResolver::Resolve (*trait_path);

      if (!trait_ref->is_error ())
	trait_references.emplace_back (trait_ref, path.second);
    }

  // marker traits...
  assemble_marker_builtins ();

  // add auto trait bounds
  for (auto *auto_trait : mappings.get_auto_traits ())
    add_trait_bound (auto_trait);
}

void
TypeBoundsProbe::assemble_marker_builtins ()
{
  const TyTy::BaseType *raw = receiver->destructure ();

  // https://runrust.miraheze.org/wiki/Dynamically_Sized_Type
  // everything is sized except for:
  //
  //   1. dyn traits
  //   2. slices
  //   3. str
  //   4. ADT's which contain any of the above
  //   t. tuples which contain any of the above
  switch (raw->get_kind ())
    {
    case TyTy::ARRAY:
    case TyTy::REF:
    case TyTy::POINTER:
    case TyTy::PARAM:
    case TyTy::FNDEF:
    case TyTy::BOOL:
    case TyTy::CHAR:
    case TyTy::INT:
    case TyTy::UINT:
    case TyTy::FLOAT:
    case TyTy::USIZE:
    case TyTy::ISIZE:
    case TyTy::INFER:
    case TyTy::NEVER:
    case TyTy::PLACEHOLDER:
    case TyTy::PROJECTION:
    case TyTy::OPAQUE:
      assemble_builtin_candidate (LangItem::Kind::SIZED);
      break;

    case TyTy::FNPTR:
    case TyTy::CLOSURE:
      assemble_builtin_candidate (LangItem::Kind::SIZED);
      assemble_builtin_candidate (LangItem::Kind::FN_ONCE);
      assemble_builtin_candidate (LangItem::Kind::FN);
      assemble_builtin_candidate (LangItem::Kind::FN_MUT);
      break;

      // FIXME str and slice need to be moved and test cases updated
    case TyTy::SLICE:
    case TyTy::STR:
    case TyTy::TUPLE:
      // FIXME add extra checks
      assemble_builtin_candidate (LangItem::Kind::SIZED);
      break;

    case TyTy::ADT:
      {
	const auto &adt = *static_cast<const TyTy::ADTType *> (raw);
	if (adt.get_adt_kind () != TyTy::ADTType::ADTKind::EXTERN)
	  assemble_builtin_candidate (LangItem::Kind::SIZED);
      }
      break;

    case TyTy::CONST:
    case TyTy::DYNAMIC:
    case TyTy::ERROR:
      break;
    }
}

void
TypeBoundsProbe::add_trait_bound (HIR::Trait *trait)
{
  auto trait_ref = TraitResolver::Resolve (*trait);

  for (const auto &existing : trait_references)
    if (existing.first->is_equal (*trait_ref))
      return;

  if (receiver->get_kind () == TyTy::TypeKind::ADT)
    {
      TyTy::ADTType *adt = static_cast<TyTy::ADTType *> (receiver);
      for (auto &variant : adt->get_variants ())
	{
	  for (auto &field : variant->get_fields ())
	    {
	      TyTy::BaseType *field_ty = field->get_field_type ();

	      // TODO: A loop guard is needed here to prevent infinite
	      // recursion, but self-referential types currently crash due to
	      // issue Rust-GCC/gccrs#4709. Therefore, I avoided adding an
	      // untested guard for now.

	      if (!field_ty->satisfies_bound (
		    TyTy::TypeBoundPredicate (*trait_ref,
					      BoundPolarity::RegularBound,
					      UNDEF_LOCATION),
		    false))
		return;
	    }
	}
    }

  trait_references.emplace_back (trait_ref, mappings.lookup_builtin_marker ());
}

void
TypeBoundsProbe::assemble_builtin_candidate (LangItem::Kind lang_item)
{
  auto lang_item_defined = mappings.lookup_lang_item (lang_item);
  if (!lang_item_defined)
    return;
  DefId &id = lang_item_defined.value ();

  auto defid = mappings.lookup_defid (id);
  if (!defid)
    return;
  auto item = defid.value ();

  rust_assert (item->get_item_kind () == HIR::Item::ItemKind::Trait);
  HIR::Trait *trait = static_cast<HIR::Trait *> (item);
  const TyTy::BaseType *raw = receiver->destructure ();

  add_trait_bound (trait);

  rust_debug ("Added builtin lang_item: %s for %s",
	      LangItem::ToString (lang_item).c_str (),
	      raw->get_name ().c_str ());
}

TraitReference *
TypeCheckBase::resolve_trait_path (HIR::TypePath &path)
{
  return TraitResolver::Resolve (path);
}

TyTy::TypeBoundPredicate
TypeCheckBase::get_predicate_from_bound (
  HIR::TypePath &type_path,
  tl::optional<std::reference_wrapper<HIR::Type>> associated_self,
  BoundPolarity polarity, bool is_qualified_type_path, bool is_super_trait,
  bool defer_bindings)
{
  TyTy::TypeBoundPredicate lookup = TyTy::TypeBoundPredicate::error ();
  bool already_resolved
    = context->lookup_predicate (type_path.get_mappings ().get_hirid (),
				 &lookup);
  if (already_resolved)
    return lookup;

  TraitReference *trait = resolve_trait_path (type_path);
  if (trait->is_error ())
    return TyTy::TypeBoundPredicate::error ();

  TyTy::TypeBoundPredicate predicate (*trait, polarity, type_path.get_locus ());
  HIR::GenericArgs args
    = HIR::GenericArgs::create_empty (type_path.get_locus ());

  auto &final_seg = type_path.get_final_segment ();
  switch (final_seg.get_type ())
    {
    case HIR::TypePathSegment::SegmentType::GENERIC:
      {
	auto &final_generic_seg
	  = static_cast<HIR::TypePathSegmentGeneric &> (final_seg);
	if (final_generic_seg.has_generic_args ())
	  {
	    args = final_generic_seg.get_generic_args ();
	    if (args.get_binding_args ().size () > 0
		&& associated_self.has_value () && is_qualified_type_path)
	      {
		auto &binding_args = args.get_binding_args ();

		rich_location r (line_table, args.get_locus ());
		for (auto it = binding_args.begin (); it != binding_args.end ();
		     it++)
		  {
		    auto &arg = *it;
		    r.add_fixit_remove (arg.get_locus ());
		  }
		rust_error_at (r, ErrorCode::E0229,
			       "associated type bindings are not allowed here");
	      }
	  }
      }
      break;

    case HIR::TypePathSegment::SegmentType::FUNCTION:
      {
	auto &final_function_seg
	  = static_cast<HIR::TypePathSegmentFunction &> (final_seg);
	auto &fn = final_function_seg.get_function_path ();

	// we need to make implicit generic args which must be an implicit
	// Tuple
	auto crate_num = mappings.get_current_crate ();
	HirId implicit_args_id = mappings.get_next_hir_id ();
	Analysis::NodeMapping mapping (crate_num,
				       final_seg.get_mappings ().get_nodeid (),
				       implicit_args_id, UNKNOWN_LOCAL_DEFID);

	std::vector<std::unique_ptr<HIR::Type>> params_copy;
	for (auto &p : fn.get_params ())
	  {
	    params_copy.push_back (p->clone_type ());
	  }

	std::vector<std::unique_ptr<HIR::Type>> inputs;
	inputs.push_back (
	  std::make_unique<HIR::TupleType> (mapping, std::move (params_copy),
					    final_seg.get_locus ()));

	HIR::TraitItem *trait_item
	  = mappings
	      .lookup_trait_item_lang_item (LangItem::Kind::FN_ONCE_OUTPUT,
					    final_seg.get_locus ())
	      .value ();

	std::vector<HIR::GenericArgsBinding> bindings;

	if (fn.has_return_type () && !defer_bindings)
	  {
	    TypeCheckType::Resolve (fn.get_return_type ());

	    location_t output_locus = fn.get_return_type ().get_locus ();
	    bindings.emplace_back (Identifier (trait_item->trait_identifier ()),
				   fn.get_return_type ().clone_type (),
				   output_locus);
	  }

	args = HIR::GenericArgs ({} /* lifetimes */,
				 std::move (inputs) /* type_args*/,
				 std::move (bindings) /* binding_args*/,
				 {} /* const_args */, final_seg.get_locus ());
      }
      break;

    default:
      /* nothing to do */
      break;
    }

  if (associated_self.has_value ())
    {
      std::vector<std::unique_ptr<HIR::Type>> type_args;
      type_args.push_back (std::unique_ptr<HIR::Type> (
	associated_self.value ().get ().clone_type ()));
      for (auto &arg : args.get_type_args ())
	{
	  type_args.push_back (std::unique_ptr<HIR::Type> (arg->clone_type ()));
	}

      args = HIR::GenericArgs (args.get_lifetime_args (), std::move (type_args),
			       args.get_binding_args (), args.get_const_args (),
			       args.get_locus ());
    }

  if (defer_bindings)
    args.get_binding_args ().clear ();

  // we try to apply generic arguments when they are non empty and or when the
  // predicate requires them so that we get the relevant Foo expects x number
  // arguments but got zero see test case rust/compile/traits12.rs
  if (!args.is_empty () || predicate.requires_generic_args ())
    {
      // this is applying generic arguments to a trait reference
      predicate.apply_generic_arguments (&args, associated_self.has_value (),
					 is_super_trait);
    }

  if (!defer_bindings)
    context->insert_resolved_predicate (type_path.get_mappings ().get_hirid (),
					predicate);

  return predicate;
}

} // namespace Resolver

namespace TyTy {

void
PredicateSet::add (BaseType *self, const TypeBoundPredicate &bound)
{
  predicates.emplace_back (self, bound);
}

std::vector<TypeBoundPredicate *>
PredicateSet::specified_bounds_for (const BaseType *self)
{
  // Subjects can be any type:
  //   T: Clone           -> subject T
  //   Wrapper<T>: Clone  -> subject Wrapper<T>
  //   I::Item: Display   -> subject I::Item
  std::vector<TypeBoundPredicate *> result;
  for (auto &predicate : predicates)
    {
      if (same_subject (predicate.self, self))
	result.push_back (&predicate.bound);
    }
  return result;
}

// A generic argument is recorded by binding its declared param slot to the
// argument (or, for a param argument, replacing the slot with that param).
// Follow only that binding: a rigid param resolves to itself, and placeholder
// or opaque resolutions are never followed.
static const BaseType *
follow_substituted_slot (const BaseType *ty)
{
  if (const auto *param = ty->try_as<const ParamType> ())
    return param->can_resolve () ? param->resolve () : ty;

  if (ty->get_kind () == TypeKind::CONST)
    {
      const auto *const_type = ty->as_const_type ();
      bool is_const_param
	= const_type->const_kind () == BaseConstType::ConstKind::Decl;
      if (is_const_param)
	{
	  const auto *decl = static_cast<const ConstParamType *> (const_type);
	  return decl->can_resolve () ? decl->resolve () : ty;
	}
    }

  return ty;
}

// ADTs and projections both record their effective generic arguments in
// their substitution slots, positionally. Lifetimes live outside the slots
// and are ignored.
static bool
same_generic_args (const SubstitutionRef &a, const SubstitutionRef &b)
{
  const auto &a_substs = a.get_substs ();
  const auto &b_substs = b.get_substs ();
  if (a_substs.size () != b_substs.size ())
    return false;

  for (size_t i = 0; i < a_substs.size (); i++)
    {
      if (!same_subject (a_substs.at (i).get_param_ty (),
			 b_substs.at (i).get_param_ty ()))
	return false;
    }
  return true;
}

static bool
same_const (const BaseType *a, const BaseType *b)
{
  const auto *ca = a->as_const_type ();
  const auto *cb = b->as_const_type ();
  if (ca->const_kind () != cb->const_kind ())
    return false;

  switch (ca->const_kind ())
    {
    case BaseConstType::ConstKind::Decl:
      {
	HirId decl = static_cast<const ConstParamType *> (ca)->get_decl_id ();
	bool known_decl = decl != UNKNOWN_HIRID;
	return known_decl
	       && decl
		    == static_cast<const ConstParamType *> (cb)->get_decl_id ();
      }

    case BaseConstType::ConstKind::Value:
      return a->is_equal (*b);

    case BaseConstType::ConstKind::Infer:
      return a->get_ref () == b->get_ref ();

    case BaseConstType::ConstKind::Error:
      return false;
    }
  return false;
}

bool
same_subject (const BaseType *a, const BaseType *b)
{
  a = follow_substituted_slot (a);
  b = follow_substituted_slot (b);

  bool either_error = a->get_kind () == TypeKind::ERROR
		      || b->get_kind () == TypeKind::ERROR;
  if (either_error)
    return false;

  if (a == b)
    return true;

  if (a->get_kind () != b->get_kind ())
    return false;

  switch (a->get_kind ())
    {
    case TypeKind::INFER:
      // Only the same inference variable; never guess what it will become.
      return a->get_ref () == b->get_ref ();

    case TypeKind::PARAM:
      {
	HirId decl = a->as<const ParamType> ()->get_decl_id ();
	bool known_decl = decl != UNKNOWN_HIRID;
	return known_decl && decl == b->as<const ParamType> ()->get_decl_id ();
      }

    case TypeKind::CONST:
      return same_const (a, b);

    case TypeKind::ADT:
      {
	const auto *aa = a->as<const ADTType> ();
	const auto *ab = b->as<const ADTType> ();
	return aa->get_id () == ab->get_id () && same_generic_args (*aa, *ab);
      }

    case TypeKind::PROJECTION:
      {
	const auto *pa = a->as<const ProjectionType> ();
	const auto *pb = b->as<const ProjectionType> ();

	const auto *trait_a = pa->get_trait_ref ();
	const auto *trait_b = pb->get_trait_ref ();
	bool same_trait
	  = trait_a != nullptr && trait_b != nullptr
	    && trait_a->get_mappings ().get_defid ()
		 == trait_b->get_mappings ().get_defid ();
	bool same_item = pa->get_item_defid () == pb->get_item_defid ();
	bool same_position = pa->is_trait_position () == pb->is_trait_position ();
	bool has_selves = pa->get_self () != nullptr && pb->get_self () != nullptr;

	return same_trait && same_item && same_position && has_selves
	       && same_subject (pa->get_self (), pb->get_self ())
	       && same_generic_args (*pa, *pb);
      }

    case TypeKind::REF:
      {
	const auto *ra = a->as<const ReferenceType> ();
	const auto *rb = b->as<const ReferenceType> ();
	return ra->mutability () == rb->mutability ()
	       && same_subject (ra->get_base (), rb->get_base ());
      }

    case TypeKind::POINTER:
      {
	const auto *pa = a->as<const PointerType> ();
	const auto *pb = b->as<const PointerType> ();
	return pa->mutability () == pb->mutability ()
	       && same_subject (pa->get_base (), pb->get_base ());
      }

    case TypeKind::SLICE:
      return same_subject (a->as<const SliceType> ()->get_element_type (),
			   b->as<const SliceType> ()->get_element_type ());

    case TypeKind::ARRAY:
      {
	const auto *aa = a->as<const ArrayType> ();
	const auto *ab = b->as<const ArrayType> ();
	return same_subject (aa->get_element_type (), ab->get_element_type ())
	       && same_subject (aa->get_capacity (), ab->get_capacity ());
      }

    case TypeKind::TUPLE:
      {
	const auto *ta = a->as<const TupleType> ();
	const auto *tb = b->as<const TupleType> ();
	if (ta->num_fields () != tb->num_fields ())
	  return false;

	for (size_t i = 0; i < ta->num_fields (); i++)
	  {
	    if (!same_subject (ta->get_field (i), tb->get_field (i)))
	      return false;
	  }
	return true;
      }

    // Leaf kinds: is_equal is an exact comparison for these.
    case TypeKind::BOOL:
    case TypeKind::CHAR:
    case TypeKind::INT:
    case TypeKind::UINT:
    case TypeKind::FLOAT:
    case TypeKind::USIZE:
    case TypeKind::ISIZE:
    case TypeKind::STR:
    case TypeKind::NEVER:
      return a->is_equal (*b);

    // Not supported yet: only the identical object matches (handled above).
    // Structural matching of these needs its own rules (dyn/opaque bounds,
    // closure identity, fn signatures, placeholders).
    default:
      return false;
    }
}

TypeBoundPredicate::TypeBoundPredicate (
  const Resolver::TraitReference &trait_reference, BoundPolarity polarity,
  location_t locus)
  : SubstitutionRef (trait_reference.get_mappings ().get_defid (), {},
		     SubstitutionArgumentMappings::empty (), {}),
    reference (trait_reference.get_mappings ().get_defid ()), locus (locus),
    error_flag (false), polarity (polarity),
    super_traits (trait_reference.get_super_traits ())
{
  rust_assert (!trait_reference.get_trait_substs ().empty ());

  substitutions.clear ();
  for (const auto &p : trait_reference.get_trait_substs ())
    substitutions.push_back (p.clone ());

  // we setup a dummy implict self argument
  SubstitutionArg placeholder_self (&get_substs ().front (), nullptr);
  used_arguments.get_mappings ().push_back (placeholder_self);
}

TypeBoundPredicate::TypeBoundPredicate (
  DefId reference, std::vector<SubstitutionParamMapping> subst,
  BoundPolarity polarity, location_t locus)
  : SubstitutionRef (reference, {}, SubstitutionArgumentMappings::empty (), {}),
    reference (reference), locus (locus), error_flag (false),
    polarity (polarity)
{
  rust_assert (!subst.empty ());

  substitutions.clear ();
  for (const auto &p : subst)
    substitutions.push_back (p.clone ());

  // we setup a dummy implict self argument
  SubstitutionArg placeholder_self (&get_substs ().front (), nullptr);
  used_arguments.get_mappings ().push_back (placeholder_self);
}

TypeBoundPredicate::TypeBoundPredicate (mark_is_error)
  : SubstitutionRef (UNKNOWN_DEFID, {}, SubstitutionArgumentMappings::empty (),
		     {}),
    reference (UNKNOWN_DEFID), locus (UNDEF_LOCATION), error_flag (true),
    polarity (BoundPolarity::RegularBound)
{}

TypeBoundPredicate::TypeBoundPredicate (const TypeBoundPredicate &other)
  : SubstitutionRef (other.get_predicate_owner (), {},
		     SubstitutionArgumentMappings::empty (), {}),
    reference (other.reference), locus (other.locus),
    error_flag (other.error_flag), polarity (other.polarity),
    super_traits (other.super_traits)
{
  substitutions.clear ();
  for (const auto &p : other.get_substs ())
    substitutions.push_back (p.clone ());

  // we need to remap the argument mappings based on this copied constructor
  std::vector<SubstitutionArg> copied_arg_mappings;
  size_t i = 0;
  for (const auto &m : other.used_arguments.get_mappings ())
    {
      TyTy::BaseType *argument
	= m.get_tyty () == nullptr ? nullptr : m.get_tyty ()->clone ();
      SubstitutionArg c (&substitutions.at (i++), argument);
      copied_arg_mappings.push_back (std::move (c));
    }

  used_arguments = SubstitutionArgumentMappings (
    copied_arg_mappings, other.used_arguments.get_binding_args (),
    other.used_arguments.get_regions (), other.used_arguments.get_locus (),
    false, false, other.used_arguments.get_constraint_args ());
}

TypeBoundPredicate &
TypeBoundPredicate::operator= (const TypeBoundPredicate &other)
{
  predicate_owner = other.get_predicate_owner ();
  reference = other.reference;
  locus = other.locus;
  error_flag = other.error_flag;
  polarity = other.polarity;
  used_arguments = SubstitutionArgumentMappings::empty ();

  substitutions.clear ();
  for (const auto &p : other.get_substs ())
    substitutions.push_back (p.clone ());

  if (other.is_error ())
    return *this;

  // we need to remap the argument mappings based on this copied constructor
  std::vector<SubstitutionArg> copied_arg_mappings;
  size_t i = 0;
  for (const auto &m : other.used_arguments.get_mappings ())
    {
      TyTy::BaseType *argument
	= m.get_tyty () == nullptr ? nullptr : m.get_tyty ()->clone ();

      copied_arg_mappings.emplace_back (&substitutions.at (i++), argument);
    }

  used_arguments = SubstitutionArgumentMappings (
    copied_arg_mappings, other.used_arguments.get_binding_args (),
    other.used_arguments.get_regions (), other.used_arguments.get_locus (),
    false, false, other.used_arguments.get_constraint_args ());
  super_traits = other.super_traits;

  return *this;
}

TypeBoundPredicate
TypeBoundPredicate::error ()
{
  return TypeBoundPredicate (mark_is_error ());
}

std::string
TypeBoundPredicate::as_string () const
{
  return get ()->as_string () + subst_as_string ();
}

std::string
TypeBoundPredicate::as_name () const
{
  return get ()->get_name () + subst_as_string ();
}

const Resolver::TraitReference *
TypeBoundPredicate::get () const
{
  auto context = Resolver::TypeCheckContext::get ();

  Resolver::TraitReference *ref = nullptr;
  bool ok = context->lookup_trait_reference (reference, &ref);
  rust_assert (ok);

  return ref;
}

std::string
TypeBoundPredicate::get_name () const
{
  return get ()->get_name ();
}

bool
TypeBoundPredicate::is_object_safe (bool emit_error, location_t locus) const
{
  const Resolver::TraitReference *trait = get ();
  rust_assert (trait != nullptr);
  return trait->is_object_safe (emit_error, locus);
}

void
TypeBoundPredicate::apply_generic_arguments (HIR::GenericArgs *generic_args,
					     bool has_associated_self,
					     bool is_super_trait)
{
  rust_assert (!substitutions.empty ());
  if (has_associated_self)
    {
      used_arguments = SubstitutionArgumentMappings::empty ();
    }
  else
    {
      // we need to get the substitutions argument mappings but also remember
      // that we have an implicit Self argument which we must be careful to
      // respect
      rust_assert (!used_arguments.is_empty ());
    }

  // now actually perform a substitution
  auto args = get_mappings_from_generic_args (
    *generic_args,
    Resolver::TypeCheckContext::get ()->regions_from_generic_args (
      *generic_args));

  apply_argument_mappings (args, is_super_trait);
}

void
TypeBoundPredicate::apply_argument_mappings (
  SubstitutionArgumentMappings &arguments, bool is_super_trait)
{
  used_arguments = arguments;
  error_flag |= used_arguments.is_error ();
  auto &subst_mappings = used_arguments;

  bool substs_need_bounds_check = !is_super_trait;
  for (auto &sub : get_substs ())
    {
      SubstitutionArg arg = SubstitutionArg::error ();
      bool ok
	= subst_mappings.get_argument_for_symbol (sub.get_param_ty (), &arg);
      if (ok && arg.get_tyty () != nullptr)
	sub.fill_param_ty (subst_mappings, subst_mappings.get_locus (),
			   substs_need_bounds_check);
    }

  // Associated type binding args (Iterator<Item = i32>) are consumed
  // by BaseType::satisfies_bound at check time
  for (auto &super_trait : super_traits)
    {
      auto adjusted
	= super_trait.adjust_mappings_for_this (used_arguments,
						true /*trait mode*/);
      super_trait.apply_argument_mappings (adjusted, is_super_trait);
    }
}

bool
TypeBoundPredicate::contains_item (const std::string &search) const
{
  auto trait_ref = get ();
  const Resolver::TraitItemReference *trait_item_ref = nullptr;
  return trait_ref->lookup_trait_item (search, &trait_item_ref);
}

tl::optional<TypeBoundPredicateItem>
TypeBoundPredicate::lookup_associated_item (const std::string &search) const
{
  auto trait_ref = get ();
  const Resolver::TraitItemReference *trait_item_ref = nullptr;
  if (trait_ref->lookup_trait_item (search, &trait_item_ref,
				    false /*lookup supers*/))
    return TypeBoundPredicateItem (*this, trait_item_ref);

  for (auto &super_trait : super_traits)
    {
      auto lookup = super_trait.lookup_associated_item (search);
      if (lookup.has_value ())
	return lookup;
    }

  return tl::nullopt;
}

TypeBoundPredicateItem::TypeBoundPredicateItem (
  const TypeBoundPredicate parent,
  const Resolver::TraitItemReference *trait_item_ref)
  : parent (parent), trait_item_ref (trait_item_ref)
{}

TypeBoundPredicateItem::TypeBoundPredicateItem (
  const TypeBoundPredicateItem &other)
  : parent (other.parent), trait_item_ref (other.trait_item_ref)
{}

TypeBoundPredicateItem &
TypeBoundPredicateItem::operator= (const TypeBoundPredicateItem &other)
{
  parent = other.parent;
  trait_item_ref = other.trait_item_ref;

  return *this;
}

TypeBoundPredicateItem
TypeBoundPredicateItem::error ()
{
  return TypeBoundPredicateItem (TypeBoundPredicate::error (), nullptr);
}

bool
TypeBoundPredicateItem::is_error () const
{
  return parent.is_error () || trait_item_ref == nullptr;
}

const TypeBoundPredicate *
TypeBoundPredicateItem::get_parent () const
{
  return &parent;
}

tl::optional<TypeBoundPredicateItem>
TypeBoundPredicate::lookup_associated_item (
  const Resolver::TraitItemReference *ref) const
{
  return lookup_associated_item (ref->get_identifier ());
}

BaseType *
TypeBoundPredicateItem::get_tyty_for_receiver (const TyTy::BaseType *receiver)
{
  auto ctx = Resolver::TypeCheckContext::get ();

  TyTy::BaseType *trait_item_tyty = get_raw_item ()->get_tyty ();
  if (parent.get_substitution_arguments ().is_empty ())
    return trait_item_tyty;

  // set up the self mapping
  SubstitutionArgumentMappings gargs = parent.get_substitution_arguments ();
  rust_assert (!gargs.is_empty ());

  // The associated-type projection we are rebasing is stored in trait
  // coordinates
  //
  //   Self/X for trait SliceIndex<X>
  //
  // The predicate's own  SubstitutionParamMappings may have already been
  // mutated by SubstitutionParamMapping::fill_param_ty when the bound is a
  // where-clause like I: SliceIndex<[T]>: substituting Self with the
  // ParamType I rebinds the predicate's first param from Self to I.
  // Building adjusted_mappings from those renamed mappings would make
  // name-based lookup (get_argument_for_symbol) miss the trait's Self/X
  // symbols inside the projection.
  const auto &trait_substs = parent.get ()->get_trait_substs ();
  rust_assert (gargs.get_mappings ().size () <= trait_substs.size ());

  std::vector<SubstitutionArg> adjusted_mappings;
  for (size_t i = 0; i < gargs.get_mappings ().size (); i++)
    {
      auto &mapping = gargs.get_mappings ().at (i);

      bool is_implicit_self = i == 0;
      TyTy::BaseType *argument
	= is_implicit_self ? receiver->clone () : mapping.get_tyty ();

      adjusted_mappings.emplace_back (&trait_substs.at (i), argument);
    }

  SubstitutionArgumentMappings adjusted (adjusted_mappings, {},
					 gargs.get_regions (),
					 gargs.get_locus (),
					 true /* trait-mode-flag */);
  TyTy::BaseType *res
    = Resolver::SubstMapperInternal::Resolve (trait_item_tyty, adjusted);

  if (res != trait_item_tyty)
    {
      auto &mappings = Analysis::Mappings::get ();
      HirId fresh = mappings.get_next_hir_id ();
      res->set_ref (fresh);
      res->set_ty_ref (fresh);
      ctx->insert_implicit_type (fresh, res);
    }

  return res;
}
bool
TypeBoundPredicate::is_error () const
{
  auto context = Resolver::TypeCheckContext::get ();

  Resolver::TraitReference *ref = nullptr;
  bool ok = context->lookup_trait_reference (reference, &ref);

  return !ok || error_flag;
}

BaseType *
TypeBoundPredicate::handle_substitions (
  SubstitutionArgumentMappings &subst_mappings)
{
  for (auto &sub : get_substs ())
    {
      if (sub.get_param_ty () == nullptr)
	continue;

      auto p = sub.get_param_ty ();
      BaseType *r = p->resolve ();
      BaseType *s = Resolver::SubstMapperInternal::Resolve (r, subst_mappings);

      p->set_ty_ref (s->get_ty_ref ());
    }

  // FIXME more error handling at some point
  // used_arguments = subst_mappings;
  // error_flag |= used_arguments.is_error ();

  return nullptr;
}

bool
TypeBoundPredicate::requires_generic_args () const
{
  if (is_error ())
    return false;

  return substitutions.size () > 1;
}

bool
TypeBoundPredicate::contains_associated_types () const
{
  return get_num_associated_bindings () > 0;
}

size_t
TypeBoundPredicate::get_num_associated_bindings () const
{
  size_t count = 0;

  get_trait_hierachy ([&count] (const Resolver::TraitReference &ref) {
    for (const auto &trait_item : ref.get_trait_items ())
      {
	bool is_associated_type
	  = trait_item.get_trait_item_type ()
	    == Resolver::TraitItemReference::TraitItemType::TYPE;
	if (is_associated_type)
	  count++;
      }
  });

  return count;
}

void
TypeBoundPredicate::get_trait_hierachy (
  std::function<void (const Resolver::TraitReference &)> callback) const
{
  auto trait_ref = get ();
  callback (*trait_ref);

  for (auto &super : super_traits)
    {
      const auto &super_trait_ref = *super.get ();
      callback (super_trait_ref);
      super.get_trait_hierachy (callback);
    }
}

TypeBoundPredicateItem
TypeBoundPredicate::lookup_associated_type (const std::string &search)
{
  tl::optional<TypeBoundPredicateItem> item = lookup_associated_item (search);

  // only need to check that it is infact an associated type because other
  // wise if it was not found it will just be an error node anyway
  if (item.has_value ())
    {
      const auto raw = item->get_raw_item ();
      if (raw->get_trait_item_type ()
	  != Resolver::TraitItemReference::TraitItemType::TYPE)
	return TypeBoundPredicateItem::error ();
    }
  return item.value ();
}

std::vector<TypeBoundPredicateItem>
TypeBoundPredicate::get_associated_type_items ()
{
  std::vector<TypeBoundPredicateItem> items;
  auto trait_ref = get ();
  for (const auto &trait_item : trait_ref->get_trait_items ())
    {
      bool is_associated_type
	= trait_item.get_trait_item_type ()
	  == Resolver::TraitItemReference::TraitItemType::TYPE;
      if (is_associated_type)
	items.emplace_back (*this, &trait_item);
    }
  return items;
}

bool
TypeBoundPredicate::is_equal (const TypeBoundPredicate &other) const
{
  // check they match the same trait reference
  if (reference != other.reference)
    return false;

  // check that the generics match
  if (get_num_substitutions () != other.get_num_substitutions ())
    return false;

  // then match the generics applied
  for (size_t i = 0; i < get_num_substitutions (); i++)
    {
      SubstitutionParamMapping a = substitutions.at (i);
      SubstitutionParamMapping b = other.substitutions.at (i);

      auto ap = a.get_param_ty ();
      auto bp = b.get_param_ty ();

      BaseType *apd = ap->destructure ();
      BaseType *bpd = bp->destructure ();

      if (!Resolver::types_compatable (TyTy::TyWithLocation (apd),
				       TyTy::TyWithLocation (bpd),
				       UNKNOWN_LOCATION, false))
	return false;
    }

  return true;
}

bool
TypeBoundPredicate::validate_type_implements_super_traits (
  TyTy::BaseType &self, HIR::Type &impl_type, HIR::Type &trait) const
{
  if (get_polarity () != BoundPolarity::RegularBound)
    return true;

  auto &ptref = *get ();
  for (auto &super : super_traits)
    {
      if (super.get_polarity () != BoundPolarity::RegularBound)
	continue;

      if (!super.validate_type_implements_this (self, impl_type, trait))
	{
	  auto &sptref = *super.get ();

	  // emit error
	  std::string fixit1
	    = "required by this bound in: " + ptref.get_name ();
	  std::string fixit2 = "the trait " + sptref.get_name ()
			       + " is not implemented for "
			       + impl_type.to_string ();

	  rich_location r (line_table, trait.get_locus ());
	  r.add_fixit_insert_after (super.get_locus (), fixit1.c_str ());
	  r.add_fixit_insert_after (trait.get_locus (), fixit2.c_str ());
	  rust_error_at (r, ErrorCode::E0277,
			 "the trait bound %<%s: %s%> is not satisfied",
			 impl_type.to_string ().c_str (),
			 sptref.get_name ().c_str ());

	  return false;
	}

      if (!super.validate_type_implements_super_traits (self, impl_type, trait))
	return false;
    }

  return true;
}

bool
TypeBoundPredicate::validate_type_implements_this (TyTy::BaseType &self,
						   HIR::Type &impl_type,
						   HIR::Type &trait) const
{
  const auto &ptref = *get ();
  auto probed_bounds
    = Resolver::TypeBoundsProbe::Probe (&self, ptref.get_hir_trait_ref ());
  for (auto &elem : probed_bounds)
    {
      auto &tref = *(elem.first);
      if (ptref.is_equal (tref))
	return true;
    }

  return false;
}

// trait item reference

const Resolver::TraitItemReference *
TypeBoundPredicateItem::get_raw_item () const
{
  return trait_item_ref;
}

bool
TypeBoundPredicateItem::needs_implementation () const
{
  return !get_raw_item ()->is_optional ();
}

location_t
TypeBoundPredicateItem::get_locus () const
{
  return get_raw_item ()->get_locus ();
}

} // namespace TyTy
} // namespace Rust
