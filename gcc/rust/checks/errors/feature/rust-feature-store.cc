
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

#include "rust-feature-store.h"

namespace Rust {
namespace Features {

EarlyFeatureGateStore &
EarlyFeatureGateStore::get ()
{
  static EarlyFeatureGateStore instance{};
  return instance;
}

void
EarlyFeatureGateStore::add (Feature::Name name, Error error)
{
  potential_errors.emplace (name, error);
}

std::pair<Feature::Name, Error>
EarlyFeatureGateStore::get_error ()
{
  auto ret = potential_errors.front ();
  potential_errors.pop ();
  return ret;
}

} // namespace Features
} // namespace Rust
