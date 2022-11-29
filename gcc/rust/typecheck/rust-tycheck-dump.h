// Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ifndef RUST_TYCHECK_DUMP
#define RUST_TYCHECK_DUMP

#include "rust-hir-type-check-base.h"
#include "rust-hir-full.h"

namespace Rust {
namespace Resolver {

class TypeResolverDump : private TypeCheckBase, private HIR::HIRFullVisitorBase
{
  using HIR::HIRFullVisitorBase::visit;

public:
  static void go (HIR::Crate &crate, std::ofstream &out);

  void visit (HIR::StructStruct &struct_decl) override;
  void visit (HIR::Union &union_decl) override;
  void visit (HIR::TupleStruct &struct_decl) override;
  void visit (HIR::ImplBlock &impl_block) override;
  void visit (HIR::ConstantItem &constant) override;
  void visit (HIR::Function &function) override;
  void visit (HIR::BlockExpr &expr) override;
  void visit (HIR::UnsafeBlockExpr &expr) override;
  void visit (HIR::LetStmt &stmt) override;
  void visit (HIR::ExprStmtWithBlock &stmt) override;
  void visit (HIR::ExprStmtWithoutBlock &stmt) override;
  void visit (HIR::AssignmentExpr &expr) override;
  void visit (HIR::LiteralExpr &expr) override;
  void visit (HIR::ArrayExpr &expr) override;
  void visit (HIR::ArrayElemsValues &elems) override;
  void visit (HIR::GroupedExpr &expr) override;
  void visit (HIR::PathInExpression &expr) override;
  void visit (HIR::StructExprStructFields &expr) override;

protected:
  std::string type_string (const Analysis::NodeMapping &mappings);
  std::string indent ();

private:
  TypeResolverDump () : TypeCheckBase (), indentation_level (0) {}

  std::string dump;
  size_t indentation_level;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_TYCHECK_DUMP
