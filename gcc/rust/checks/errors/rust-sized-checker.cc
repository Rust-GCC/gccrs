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

namespace Rust {
namespace HIR {

// utility functions

// set in SizedChecker::go
TraitReference *
SizedChecker::sized_trait_cache;

bool
SizedChecker::is_sized (TyTy::BaseType *type)
{
  return TypeBoundsProbe::is_bound_satisfied_for_type (type, sized_trait_cache);
}

void
SizedChecker::expect_sized (TyTy::BaseType *type, location_t locus)
{
  if (!TypeBoundsProbe::is_bound_satisfied_for_type (type, sized_trait_cache))
  {
    rust_error_at (locus, "type %s does not have a set size", type->as_string ());
  }
}

void
SizedChecker::expect_sized (std::vector<FunctionParam> &params)
{
  for (auto &param : params)
    expect_sized (param->get_type ());
}

void
SizedChecker::expect_sized (Type &type)
{
  auto ctx = TypeCheckContext::get ();
  TyTy::BaseType *resolved = nullptr;
  if (context->lookup_type (type.get_mappings ().get_hirid (), &resolved))
    expect_sized (resolved, type.get_locus ());
}

void SizedChecker::expect_sized (SelfParam &param)
{
  if (param.get_self_kind != SelfParam::IMM)
    return;

}

// items containing other items

SizedChecker::go (Crate &crate)
{
  if (!sized_trait_cache)
  {
    DefId item_defid = UNKNOWN_DEFID;
    if (!mappings->lookup_lang_item (Analysis::RustLangItem::ItemType::SIZED, &item_defid)
      return;
    Item *item = mappings->lookup_defid (item_defid);
    rust_assert (item);
    rust_assert (item->get_item_kind () == Item::ItemKind::Trait);
    sized_trait_cache = TraitResolver::Resolve (static_cast<Trait *> (item));
  }

  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}

SizedChecker::visit (Module &module)
{
  for (auto &item : module.get_items ())
    item->accept_vis (*this);
}

SizedChecker::visit (Trait &trait)
{
  for (auto &trait_item : trait.get_trait_items ())
    trait_item->accept_vis (*this);
}

SizedChecker::visit (ImplBlock &impl)
{
  for (auto &impl_item : impl.get_impl_items ())
    impl_item->accept_vis (*this);
}

SizedChecker::visit (ExternBlock &block)
{
  for (auto &extern_item : block.get_extern_items ())
    extern_item->accept_vis (*this);
}

// function-like items
// TODO: check return/self parameters

SizedChecker::visit (Function &function)
{
  TyTy::BaseType *fntype_base;
  rust_assert (ctx->get_tyctx ()->lookup_type (function.get_mappings ().get_hirid (), &fntype_base));
  rust_assert (fntype_base->get_kind () == TyTy::TypeKind::FNDEF);
  TyTy::FnType *fntype = static_cast<TyTy::FnType *> (fntype_base);

  
  for (auto &param : function.get_function_params ())
    context
}

SizedChecker::visit (TraitItemFunc &function)
{
}

SizedChecker::visit (ExternalFunctionItem &function)
{
}

} // namespace HIR
} // namespace Rust
