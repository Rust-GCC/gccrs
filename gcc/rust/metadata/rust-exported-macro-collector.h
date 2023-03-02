// Copyright (C) 2020-2023 Free Software Foundation, Inc.

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

#include "rust-system.h"
#include "rust-hir-visitor.h"
#include "rust-hir-full-decls.h"

namespace Rust {
namespace Metadata {

class ExportedMacroCollector : HIR::HIRVisItemVisitor
{
public:
  ExportedMacroCollector (HIR::Crate &crate);
  void collect ();

private:
  HIR::Crate &crate;
  std::vector<HIR::Item *> exported_macros;

  virtual void visit (HIR::Module &module);
  virtual void visit (HIR::ExternCrate &crate);
  virtual void visit (HIR::UseDeclaration &use_decl);
  virtual void visit (HIR::Function &function);
  virtual void visit (HIR::TypeAlias &type_alias);
  virtual void visit (HIR::StructStruct &struct_item);
  virtual void visit (HIR::TupleStruct &tuple_struct);
  virtual void visit (HIR::Enum &enum_item);
  virtual void visit (HIR::Union &union_item);
  virtual void visit (HIR::ConstantItem &const_item);
  virtual void visit (HIR::StaticItem &static_item);
  virtual void visit (HIR::Trait &trait);
  virtual void visit (HIR::ImplBlock &impl);
  virtual void visit (HIR::ExternBlock &block);
  virtual void visit (HIR::ExportedMacro &macro);
};

} // namespace Metadata
} // namespace Rust
