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

#ifndef RUST_FINALIZED_NRCTX_H
#define RUST_FINALIZED_NRCTX_H

#include "rust-name-resolution-context.h"
#include "rust-rib.h"

namespace Rust {
namespace Resolver2_0 {

/**
 * Once the name resolution pass is complete, the typechecker can access it
 *
 * FIXME: More documentation
 */
class FinalizedNameResolutionContext
{
public:
  /** FIXME: Documentation */
  static const FinalizedNameResolutionContext &
  init (NameResolutionContext &ctx);

  /** FIXME: Documentation */
  static FinalizedNameResolutionContext &get ();

  /**
   * The exact same method as NameResolutionContext::map_usage, but this one
   * uses the leafmost definition by default - as we are past the name
   * resolution stage, every import chain has been resolved, and can be followed
   * to an actual definition instead of an import definition
   */
  void map_usage (Usage usage, Definition definition, Namespace ns);

  /**
   * Same as NameResolutionContext::lookup
   */
  tl::optional<NodeId> lookup (NodeId usage, Namespace ns) const;
  tl::optional<NameResolutionContext::NSLookup>
  lookup (NodeId usage, Namespace ns1, Namespace ns2) const;

  /**
   * Same as NameResolutionContext::to_canonical_path
   */
  Resolver::CanonicalPath to_canonical_path (NodeId id, Namespace ns) const;

  /**
   * Avoid using these and prefer adding wrapper methods to this class instead.
   *
   * Let's limit the number of times we use these, and eventually try to remove
   * them.
   */
  const NameResolutionContext &get_underlying () const { return ctx; }
  NameResolutionContext &get_underlying () { return ctx; }

private:
  FinalizedNameResolutionContext (NameResolutionContext &ctx);
  FinalizedNameResolutionContext (const FinalizedNameResolutionContext &other)
    = default;

  NameResolutionContext &ctx;
};

} // namespace Resolver2_0

} // namespace Rust

#endif //! RUST_FINALIZED_NRCTX_H
