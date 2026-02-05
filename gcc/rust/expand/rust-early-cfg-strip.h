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

#ifndef RUST_EARLY_CFG_STRIP_H
#define RUST_EARLY_CFG_STRIP_H

#include "rust-ast-visitor.h"

namespace Rust {

/**
 * Some parts cannot be stripped during the expansion passes
 */
class EarlyCfgStrip : AST::DefaultASTVisitor
{
public:
  using DefaultASTVisitor::visit;

  void go (AST::Crate &crate);

  void visit (AST::Crate &crate) override;
};

} // namespace Rust

#endif
