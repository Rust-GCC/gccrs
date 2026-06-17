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

#ifndef RUST_HIR_TYPE_CHECK_INTRINSIC_H
#define RUST_HIR_TYPE_CHECK_INTRINSIC_H

#include "rust-tyty.h"

namespace Rust {
namespace Resolver {

enum class IntrinsicRuleType
{
  Bool,
  F32,
  F64,
  I32,
  Isize,
  U8,
  U64,
  Usize,
  Unit,
  Never,

  FirstGeneric,	 // <T>
  SecondGeneric, // <U>

  ConstPtrFirstGeneric, // *const T
  MutPtrFirstGeneric,	// *mut T
  RefFirstGeneric,	// &T

  RefAdt,   // for caller_location() -> &'static crate::panic::Location<'static>
  MutPtrU8, // *mut u8
  RefStaticStr, // &'static str
  AssocTypePlaceholder,
  // for discriminant_value<T>(v: &T) -> <T as DiscriminantKind>::Discriminant

  TupleFirstGenericAndBool, // (T, bool)

  Fn_MutPtrU8,	       // fn(*mut u8)
  Fn_MutPtrU8MutPtrU8, // fn(*mut u8, *mut u8)
};

struct IntrinsicRules
{
  size_t generic_count;
  std::vector<IntrinsicRuleType> param_types;
  IntrinsicRuleType return_type;
};

enum class IntrinsicCheckResult
{
  Valid,
  Invalid,
};

class IntrinsicChecker
{
public:
  static IntrinsicCheckResult check (const TyTy::FnType *fntype);

private:
  static const std::unordered_map<std::string, IntrinsicRules> intrinsic_rules;
  static bool check_type (const TyTy::BaseType *actual,
			  IntrinsicRuleType expected,
			  const TyTy::FnType *fntype);
  static std::string expected_intrinsic_as_string (const TyTy::FnType *fntype);
  static std::string found_intrinsic_as_string (const TyTy::FnType *fntype);
};

} // namespace Resolver
} // namespace Rust

#endif
