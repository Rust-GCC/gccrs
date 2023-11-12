// Copyright (C) 2023 Free Software Foundation, Inc.

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

#include "rust-sized-checker.h"
#include "rust-hir.h"
#include "rust-hir-type-bounds.h"
#include "rust-hir-trait-resolve.h"

namespace Rust {
namespace HIR {

// utility functions

// set in SizedChecker::go
Resolver::TraitReference *
SizedChecker::sized_trait_cache;

bool
SizedChecker::is_sized (TyTy::BaseType *type)
{
  return Resolver::TypeBoundsProbe::is_bound_satisfied_for_type (type, sized_trait_cache);
}

// items containing other items

void
SizedChecker::go (Crate &crate)
{
  if (!sized_trait_cache)
  {
    DefId item_defid = UNKNOWN_DEFID;
    if (!mappings.lookup_lang_item (Analysis::RustLangItem::ItemType::SIZED, &item_defid))
      return;
    Item *item = mappings.lookup_defid (item_defid);
    rust_assert (item);
    rust_assert (item->get_item_kind () == Item::ItemKind::Trait);
    sized_trait_cache = Resolver::TraitResolver::Resolve (static_cast<Trait &> (*item));
  }

  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}

void
SizedChecker::visit (Module &module)
{
  for (auto &item : module.get_items ())
    item->accept_vis (*this);
}

void
SizedChecker::visit (Trait &trait)
{
  for (auto &trait_item : trait.get_trait_items ())
    trait_item->accept_vis (*this);
}

void
SizedChecker::visit (ImplBlock &impl)
{
  for (auto &impl_item : impl.get_impl_items ())
    impl_item->accept_vis (*this);
}

void
SizedChecker::visit (ExternBlock &block)
{
  for (auto &extern_item : block.get_extern_items ())
    extern_item->accept_vis (*this);
}

// function-like items

void
SizedChecker::visit (Function &function)
{
  TyTy::BaseType *fntype_base;
  rust_assert (tyctx->lookup_type (function.get_mappings ().get_hirid (), &fntype_base));
  rust_assert (fntype_base->get_kind () == TyTy::TypeKind::FNDEF);
  TyTy::FnType *fntype = static_cast<TyTy::FnType *> (fntype_base);

  for (auto &param : fntype->get_params ())
    if (!is_sized (param.second))
      rust_error_at (param.first->get_locus (), "function parameter type %<%s%> is dynamically sized", param.second->as_string ().c_str ());

  TyTy::BaseType *ret_type = fntype->get_return_type ();
  if (!is_sized (ret_type))
    // TODO: give this a better location_t
    rust_error_at (function.get_locus (), "function return type %<%s%> is dynamically sized", ret_type->as_string ().c_str ());
}

void
SizedChecker::visit (TraitItemFunc &function)
{
  // ignore functions with no body
  if (!function.has_definition ()) return;

  TyTy::BaseType *fntype_base;
  rust_assert (tyctx->lookup_type (function.get_mappings ().get_hirid (), &fntype_base));
  rust_assert (fntype_base->get_kind () == TyTy::TypeKind::FNDEF);
  TyTy::FnType *fntype = static_cast<TyTy::FnType *> (fntype_base);

  for (auto &param : fntype->get_params ())
    if (!is_sized (param.second))
      rust_error_at (param.first->get_locus (), "function parameter type %<%s%> is dynamically sized", param.second->as_string ().c_str ());

  TyTy::BaseType *ret_type = fntype->get_return_type ();
  if (!is_sized (ret_type))
    // TODO: give this a better location_t
    rust_error_at (function.get_locus (), "function return type %<%s%> is dynamically sized", ret_type->as_string ().c_str ());
}

// ignore extern functions

// TODO: visit items inside block expressions

} // namespace HIR
} // namespace Rust
