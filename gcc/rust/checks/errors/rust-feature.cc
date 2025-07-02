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

#include "rust-feature.h"

namespace Rust {

Feature
Feature::create (Feature::Name f)
{
  switch (f)
    {
    case Feature::Name::ASSOCIATED_TYPE_BOUNDS:
      return Feature (Feature::Name::ASSOCIATED_TYPE_BOUNDS,
		      Feature::State::ACCEPTED, "associated_type_bounds",
		      "1.34.0", 52662);
    case Feature::Name::INTRINSICS:
      return Feature (f, Feature::State::ACCEPTED, "intrinsics", "1.0.0");
    case Feature::Name::RUSTC_ATTRS:
      return Feature (f, Feature::State::ACCEPTED, "rustc_attrs", "1.0.0");
    case Feature::Name::DECL_MACRO:
      return Feature (f, Feature::State::ACCEPTED, "decl_macro", "1.0.0",
		      39412);
    case Feature::Name::EXTERN_TYPES:
      return Feature (f, Feature::State::ACTIVE, "extern_types", "1.23.0",
		      43467);
    case Feature::Name::NEGATIVE_IMPLS:
      return Feature (f, Feature::State::ACTIVE, "negative_impls", "1.0.0",
		      68318);
    case Feature::Name::BOX_SYNTAX:
      return Feature (f, Feature::State::ACTIVE, "box_syntax", "1.0.0", 49733);
    case Feature::Name::DROPCK_EYEPATCH:
      return Feature (f, Feature::State::ACTIVE, "dropck_eyepatch", "1.10.0",
		      34761);
    case Feature::Name::RAW_REF_OP:
      return Feature (f, Feature::State::ACTIVE, "raw_ref_op", "1.41.0", 64490);
    case Feature::Name::EXCLUSIVE_RANGE_PATTERN:
      return Feature (Feature::Name::EXCLUSIVE_RANGE_PATTERN,
		      Feature::State::ACTIVE, "exclusive_range_pattern",
		      "1.11.0", 37854);
    case Feature::Name::PRELUDE_IMPORT:
      return Feature (f, Feature::State::ACTIVE, "prelude_import", "1.0.0");
    case Feature::Name::MIN_SPECIALIZATION:
      return Feature (f, Feature::State::ACTIVE, "min_specialization",
		      "1.0.0" /* FIXME: What version here? */, 31844);
    case Feature::Name::AUTO_TRAITS:
      return Feature (f, Feature::State::ACTIVE, "optin_builtin_traits",
		      "1.0.0", 13231);
    default:
      rust_unreachable ();
    }
}

const std::map<std::string, Feature::Name> Feature::name_hash_map = {
  {"associated_type_bounds", Feature::Name::ASSOCIATED_TYPE_BOUNDS},
  {"intrinsics", Feature::Name::INTRINSICS},
  {"rustc_attrs", Feature::Name::RUSTC_ATTRS},
  {"decl_macro", Feature::Name::DECL_MACRO},
  {"negative_impls", Feature::Name::NEGATIVE_IMPLS},
  // TODO: Rename to "auto_traits" when supporting
  // later Rust versions
  {"optin_builtin_traits", Feature::Name::AUTO_TRAITS},
  {"extern_types", Feature::Name::EXTERN_TYPES},
  {"lang_items", Feature::Name::LANG_ITEMS},
  {"no_core", Feature::Name::NO_CORE},
  {"box_syntax", Feature::Name::BOX_SYNTAX},
  {"dropck_eyepatch", Feature::Name::DROPCK_EYEPATCH},
  {"raw_ref_op", Feature::Name::RAW_REF_OP},
  {"exclusive_range_pattern", Feature::Name::EXCLUSIVE_RANGE_PATTERN},
  {"prelude_import", Feature::Name::PRELUDE_IMPORT},
  {"min_specialization", Feature::Name::MIN_SPECIALIZATION},
  // unhandled features
  {"aarch64_target_feature", Feature::Name::AARCH64_TARGET_FEATURE},
  {"abi_unadjusted", Feature::Name::ABI_UNADJUSTED},
  {"adx_target_feature", Feature::Name::ADX_TARGET_FEATURE},
  {"allow_internal_unstable", Feature::Name::ALLOW_INTERNAL_UNSTABLE},
  {"arbitrary_self_types", Feature::Name::ARBITRARY_SELF_TYPES},
  {"arm_target_feature", Feature::Name::ARM_TARGET_FEATURE},
  {"asm", Feature::Name::ASM},
  {"avx512_target_feature", Feature::Name::AVX512_TARGET_FEATURE},
  {"cfg_target_has_atomic", Feature::Name::CFG_TARGET_HAS_ATOMIC},
  {"cmpxchg16b_target_feature", Feature::Name::CMPXCHG16B_TARGET_FEATURE},
  {"const_align_of_val", Feature::Name::CONST_ALIGN_OF_VAL},
  {"const_alloc_layout", Feature::Name::CONST_ALLOC_LAYOUT},
  {"const_caller_location", Feature::Name::CONST_CALLER_LOCATION},
  {"const_cell_into_inner", Feature::Name::CONST_CELL_INTO_INNER},
  {"const_checked_int_methods", Feature::Name::CONST_CHECKED_INT_METHODS},
  {"const_discriminant", Feature::Name::CONST_DISCRIMINANT},
  {"const_euclidean_int_methods", Feature::Name::CONST_EUCLIDEAN_INT_METHODS},
  {"const_float_bits_conv", Feature::Name::CONST_FLOAT_BITS_CONV},
  {"const_float_classify", Feature::Name::CONST_FLOAT_CLASSIFY},
  {"const_fn", Feature::Name::CONST_FN},
  {"const_fn_floating_point_arithmetic", Feature::Name::CONST_FN_FLOATING_POINT_ARITHMETIC},
  {"const_fn_fn_ptr_basics", Feature::Name::CONST_FN_FN_PTR_BASICS},
  {"const_fn_transmute", Feature::Name::CONST_FN_TRANSMUTE},
  {"const_fn_union", Feature::Name::CONST_FN_UNION},
  {"const_generics", Feature::Name::CONST_GENERICS},
  {"const_int_pow", Feature::Name::CONST_INT_POW},
  {"const_int_unchecked_arith", Feature::Name::CONST_INT_UNCHECKED_ARITH},
  {"const_likely", Feature::Name::CONST_LIKELY},
  {"const_mut_refs", Feature::Name::CONST_MUT_REFS},
  {"const_option", Feature::Name::CONST_OPTION},
  {"const_overflowing_int_methods", Feature::Name::CONST_OVERFLOWING_INT_METHODS},
  {"const_panic", Feature::Name::CONST_PANIC},
  {"const_pin", Feature::Name::CONST_PIN},
  {"const_precise_live_drops", Feature::Name::CONST_PRECISE_LIVE_DROPS},
  {"const_ptr_offset", Feature::Name::CONST_PTR_OFFSET},
  {"const_ptr_offset_from", Feature::Name::CONST_PTR_OFFSET_FROM},
  {"const_raw_ptr_comparison", Feature::Name::CONST_RAW_PTR_COMPARISON},
  {"const_size_of_val", Feature::Name::CONST_SIZE_OF_VAL},
  {"const_slice_from_raw_parts", Feature::Name::CONST_SLICE_FROM_RAW_PARTS},
  {"const_slice_ptr_len", Feature::Name::CONST_SLICE_PTR_LEN},
  {"const_type_id", Feature::Name::CONST_TYPE_ID},
  {"const_type_name", Feature::Name::CONST_TYPE_NAME},
  {"const_unreachable_unchecked", Feature::Name::CONST_UNREACHABLE_UNCHECKED},
  {"constctlz", Feature::Name::CONSTCTLZ},
  {"custom_inner_attributes", Feature::Name::CUSTOM_INNER_ATTRIBUTES},
  {"doc_cfg", Feature::Name::DOC_CFG},
  {"doc_spotlight", Feature::Name::DOC_SPOTLIGHT},
  {"duration_consts_2", Feature::Name::DURATION_CONSTS_2},
  {"duration_saturating_ops", Feature::Name::DURATION_SATURATING_OPS},
  {"exhaustive_patterns", Feature::Name::EXHAUSTIVE_PATTERNS},
  {"external_doc", Feature::Name::EXTERNAL_DOC},
  {"f16c_target_feature", Feature::Name::F16C_TARGET_FEATURE},
  {"fundamental", Feature::Name::FUNDAMENTAL},
  {"hexagon_target_feature", Feature::Name::HEXAGON_TARGET_FEATURE},
  {"int_error_matching", Feature::Name::INT_ERROR_MATCHING},
  {"link_llvm_intrinsics", Feature::Name::LINK_LLVM_INTRINSICS},
  {"llvm_asm", Feature::Name::LLVM_ASM},
  {"mips_target_feature", Feature::Name::MIPS_TARGET_FEATURE},
  {"never_type", Feature::Name::NEVER_TYPE},
  {"nll", Feature::Name::NLL},
  {"no_niche", Feature::Name::NO_NICHE},
  {"or_patterns", Feature::Name::OR_PATTERNS},
  {"platform_intrinsics", Feature::Name::PLATFORM_INTRINSICS},
  {"powerpc_target_feature", Feature::Name::POWERPC_TARGET_FEATURE},
  {"repr_simd", Feature::Name::REPR_SIMD},
  {"rtm_target_feature", Feature::Name::RTM_TARGET_FEATURE},
  {"simd_ffi", Feature::Name::SIMD_FFI},
  {"slice_ptr_get", Feature::Name::SLICE_PTR_GET},
  {"sse4a_target_feature", Feature::Name::SSE4A_TARGET_FEATURE},
  {"staged_api", Feature::Name::STAGED_API},
  {"std_internals", Feature::Name::STD_INTERNALS},
  {"stmt_expr_attributes", Feature::Name::STMT_EXPR_ATTRIBUTES},
  {"str_split_as_str", Feature::Name::STR_SPLIT_AS_STR},
  {"str_split_inclusive_as_str", Feature::Name::STR_SPLIT_INCLUSIVE_AS_STR},
  {"tbm_target_feature", Feature::Name::TBM_TARGET_FEATURE},
  {"transparent_unions", Feature::Name::TRANSPARENT_UNIONS},
  {"try_blocks", Feature::Name::TRY_BLOCKS},
  {"unboxed_closures", Feature::Name::UNBOXED_CLOSURES},
  {"unsafe_block_in_unsafe_fn", Feature::Name::UNSAFE_BLOCK_IN_UNSAFE_FN},
  {"unwind_attributes", Feature::Name::UNWIND_ATTRIBUTES},
  {"variant_count", Feature::Name::VARIANT_COUNT},
  {"wasm_target_feature", Feature::Name::WASM_TARGET_FEATURE},
};

tl::optional<Feature::Name>
Feature::as_name (const std::string &name)
{
  if (Feature::name_hash_map.count (name))
    return Feature::name_hash_map.at (name);

  return tl::nullopt;
}

} // namespace Rust
