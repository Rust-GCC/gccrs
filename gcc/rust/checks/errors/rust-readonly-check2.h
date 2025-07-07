// Copyright (C) 2020-2025 Free Software Foundation, Inc.

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

#include "rust-hir-expr.h"
#include "rust-hir-full-decls.h"
#include "rust-hir-path.h"
#include "rust-hir-visitor.h"
#include "rust-name-resolver.h"

namespace Rust {
namespace HIR {
class ReadonlyChecker : public DefaultHIRVisitor
{
public:
  ReadonlyChecker ();

  void go (HIR::Crate &crate);

private:
  Resolver::Resolver &resolver;
  Analysis::Mappings &mappings;

  // TODO: Replace with a stacked context.
  bool in_mutable_context;

  void visit (AssignmentExpr &expr) override;
  void visit (PathInExpression &expr) override;
  void visit (FieldAccessExpr &expr) override;
  void visit (ArrayIndexExpr &expr) override;
  void visit (TupleExpr &expr) override;
  void visit (TupleIndexExpr &expr) override;
  void visit (LetStmt &stmt) override;
};

} // namespace HIR
} // namespace Rust