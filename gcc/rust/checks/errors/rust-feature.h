// Copyright (C) 2020-2025 Free Software Foundation, Inc.

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

#ifndef RUST_FEATURE_H
#define RUST_FEATURE_H

#include "rust-edition.h"
#include "optional.h"

namespace Rust {

class Feature
{
public:
  enum class State
  {
    ACCEPTED,
    ACTIVE,
    REMOVED,
    STABILIZED,
  };

  enum class Name
  {
    ASSOCIATED_TYPE_BOUNDS,
    INTRINSICS,
    NEGATIVE_IMPLS,
    RUSTC_ATTRS,
    DECL_MACRO,
    AUTO_TRAITS,
    EXTERN_TYPES,
    LANG_ITEMS,
    NO_CORE,
    BOX_SYNTAX,
    DROPCK_EYEPATCH,
    RAW_REF_OP,
    EXCLUSIVE_RANGE_PATTERN,
    PRELUDE_IMPORT,
    MIN_SPECIALIZATION,

    AARCH64_TARGET_FEATURE,
    ABI_UNADJUSTED,
    ADX_TARGET_FEATURE,
    ALLOW_INTERNAL_UNSTABLE,
    ARBITRARY_SELF_TYPES,
    ARM_TARGET_FEATURE,
    ASM,
    AVX512_TARGET_FEATURE,
    CFG_TARGET_HAS_ATOMIC,
    CMPXCHG16B_TARGET_FEATURE,
    CONST_ALIGN_OF_VAL,
    CONST_ALLOC_LAYOUT,
    CONST_CALLER_LOCATION,
    CONST_CELL_INTO_INNER,
    CONST_CHECKED_INT_METHODS,
    CONSTCTLZ,
    CONST_DISCRIMINANT,
    CONST_EUCLIDEAN_INT_METHODS,
    CONST_FLOAT_BITS_CONV,
    CONST_FLOAT_CLASSIFY,
    CONST_FN,
    CONST_FN_FLOATING_POINT_ARITHMETIC,
    CONST_FN_FN_PTR_BASICS,
    CONST_FN_TRANSMUTE,
    CONST_FN_UNION,
    CONST_GENERICS,
    CONST_INT_POW,
    CONST_INT_UNCHECKED_ARITH,
    CONST_LIKELY,
    CONST_MUT_REFS,
    CONST_OPTION,
    CONST_OVERFLOWING_INT_METHODS,
    CONST_PANIC,
    CONST_PIN,
    CONST_PRECISE_LIVE_DROPS,
    CONST_PTR_OFFSET,
    CONST_PTR_OFFSET_FROM,
    CONST_RAW_PTR_COMPARISON,
    CONST_SIZE_OF_VAL,
    CONST_SLICE_FROM_RAW_PARTS,
    CONST_SLICE_PTR_LEN,
    CONST_TYPE_ID,
    CONST_TYPE_NAME,
    CONST_UNREACHABLE_UNCHECKED,
    CUSTOM_INNER_ATTRIBUTES,
    DOC_CFG,
    DOC_SPOTLIGHT,
    DURATION_CONSTS_2,
    DURATION_SATURATING_OPS,
    EXHAUSTIVE_PATTERNS,
    EXTERNAL_DOC,
    F16C_TARGET_FEATURE,
    FUNDAMENTAL,
    HEXAGON_TARGET_FEATURE,
    INT_ERROR_MATCHING,
    LINK_LLVM_INTRINSICS,
    LLVM_ASM,
    MIPS_TARGET_FEATURE,
    NEVER_TYPE,
    NLL,
    NO_NICHE,
    OR_PATTERNS,
    POWERPC_TARGET_FEATURE,
    REPR_SIMD,
    PLATFORM_INTRINSICS,
    RTM_TARGET_FEATURE,
    SIMD_FFI,
    SLICE_PTR_GET,
    SSE4A_TARGET_FEATURE,
    STAGED_API,
    STD_INTERNALS,
    STMT_EXPR_ATTRIBUTES,
    STR_SPLIT_AS_STR,
    STR_SPLIT_INCLUSIVE_AS_STR,
    TBM_TARGET_FEATURE,
    TRANSPARENT_UNIONS,
    TRY_BLOCKS,
    UNBOXED_CLOSURES,
    UNSAFE_BLOCK_IN_UNSAFE_FN,
    UNWIND_ATTRIBUTES,
    VARIANT_COUNT,
    WASM_TARGET_FEATURE,

  };

  const std::string &as_string () { return m_name_str; }
  Name name () { return m_name; }
  const std::string &description () { return m_description; }
  State state () { return m_state; }
  tl::optional<unsigned> issue () { return m_issue; }

  static tl::optional<Name> as_name (const std::string &name);
  static Feature create (Name name);

private:
  Feature (Name name, State state, const char *name_str,
	   const char *rustc_since,
	   tl::optional<unsigned> issue_number = tl::nullopt,
	   const tl::optional<Edition> &edition = tl::nullopt,
	   const char *description = "")
    : m_state (state), m_name (name), m_name_str (name_str),
      m_rustc_since (rustc_since), m_issue (issue_number), edition (edition),
      m_description (description)
  {}

  State m_state;
  Name m_name;
  std::string m_name_str;
  std::string m_rustc_since;
  tl::optional<unsigned> m_issue;
  tl::optional<Edition> edition;
  std::string m_description; // TODO: Switch to optional?

  static const std::map<std::string, Name> name_hash_map;
};

} // namespace Rust
#endif
