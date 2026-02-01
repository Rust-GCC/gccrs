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

#ifndef RUST_HIR_IMPL_TRAIT_CONTEXT_H
#define RUST_HIR_IMPL_TRAIT_CONTEXT_H

#include "rust-hir-map.h"

namespace Rust {

namespace TyTy {
class BaseType;
}

namespace Resolver {

class TraitReference;

struct AssocTypeEntry
{
  DefId trait_item_defid;
  DefId impl_item_defid;
  TyTy::BaseType *value;
};

struct ImplTraitContextFrame
{
  const TraitReference *trait;
  TyTy::BaseType *self;
  std::map<DefId, AssocTypeEntry> assoc_types_by_trait_item;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_HIR_IMPL_TRAIT_CONTEXT_H
