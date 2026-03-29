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

#ifndef RUST_BUILTIN_ATTRIBUTE_CHECKER_H
#define RUST_BUILTIN_ATTRIBUTE_CHECKER_H

#include "rust-ast-visitor.h"

namespace Rust {
namespace Analysis {

void check_valid_attribute_for_item (const AST::Attribute &attr,
				     const AST::Item &item);

class BuiltinAttributeChecker : public AST::DefaultASTVisitor
{
  using AST::DefaultASTVisitor::visit;

  template <class I> void default_outer_attribute_check (I item)
  {
    for (auto &attr : item.get_outer_attrs ())
      check_valid_attribute_for_item (attr, item);
    AST::DefaultASTVisitor::visit (item);
  }

public:
  BuiltinAttributeChecker ();
  void go (AST::Crate &crate);
  void visit (AST::Crate &crate) override;
  void visit (AST::Attribute &attribute) override;

  // Items
  void visit (AST::Module &module) override;
  void visit (AST::ExternCrate &crate) override;
  void visit (AST::UseDeclaration &use_decl) override;
  void visit (AST::Function &function) override;
  void visit (AST::TypeAlias &type_alias) override;
  void visit (AST::StructStruct &struct_item) override;
  void visit (AST::TupleStruct &tuple_struct) override;
  void visit (AST::Enum &enum_item) override;
  void visit (AST::Union &union_item) override;
  void visit (AST::ConstantItem &const_item) override;
  void visit (AST::StaticItem &static_item) override;
  void visit (AST::Trait &trait) override;
  void visit (AST::InherentImpl &impl) override;
  void visit (AST::TraitImpl &impl) override;
  void visit (AST::ExternBlock &block) override;
};

} // namespace Analysis
} // namespace Rust

#endif /* ! RUST_BUILTIN_ATTRIBUTE_CHECKER_H */
