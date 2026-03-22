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

#ifndef RUST_RESOLVE_IDENTIFIER_PATH_H
#define RUST_RESOLVE_IDENTIFIER_PATH_H

#include "rust-ast-pointer-visitor.h"
#include "rust-name-resolution-context.h"

namespace Rust {
namespace Resolver2_0 {

// changes IdentifierPattern instances to PathInExpression instances
class IdentifierPathPass : public AST::PointerVisitor
{
public:
  IdentifierPathPass (NameResolutionContext &ctx,
		      std::set<NodeId> ident_path_to_convert);

  static void go (AST::Crate &crate, NameResolutionContext &ctx,
		  std::set<NodeId> ident_path_to_convert);

  using AST::PointerVisitor::reseat;

  void reseat (std::unique_ptr<AST::Pattern> &ptr) override;

private:
  NameResolutionContext *ctx;
  std::set<NodeId> ident_path_to_convert;
};

} // namespace Resolver2_0
} // namespace Rust

#endif // ! RUST_RESOLVE_IDENTIFIER_PATH_H
