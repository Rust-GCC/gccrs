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

#include "rust-drop-check.h"
#include "rust-diagnostics.h"
#include "rust-lang-item.h"
#include "rust-hir-type-check.h"
#include "optional.h"
#include "rust-mapping-common.h"
#include "rust-tyty.h"

namespace Rust {
namespace DropCheck {

tl::optional<HirId>
DropChecker::get_trait_hirid (LangItem::Kind lang_kind)
{
  auto &mappings = Analysis::Mappings::get ();
  auto opt_trait = mappings.lookup_lang_item_node (lang_kind);
  if (!opt_trait.has_value ())
    return tl::nullopt;
  return mappings.lookup_node_to_hir (opt_trait.value ());
}

tl::optional<HirId>
DropChecker::get_drop_hirid ()
{
  return get_trait_hirid (LangItem::Kind::DROP);
}

tl::optional<HirId>
DropChecker::get_copy_hirid ()
{
  return get_trait_hirid (LangItem::Kind::COPY);
}

bool
DropChecker::is_manually_drop (const TyTy::BaseType *ty)
{
  auto &mappings = Analysis::Mappings::get ();
  auto manually_drop
    = mappings.lookup_lang_item (LangItem::Kind::MANUALLY_DROP);
  if (!manually_drop.has_value ())
    return false;
  return ty->as<const TyTy::ADTType> ()->get_id () == manually_drop.value ();
}

tl::optional<HirId>
DropChecker::impl_trait (const TyTy::BaseType *ty, HirId TraitHirId)
{
  TyTy::BaseType *orig = nullptr;
  auto tyctx = Resolver::TypeCheckContext::get ();
  if (!tyctx->lookup_type (ty->get_ref (), &orig))
    return tl::nullopt;

  std::vector<std::pair<TyTy::BaseType *, HirId>> impl_map;
  if (!tyctx->peek_associated_impl_mapping_for_self (TraitHirId, &impl_map))
    return tl::nullopt;

  if (orig->get_kind () != TyTy::TypeKind::ADT)
    return tl::nullopt;

  auto orig_adt = orig->as<TyTy::ADTType> ();
  for (auto &entry : impl_map)
    {
      auto RustType = entry.first;
      auto ImplBlockHirId = entry.second;

      if (RustType->get_kind () != TyTy::TypeKind::ADT)
	continue;

      if (orig_adt->get_id () == RustType->as<TyTy::ADTType> ()->get_id ())
	return ImplBlockHirId;
    }
  return tl::nullopt;
}

tl::optional<HirId>
DropChecker::impl_drop (const TyTy::BaseType *ty)
{
  auto opt_hirid = get_drop_hirid ();
  if (!opt_hirid.has_value ())
    return tl::nullopt;
  return impl_trait (ty, opt_hirid.value ());
}
tl::optional<HirId>
DropChecker::impl_copy (const TyTy::BaseType *ty)
{
  auto opt_hirid = get_copy_hirid ();
  if (!opt_hirid.has_value ())
    return tl::nullopt;
  return impl_trait (ty, opt_hirid.value ());
}

bool
DropChecker::needs_drop (const TyTy::BaseType *ty)
{
  // TODO : handle remaining types
  if (is_manually_drop (ty))
    return false;

  if (impl_drop (ty).has_value ())
    return true;

  switch (ty->get_kind ())
    {
    case TyTy::ADT:
      {
	auto adt_type = ty->as<const TyTy::ADTType> ();
	for (auto variant : adt_type->get_variants ())
	  for (auto field : variant->get_fields ())
	    if (needs_drop (field->get_field_type ()))
	      return true;
	return false;
      }
    case TyTy::TUPLE:
      {
	auto tuple_type = ty->as<const TyTy::TupleType> ();
	for (auto &field : tuple_type->get_fields ())
	  if (needs_drop (field.get_tyty ()))
	    return true;
	return false;
      }
    case TyTy::ARRAY:
      return needs_drop (ty->as<const TyTy::ArrayType> ()->get_element_type ());
    default:
      return false;
    }
}

void
DropChecker::check_copy_drop (const TyTy::BaseType *ty, HirId trait_hirid,
			      location_t locus)
{
  bool is_e0184 = false;
  is_e0184 |= (trait_hirid == get_copy_hirid () && impl_drop (ty).has_value ());
  is_e0184 |= (trait_hirid == get_drop_hirid () && impl_copy (ty).has_value ());
  if (is_e0184)
    rust_error_at (
      locus, ErrorCode::E0184,
      "the trait Copy cannot be implemented for type that has a destructor");
}

void
DropChecker::check_union (const TyTy::BaseType *ty)
{
  if (ty->get_kind () != TyTy::ADT)
    return;
  auto adt_type = ty->as<const TyTy::ADTType> ();
  if (adt_type->is_union ())
    {
      for (auto variant : adt_type->get_variants ())
	for (auto field : variant->get_fields ())
	  if (needs_drop (field->get_field_type ()))
	    {
	      rust_error_at (field->get_locus (), ErrorCode::E0740,
			     "field must implement Copy or be wrapped in "
			     "ManuallyDrop to be used in a union");
	    }
    }
}

void
DropChecker::check_drop_call (HirId trait_hirid, location_t locus)
{
  if (trait_hirid == get_drop_hirid ())
    {
      rust_error_at (locus, ErrorCode::E0040,
		     "explicit use of destructor method");
    }
}

} // namespace DropCheck
} // namespace Rust
