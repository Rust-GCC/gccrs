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

#ifndef RUST_DROP_CHECK_H
#define RUST_DROP_CHECK_H

#include "optional.h"
#include "rust-mapping-common.h"
#include "rust-tyty.h"
#include "rust-hir-map.h"

namespace Rust {

class TypeCheckContext;

namespace DropCheck {

class DropChecker
{
public:
  static tl::optional<HirId> get_trait_hirid (LangItem::Kind lang_kind);
  static tl::optional<HirId> get_drop_hirid ();
  static tl::optional<HirId> get_copy_hirid ();

  static tl::optional<HirId> impl_trait (const TyTy::BaseType *ty,
					 HirId TraitHirId);
  static tl::optional<HirId> impl_drop (const TyTy::BaseType *ty);
  static tl::optional<HirId> impl_copy (const TyTy::BaseType *ty);

  static bool needs_drop (const TyTy::BaseType *tyty);
  static bool is_manually_drop (const TyTy::BaseType *ty);

  static void check_copy_drop (const TyTy::BaseType *self, HirId trait_hirid,
			       location_t locus);
  static void check_union (const TyTy::BaseType *ty);
  static void check_drop_call (HirId trait_hirid, location_t locus);
};

} // namespace DropCheck
} // namespace Rust

#endif // RUST_DROP_CHECK_H
