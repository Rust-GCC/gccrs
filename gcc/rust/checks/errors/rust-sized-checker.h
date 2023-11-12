// Copyright (C) 2023 Free Software Foundation, Inc.

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

#ifndef RUST_SIZED_CHECKER_H
#define RUST_SIZED_CHECKER_H

#include "rust-hir-visitor.h"
#include "rust-tyty.h"
#include "rust-hir-trait-reference.h"
#include "rust-hir-type-check.h"

namespace Rust {
namespace HIR {

class SizedChecker : public HIRFullVisitorBase
{
  static Resolver::TraitReference *sized_trait_cache;

  static bool is_sized (TyTy::BaseType *);

  Analysis::Mappings &mappings;
  Resolver::TypeCheckContext *tyctx;

public:
  SizedChecker () : mappings (*Analysis::Mappings::get ()), tyctx (Resolver::TypeCheckContext::get ()) {}

  void go (Crate &crate);

  // all items which could contain variables/functions

  using HIRFullVisitorBase::visit;

  void visit (Module &) override;
  void visit (Function &) override;
  void visit (Trait &) override;
  void visit (ImplBlock &) override;
  void visit (ExternBlock &) override;

  void visit (TraitItemFunc &) override;

  // void visit (ExternalFunctionItem &) override;
};

} // namespace HIR
} // namespace Rust

#endif /* !RUST_SIZED_CHECKER_H */
