// Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ifndef RUST_PRIVACY_CTX_H
#define RUST_PRIVACY_CTX_H

#include "rust-hir-map.h"
#include "rust-privacy-check.h"

namespace Rust {
namespace Privacy {

enum ReachLevel
{
  Unreachable,
  Reachable,
};

class PrivacyContext
{
public:
  ReachLevel update_reachability (const Analysis::NodeMapping &mapping,
				  ReachLevel reach);

  const ReachLevel *lookup_reachability (const Analysis::NodeMapping &mapping);

private:
  std::unordered_map<DefId, ReachLevel> reachability_map;
};
} // namespace Privacy
} // namespace Rust

#if CHECKING_P
namespace selftest {
void
rust_privacy_ctx_test (void);
}
#endif // !CHECKING_P

#endif // !RUST_PRIVACY_CTX_H
