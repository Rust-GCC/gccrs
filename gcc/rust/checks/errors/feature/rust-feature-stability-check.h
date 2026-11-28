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

#ifndef RUST_FEATURE_STABILITY_CHECK_H
#define RUST_FEATURE_STABILITY_CHECK_H

#include "rust-ast-visitor.h"
#include "rust-feature-collector.h"
#include "rust-finalized-name-resolution-context.h"
#include "rust-hir-visitor.h"
#include "rust-item.h"

namespace Rust {
namespace HIR {

class StabilityChecker : public AST::DefaultASTVisitor,
			 public HIR::DefaultHIRVisitor
{
public:
  using AST::DefaultASTVisitor::visit;
  using HIR::DefaultHIRVisitor::visit;

  StabilityChecker (const Features::CrateFeatures &features)
    : active_features (features), mappings (Analysis::Mappings::get ()),
      name_ctx (Resolver2_0::FinalizedNameResolutionContext::get ())
  {}

  void check_early (AST::Crate &crate);
  void check_late (HIR::Crate &crate);
  void check_macros ();

  void visit (AST::UseTreeRebind &use_tree) override;
  void visit (AST::UseTreeList &use_tree_list) override;
  void visit (AST::ExternCrate &crate) override;

  void visit (HIR::PathInExpression &expr) override;
  void visit (HIR::MethodCallExpr &expr) override;
  void visit (HIR::TypePath &expr) override;
  void visit (HIR::QualifiedPathInExpression &expr) override;
  void visit (HIR::QualifiedPathInType &expr) override;

private:
  const Features::CrateFeatures &active_features;
  Analysis::Mappings &mappings;
  Resolver2_0::FinalizedNameResolutionContext &name_ctx;

  tl::optional<std::pair<CrateNum, const AST::AttrVec &>>
  get_crate_num_and_outer_attrs (HirId hir_id);
  void stability_error (location_t locus, const char *feature_name);
  void check_stability (location_t locus, NodeId nodeid);
  void check_stability_macro (location_t locus, NodeId nodeid);
};

} // namespace HIR
} // namespace Rust
#endif
