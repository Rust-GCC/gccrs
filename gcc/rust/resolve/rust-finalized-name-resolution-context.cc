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

#include "rust-finalized-name-resolution-context.h"

namespace Rust {
namespace Resolver2_0 {

static FinalizedNameResolutionContext *instance = nullptr;

const FinalizedNameResolutionContext &
FinalizedNameResolutionContext::init (NameResolutionContext &ctx)
{
  rust_assert (!instance);

  instance = new FinalizedNameResolutionContext (ctx);

  return *instance;
}

FinalizedNameResolutionContext &
FinalizedNameResolutionContext::get ()
{
  rust_assert (instance);

  return *instance;
}

void
FinalizedNameResolutionContext::map_usage (Usage usage, Definition definition,
					   Namespace ns)
{
  auto leaf_definition
    = ctx.find_leaf_definition (definition.id).value_or (definition);

  ctx.map_usage (usage, leaf_definition, ns);
}

tl::optional<NodeId>
FinalizedNameResolutionContext::lookup (NodeId usage, Namespace ns) const
{
  return ctx.lookup (usage, ns);
}

tl::optional<NameResolutionContext::NSLookup>
FinalizedNameResolutionContext::lookup (NodeId usage, Namespace ns1,
					Namespace ns2) const
{
  return ctx.lookup (usage, ns1, ns2);
}

Resolver::CanonicalPath
FinalizedNameResolutionContext::to_canonical_path (NodeId id,
						   Namespace ns) const
{
  return ctx.canonical_ctx.get_path (id, ns);
}

FinalizedNameResolutionContext::FinalizedNameResolutionContext (
  NameResolutionContext &ctx)
  : ctx (ctx)
{}

} // namespace Resolver2_0
} // namespace Rust
