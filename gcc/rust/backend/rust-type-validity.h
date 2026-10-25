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

#ifndef RUST_TYPE_VALIDITY_H
#define RUST_TYPE_VALIDITY_H

#include "rust-tyty.h"

namespace Rust {

class TypeValidity
{
public:
  enum class RawInitKind
  {
    ZeroInit,
    Uninit,
  };

  static bool might_permit_raw_init (TyTy::BaseType *type, tree type_tree,
				     RawInitKind init_kind);
  static bool is_uninhabited (TyTy::BaseType *t);

private:
  static inline bool check_and_step_field (TyTy::BaseType *field_type,
					   tree &tree_iter,
					   RawInitKind init_kind);
};

} // namespace Rust

#endif // RUST_TYPE_VALIDITY_H
