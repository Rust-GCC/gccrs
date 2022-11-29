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

#ifndef RUST_CANONICAL_PATH
#define RUST_CANONICAL_PATH

#include "rust-system.h"
#include "rust-mapping-common.h"

namespace Rust {
namespace Resolver {

// https://doc.rust-lang.org/reference/paths.html#canonical-paths
//
// struct X - path X
// impl X { fn test - path X::test }
//
// struct X<T> - path X
//
// impl X<T>   { fn test - path X::test}
// impl X<i32> { fn test - path X<i32>::test }
// impl X<f32> { fn test - path X<f32>::test }
//
// pub trait Trait { // ::a::Trait
//   fn f(&self); // ::a::Trait::f
// }
//
// impl Trait for Struct {
//    fn f(&self) {} // <::a::Struct as ::a::Trait>::f
// }
class CanonicalPath
{
public:
  CanonicalPath (const CanonicalPath &other) : segs (other.segs) {}

  CanonicalPath &operator= (const CanonicalPath &other);

  static CanonicalPath new_seg (NodeId id, const std::string &path);

  static CanonicalPath
  trait_impl_projection_seg (NodeId id, const CanonicalPath &trait_seg,
			     const CanonicalPath &impl_type_seg);

  std::string get () const;

  static CanonicalPath get_big_self (NodeId id);

  static CanonicalPath create_empty ();

  bool is_empty () const;

  CanonicalPath append (const CanonicalPath &other) const;

  void iterate (std::function<bool (const CanonicalPath &)> cb) const;

  void iterate_segs (std::function<bool (const CanonicalPath &)> cb) const;

  size_t size () const;

  NodeId get_node_id () const;

  const std::pair<NodeId, std::string> &get_seg_at (size_t index) const;

  bool is_equal (const CanonicalPath &b) const;

  void set_crate_num (CrateNum n);

  CrateNum get_crate_num () const;

  bool operator== (const CanonicalPath &b) const;

  bool operator< (const CanonicalPath &b) const;

private:
  explicit CanonicalPath (std::vector<std::pair<NodeId, std::string>> path,
			  CrateNum crate_num)
    : segs (path), crate_num (crate_num)
  {}

  std::vector<std::pair<NodeId, std::string>> segs;
  CrateNum crate_num;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_CANONICAL_PATH
