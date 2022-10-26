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

#include "rust-tyty.h"
#include "rust-tyty-visitor.h"
#include "rust-tyty-call.h"
#include "rust-hir-type-check-expr.h"
#include "rust-hir-type-check-type.h"
#include "rust-tyty-rules.h"
#include "rust-tyty-cmp.h"
#include "rust-hir-map.h"
#include "rust-substitution-mapper.h"
#include "rust-hir-trait-ref.h"
#include "rust-hir-type-bounds.h"

namespace Rust {
namespace TyTy {

std::string
TypeKindFormat::to_string (TypeKind kind)
{
  switch (kind)
    {
    case TypeKind::INFER:
      return "Infer";

    case TypeKind::ADT:
      return "ADT";

    case TypeKind::STR:
      return "STR";

    case TypeKind::REF:
      return "REF";

    case TypeKind::POINTER:
      return "POINTER";

    case TypeKind::PARAM:
      return "PARAM";

    case TypeKind::ARRAY:
      return "ARRAY";

    case TypeKind::SLICE:
      return "SLICE";

    case TypeKind::FNDEF:
      return "FnDef";

    case TypeKind::FNPTR:
      return "FnPtr";

    case TypeKind::TUPLE:
      return "Tuple";

    case TypeKind::BOOL:
      return "Bool";

    case TypeKind::CHAR:
      return "Char";

    case TypeKind::INT:
      return "Int";

    case TypeKind::UINT:
      return "Uint";

    case TypeKind::FLOAT:
      return "Float";

    case TypeKind::USIZE:
      return "Usize";

    case TypeKind::ISIZE:
      return "Isize";

    case TypeKind::NEVER:
      return "Never";

    case TypeKind::PLACEHOLDER:
      return "Placeholder";

    case TypeKind::PROJECTION:
      return "Projection";

    case TypeKind::DYNAMIC:
      return "Dynamic";

    case TypeKind::CLOSURE:
      return "Closure";

    case TypeKind::ERROR:
      return "ERROR";
    }
  gcc_unreachable ();
}

bool
is_primitive_type_kind (TypeKind kind)
{
  switch (kind)
    {
    case TypeKind::BOOL:
    case TypeKind::CHAR:
    case TypeKind::INT:
    case TypeKind::UINT:
    case TypeKind::ISIZE:
    case TypeKind::USIZE:
    case TypeKind::FLOAT:
    case TypeKind::NEVER:
    case TypeKind::STR:
      return true;
    default:
      return false;
    }
}

bool
BaseType::satisfies_bound (const TypeBoundPredicate &predicate) const
{
  const Resolver::TraitReference *query = predicate.get ();
  for (auto &bound : specified_bounds)
    {
      const Resolver::TraitReference *item = bound.get ();
      bool found = item->get_mappings ().get_defid ()
		   == query->get_mappings ().get_defid ();
      if (found)
	return true;
    }

  auto probed = Resolver::TypeBoundsProbe::Probe (this);
  for (auto &b : probed)
    {
      const Resolver::TraitReference *bound = b.first;
      bool found = bound->get_mappings ().get_defid ()
		   == query->get_mappings ().get_defid ();
      if (found)
	return true;
    }

  return false;
}

bool
BaseType::bounds_compatible (const BaseType &other, Location locus,
			     bool emit_error) const
{
  std::vector<std::reference_wrapper<const TypeBoundPredicate>>
    unsatisfied_bounds;
  for (auto &bound : get_specified_bounds ())
    {
      if (!other.satisfies_bound (bound))
	unsatisfied_bounds.push_back (bound);
    }

  // lets emit a single error for this
  if (unsatisfied_bounds.size () > 0)
    {
      RichLocation r (locus);
      std::string missing_preds;
      for (size_t i = 0; i < unsatisfied_bounds.size (); i++)
	{
	  const TypeBoundPredicate &pred = unsatisfied_bounds.at (i);
	  r.add_range (pred.get_locus ());
	  missing_preds += pred.get_name ();

	  bool have_next = (i + 1) < unsatisfied_bounds.size ();
	  if (have_next)
	    missing_preds += ", ";
	}

      if (emit_error)
	{
	  rust_error_at (r,
			 "bounds not satisfied for %s %<%s%> is not satisfied",
			 other.get_name ().c_str (), missing_preds.c_str ());
	  // rust_assert (!emit_error);
	}
    }

  return unsatisfied_bounds.size () == 0;
}

void
BaseType::inherit_bounds (const BaseType &other)
{
  inherit_bounds (other.get_specified_bounds ());
}

void
BaseType::inherit_bounds (
  const std::vector<TyTy::TypeBoundPredicate> &specified_bounds)
{
  // FIXME
  // 1. This needs to union the bounds
  // 2. Do some checking for trait polarity to ensure compatibility
  for (auto &bound : specified_bounds)
    {
      add_bound (bound);
    }
}

const BaseType *
BaseType::get_root () const
{
  // FIXME this needs to be it its own visitor class with a vector adjustments
  const TyTy::BaseType *root = this;
  if (get_kind () == TyTy::REF)
    {
      const ReferenceType *r = static_cast<const ReferenceType *> (root);
      root = r->get_base ()->get_root ();
    }
  else if (get_kind () == TyTy::POINTER)
    {
      const PointerType *r = static_cast<const PointerType *> (root);
      root = r->get_base ()->get_root ();
    }

  // these are an unsize
  else if (get_kind () == TyTy::SLICE)
    {
      const SliceType *r = static_cast<const SliceType *> (root);
      root = r->get_element_type ()->get_root ();
    }
  // else if (get_kind () == TyTy::ARRAY)
  //   {
  //     const ArrayType *r = static_cast<const ArrayType *> (root);
  //     root = r->get_element_type ()->get_root ();
  //   }

  return root;
}

const BaseType *
BaseType::destructure () const
{
  int recurisve_ops = 0;
  const BaseType *x = this;
  while (true)
    {
      if (recurisve_ops++ >= rust_max_recursion_depth)
	{
	  rust_error_at (
	    Location (),
	    "%<recursion depth%> count exceeds limit of %i (use "
	    "%<frust-max-recursion-depth=%> to increase the limit)",
	    rust_max_recursion_depth);
	  return new ErrorType (get_ref ());
	}

      switch (x->get_kind ())
	{
	  case TyTy::TypeKind::PARAM: {
	    const TyTy::ParamType *p = static_cast<const TyTy::ParamType *> (x);
	    x = p->resolve ();
	  }
	  break;

	  case TyTy::TypeKind::PLACEHOLDER: {
	    const TyTy::PlaceholderType *p
	      = static_cast<const TyTy::PlaceholderType *> (x);
	    rust_assert (p->can_resolve ());
	    x = p->resolve ();
	  }
	  break;

	  case TyTy::TypeKind::PROJECTION: {
	    const TyTy::ProjectionType *p
	      = static_cast<const TyTy::ProjectionType *> (x);
	    x = p->get ();
	  }
	  break;

	default:
	  return x;
	}
    }

  return x;
}

TyVar::TyVar (HirId ref) : ref (ref)
{
  // ensure this reference is defined within the context
  auto context = Resolver::TypeCheckContext::get ();
  BaseType *lookup = nullptr;
  bool ok = context->lookup_type (ref, &lookup);
  rust_assert (ok);
}

BaseType *
TyVar::get_tyty () const
{
  auto context = Resolver::TypeCheckContext::get ();
  BaseType *lookup = nullptr;
  bool ok = context->lookup_type (ref, &lookup);
  rust_assert (ok);
  return lookup;
}

TyVar
TyVar::get_implicit_infer_var (Location locus)
{
  auto mappings = Analysis::Mappings::get ();
  auto context = Resolver::TypeCheckContext::get ();

  InferType *infer = new InferType (mappings->get_next_hir_id (),
				    InferType::InferTypeKind::GENERAL, locus);
  context->insert_type (Analysis::NodeMapping (mappings->get_current_crate (),
					       UNKNOWN_NODEID,
					       infer->get_ref (),
					       UNKNOWN_LOCAL_DEFID),
			infer);
  mappings->insert_location (infer->get_ref (), locus);

  return TyVar (infer->get_ref ());
}

TyVar
TyVar::subst_covariant_var (TyTy::BaseType *orig, TyTy::BaseType *subst)
{
  if (orig->get_kind () != TyTy::TypeKind::PARAM)
    return TyVar (subst->get_ty_ref ());
  else if (subst->get_kind () == TyTy::TypeKind::PARAM)
    {
      TyTy::ParamType *p = static_cast<TyTy::ParamType *> (subst);
      if (p->resolve ()->get_kind () == TyTy::TypeKind::PARAM)
	{
	  return TyVar (subst->get_ty_ref ());
	}
    }

  return TyVar (subst->get_ref ());
}

TyVar
TyVar::clone () const
{
  TyTy::BaseType *c = get_tyty ()->clone ();
  return TyVar (c->get_ref ());
}

TyVar
TyVar::monomorphized_clone () const
{
  auto mappings = Analysis::Mappings::get ();
  auto context = Resolver::TypeCheckContext::get ();

  // this needs a new hirid
  TyTy::BaseType *c = get_tyty ()->monomorphized_clone ();
  c->set_ref (mappings->get_next_hir_id ());

  // insert it
  context->insert_type (Analysis::NodeMapping (mappings->get_current_crate (),
					       UNKNOWN_NODEID, c->get_ref (),
					       UNKNOWN_LOCAL_DEFID),
			c);

  return TyVar (c->get_ref ());
}

TyWithLocation::TyWithLocation (BaseType *ty, Location locus)
  : ty (ty), locus (locus)
{}

TyWithLocation::TyWithLocation (BaseType *ty) : ty (ty)
{
  auto mappings = Analysis::Mappings::get ();
  locus = mappings->lookup_location (ty->get_ref ());
}

void
InferType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
InferType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
InferType::as_string () const
{
  switch (infer_kind)
    {
    case GENERAL:
      return "T?";
    case INTEGRAL:
      return "<integer>";
    case FLOAT:
      return "<float>";
    }
  return "<infer::error>";
}

BaseType *
InferType::unify (BaseType *other)
{
  InferRules r (this);
  return r.unify (other);
}

bool
InferType::can_eq (const BaseType *other, bool emit_errors) const
{
  InferCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
InferType::clone () const
{
  // clones for inference variables are special in that they _must_ exist within
  // the type check context and we must ensure we don't loose the chain
  // otherwise we will end up in the missing type annotations case
  //
  // This means we cannot simply take over the same reference we must generate a
  // new ref just like the get_implicit_infer_var code then we can setup the
  // chain of references accordingly to ensure we don't loose the ability to
  // update the inference variables when we solve the type

  auto mappings = Analysis::Mappings::get ();
  auto context = Resolver::TypeCheckContext::get ();

  InferType *clone
    = new InferType (mappings->get_next_hir_id (), get_infer_kind (),
		     get_ident ().locus, get_combined_refs ());

  context->insert_type (Analysis::NodeMapping (mappings->get_current_crate (),
					       UNKNOWN_NODEID,
					       clone->get_ref (),
					       UNKNOWN_LOCAL_DEFID),
			clone);
  mappings->insert_location (clone->get_ref (),
			     mappings->lookup_location (get_ref ()));

  // setup the chain to reference this
  clone->append_reference (get_ref ());

  return clone;
}

BaseType *
InferType::monomorphized_clone () const
{
  return clone ();
}

bool
InferType::default_type (BaseType **type) const
{
  auto context = Resolver::TypeCheckContext::get ();
  bool ok = false;
  switch (infer_kind)
    {
    case GENERAL:
      return false;

      case INTEGRAL: {
	ok = context->lookup_builtin ("i32", type);
	rust_assert (ok);
	return ok;
      }

      case FLOAT: {
	ok = context->lookup_builtin ("f64", type);
	rust_assert (ok);
	return ok;
      }
    }
  return false;
}

void
ErrorType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ErrorType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ErrorType::as_string () const
{
  return "<tyty::error>";
}

BaseType *
ErrorType::unify (BaseType *other)
{
  return this;
}

bool
ErrorType::can_eq (const BaseType *other, bool emit_errors) const
{
  return get_kind () == other->get_kind ();
}

BaseType *
ErrorType::clone () const
{
  return new ErrorType (get_ref (), get_ty_ref (), get_combined_refs ());
}

BaseType *
ErrorType::monomorphized_clone () const
{
  return clone ();
}

std::string
StructFieldType::as_string () const
{
  return name + ":" + get_field_type ()->debug_str ();
}

bool
StructFieldType::is_equal (const StructFieldType &other) const
{
  bool names_eq = get_name ().compare (other.get_name ()) == 0;

  TyTy::BaseType *o = other.get_field_type ();
  if (o->get_kind () == TypeKind::PARAM)
    {
      ParamType *op = static_cast<ParamType *> (o);
      o = op->resolve ();
    }

  bool types_eq = get_field_type ()->is_equal (*o);

  return names_eq && types_eq;
}

StructFieldType *
StructFieldType::clone () const
{
  return new StructFieldType (get_ref (), get_name (),
			      get_field_type ()->clone ());
}

StructFieldType *
StructFieldType::monomorphized_clone () const
{
  return new StructFieldType (get_ref (), get_name (),
			      get_field_type ()->monomorphized_clone ());
}

bool
SubstitutionParamMapping::need_substitution () const
{
  if (!param->can_resolve ())
    return true;

  auto resolved = param->resolve ();
  return !resolved->is_concrete ();
}

bool
SubstitutionParamMapping::fill_param_ty (
  SubstitutionArgumentMappings &subst_mappings, Location locus)
{
  SubstitutionArg arg = SubstitutionArg::error ();
  bool ok = subst_mappings.get_argument_for_symbol (get_param_ty (), &arg);
  if (!ok)
    return true;

  TyTy::BaseType &type = *arg.get_tyty ();
  if (type.get_kind () == TyTy::TypeKind::INFER)
    {
      type.inherit_bounds (*param);
    }
  else
    {
      if (!param->bounds_compatible (type, locus, true))
	return false;
    }

  if (type.get_kind () == TypeKind::PARAM)
    {
      // delete param;
      param = static_cast<ParamType *> (type.clone ());
    }
  else
    {
      // check the substitution is compatible with bounds
      if (!param->bounds_compatible (type, locus, true))
	return false;

      // recursively pass this down to all HRTB's
      for (auto &bound : param->get_specified_bounds ())
	bound.handle_substitions (subst_mappings);

      param->set_ty_ref (type.get_ref ());
    }

  return true;
}

void
SubstitutionParamMapping::override_context ()
{
  if (!param->can_resolve ())
    return;

  auto mappings = Analysis::Mappings::get ();
  auto context = Resolver::TypeCheckContext::get ();

  context->insert_type (Analysis::NodeMapping (mappings->get_current_crate (),
					       UNKNOWN_NODEID,
					       param->get_ref (),
					       UNKNOWN_LOCAL_DEFID),
			param->resolve ());
}

SubstitutionArgumentMappings
SubstitutionRef::get_mappings_from_generic_args (HIR::GenericArgs &args)
{
  if (args.get_binding_args ().size () > 0)
    {
      RichLocation r (args.get_locus ());
      for (auto &binding : args.get_binding_args ())
	r.add_range (binding.get_locus ());

      rust_error_at (r, "associated type bindings are not allowed here");
      return SubstitutionArgumentMappings::error ();
    }

  // for inherited arguments
  size_t offs = used_arguments.size ();
  if (args.get_type_args ().size () + offs > substitutions.size ())
    {
      RichLocation r (args.get_locus ());
      r.add_range (substitutions.front ().get_param_locus ());

      rust_error_at (
	r,
	"generic item takes at most %lu type arguments but %lu were supplied",
	(unsigned long) substitutions.size (),
	(unsigned long) args.get_type_args ().size ());
      return SubstitutionArgumentMappings::error ();
    }

  if (args.get_type_args ().size () + offs < min_required_substitutions ())
    {
      RichLocation r (args.get_locus ());
      r.add_range (substitutions.front ().get_param_locus ());

      rust_error_at (
	r,
	"generic item takes at least %lu type arguments but %lu were supplied",
	(unsigned long) (min_required_substitutions () - offs),
	(unsigned long) args.get_type_args ().size ());
      return SubstitutionArgumentMappings::error ();
    }

  std::vector<SubstitutionArg> mappings = used_arguments.get_mappings ();
  for (auto &arg : args.get_type_args ())
    {
      BaseType *resolved = Resolver::TypeCheckType::Resolve (arg.get ());
      if (resolved == nullptr || resolved->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (args.get_locus (), "failed to resolve type arguments");
	  return SubstitutionArgumentMappings::error ();
	}

      SubstitutionArg subst_arg (&substitutions.at (offs), resolved);
      offs++;
      mappings.push_back (std::move (subst_arg));
    }

  // we must need to fill out defaults
  size_t left_over
    = num_required_substitutions () - min_required_substitutions ();
  if (left_over > 0)
    {
      for (size_t offs = mappings.size (); offs < substitutions.size (); offs++)
	{
	  SubstitutionParamMapping &param = substitutions.at (offs);
	  rust_assert (param.param_has_default_ty ());

	  BaseType *resolved = param.get_default_ty ();
	  if (resolved->get_kind () == TypeKind::ERROR)
	    return SubstitutionArgumentMappings::error ();

	  // this resolved default might already contain default parameters
	  if (resolved->contains_type_parameters ())
	    {
	      SubstitutionArgumentMappings intermediate (mappings,
							 args.get_locus ());
	      resolved = Resolver::SubstMapperInternal::Resolve (resolved,
								 intermediate);

	      if (resolved->get_kind () == TypeKind::ERROR)
		return SubstitutionArgumentMappings::error ();
	    }

	  SubstitutionArg subst_arg (&param, resolved);
	  mappings.push_back (std::move (subst_arg));
	}
    }

  return SubstitutionArgumentMappings (mappings, args.get_locus ());
}

SubstitutionArgumentMappings
SubstitutionRef::adjust_mappings_for_this (
  SubstitutionArgumentMappings &mappings)
{
  std::vector<SubstitutionArg> resolved_mappings;
  for (size_t i = 0; i < substitutions.size (); i++)
    {
      auto &subst = substitutions.at (i);

      SubstitutionArg arg = SubstitutionArg::error ();
      if (mappings.size () == substitutions.size ())
	{
	  mappings.get_argument_at (i, &arg);
	}
      else
	{
	  if (subst.needs_substitution ())
	    {
	      // get from passed in mappings
	      mappings.get_argument_for_symbol (subst.get_param_ty (), &arg);
	    }
	  else
	    {
	      // we should already have this somewhere
	      used_arguments.get_argument_for_symbol (subst.get_param_ty (),
						      &arg);
	    }
	}

      bool ok = !arg.is_error ();
      if (ok)
	{
	  SubstitutionArg adjusted (&subst, arg.get_tyty ());
	  resolved_mappings.push_back (std::move (adjusted));
	}
    }

  if (resolved_mappings.empty ())
    return SubstitutionArgumentMappings::error ();

  return SubstitutionArgumentMappings (resolved_mappings, mappings.get_locus (),
				       mappings.get_subst_cb (),
				       mappings.trait_item_mode ());
}

bool
SubstitutionRef::are_mappings_bound (SubstitutionArgumentMappings &mappings)
{
  std::vector<SubstitutionArg> resolved_mappings;
  for (size_t i = 0; i < substitutions.size (); i++)
    {
      auto &subst = substitutions.at (i);

      SubstitutionArg arg = SubstitutionArg::error ();
      if (mappings.size () == substitutions.size ())
	{
	  mappings.get_argument_at (i, &arg);
	}
      else
	{
	  if (subst.needs_substitution ())
	    {
	      // get from passed in mappings
	      mappings.get_argument_for_symbol (subst.get_param_ty (), &arg);
	    }
	  else
	    {
	      // we should already have this somewhere
	      used_arguments.get_argument_for_symbol (subst.get_param_ty (),
						      &arg);
	    }
	}

      bool ok = !arg.is_error ();
      if (ok)
	{
	  SubstitutionArg adjusted (&subst, arg.get_tyty ());
	  resolved_mappings.push_back (std::move (adjusted));
	}
    }

  return !resolved_mappings.empty ();
}

// this function assumes that the mappings being passed are for the same type as
// this new substitution reference so ordering matters here
SubstitutionArgumentMappings
SubstitutionRef::solve_mappings_from_receiver_for_self (
  SubstitutionArgumentMappings &mappings) const
{
  std::vector<SubstitutionArg> resolved_mappings;

  rust_assert (mappings.size () == get_num_substitutions ());
  for (size_t i = 0; i < get_num_substitutions (); i++)
    {
      const SubstitutionParamMapping &param_mapping = substitutions.at (i);
      SubstitutionArg &arg = mappings.get_mappings ().at (i);

      if (param_mapping.needs_substitution ())
	{
	  SubstitutionArg adjusted (&param_mapping, arg.get_tyty ());
	  resolved_mappings.push_back (std::move (adjusted));
	}
    }

  return SubstitutionArgumentMappings (resolved_mappings,
				       mappings.get_locus ());
}

SubstitutionArgumentMappings
SubstitutionRef::solve_missing_mappings_from_this (SubstitutionRef &ref,
						   SubstitutionRef &to)
{
  rust_assert (!ref.needs_substitution ());
  rust_assert (needs_substitution ());
  rust_assert (get_num_substitutions () == ref.get_num_substitutions ());

  Location locus = used_arguments.get_locus ();
  std::vector<SubstitutionArg> resolved_mappings;

  std::map<HirId, std::pair<ParamType *, BaseType *>> substs;
  for (size_t i = 0; i < get_num_substitutions (); i++)
    {
      SubstitutionParamMapping &a = substitutions.at (i);
      SubstitutionParamMapping &b = ref.substitutions.at (i);

      if (a.need_substitution ())
	{
	  const BaseType *root = a.get_param_ty ()->resolve ()->get_root ();
	  rust_assert (root->get_kind () == TyTy::TypeKind::PARAM);
	  const ParamType *p = static_cast<const TyTy::ParamType *> (root);

	  substs[p->get_ty_ref ()] = {static_cast<ParamType *> (p->clone ()),
				      b.get_param_ty ()->resolve ()};
	}
    }

  for (auto it = substs.begin (); it != substs.end (); it++)
    {
      HirId param_id = it->first;
      BaseType *arg = it->second.second;

      const SubstitutionParamMapping *associate_param = nullptr;
      for (SubstitutionParamMapping &p : to.substitutions)
	{
	  if (p.get_param_ty ()->get_ty_ref () == param_id)
	    {
	      associate_param = &p;
	      break;
	    }
	}

      rust_assert (associate_param != nullptr);
      SubstitutionArg argument (associate_param, arg);
      resolved_mappings.push_back (std::move (argument));
    }

  return SubstitutionArgumentMappings (resolved_mappings, locus);
}

bool
SubstitutionRef::monomorphize ()
{
  auto context = Resolver::TypeCheckContext::get ();
  for (const auto &subst : get_substs ())
    {
      const TyTy::ParamType *pty = subst.get_param_ty ();

      if (!pty->can_resolve ())
	continue;

      const TyTy::BaseType *binding = pty->resolve ();
      if (binding->get_kind () == TyTy::TypeKind::PARAM)
	continue;

      for (const auto &bound : pty->get_specified_bounds ())
	{
	  const Resolver::TraitReference *specified_bound_ref = bound.get ();

	  // setup any associated type mappings for the specified bonds and this
	  // type
	  auto candidates = Resolver::TypeBoundsProbe::Probe (binding);

	  Resolver::AssociatedImplTrait *associated_impl_trait = nullptr;
	  for (auto &probed_bound : candidates)
	    {
	      const Resolver::TraitReference *bound_trait_ref
		= probed_bound.first;
	      const HIR::ImplBlock *associated_impl = probed_bound.second;

	      HirId impl_block_id
		= associated_impl->get_mappings ().get_hirid ();
	      Resolver::AssociatedImplTrait *associated = nullptr;
	      bool found_impl_trait
		= context->lookup_associated_trait_impl (impl_block_id,
							 &associated);
	      if (found_impl_trait)
		{
		  bool found_trait
		    = specified_bound_ref->is_equal (*bound_trait_ref);
		  bool found_self
		    = associated->get_self ()->can_eq (binding, false);
		  if (found_trait && found_self)
		    {
		      associated_impl_trait = associated;
		      break;
		    }
		}
	    }

	  if (associated_impl_trait != nullptr)
	    {
	      associated_impl_trait->setup_associated_types (binding, bound);
	    }
	}
    }

  return true;
}

void
ADTType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ADTType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ADTType::as_string () const
{
  std::string variants_buffer;
  for (size_t i = 0; i < number_of_variants (); ++i)
    {
      TyTy::VariantDef *variant = variants.at (i);
      variants_buffer += variant->as_string ();
      if ((i + 1) < number_of_variants ())
	variants_buffer += ", ";
    }

  return identifier + subst_as_string () + "{" + variants_buffer + "}";
}

BaseType *
ADTType::unify (BaseType *other)
{
  ADTRules r (this);
  return r.unify (other);
}

bool
ADTType::can_eq (const BaseType *other, bool emit_errors) const
{
  ADTCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
ADTType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const ADTType &> (other);
  if (get_adt_kind () != other2.get_adt_kind ())
    return false;

  if (number_of_variants () != other2.number_of_variants ())
    return false;

  if (has_subsititions_defined () != other2.has_subsititions_defined ())
    return false;

  if (has_subsititions_defined ())
    {
      if (get_num_substitutions () != other2.get_num_substitutions ())
	return false;

      for (size_t i = 0; i < get_num_substitutions (); i++)
	{
	  const SubstitutionParamMapping &a = substitutions.at (i);
	  const SubstitutionParamMapping &b = other2.substitutions.at (i);

	  const ParamType *aa = a.get_param_ty ();
	  const ParamType *bb = b.get_param_ty ();
	  BaseType *aaa = aa->resolve ();
	  BaseType *bbb = bb->resolve ();
	  if (!aaa->is_equal (*bbb))
	    return false;
	}
    }

  for (size_t i = 0; i < number_of_variants (); i++)
    {
      const TyTy::VariantDef *a = get_variants ().at (i);
      const TyTy::VariantDef *b = other2.get_variants ().at (i);

      if (!a->is_equal (*b))
	return false;
    }

  return true;
}

BaseType *
ADTType::clone () const
{
  std::vector<VariantDef *> cloned_variants;
  for (auto &variant : variants)
    cloned_variants.push_back (variant->clone ());

  return new ADTType (get_ref (), get_ty_ref (), identifier, ident,
		      get_adt_kind (), cloned_variants, clone_substs (),
		      get_repr_options (), used_arguments,
		      get_combined_refs ());
}

BaseType *
ADTType::monomorphized_clone () const
{
  std::vector<VariantDef *> cloned_variants;
  for (auto &variant : variants)
    cloned_variants.push_back (variant->monomorphized_clone ());

  return new ADTType (get_ref (), get_ty_ref (), identifier, ident,
		      get_adt_kind (), cloned_variants, clone_substs (),
		      get_repr_options (), used_arguments,
		      get_combined_refs ());
}

static bool
handle_substitions (SubstitutionArgumentMappings &subst_mappings,
		    StructFieldType *field)
{
  auto fty = field->get_field_type ();
  bool is_param_ty = fty->get_kind () == TypeKind::PARAM;
  if (is_param_ty)
    {
      ParamType *p = static_cast<ParamType *> (fty);

      SubstitutionArg arg = SubstitutionArg::error ();
      bool ok = subst_mappings.get_argument_for_symbol (p, &arg);
      if (ok)
	{
	  auto argt = arg.get_tyty ();
	  bool arg_is_param = argt->get_kind () == TyTy::TypeKind::PARAM;
	  bool arg_is_concrete = argt->get_kind () != TyTy::TypeKind::INFER;

	  if (arg_is_param || arg_is_concrete)
	    {
	      auto new_field = argt->clone ();
	      new_field->set_ref (fty->get_ref ());
	      field->set_field_type (new_field);
	    }
	  else
	    {
	      field->get_field_type ()->set_ty_ref (argt->get_ref ());
	    }
	}
    }
  else if (fty->has_subsititions_defined () || fty->contains_type_parameters ())
    {
      BaseType *concrete
	= Resolver::SubstMapperInternal::Resolve (fty, subst_mappings);

      if (concrete->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (subst_mappings.get_locus (),
			 "Failed to resolve field substitution type: %s",
			 fty->as_string ().c_str ());
	  return false;
	}

      auto new_field = concrete->clone ();
      new_field->set_ref (fty->get_ref ());
      field->set_field_type (new_field);
    }

  return true;
}

ADTType *
ADTType::handle_substitions (SubstitutionArgumentMappings subst_mappings)
{
  ADTType *adt = static_cast<ADTType *> (clone ());
  adt->set_ty_ref (mappings->get_next_hir_id ());
  adt->used_arguments = subst_mappings;

  for (auto &sub : adt->get_substs ())
    {
      SubstitutionArg arg = SubstitutionArg::error ();
      bool ok
	= subst_mappings.get_argument_for_symbol (sub.get_param_ty (), &arg);
      if (ok)
	sub.fill_param_ty (subst_mappings, subst_mappings.get_locus ());
    }

  for (auto &variant : adt->get_variants ())
    {
      if (variant->is_dataless_variant ())
	continue;

      for (auto &field : variant->get_fields ())
	{
	  bool ok = ::Rust::TyTy::handle_substitions (subst_mappings, field);
	  if (!ok)
	    return adt;
	}
    }

  return adt;
}

void
TupleType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
TupleType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
TupleType::as_string () const
{
  std::string fields_buffer;
  for (const TyVar &field : get_fields ())
    {
      fields_buffer += field.get_tyty ()->as_string ();
      fields_buffer += ", ";
    }
  return "(" + fields_buffer + ")";
}

BaseType *
TupleType::get_field (size_t index) const
{
  return fields.at (index).get_tyty ();
}

BaseType *
TupleType::unify (BaseType *other)
{
  TupleRules r (this);
  return r.unify (other);
}

bool
TupleType::can_eq (const BaseType *other, bool emit_errors) const
{
  TupleCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
TupleType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const TupleType &> (other);
  if (num_fields () != other2.num_fields ())
    return false;

  for (size_t i = 0; i < num_fields (); i++)
    {
      if (!get_field (i)->is_equal (*other2.get_field (i)))
	return false;
    }
  return true;
}

BaseType *
TupleType::clone () const
{
  std::vector<TyVar> cloned_fields;
  for (const auto &f : fields)
    cloned_fields.push_back (f.clone ());

  return new TupleType (get_ref (), get_ty_ref (), get_ident ().locus,
			cloned_fields, get_combined_refs ());
}

BaseType *
TupleType::monomorphized_clone () const
{
  std::vector<TyVar> cloned_fields;
  for (const auto &f : fields)
    cloned_fields.push_back (f.monomorphized_clone ());

  return new TupleType (get_ref (), get_ty_ref (), get_ident ().locus,
			cloned_fields, get_combined_refs ());
}

TupleType *
TupleType::handle_substitions (SubstitutionArgumentMappings mappings)
{
  auto mappings_table = Analysis::Mappings::get ();

  TupleType *tuple = static_cast<TupleType *> (clone ());
  tuple->set_ref (mappings_table->get_next_hir_id ());
  tuple->set_ty_ref (mappings_table->get_next_hir_id ());

  for (size_t i = 0; i < tuple->fields.size (); i++)
    {
      TyVar &field = fields.at (i);
      if (field.get_tyty ()->contains_type_parameters ())
	{
	  BaseType *concrete
	    = Resolver::SubstMapperInternal::Resolve (field.get_tyty (),
						      mappings);
	  tuple->fields[i]
	    = TyVar::subst_covariant_var (field.get_tyty (), concrete);
	}
    }

  return tuple;
}

void
FnType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
FnType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
FnType::as_string () const
{
  std::string params_str = "";
  for (auto &param : params)
    {
      auto pattern = param.first;
      auto ty = param.second;
      params_str += pattern->as_string () + " " + ty->as_string ();
      params_str += ",";
    }

  std::string ret_str = type->as_string ();
  return "fn" + subst_as_string () + " (" + params_str + ") -> " + ret_str;
}

BaseType *
FnType::unify (BaseType *other)
{
  FnRules r (this);
  return r.unify (other);
}

bool
FnType::can_eq (const BaseType *other, bool emit_errors) const
{
  FnCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
FnType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const FnType &> (other);
  if (get_identifier ().compare (other2.get_identifier ()) != 0)
    return false;

  if (!get_return_type ()->is_equal (*other2.get_return_type ()))
    return false;

  if (has_subsititions_defined () != other2.has_subsititions_defined ())
    return false;

  if (has_subsititions_defined ())
    {
      if (get_num_substitutions () != other2.get_num_substitutions ())
	return false;

      const FnType &ofn = static_cast<const FnType &> (other);
      for (size_t i = 0; i < get_num_substitutions (); i++)
	{
	  const SubstitutionParamMapping &a = get_substs ().at (i);
	  const SubstitutionParamMapping &b = ofn.get_substs ().at (i);

	  const ParamType *pa = a.get_param_ty ();
	  const ParamType *pb = b.get_param_ty ();

	  if (!pa->is_equal (*pb))
	    return false;
	}
    }

  if (num_params () != other2.num_params ())
    return false;

  for (size_t i = 0; i < num_params (); i++)
    {
      auto lhs = param_at (i).second;
      auto rhs = other2.param_at (i).second;
      if (!lhs->is_equal (*rhs))
	return false;
    }
  return true;
}

BaseType *
FnType::clone () const
{
  std::vector<std::pair<HIR::Pattern *, BaseType *>> cloned_params;
  for (auto &p : params)
    cloned_params.push_back ({p.first, p.second->clone ()});

  return new FnType (get_ref (), get_ty_ref (), get_id (), get_identifier (),
		     ident, flags, abi, std::move (cloned_params),
		     get_return_type ()->clone (), clone_substs (),
		     get_combined_refs ());
}

BaseType *
FnType::monomorphized_clone () const
{
  std::vector<std::pair<HIR::Pattern *, BaseType *>> cloned_params;
  for (auto &p : params)
    cloned_params.push_back ({p.first, p.second->monomorphized_clone ()});

  return new FnType (get_ref (), get_ty_ref (), get_id (), get_identifier (),
		     ident, flags, abi, std::move (cloned_params),
		     get_return_type ()->clone (), clone_substs (),
		     get_combined_refs ());
}

FnType *
FnType::handle_substitions (SubstitutionArgumentMappings subst_mappings)
{
  FnType *fn = static_cast<FnType *> (clone ());
  fn->set_ty_ref (mappings->get_next_hir_id ());
  fn->used_arguments = subst_mappings;

  for (auto &sub : fn->get_substs ())
    {
      SubstitutionArg arg = SubstitutionArg::error ();

      bool ok
	= subst_mappings.get_argument_for_symbol (sub.get_param_ty (), &arg);
      if (ok)
	{
	  sub.fill_param_ty (subst_mappings, subst_mappings.get_locus ());
	}
    }

  auto fty = fn->get_return_type ();
  bool is_param_ty = fty->get_kind () == TypeKind::PARAM;
  if (is_param_ty)
    {
      ParamType *p = static_cast<ParamType *> (fty);

      SubstitutionArg arg = SubstitutionArg::error ();
      bool ok = subst_mappings.get_argument_for_symbol (p, &arg);
      if (ok)
	{
	  auto argt = arg.get_tyty ();
	  bool arg_is_param = argt->get_kind () == TyTy::TypeKind::PARAM;
	  bool arg_is_concrete = argt->get_kind () != TyTy::TypeKind::INFER;

	  if (arg_is_param || arg_is_concrete)
	    {
	      auto new_field = argt->clone ();
	      new_field->set_ref (fty->get_ref ());
	      fn->type = new_field;
	    }
	  else
	    {
	      fty->set_ty_ref (argt->get_ref ());
	    }
	}
    }
  else if (fty->needs_generic_substitutions ()
	   || fty->contains_type_parameters ())
    {
      BaseType *concrete
	= Resolver::SubstMapperInternal::Resolve (fty, subst_mappings);

      if (concrete == nullptr || concrete->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (subst_mappings.get_locus (),
			 "Failed to resolve field substitution type: %s",
			 fty->as_string ().c_str ());
	  return nullptr;
	}

      auto new_field = concrete->clone ();
      new_field->set_ref (fty->get_ref ());
      fn->type = new_field;
    }

  for (auto &param : fn->get_params ())
    {
      auto fty = param.second;

      bool is_param_ty = fty->get_kind () == TypeKind::PARAM;
      if (is_param_ty)
	{
	  ParamType *p = static_cast<ParamType *> (fty);

	  SubstitutionArg arg = SubstitutionArg::error ();
	  bool ok = subst_mappings.get_argument_for_symbol (p, &arg);
	  if (ok)
	    {
	      auto argt = arg.get_tyty ();
	      bool arg_is_param = argt->get_kind () == TyTy::TypeKind::PARAM;
	      bool arg_is_concrete = argt->get_kind () != TyTy::TypeKind::INFER;

	      if (arg_is_param || arg_is_concrete)
		{
		  auto new_field = argt->clone ();
		  new_field->set_ref (fty->get_ref ());
		  param.second = new_field;
		}
	      else
		{
		  fty->set_ty_ref (argt->get_ref ());
		}
	    }
	}
      else if (fty->has_subsititions_defined ()
	       || fty->contains_type_parameters ())
	{
	  BaseType *concrete
	    = Resolver::SubstMapperInternal::Resolve (fty, subst_mappings);

	  if (concrete == nullptr
	      || concrete->get_kind () == TyTy::TypeKind::ERROR)
	    {
	      rust_error_at (subst_mappings.get_locus (),
			     "Failed to resolve field substitution type: %s",
			     fty->as_string ().c_str ());
	      return nullptr;
	    }

	  auto new_field = concrete->clone ();
	  new_field->set_ref (fty->get_ref ());
	  param.second = new_field;
	}
    }

  return fn;
}

void
FnPtr::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
FnPtr::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
FnPtr::as_string () const
{
  std::string params_str;

  auto &params = get_params ();
  for (auto &p : params)
    {
      params_str += p.get_tyty ()->as_string () + " ,";
    }

  return "fnptr (" + params_str + ") -> " + get_return_type ()->as_string ();
}

BaseType *
FnPtr::unify (BaseType *other)
{
  FnptrRules r (this);
  return r.unify (other);
}

bool
FnPtr::can_eq (const BaseType *other, bool emit_errors) const
{
  FnptrCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
FnPtr::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const FnPtr &> (other);
  auto this_ret_type = get_return_type ();
  auto other_ret_type = other2.get_return_type ();
  if (this_ret_type->is_equal (*other_ret_type))
    return false;

  if (num_params () != other2.num_params ())
    return false;

  for (size_t i = 0; i < num_params (); i++)
    {
      if (!param_at (i)->is_equal (*other2.param_at (i)))
	return false;
    }
  return true;
}

BaseType *
FnPtr::clone () const
{
  std::vector<TyVar> cloned_params;
  for (auto &p : params)
    cloned_params.push_back (TyVar (p.get_ref ()));

  return new FnPtr (get_ref (), get_ty_ref (), ident.locus,
		    std::move (cloned_params), result_type,
		    get_combined_refs ());
}

BaseType *
FnPtr::monomorphized_clone () const
{
  std::vector<TyVar> cloned_params;
  for (auto &p : params)
    cloned_params.push_back (p.monomorphized_clone ());

  return new FnPtr (get_ref (), get_ty_ref (), ident.locus,
		    std::move (cloned_params), result_type,
		    get_combined_refs ());
}

void
ClosureType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ClosureType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ClosureType::as_string () const
{
  return "TODO";
}

BaseType *
ClosureType::unify (BaseType *other)
{
  ClosureRules r (this);
  return r.unify (other);
}

bool
ClosureType::can_eq (const BaseType *other, bool emit_errors) const
{
  ClosureCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
ClosureType::is_equal (const BaseType &other) const
{
  gcc_unreachable ();
  return false;
}

BaseType *
ClosureType::clone () const
{
  return new ClosureType (get_ref (), get_ty_ref (), ident, id, parameter_types,
			  result_type, clone_substs (), get_combined_refs ());
}

BaseType *
ClosureType::monomorphized_clone () const
{
  return clone ();
}

ClosureType *
ClosureType::handle_substitions (SubstitutionArgumentMappings mappings)
{
  gcc_unreachable ();
  return nullptr;
}

void
ArrayType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ArrayType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ArrayType::as_string () const
{
  return "[" + get_element_type ()->as_string () + ":" + "CAPACITY" + "]";
}

BaseType *
ArrayType::unify (BaseType *other)
{
  ArrayRules r (this);
  return r.unify (other);
}

bool
ArrayType::can_eq (const BaseType *other, bool emit_errors) const
{
  ArrayCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
ArrayType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const ArrayType &> (other);

  auto this_element_type = get_element_type ();
  auto other_element_type = other2.get_element_type ();

  return this_element_type->is_equal (*other_element_type);
}

BaseType *
ArrayType::get_element_type () const
{
  return element_type.get_tyty ();
}

BaseType *
ArrayType::clone () const
{
  return new ArrayType (get_ref (), get_ty_ref (), ident.locus, capacity_expr,
			element_type, get_combined_refs ());
}

BaseType *
ArrayType::monomorphized_clone () const
{
  return new ArrayType (get_ref (), get_ty_ref (), ident.locus, capacity_expr,
			element_type.monomorphized_clone (),
			get_combined_refs ());
}

ArrayType *
ArrayType::handle_substitions (SubstitutionArgumentMappings mappings)
{
  auto mappings_table = Analysis::Mappings::get ();

  ArrayType *ref = static_cast<ArrayType *> (clone ());
  ref->set_ty_ref (mappings_table->get_next_hir_id ());

  // might be &T or &ADT so this needs to be recursive
  auto base = ref->get_element_type ();
  BaseType *concrete = Resolver::SubstMapperInternal::Resolve (base, mappings);
  ref->element_type = TyVar::subst_covariant_var (base, concrete);

  return ref;
}

void
SliceType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
SliceType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
SliceType::as_string () const
{
  return "[" + get_element_type ()->as_string () + "]";
}

BaseType *
SliceType::unify (BaseType *other)
{
  SliceRules r (this);
  return r.unify (other);
}

bool
SliceType::can_eq (const BaseType *other, bool emit_errors) const
{
  SliceCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
SliceType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const SliceType &> (other);

  auto this_element_type = get_element_type ();
  auto other_element_type = other2.get_element_type ();

  return this_element_type->is_equal (*other_element_type);
}

BaseType *
SliceType::get_element_type () const
{
  return element_type.get_tyty ();
}

BaseType *
SliceType::clone () const
{
  return new SliceType (get_ref (), get_ty_ref (), ident.locus,
			element_type.clone (), get_combined_refs ());
}

BaseType *
SliceType::monomorphized_clone () const
{
  return new SliceType (get_ref (), get_ty_ref (), ident.locus,
			element_type.monomorphized_clone (),
			get_combined_refs ());
}

SliceType *
SliceType::handle_substitions (SubstitutionArgumentMappings mappings)
{
  auto mappings_table = Analysis::Mappings::get ();

  SliceType *ref = static_cast<SliceType *> (clone ());
  ref->set_ty_ref (mappings_table->get_next_hir_id ());

  // might be &T or &ADT so this needs to be recursive
  auto base = ref->get_element_type ();
  BaseType *concrete = Resolver::SubstMapperInternal::Resolve (base, mappings);
  ref->element_type = TyVar::subst_covariant_var (base, concrete);

  return ref;
}

void
BoolType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
BoolType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
BoolType::as_string () const
{
  return "bool";
}

BaseType *
BoolType::unify (BaseType *other)
{
  BoolRules r (this);
  return r.unify (other);
}

bool
BoolType::can_eq (const BaseType *other, bool emit_errors) const
{
  BoolCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
BoolType::clone () const
{
  return new BoolType (get_ref (), get_ty_ref (), get_combined_refs ());
}

BaseType *
BoolType::monomorphized_clone () const
{
  return clone ();
}

void
IntType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
IntType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
IntType::as_string () const
{
  switch (int_kind)
    {
    case I8:
      return "i8";
    case I16:
      return "i16";
    case I32:
      return "i32";
    case I64:
      return "i64";
    case I128:
      return "i128";
    }
  gcc_unreachable ();
  return "__unknown_int_type";
}

BaseType *
IntType::unify (BaseType *other)
{
  IntRules r (this);
  return r.unify (other);
}

bool
IntType::can_eq (const BaseType *other, bool emit_errors) const
{
  IntCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
IntType::clone () const
{
  return new IntType (get_ref (), get_ty_ref (), get_int_kind (),
		      get_combined_refs ());
}

BaseType *
IntType::monomorphized_clone () const
{
  return clone ();
}

bool
IntType::is_equal (const BaseType &other) const
{
  if (!BaseType::is_equal (other))
    return false;

  const IntType &o = static_cast<const IntType &> (other);
  return get_int_kind () == o.get_int_kind ();
}

void
UintType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
UintType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
UintType::as_string () const
{
  switch (uint_kind)
    {
    case U8:
      return "u8";
    case U16:
      return "u16";
    case U32:
      return "u32";
    case U64:
      return "u64";
    case U128:
      return "u128";
    }
  gcc_unreachable ();
  return "__unknown_uint_type";
}

BaseType *
UintType::unify (BaseType *other)
{
  UintRules r (this);
  return r.unify (other);
}

bool
UintType::can_eq (const BaseType *other, bool emit_errors) const
{
  UintCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
UintType::clone () const
{
  return new UintType (get_ref (), get_ty_ref (), get_uint_kind (),
		       get_combined_refs ());
}

BaseType *
UintType::monomorphized_clone () const
{
  return clone ();
}

bool
UintType::is_equal (const BaseType &other) const
{
  if (!BaseType::is_equal (other))
    return false;

  const UintType &o = static_cast<const UintType &> (other);
  return get_uint_kind () == o.get_uint_kind ();
}

void
FloatType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
FloatType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
FloatType::as_string () const
{
  switch (float_kind)
    {
    case F32:
      return "f32";
    case F64:
      return "f64";
    }
  gcc_unreachable ();
  return "__unknown_float_type";
}

BaseType *
FloatType::unify (BaseType *other)
{
  FloatRules r (this);
  return r.unify (other);
}

bool
FloatType::can_eq (const BaseType *other, bool emit_errors) const
{
  FloatCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
FloatType::clone () const
{
  return new FloatType (get_ref (), get_ty_ref (), get_float_kind (),
			get_combined_refs ());
}

BaseType *
FloatType::monomorphized_clone () const
{
  return clone ();
}

bool
FloatType::is_equal (const BaseType &other) const
{
  if (!BaseType::is_equal (other))
    return false;

  const FloatType &o = static_cast<const FloatType &> (other);
  return get_float_kind () == o.get_float_kind ();
}

void
USizeType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
USizeType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
USizeType::as_string () const
{
  return "usize";
}

BaseType *
USizeType::unify (BaseType *other)
{
  USizeRules r (this);
  return r.unify (other);
}

bool
USizeType::can_eq (const BaseType *other, bool emit_errors) const
{
  USizeCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
USizeType::clone () const
{
  return new USizeType (get_ref (), get_ty_ref (), get_combined_refs ());
}

BaseType *
USizeType::monomorphized_clone () const
{
  return clone ();
}

void
ISizeType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ISizeType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ISizeType::as_string () const
{
  return "isize";
}

BaseType *
ISizeType::unify (BaseType *other)
{
  ISizeRules r (this);
  return r.unify (other);
}

bool
ISizeType::can_eq (const BaseType *other, bool emit_errors) const
{
  ISizeCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
ISizeType::clone () const
{
  return new ISizeType (get_ref (), get_ty_ref (), get_combined_refs ());
}

BaseType *
ISizeType::monomorphized_clone () const
{
  return clone ();
}

void
CharType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
CharType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
CharType::as_string () const
{
  return "char";
}

BaseType *
CharType::unify (BaseType *other)
{
  CharRules r (this);
  return r.unify (other);
}

bool
CharType::can_eq (const BaseType *other, bool emit_errors) const
{
  CharCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
CharType::clone () const
{
  return new CharType (get_ref (), get_ty_ref (), get_combined_refs ());
}

BaseType *
CharType::monomorphized_clone () const
{
  return clone ();
}

void
ReferenceType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ReferenceType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ReferenceType::as_string () const
{
  return std::string ("&") + (is_mutable () ? "mut" : "") + " "
	 + get_base ()->as_string ();
}

BaseType *
ReferenceType::unify (BaseType *other)
{
  ReferenceRules r (this);
  return r.unify (other);
}

bool
ReferenceType::can_eq (const BaseType *other, bool emit_errors) const
{
  ReferenceCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
ReferenceType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const ReferenceType &> (other);
  if (mutability () != other2.mutability ())
    return false;

  return get_base ()->is_equal (*other2.get_base ());
}

BaseType *
ReferenceType::get_base () const
{
  return base.get_tyty ();
}

BaseType *
ReferenceType::clone () const
{
  return new ReferenceType (get_ref (), get_ty_ref (), base, mutability (),
			    get_combined_refs ());
}

BaseType *
ReferenceType::monomorphized_clone () const
{
  return new ReferenceType (get_ref (), get_ty_ref (),
			    base.monomorphized_clone (), mutability (),
			    get_combined_refs ());
}

ReferenceType *
ReferenceType::handle_substitions (SubstitutionArgumentMappings mappings)
{
  auto mappings_table = Analysis::Mappings::get ();

  ReferenceType *ref = static_cast<ReferenceType *> (clone ());
  ref->set_ty_ref (mappings_table->get_next_hir_id ());

  // might be &T or &ADT so this needs to be recursive
  auto base = ref->get_base ();
  BaseType *concrete = Resolver::SubstMapperInternal::Resolve (base, mappings);
  ref->base = TyVar::subst_covariant_var (base, concrete);

  return ref;
}

void
PointerType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
PointerType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
PointerType::as_string () const
{
  return std::string ("* ") + (is_mutable () ? "mut" : "const") + " "
	 + get_base ()->as_string ();
}

BaseType *
PointerType::unify (BaseType *other)
{
  PointerRules r (this);
  return r.unify (other);
}

bool
PointerType::can_eq (const BaseType *other, bool emit_errors) const
{
  PointerCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
PointerType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  auto other2 = static_cast<const PointerType &> (other);
  if (mutability () != other2.mutability ())
    return false;

  return get_base ()->is_equal (*other2.get_base ());
}

BaseType *
PointerType::get_base () const
{
  return base.get_tyty ();
}

BaseType *
PointerType::clone () const
{
  return new PointerType (get_ref (), get_ty_ref (), base, mutability (),
			  get_combined_refs ());
}

BaseType *
PointerType::monomorphized_clone () const
{
  return new PointerType (get_ref (), get_ty_ref (),
			  base.monomorphized_clone (), mutability (),
			  get_combined_refs ());
}

PointerType *
PointerType::handle_substitions (SubstitutionArgumentMappings mappings)
{
  auto mappings_table = Analysis::Mappings::get ();

  PointerType *ref = static_cast<PointerType *> (clone ());
  ref->set_ty_ref (mappings_table->get_next_hir_id ());

  // might be &T or &ADT so this needs to be recursive
  auto base = ref->get_base ();
  BaseType *concrete = Resolver::SubstMapperInternal::Resolve (base, mappings);
  ref->base = TyVar::subst_covariant_var (base, concrete);

  return ref;
}

void
ParamType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ParamType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ParamType::as_string () const
{
  if (!can_resolve ())
    {
      return get_symbol () + " REF: " + std::to_string (get_ref ());
    }

  BaseType *lookup = resolve ();
  return get_symbol () + "=" + lookup->as_string ();
}

std::string
ParamType::get_name () const
{
  if (!can_resolve ())
    return get_symbol ();

  return resolve ()->get_name ();
}

BaseType *
ParamType::unify (BaseType *other)
{
  ParamRules r (this);
  return r.unify (other);
}

bool
ParamType::can_eq (const BaseType *other, bool emit_errors) const
{
  ParamCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
ParamType::clone () const
{
  return new ParamType (get_symbol (), ident.locus, get_ref (), get_ty_ref (),
			param, get_specified_bounds (), get_combined_refs ());
}

BaseType *
ParamType::monomorphized_clone () const
{
  return resolve ()->clone ();
}

std::string
ParamType::get_symbol () const
{
  return symbol;
}

BaseType *
ParamType::resolve () const
{
  TyVar var (get_ty_ref ());
  BaseType *r = var.get_tyty ();

  while (r->get_kind () == TypeKind::PARAM)
    {
      ParamType *rr = static_cast<ParamType *> (r);
      if (!rr->can_resolve ())
	break;

      TyVar v (rr->get_ty_ref ());
      r = v.get_tyty ();
    }

  if (r->get_kind () == TypeKind::PARAM && (r->get_ref () == r->get_ty_ref ()))
    return TyVar (r->get_ty_ref ()).get_tyty ();

  return r;
}

bool
ParamType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    {
      if (!can_resolve ())
	return false;

      return resolve ()->is_equal (other);
    }

  auto other2 = static_cast<const ParamType &> (other);
  if (can_resolve () != other2.can_resolve ())
    return false;

  if (can_resolve ())
    return resolve ()->can_eq (other2.resolve (), false);

  return get_symbol ().compare (other2.get_symbol ()) == 0;
}

ParamType *
ParamType::handle_substitions (SubstitutionArgumentMappings subst_mappings)
{
  SubstitutionArg arg = SubstitutionArg::error ();
  bool ok = subst_mappings.get_argument_for_symbol (this, &arg);
  if (!ok || arg.is_error ())
    return this;

  ParamType *p = static_cast<ParamType *> (clone ());
  subst_mappings.on_param_subst (*p, arg);

  // there are two cases one where we substitute directly to a new PARAM and
  // otherwise
  if (arg.get_tyty ()->get_kind () == TyTy::TypeKind::PARAM)
    {
      p->set_ty_ref (arg.get_tyty ()->get_ref ());
      return p;
    }

  // this is the new subst that this needs to pass
  p->set_ref (mappings->get_next_hir_id ());
  p->set_ty_ref (arg.get_tyty ()->get_ref ());

  return p;
}

BaseType *
StrType::clone () const
{
  return new StrType (get_ref (), get_ty_ref (), get_combined_refs ());
}

BaseType *
StrType::monomorphized_clone () const
{
  return clone ();
}

void
StrType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
StrType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
StrType::as_string () const
{
  return "str";
}

BaseType *
StrType::unify (BaseType *other)
{
  StrRules r (this);
  return r.unify (other);
}

bool
StrType::can_eq (const BaseType *other, bool emit_errors) const
{
  StrCmp r (this, emit_errors);
  return r.can_eq (other);
}

bool
StrType::is_equal (const BaseType &other) const
{
  return get_kind () == other.get_kind ();
}

void
NeverType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
NeverType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
NeverType::as_string () const
{
  return "!";
}

BaseType *
NeverType::unify (BaseType *other)
{
  NeverRules r (this);
  return r.unify (other);
}

bool
NeverType::can_eq (const BaseType *other, bool emit_errors) const
{
  NeverCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
NeverType::clone () const
{
  return new NeverType (get_ref (), get_ty_ref (), get_combined_refs ());
}

BaseType *
NeverType::monomorphized_clone () const
{
  return clone ();
}

// placeholder type

void
PlaceholderType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
PlaceholderType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
PlaceholderType::as_string () const
{
  return "<placeholder:" + (can_resolve () ? resolve ()->as_string () : "")
	 + ">";
}

BaseType *
PlaceholderType::unify (BaseType *other)
{
  PlaceholderRules r (this);
  return r.unify (other);
}

bool
PlaceholderType::can_eq (const BaseType *other, bool emit_errors) const
{
  PlaceholderCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
PlaceholderType::clone () const
{
  return new PlaceholderType (get_symbol (), get_ref (), get_ty_ref (),
			      get_combined_refs ());
}

BaseType *
PlaceholderType::monomorphized_clone () const
{
  if (can_resolve ())
    return resolve ()->monomorphized_clone ();

  return clone ();
}

void
PlaceholderType::set_associated_type (HirId ref)
{
  auto context = Resolver::TypeCheckContext::get ();
  context->insert_associated_type_mapping (get_ty_ref (), ref);
}

void
PlaceholderType::clear_associated_type ()
{
  auto context = Resolver::TypeCheckContext::get ();
  context->clear_associated_type_mapping (get_ty_ref ());
}

bool
PlaceholderType::can_resolve () const
{
  auto context = Resolver::TypeCheckContext::get ();
  return context->lookup_associated_type_mapping (get_ty_ref (), nullptr);
}

BaseType *
PlaceholderType::resolve () const
{
  auto context = Resolver::TypeCheckContext::get ();

  HirId mapping;
  bool ok = context->lookup_associated_type_mapping (get_ty_ref (), &mapping);
  rust_assert (ok);

  return TyVar (mapping).get_tyty ();
}

bool
PlaceholderType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    {
      if (!can_resolve ())
	return false;

      return resolve ()->is_equal (other);
    }

  auto other2 = static_cast<const PlaceholderType &> (other);
  return get_symbol ().compare (other2.get_symbol ()) == 0;
}

// Projection type

void
ProjectionType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
ProjectionType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
ProjectionType::as_string () const
{
  return "<Projection=" + subst_as_string () + "::" + base->as_string () + ">";
}

BaseType *
ProjectionType::unify (BaseType *other)
{
  return base->unify (other);
}

bool
ProjectionType::can_eq (const BaseType *other, bool emit_errors) const
{
  return base->can_eq (other, emit_errors);
}

BaseType *
ProjectionType::clone () const
{
  return new ProjectionType (get_ref (), get_ty_ref (), base->clone (), trait,
			     item, clone_substs (), used_arguments,
			     get_combined_refs ());
}

BaseType *
ProjectionType::monomorphized_clone () const
{
  return get ()->monomorphized_clone ();
}

ProjectionType *
ProjectionType::handle_substitions (SubstitutionArgumentMappings subst_mappings)
{
  // // do we really need to substitute this?
  // if (base->needs_generic_substitutions () || base->contains_type_parameters
  // ())
  //   {
  //     return this;
  //   }

  ProjectionType *projection = static_cast<ProjectionType *> (clone ());
  projection->set_ty_ref (mappings->get_next_hir_id ());
  projection->used_arguments = subst_mappings;

  auto context = Resolver::TypeCheckContext::get ();
  context->insert_implicit_type (projection->get_ty_ref (), projection);

  for (auto &sub : projection->get_substs ())
    {
      SubstitutionArg arg = SubstitutionArg::error ();
      bool ok
	= subst_mappings.get_argument_for_symbol (sub.get_param_ty (), &arg);
      if (ok)
	sub.fill_param_ty (subst_mappings, subst_mappings.get_locus ());
    }

  auto fty = projection->base;
  bool is_param_ty = fty->get_kind () == TypeKind::PARAM;
  if (is_param_ty)
    {
      ParamType *p = static_cast<ParamType *> (fty);

      SubstitutionArg arg = SubstitutionArg::error ();
      bool ok = subst_mappings.get_argument_for_symbol (p, &arg);
      if (ok)
	{
	  auto argt = arg.get_tyty ();
	  bool arg_is_param = argt->get_kind () == TyTy::TypeKind::PARAM;
	  bool arg_is_concrete = argt->get_kind () != TyTy::TypeKind::INFER;

	  if (arg_is_param || arg_is_concrete)
	    {
	      auto new_field = argt->clone ();
	      new_field->set_ref (fty->get_ref ());
	      projection->base = new_field;
	    }
	  else
	    {
	      fty->set_ty_ref (argt->get_ref ());
	    }
	}
    }
  else if (fty->needs_generic_substitutions ()
	   || fty->contains_type_parameters ())
    {
      BaseType *concrete
	= Resolver::SubstMapperInternal::Resolve (fty, subst_mappings);

      if (concrete == nullptr || concrete->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (subst_mappings.get_locus (),
			 "Failed to resolve field substitution type: %s",
			 fty->as_string ().c_str ());
	  return nullptr;
	}

      projection->base = concrete;
    }

  return projection;
}

void
DynamicObjectType::accept_vis (TyVisitor &vis)
{
  vis.visit (*this);
}

void
DynamicObjectType::accept_vis (TyConstVisitor &vis) const
{
  vis.visit (*this);
}

std::string
DynamicObjectType::as_string () const
{
  return "dyn [" + raw_bounds_as_string () + "]";
}

BaseType *
DynamicObjectType::unify (BaseType *other)
{
  DynamicRules r (this);
  return r.unify (other);
}

bool
DynamicObjectType::can_eq (const BaseType *other, bool emit_errors) const
{
  DynamicCmp r (this, emit_errors);
  return r.can_eq (other);
}

BaseType *
DynamicObjectType::clone () const
{
  return new DynamicObjectType (get_ref (), get_ty_ref (), ident,
				specified_bounds, get_combined_refs ());
}

BaseType *
DynamicObjectType::monomorphized_clone () const
{
  return clone ();
}

std::string
DynamicObjectType::get_name () const
{
  return "dyn [" + raw_bounds_as_name () + "]";
}

bool
DynamicObjectType::is_equal (const BaseType &other) const
{
  if (get_kind () != other.get_kind ())
    return false;

  if (num_specified_bounds () != other.num_specified_bounds ())
    return false;

  return bounds_compatible (other, Location (), false);
}

const std::vector<
  std::pair<const Resolver::TraitItemReference *, const TypeBoundPredicate *>>
DynamicObjectType::get_object_items () const
{
  std::vector<
    std::pair<const Resolver::TraitItemReference *, const TypeBoundPredicate *>>
    items;
  for (auto &bound : get_specified_bounds ())
    {
      const Resolver::TraitReference *trait = bound.get ();
      for (auto &item : trait->get_trait_items ())
	{
	  if (item.get_trait_item_type ()
		== Resolver::TraitItemReference::TraitItemType::FN
	      && item.is_object_safe ())
	    items.push_back ({&item, &bound});
	}

      for (auto &super_trait : trait->get_super_traits ())
	{
	  for (auto &item : super_trait->get_trait_items ())
	    {
	      if (item.get_trait_item_type ()
		    == Resolver::TraitItemReference::TraitItemType::FN
		  && item.is_object_safe ())
		items.push_back ({&item, &bound});
	    }
	}
    }
  return items;
}

} // namespace TyTy
} // namespace Rust
