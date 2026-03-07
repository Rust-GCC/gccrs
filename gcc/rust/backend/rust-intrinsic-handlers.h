// Copyright (C) 2026 Free Software Foundation, Inc.
//
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

#ifndef RUST_INTRINSIC_HANDLERS_H
#define RUST_INTRINSIC_HANDLERS_H

#include "rust-compile-context.h"

namespace Rust {
namespace Compile {

enum class Prefetch
{
  Read,
  Write
};

namespace handlers {

namespace inner {
tree wrapping_op (Context *ctx, TyTy::FnType *fntype, tree_code op);

tree atomic_store (Context *ctx, TyTy::FnType *fntype, int ordering);
tree atomic_load (Context *ctx, TyTy::FnType *fntype, int ordering);
inline tree copy (Context *ctx, TyTy::FnType *fntype, bool overlaps);
inline tree expect (Context *ctx, TyTy::FnType *fntype, bool likely);
tree try_handler (Context *ctx, TyTy::FnType *fntype, bool is_new_api);

tree op_with_overflow (Context *ctx, TyTy::FnType *fntype, tree_code op);

inline tree unchecked_op (Context *ctx, TyTy::FnType *fntype, tree_code op);

} // namespace inner

using HandlerBuilder = std::function<tree (Context *, TyTy::FnType *)>;

const HandlerBuilder op_with_overflow (tree_code op);

tree rotate_left (Context *ctx, TyTy::FnType *fntype);
tree rotate_right (Context *ctx, TyTy::FnType *fntype);

const HandlerBuilder wrapping_op (tree_code op);
tree offset (Context *ctx, TyTy::FnType *fntype);
tree sizeof_handler (Context *ctx, TyTy::FnType *fntype);
tree transmute (Context *ctx, TyTy::FnType *fntype);
tree rotate (Context *ctx, TyTy::FnType *fntype, tree_code op);
tree uninit (Context *ctx, TyTy::FnType *fntype);
tree move_val_init (Context *ctx, TyTy::FnType *fntype);
tree assume (Context *ctx, TyTy::FnType *fntype);
tree discriminant_value (Context *ctx, TyTy::FnType *fntype);
tree variant_count (Context *ctx, TyTy::FnType *fntype);
tree bswap_handler (Context *ctx, TyTy::FnType *fntype);
tree ctlz_handler (Context *ctx, TyTy::FnType *fntype);
tree ctlz_nonzero_handler (Context *ctx, TyTy::FnType *fntype);

tree prefetch_data (Context *ctx, TyTy::FnType *fntype, Prefetch kind);

const std::function<tree (Context *, TyTy::FnType *)>
wrapping_op (tree_code op);

HandlerBuilder atomic_store (int ordering);

HandlerBuilder atomic_load (int ordering);

const HandlerBuilder unchecked_op (tree_code op);

const HandlerBuilder copy (bool overlaps);

const HandlerBuilder expect (bool likely);

const HandlerBuilder try_handler (bool is_new_api);

tree prefetch_read_data (Context *ctx, TyTy::FnType *fntype);
tree prefetch_write_data (Context *ctx, TyTy::FnType *fntype);
tree sorry (Context *ctx, TyTy::FnType *fntype);

} // namespace handlers

} // namespace Compile
} // namespace Rust

#endif /* ! RUST_INTRINSIC_HANDLERS_H */
