// Copyright (C) 2025 Free Software Foundation, Inc.

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

#include "rust-std-prelude.h"
#include "input.h"
#include "rust-ast-builder.h"
#include "rust-ast.h"
#include "rust-item.h"

namespace Rust {
namespace AST {

void
StdPrelude::inject (AST::Crate &crate)
{
  // The actual logic is quite complicated, as this depends on editions and on
  // whether or not we're compiling in #[no_std] mode or not. For now, we just
  // want core's prelude, so that's what we'll inject

  auto loc = UNKNOWN_LOCATION;

  auto prelude_import
    = Attribute (SimplePath::from_str ("prelude_import", loc), nullptr);
  std::vector<Attribute> attributes = {prelude_import};

  // core::prelude::rust_2018::*
  auto core = SimplePathSegment ("core", loc);
  auto prelude = SimplePathSegment ("prelude", loc);
  auto rust_2018 = SimplePathSegment ("rust_2018", loc);

  auto path = SimplePath ({core, prelude, rust_2018});

  auto use
    = std::make_unique<UseTreeGlob> (UseTreeGlob::PathType::PATH_PREFIXED, path,
				     loc);

  auto final_import
    = std::make_unique<UseDeclaration> (std::move (use),
					Visibility::create_private (),
					std::move (attributes), loc);

  crate.items.insert (crate.items.begin (), std::move (final_import));
}

} // namespace AST
} // namespace Rust
