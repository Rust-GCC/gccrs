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

#include "rust-ast-visitor.h"

namespace Rust {

// This class reassigns node ids to AST nodes
// It's a bit of a hack to work around cloning copying node ids

class NodeIdFixVisitor : public AST::DefaultASTVisitor
{
public:
  using DefaultASTVisitor::visit;

  template <typename T> static void fix (T &node)
  {
    NodeIdFixVisitor instance;
    instance.visit (node);
  }

  template <typename T> void visit (std::vector<T> &nodes)
  {
    for (auto &ent : nodes)
      visit (ent);
  }

  // TODO: add more visitors

  void visit (AST::TypePathSegment &) override;
  void visit (AST::TypePathSegmentGeneric &) override;
  void visit (AST::TypePathSegmentFunction &) override;
  void visit (AST::TypePath &) override;
  void visit (AST::QualifiedPathInType &) override;
};

} // namespace Rust
