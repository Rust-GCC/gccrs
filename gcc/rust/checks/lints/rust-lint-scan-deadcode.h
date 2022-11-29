// Copyright (C) 2021-2022 Free Software Foundation, Inc.

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

#ifndef RUST_HIR_SCAN_DEADCODE
#define RUST_HIR_SCAN_DEADCODE

#include "rust-hir-full-decls.h"
#include "rust-hir-map.h"
#include "rust-lint-marklive.h"
#include "rust-name-resolver.h"
#include "rust-diagnostics.h"

namespace Rust {
namespace Analysis {

class ScanDeadcode : public MarkLiveBase
{
  using Rust::Analysis::MarkLiveBase::visit;

public:
  static void Scan (HIR::Crate &crate);

  void visit (HIR::Function &function) override;
  void visit (HIR::StructStruct &stct) override;
  void visit (HIR::TupleStruct &stct) override;
  void visit (HIR::ImplBlock &blc) override;
  void visit (HIR::Module &mod) override;

private:
  std::set<HirId> live_symbols;
  Resolver::Resolver *resolver;
  Analysis::Mappings *mappings;

  ScanDeadcode (std::set<HirId> &live_symbols)
    : live_symbols (live_symbols), resolver (Resolver::Resolver::get ()),
      mappings (Analysis::Mappings::get ()){};

  bool should_warn (HirId hirId);
};

} // namespace Analysis
} // namespace Rust

#endif
