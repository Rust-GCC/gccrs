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

#ifndef RUST_NAME_RESOLVER_2_0_H
#define RUST_NAME_RESOLVER_2_0_H

namespace Rust {
namespace Resolver2_0 {

// FIXME: Documentation
class Usage
{
public:
  explicit Usage (NodeId id) : id (id) {}

  // TODO: move to name-resolution-ctx.cc
  // storing it as a key in a map
  bool operator< (const Usage other) const { return other.id < id; }

  NodeId id;
};

// FIXME: Documentation
class Definition
{
public:
  explicit Definition (NodeId id) : id (id) {}

  NodeId id;
};

} // namespace Resolver2_0
} // namespace Rust

#endif // RUST_NAME_RESOLVER_2_0_H
