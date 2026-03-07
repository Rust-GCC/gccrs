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

namespace Rust {
namespace Values {
class Intrinsics
{
public:
  static constexpr auto &ABORT = "abort";
  static constexpr auto &OFFSET = "offset";
  static constexpr auto &SIZE_OF = "size_of";
  static constexpr auto &TRANSMUTE = "transmute";

  static constexpr auto &ADD_WITH_OVERFLOW = "add_with_overflow";
  static constexpr auto &SUB_WITH_OVERFLOW = "sub_with_overflow";
  static constexpr auto &MUL_WITH_OVERFLOW = "mul_with_overflow";

  static constexpr auto &COPY = "copy";
  static constexpr auto &COPY_NONOVERLAPPING = "copy_nonoverlapping";

  static constexpr auto &PREFETCH_READ_DATA = "prefetch_read_data";
  static constexpr auto &PREFETCH_WRITE_DATA = "prefetch_write_data";

  static constexpr auto &ATOMIC_STORE_SEQCST = "atomic_store_seqcst";
  static constexpr auto &ATOMIC_STORE_RELEASE = "atomic_store_release";
  static constexpr auto &ATOMIC_STORE_RELAXED = "atomic_store_relaxed";
  static constexpr auto &ATOMIC_STORE_UNORDERED = "atomic_store_unordered";

  static constexpr auto &ATOMIC_LOAD_SEQCST = "atomic_load_seqcst";
  static constexpr auto &ATOMIC_LOAD_ACQUIRE = "atomic_load_acquire";
  static constexpr auto &ATOMIC_LOAD_RELAXED = "atomic_load_relaxed";
  static constexpr auto &ATOMIC_LOAD_UNORDERED = "atomic_load_unordered";

  static constexpr auto &UNCHECKED_ADD = "unchecked_add";
  static constexpr auto &UNCHECKED_SUB = "unchecked_sub";
  static constexpr auto &UNCHECKED_MUL = "unchecked_mul";
  static constexpr auto &UNCHECKED_DIV = "unchecked_div";
  static constexpr auto &UNCHECKED_REM = "unchecked_rem";
  static constexpr auto &UNCHECKED_SHL = "unchecked_shl";
  static constexpr auto &UNCHECKED_SHR = "unchecked_shr";

  static constexpr auto &UNINIT = "uninit";
  static constexpr auto &MOVE_VAL_INIT = "move_val_init";

  static constexpr auto &ROTATE_LEFT = "rotate_left";
  static constexpr auto &ROTATE_RIGHT = "rotate_right";

  static constexpr auto &WRAPPING_ADD = "wrapping_add";
  static constexpr auto &WRAPPING_SUB = "wrapping_sub";
  static constexpr auto &WRAPPING_MUL = "wrapping_mul";

  static constexpr auto &SATURATING_ADD = "saturating_add";
  static constexpr auto &SATURATING_SUB = "saturating_sub";

  static constexpr auto &LIKELY = "likely";
  static constexpr auto &UNLIKELY = "unlikely";

  static constexpr auto &DISCRIMINANT_VALUE = "discriminant_value";
  static constexpr auto &VARIANT_COUNT = "variant_count";
  static constexpr auto &CATCH_UNWIND = "catch_unwind";
  static constexpr auto &TRY = "try";
  static constexpr auto &ASSUME = "assume";

  static constexpr auto &MIN_ALIGN_OF = "min_align_of";
  static constexpr auto &NEEDS_DROP = "needs_drop";
  static constexpr auto &CALLER_LOCATION = "caller_location";
  static constexpr auto &CTPOP = "ctpop";
  static constexpr auto &CTLZ = "ctlz";
  static constexpr auto &CTLZ_NONZERO = "ctlz_nonzero";
  static constexpr auto &CTTZ = "cttz";
  static constexpr auto &BSWAP = "bswap";
  static constexpr auto &BITREVERSE = "bitreverse";
  static constexpr auto &TYPE_ID = "type_id";
  static constexpr auto &PTR_GUARANTEED_EQ = "ptr_guaranteed_eq";
  static constexpr auto &PTR_GUARANTEED_NE = "ptr_guaranteed_ne";
  static constexpr auto &MINNUMF32 = "minnumf32";
  static constexpr auto &MINNUMF64 = "minnumf64";
  static constexpr auto &MAXNUMF32 = "maxnumf32";
  static constexpr auto &MAXNUMF64 = "maxnumf64";
  static constexpr auto &RUSTC_PEEK = "rustc_peek";
  static constexpr auto &TYPE_NAME = "type_name";
  static constexpr auto &FORGET = "forget";
  static constexpr auto &BLACK_BOX = "black_box";
};
} // namespace Values
} // namespace Rust
