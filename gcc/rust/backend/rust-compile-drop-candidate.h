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

#ifndef RUST_COMPILE_DROP_CANDIDATE_H
#define RUST_COMPILE_DROP_CANDIDATE_H

#include "rust-system.h"
#include "rust-hir-map.h"

namespace Rust {
namespace Compile {

struct DropCandidate
{
  DropCandidate (HirId hirid, location_t locus) : hirid (hirid), locus (locus)
  {}

  HirId hirid;
  location_t locus;
};

} // namespace Compile
} // namespace Rust

#endif // RUST_COMPILE_DROP_CANDIDATE_H