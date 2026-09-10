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

#ifndef RUST_LINT_NAMING_H
#define RUST_LINT_NAMING_H

#include "rust-hir-item.h"
#include "rust-hir-visitor.h"

namespace Rust {
namespace Analysis {
class NamingConventionCheck : public HIR::DefaultHIRVisitor
{
public:
  void go (HIR::Crate &crate);

  using HIR::DefaultHIRVisitor::visit;
  virtual void visit (HIR::Trait &trait) override;
  virtual void visit (HIR::StructStruct &strct) override;
  virtual void visit (HIR::TupleStruct &strct) override;
  virtual void visit (HIR::Enum &enm) override;
};
} // namespace Analysis
} // namespace Rust

#endif // RUST_LINT_NAMING_H
