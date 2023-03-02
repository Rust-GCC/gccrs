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

#include "rust-exported-macro-collector.h"
#include "rust-hir.h"
#include "rust-hir-expr.h"
#include "rust-hir-item.h"

namespace Rust {
namespace Metadata {

ExportedMacroCollector::ExportedMacroCollector (HIR::Crate &crate)
  : crate (crate)
{}

void
ExportedMacroCollector::collect ()
{
  for (auto &item : crate.items)
    if (item->get_hir_kind () == HIR::Node::VIS_ITEM)
      static_cast<HIR::VisItem *> (item.get ())->accept_vis (*this);

  for (auto macro : exported_macros)
    crate.items.emplace_back (macro);
}

void
ExportedMacroCollector::visit (HIR::ExportedMacro &macro)
{
  exported_macros.emplace_back (&macro);
}

void
ExportedMacroCollector::visit (HIR::Module &module)
{
  for (auto &item : module.get_items ())
    if (item->get_hir_kind () == HIR::Node::VIS_ITEM)
      static_cast<HIR::VisItem *> (item.get ())->accept_vis (*this);
}

void
ExportedMacroCollector::visit (HIR::Function &function)
{
  auto &block = function.get_definition ();

  for (auto &stmt : block->get_statements ())
    if (stmt->get_hir_kind () == HIR::Node::VIS_ITEM)
      static_cast<HIR::VisItem *> (stmt.get ())->accept_vis (*this);
}

void
ExportedMacroCollector::visit (HIR::ExternCrate &crate)
{}

void
ExportedMacroCollector::visit (HIR::UseDeclaration &use_decl)
{}

void
ExportedMacroCollector::visit (HIR::TypeAlias &type_alias)
{}

void
ExportedMacroCollector::visit (HIR::StructStruct &struct_item)
{}

void
ExportedMacroCollector::visit (HIR::TupleStruct &tuple_struct)
{}

void
ExportedMacroCollector::visit (HIR::Enum &enum_item)
{}

void
ExportedMacroCollector::visit (HIR::Union &union_item)
{}

void
ExportedMacroCollector::visit (HIR::ConstantItem &const_item)
{}

void
ExportedMacroCollector::visit (HIR::StaticItem &static_item)
{}

void
ExportedMacroCollector::visit (HIR::Trait &trait)
{}

void
ExportedMacroCollector::visit (HIR::ImplBlock &impl)
{}

void
ExportedMacroCollector::visit (HIR::ExternBlock &block)
{}

} // namespace Metadata
} // namespace Rust
