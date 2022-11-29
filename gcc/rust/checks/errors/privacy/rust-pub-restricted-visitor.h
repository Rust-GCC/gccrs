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

#ifndef RUST_PUB_RESTRICTED_VISITOR_H
#define RUST_PUB_RESTRICTED_VISITOR_H

#include "rust-hir-visitor.h"
#include "rust-hir.h"
#include "rust-hir-expr.h"
#include "rust-hir-stmt.h"
#include "rust-hir-item.h"
#include "rust-hir-map.h"

namespace Rust {
namespace Privacy {

class PubRestrictedVisitor : public HIR::HIRVisItemVisitor
{
public:
  PubRestrictedVisitor (Analysis::Mappings &mappings);

  void go (HIR::Crate &crate);

  bool is_restriction_valid (NodeId item_id, const Location &locus);

  virtual void visit (HIR::Module &mod);
  virtual void visit (HIR::ExternCrate &crate);
  virtual void visit (HIR::UseDeclaration &use_decl);
  virtual void visit (HIR::Function &func);
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

private:
  /* Stack of ancestor modules visited by this visitor */
  std::vector<DefId> module_stack;

  // FIXME: Do we have to handle `self` and `super` as part of the name
  // resolution pass?

  Analysis::Mappings &mappings;
};

} // namespace Privacy
} // namespace Rust

#endif // !RUST_PUB_RESTRICTED_VISITOR_H
