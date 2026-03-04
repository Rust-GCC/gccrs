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

#include "rust-compile-intrinsic.h"
#include "rust-compile-context.h"
#include "rust-builtins.h"
#include "rust-diagnostics.h"
#include "tree-core.h"
#include "rust-intrinsic-handlers.h"

namespace Rust {
namespace Compile {

static const std::map<std::string, handlers::HandlerBuilder> generic_intrinsics
  = {{"offset", handlers::offset},
     {"size_of", handlers::sizeof_handler},
     {"transmute", handlers::transmute},
     {"rotate_left", handlers::rotate_left},
     {"rotate_right", handlers::rotate_right},
     {"wrapping_add", handlers::wrapping_op (PLUS_EXPR)},
     {"wrapping_sub", handlers::wrapping_op (MINUS_EXPR)},
     {"wrapping_mul", handlers::wrapping_op (MULT_EXPR)},
     {"add_with_overflow", handlers::op_with_overflow (PLUS_EXPR)},
     {"sub_with_overflow", handlers::op_with_overflow (MINUS_EXPR)},
     {"mul_with_overflow", handlers::op_with_overflow (MULT_EXPR)},
     {"copy", handlers::copy (true)},
     {"copy_nonoverlapping", handlers::copy (false)},
     {"prefetch_read_data", handlers::prefetch_read_data},
     {"prefetch_write_data", handlers::prefetch_write_data},
     {"atomic_store_seqcst", handlers::atomic_store (__ATOMIC_SEQ_CST)},
     {"atomic_store_release", handlers::atomic_store (__ATOMIC_RELEASE)},
     {"atomic_store_relaxed", handlers::atomic_store (__ATOMIC_RELAXED)},
     {"atomic_store_unordered", handlers::atomic_store (__ATOMIC_RELAXED)},
     {"atomic_load_seqcst", handlers::atomic_load (__ATOMIC_SEQ_CST)},
     {"atomic_load_acquire", handlers::atomic_load (__ATOMIC_ACQUIRE)},
     {"atomic_load_relaxed", handlers::atomic_load (__ATOMIC_RELAXED)},
     {"atomic_load_unordered", handlers::atomic_load (__ATOMIC_RELAXED)},
     {"unchecked_add", handlers::unchecked_op (PLUS_EXPR)},
     {"unchecked_sub", handlers::unchecked_op (MINUS_EXPR)},
     {"unchecked_mul", handlers::unchecked_op (MULT_EXPR)},
     {"unchecked_div", handlers::unchecked_op (TRUNC_DIV_EXPR)},
     {"unchecked_rem", handlers::unchecked_op (TRUNC_MOD_EXPR)},
     {"unchecked_shl", handlers::unchecked_op (LSHIFT_EXPR)},
     {"unchecked_shr", handlers::unchecked_op (RSHIFT_EXPR)},
     {"uninit", handlers::uninit},
     {"move_val_init", handlers::move_val_init},
     {"likely", handlers::expect (true)},
     {"unlikely", handlers::expect (false)},
     {"assume", handlers::assume},
     {"try", handlers::try_handler (false)},
     {"catch_unwind", handlers::try_handler (true)},
     {"discriminant_value", handlers::discriminant_value},
     {"variant_count", handlers::variant_count}};

Intrinsics::Intrinsics (Context *ctx) : ctx (ctx) {}

/**
 * Returns a FUNC_DECL corresponding to the intrinsic function FNTYPE. If a
 * corresponding builtin exists, returns it. If not, search in the generic
 * intrinsics declared and delegate the return to the corresponding handler.
 *
 * @param fntype The Rust function type that should be implemented by the
 * compiler
 */
tree
Intrinsics::compile (TyTy::FnType *fntype)
{
  rust_assert (fntype->get_abi () == ABI::INTRINSIC);

  tree builtin = error_mark_node;
  BuiltinsContext &builtin_ctx = BuiltinsContext::get ();

  if (builtin_ctx.lookup_simple_builtin (fntype->get_identifier (), &builtin))
    return builtin;

  // is it an generic builtin?
  auto it = generic_intrinsics.find (fntype->get_identifier ());
  if (it != generic_intrinsics.end ())
    return it->second (ctx, fntype);

  location_t locus = ctx->get_mappings ().lookup_location (fntype->get_ref ());
  rust_error_at (locus, ErrorCode::E0093,
		 "unrecognized intrinsic function: %qs",
		 fntype->get_identifier ().c_str ());

  return error_mark_node;
}

} // namespace Compile
} // namespace Rust
