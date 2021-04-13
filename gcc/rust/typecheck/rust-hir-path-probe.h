// Copyright (C) 2020 Free Software Foundation, Inc.

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

#ifndef RUST_HIR_PATH_PROBE_H
#define RUST_HIR_PATH_PROBE_H

#include "rust-hir-type-check-base.h"
#include "rust-hir-full.h"
#include "rust-tyty.h"
#include "rust-substitution-mapper.h"

namespace Rust {
namespace Resolver {

struct PathProbeCandidate
{
  HIR::InherentImplItem *impl_item;
  TyTy::BaseType *ty;
};

class PathProbeType : public TypeCheckBase
{
  using Rust::Resolver::TypeCheckBase::visit;

public:
  static std::vector<PathProbeCandidate>
  Probe (TyTy::BaseType *receiver, const HIR::PathIdentSegment &segment_name)
  {
    PathProbeType probe (receiver, segment_name);
    probe.mappings->iterate_impl_items (
      [&] (HirId id, HIR::InherentImplItem *item,
	   HIR::InherentImpl *impl) mutable -> bool {
	probe.process_candidate (id, item, impl);
	return true;
      });
    return probe.candidates;
  }

  void process_candidate (HirId id, HIR::InherentImplItem *item,
			  HIR::InherentImpl *impl)
  {
    HirId impl_ty_id = impl->get_type ()->get_mappings ().get_hirid ();
    TyTy::BaseType *impl_block_ty = nullptr;
    bool ok = context->lookup_type (impl_ty_id, &impl_block_ty);
    rust_assert (ok);

    if (!receiver->can_eq (impl_block_ty))
      return;

    // lets visit the impl_item
    item->accept_vis (*this);
  }

  void visit (HIR::ConstantItem &constant) override
  {
    Identifier name = constant.get_identifier ();
    if (search.as_string ().compare (name) == 0)
      {
	HirId tyid = constant.get_mappings ().get_hirid ();
	TyTy::BaseType *ty = nullptr;
	bool ok = context->lookup_type (tyid, &ty);
	rust_assert (ok);

	PathProbeCandidate candidate{&constant, ty};
	candidates.push_back (std::move (candidate));
      }
  }

  void visit (HIR::Function &function) override
  {
    Identifier name = function.get_function_name ();
    if (search.as_string ().compare (name) == 0)
      {
	HirId tyid = function.get_mappings ().get_hirid ();
	TyTy::BaseType *ty = nullptr;
	bool ok = context->lookup_type (tyid, &ty);
	rust_assert (ok);

	PathProbeCandidate candidate{&function, ty};
	candidates.push_back (std::move (candidate));
      }
  }

  void visit (HIR::Method &method) override
  {
    Identifier name = method.get_method_name ();
    if (search.as_string ().compare (name) == 0)
      {
	HirId tyid = method.get_mappings ().get_hirid ();
	TyTy::BaseType *ty = nullptr;
	bool ok = context->lookup_type (tyid, &ty);
	rust_assert (ok);

	PathProbeCandidate candidate{&method, ty};
	candidates.push_back (std::move (candidate));
      }
  }

private:
  PathProbeType (TyTy::BaseType *receiver, const HIR::PathIdentSegment &query)
    : TypeCheckBase (), receiver (receiver), search (query)
  {}

  TyTy::BaseType *receiver;
  const HIR::PathIdentSegment &search;
  std::vector<PathProbeCandidate> candidates;
};

class ReportMultipleCandidateError : private TypeCheckBase
{
  using Rust::Resolver::TypeCheckBase::visit;

public:
  static void Report (std::vector<PathProbeCandidate> &candidates,
		      const HIR::PathIdentSegment &query, Location query_locus)
  {
    rust_error_at (query_locus, "multiple applicable items in scope for: %s",
		   query.as_string ().c_str ());

    ReportMultipleCandidateError visitor;
    for (auto &c : candidates)
      c.impl_item->accept_vis (visitor);
  }

  void visit (HIR::ConstantItem &constant) override
  {
    rust_error_at (constant.get_locus (), "possible candidate");
  }

  void visit (HIR::Function &function) override
  {
    rust_error_at (function.get_locus (), "possible candidate");
  }

  void visit (HIR::Method &method) override
  {
    rust_error_at (method.get_locus (), "possible candidate");
  }

private:
  ReportMultipleCandidateError () : TypeCheckBase () {}
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_HIR_PATH_PROBE_H
