// Copyright (C) 2021-2025 Free Software Foundation, Inc.

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

#ifndef RUST_HIR_TYPE_BOUNDS_H
#define RUST_HIR_TYPE_BOUNDS_H

#include "rust-hir-trait-reference.h"
#include "rust-hir-type-check-base.h"
#include "rust-hir-full.h"
#include "rust-tyty.h"

namespace Rust {
namespace Resolver {

class TypeBoundsProbe : public TypeCheckBase
{
public:
  static std::vector<std::pair<TraitReference *, HIR::ImplBlock *>>
  Probe (const TyTy::BaseType *receiver);

  static bool is_bound_satisfied_for_type (TyTy::BaseType *receiver,
					   TraitReference *ref);

private:
  void scan ();
  void assemble_marker_builtins ();
  void add_trait_bound (HIR::Trait *trait);
  void assemble_builtin_candidate (LangItem::Kind item);
  bool should_impl_auto_trait (HIR::Trait *trait);
  bool is_auto_trait_bound_satisfied (const TyTy::BaseType *raw,
				      HIR::Trait *trait);

private:
  TypeBoundsProbe (const TyTy::BaseType *receiver);

  const TyTy::BaseType *receiver;
  std::vector<std::pair<TraitReference *, HIR::ImplBlock *>> trait_references;

  bool bound_cache_lookup (TyTy::BaseType *tyty, TraitReference *trait);
  void bound_cache_insert (TyTy::BaseType *tyty, TraitReference *trait,
			   bool satisfied);

  bool impl_cache_lookup (TyTy::BaseType *tyty);
  void impl_cache_insert (TyTy::BaseType *tyty, bool satisfied);

  // memoization cache (type)->(trait, impl)
  static std::map<TyTy::BaseType *,
		  std::vector<std::pair<TraitReference *, HIR::ImplBlock *>>>
    trait_reference_cache;
  // memoization cache (type, trait)->(bool)
  static std::map<std::pair<TyTy::BaseType *, TraitReference *>, bool>
    type_bound_cache;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_HIR_TYPE_BOUNDS_H
