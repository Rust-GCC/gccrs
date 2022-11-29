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

#ifndef RUST_HIR_LIVENESS
#define RUST_HIR_LIVENESS

#include "rust-hir-full-decls.h"
#include "rust-hir-map.h"
#include "rust-lint-marklive-base.h"
#include "rust-name-resolver.h"

namespace Rust {
namespace Analysis {

class MarkLive : public MarkLiveBase
{
  using Rust::Analysis::MarkLiveBase::visit;

public:
  static std::set<HirId> Analysis (HIR::Crate &crate);
  void go (HIR::Crate &crate);

  void visit (HIR::PathInExpression &expr) override;
  void visit (HIR::FieldAccessExpr &expr) override;
  void visit (HIR::TupleIndexExpr &expr) override;
  void visit (HIR::MethodCallExpr &expr) override;
  void visit (HIR::TypeAlias &alias) override;
  void visit (HIR::BorrowExpr &expr) override;
  void visit (HIR::DereferenceExpr &expr) override;
  void visit (HIR::NegationExpr &expr) override;
  void visit (HIR::LazyBooleanExpr &expr) override;
  void visit (HIR::TypeCastExpr &expr) override;
  void visit (HIR::GroupedExpr &expr) override;
  void visit (HIR::ArrayExpr &expr) override;
  void visit (HIR::ArrayIndexExpr &expr) override;
  void visit (HIR::ArrayElemsValues &expr) override;
  void visit (HIR::TupleExpr &expr) override;
  void visit (HIR::BlockExpr &expr) override;
  void visit (HIR::UnsafeBlockExpr &expr) override;
  void visit (HIR::LoopExpr &expr) override;
  void visit (HIR::BreakExpr &expr) override;
  void visit (HIR::WhileLoopExpr &expr) override;
  void visit (HIR::Function &function) override;
  void visit (HIR::ReturnExpr &expr) override;
  void visit (HIR::WhileLetLoopExpr &expr) override;
  void visit (HIR::ForLoopExpr &expr) override;
  void visit (HIR::ExprStmtWithoutBlock &stmt) override;
  void visit (HIR::ExprStmtWithBlock &stmt) override;
  void visit (HIR::CallExpr &expr) override;
  void visit (HIR::ArithmeticOrLogicalExpr &expr) override;
  void visit (HIR::ComparisonExpr &expr) override;
  void visit (HIR::AssignmentExpr &expr) override;
  void visit (HIR::CompoundAssignmentExpr &expr) override;
  void visit (HIR::IfExpr &expr) override;
  void visit (HIR::IfExprConseqElse &expr) override;
  void visit (HIR::MatchExpr &expr) override;
  void visit (HIR::IfExprConseqIf &expr) override;
  void visit (HIR::TraitItemFunc &item) override;
  void visit (HIR::ImplBlock &impl) override;
  void visit (HIR::LetStmt &stmt) override;
  void visit (HIR::StructExprStruct &stct) override;
  void visit (HIR::StructExprStructFields &stct) override;
  void visit (HIR::StructExprFieldIdentifierValue &field) override;
  void visit (HIR::StructExprStructBase &stct) override;
  void visit (HIR::Module &module) override;

private:
  std::vector<HirId> worklist;
  std::set<HirId> liveSymbols;
  std::set<HirId> scannedSymbols;
  Analysis::Mappings *mappings;
  Resolver::Resolver *resolver;
  Resolver::TypeCheckContext *tyctx;
  MarkLive (std::vector<HirId> worklist)
    : worklist (worklist), mappings (Analysis::Mappings::get ()),
      resolver (Resolver::Resolver::get ()),
      tyctx (Resolver::TypeCheckContext::get ()){};

  void mark_hir_id (HirId);
  bool visit_path_segment (HIR::PathExprSegment);
  void find_ref_node_id (NodeId ast_node_id, NodeId &ref_node_id);
};

} // namespace Analysis
} // namespace Rust

#endif // RUST_HIR_LIVENESS
