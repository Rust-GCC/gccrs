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

#ifndef RUST_TYTY_UTIL_H
#define RUST_TYTY_UTIL_H

#include "rust-hir-map.h"

namespace Rust {
namespace TyTy {

class BaseType;
class ConstType;

/* Identity for a context-owned inference variable.  This is deliberately
   separate from HIR IDs: one variable may be observed by several HIR nodes,
   and compiler-generated variables need not have a source node.  */
class TypeVarId
{
public:
  using Value = uint64_t;

  constexpr TypeVarId () : value (0) {}
  explicit constexpr TypeVarId (Value value) : value (value) {}

  constexpr Value get_value () const { return value; }
  constexpr bool is_valid () const { return value != 0; }

  friend constexpr bool operator== (TypeVarId lhs, TypeVarId rhs)
  { return lhs.value == rhs.value; }
  friend constexpr bool operator< (TypeVarId lhs, TypeVarId rhs)
  { return lhs.value < rhs.value; }

private:
  Value value;
};

// this is a placeholder for types that can change like inference variables
class TyVar
{
public:
  TyVar () : ref (UNKNOWN_HIRID), id () {}
  explicit TyVar (HirId ref);
  explicit TyVar (TypeVarId id);

  HirId get_ref () const { return ref; }
  TypeVarId get_id () const { return id; }
  bool has_id () const { return id.is_valid (); }

  BaseType *get_tyty () const;

  TyVar clone () const;

  TyVar monomorphized_clone () const;

  static TyVar get_implicit_infer_var (location_t locus);

  static TyVar get_implicit_const_infer_var (location_t locus,
					     TyVar *implicit_type = nullptr);

  static TyVar subst_covariant_var (TyTy::BaseType *orig,
				    TyTy::BaseType *subst);

private:
  HirId ref;
  TypeVarId id;
};

/* A type occurrence can either refer to a concrete TyTy node or to shared
   inference state. */
class TypeRef
{
public:
  explicit TypeRef (BaseType *type) : type (type), variable () {}
  explicit TypeRef (const TyVar &var) : type (nullptr), variable (var) {}

  BaseType *get_tyty () const;
  bool is_variable () const { return variable.has_id (); }
  const TyVar &get_variable () const { return variable; }

private:
  BaseType *type;
  TyVar variable;
};

class TyWithLocation
{
public:
  explicit TyWithLocation (BaseType *ty, location_t locus);
  explicit TyWithLocation (BaseType *ty);
  explicit TyWithLocation (const TyVar &var, location_t locus);
  explicit TyWithLocation (const TyVar &var);

  BaseType *get_ty () const { return ty; }
  location_t get_locus () const { return locus; }

private:
  BaseType *ty;
  location_t locus;
};

} // namespace TyTy
} // namespace Rust

#endif // RUST_TYTY_UTIL_H
