// Copyright (C) 2020-2026 Free Software Foundation, Inc.

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

#ifndef RUST_COMPILE_DATUM_H
#define RUST_COMPILE_DATUM_H

#include "rust-compile-drop.h"
#include "rust-location.h"
#include "rust-tyty.h"

namespace Rust {
namespace Compile {

class Context;
enum class DatumKind
{
  Lvalue,
  Rvalue,
  Invalid,
};

struct Datum
{
  tree val = NULL_TREE;
  TyTy::BaseType *ty = nullptr;
  DatumKind kind = DatumKind::Invalid;
  DropPlace *drop_place = nullptr;
  location_t locus = UNDEF_LOCATION;

  static Datum new_datum (tree val, TyTy::BaseType *ty, DatumKind kind,
			  DropPlace *drop_place, location_t locus)
  {
    Datum d;
    d.val = val;
    d.ty = ty;
    d.kind = kind;
    d.drop_place = drop_place;
    d.locus = locus;
    return d;
  }
  static Datum new_lvalue (tree val, TyTy::BaseType *ty, DropPlace *drop_place,
			   location_t locus)
  {
    return new_datum (val, ty, DatumKind::Lvalue, drop_place, locus);
  }
  static Datum new_rvalue (tree val, TyTy::BaseType *ty, location_t locus)
  {
    return new_datum (val, ty, DatumKind::Rvalue, nullptr, locus);
  }

  bool is_lvalue () const { return kind == DatumKind::Lvalue; }
  bool is_rvalue () const { return kind == DatumKind::Rvalue; }
  bool is_valid () const
  {
    return check_node (val) && ty != nullptr && kind != DatumKind::Invalid;
  }
  void set_not_valid ()
  {
    val = error_mark_node;
    ty = nullptr;
    kind = DatumKind::Invalid;
  }

  bool needs_drop (DropContext *drop_ctx) const
  {
    if (!is_valid ())
      return false;
    return drop_ctx->needs_drop (ty);
  }

  Datum get_field (Context *ctx, size_t field_index, location_t locus);
  tree raw_tree () const { return val; }

  void add_clean_if_rvalue (Context *ctx, HirId expr_id);
  Datum to_lvalue (Context *ctx, HirId expr_id);
  Datum to_rvalue (Context *ctx);
  void store_to (Context *ctx, tree dest_ptr);
  void post_store (Context *ctx);
};
} // namespace Compile
} // namespace Rust
#endif // RUST_COMPILE_DATUM_H
