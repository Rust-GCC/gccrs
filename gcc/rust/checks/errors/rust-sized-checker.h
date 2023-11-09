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

namespace Rust {
namespace HIR {

class SizedChecker : public HIRVisItemVisitor, public HIRTraitItemVisitor,
		     public HIRImplVisitor, public HIRExternalItemVisitor
{
  static TraitReference *sized_trait_cache;

  static void expect_sized (TyTy::BaseType *, location_t);
  static void expect_sized (std::vector<FunctionParam> &);
  static void expect_sized (Type &);
  static void expect_sized (SelfParam &);

  Analysis::Mappings &mappings;

public:
  SizedChecker () : mappings (*Analysis::Mappings::get ());

  void go (Crate &crate);

  void visit (Module &) override;
  void visit (Function &) override;
  void visit (Trait &) override;
  void visit (ImplBlock &) override;
  void visit (ExternBlock &) override;

  void visit (TraitItemFunc &) override;

  void visit (ExternalFunctionItem &) override;

  // unused
  void visit (ExternCrate &) override {}
  void visit (UseDeclaration &) override {}
  void visit (TypeAlias &) override {}
  void visit (StructStruct &) override {}
  void visit (TupleStruct &) override {}
  void visit (Enum &) override {}
  void visit (Union &) override {}
  void visit (ConstantItem &) override {}
  void visit (StaticItem &) override {}

  void visit (TraitItemConst &) override {}
  void visit (TraitItemType &) override {}

  void visit (ConstantItem &) override {}
  void visit (TypeAlias &) override {}

  void visit (ExternalStaticItem &) override {}
};

} // namespace HIR
} // namespace Rust

#endif /* !RUST_SIZED_CHECKER_H */
