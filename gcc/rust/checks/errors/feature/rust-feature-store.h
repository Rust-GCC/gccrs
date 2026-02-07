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

#ifndef RUST_FEATURE_STORE_H
#define RUST_FEATURE_STORE_H

#include "rust-system.h"
#include "rust-diagnostics.h"
#include "rust-feature.h"

namespace Rust {

namespace Features {

/**
 * We don't know the whole set of valid features until a crate has been parsed.
 * We're collecting in this store all the potential feature errors and check
 * them later.
 */
class EarlyFeatureGateStore
{
  std::queue<std::pair<Feature::Name, Error>> potential_errors;

public:
  static EarlyFeatureGateStore &get ();
  void add (Feature::Name name, Error error);

  bool has_error () { return !potential_errors.empty (); }

  std::pair<Feature::Name, Error> get_error ();
};
} // namespace Features

} // namespace Rust

#endif // ! RUST_FEATURE_STORE_H
