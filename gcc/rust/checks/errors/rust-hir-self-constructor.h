// Copyright (C) 2025-2026 Free Software Foundation, Inc.

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

#ifndef RUST_HIR_SELF_CONSTRUCTOR_H
#define RUST_HIR_SELF_CONSTRUCTOR_H

#include "rust-hir.h"
#include "rust-hir-visitor.h"

namespace Rust {
namespace HIR {

// A function nested inside an associated item cannot capture the outer item's
// `Self`, so referencing the `Self` constructor from it is an error.
class SelfConstructorChecker : public DefaultHIRVisitor
{
public:
  void go (Crate &crate);

  using DefaultHIRVisitor::visit;
  virtual void visit (Function &fct) override;
  virtual void visit (ImplBlock &impl) override;
  virtual void visit (Trait &trait) override;
  virtual void visit (PathInExpression &path) override;

private:
  bool in_associated_scope = false;
  int impl_trait_nesting = 0;
  bool self_ctor_from_outer = false;
};

} // namespace HIR
} // namespace Rust

#endif // RUST_HIR_SELF_CONSTRUCTOR_H
