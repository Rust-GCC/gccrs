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

#ifndef RUST_HIR_INHERENT_IMPL_ITEM_OVERLAP_H
#define RUST_HIR_INHERENT_IMPL_ITEM_OVERLAP_H

#include "rust-hir-type-check-base.h"
#include "rust-hir-full.h"

namespace Rust {
namespace Resolver {

class InherentImplItemToName : public TypeCheckBase
{
  using Rust::Resolver::TypeCheckBase::visit;

public:
  static bool resolve (HIR::InherentImplItem *item, std::string &name_result)
  {
    InherentImplItemToName resolver (name_result);
    item->accept_vis (resolver);
    return resolver.ok;
  }

  void visit (HIR::Method &method) override
  {
    ok = true;
    result.assign (method.get_method_name ());
  }

  void visit (HIR::Function &function) override
  {
    ok = true;
    result.assign (function.get_function_name ());
  }

  void visit (HIR::ConstantItem &constant) override
  {
    ok = true;
    result.assign (constant.get_identifier ());
  }

private:
  InherentImplItemToName (std::string &result)
    : TypeCheckBase (), ok (false), result (result)
  {}

  bool ok;
  std::string &result;
};

class GetLocusFromImplItem : public TypeCheckBase
{
  using Rust::Resolver::TypeCheckBase::visit;

public:
  static bool Resolve (HIR::InherentImplItem *query, Location &locus)
  {
    GetLocusFromImplItem resolver (locus);
    query->accept_vis (resolver);
    return resolver.ok;
  }

  void visit (HIR::ConstantItem &constant) override
  {
    ok = true;
    locus = constant.get_locus ();
  }

  void visit (HIR::Function &function) override
  {
    ok = true;
    locus = function.get_locus ();
  }

  void visit (HIR::Method &method) override
  {
    ok = true;
    locus = method.get_locus ();
  }

private:
  GetLocusFromImplItem (Location &locus)
    : TypeCheckBase (), ok (false), locus (locus)
  {}

  bool ok;
  Location &locus;
};

class OverlappingImplItemPass : public TypeCheckBase
{
  using Rust::Resolver::TypeCheckBase::visit;

public:
  static void go ()
  {
    OverlappingImplItemPass pass;

    // generate mappings
    pass.mappings->iterate_impl_items ([&] (HirId id,
					    HIR::InherentImplItem *impl_item,
					    HIR::InherentImpl *impl) -> bool {
      pass.process_impl_item (id, impl_item, impl);
      return true;
    });

    pass.scan ();
  }

  void process_impl_item (HirId id, HIR::InherentImplItem *impl_item,
			  HIR::InherentImpl *impl)
  {
    // lets make a mapping of impl-item Self type to (impl-item,name):
    // {
    //   impl-type -> [ (item, name), ... ]
    // }

    HirId impl_type_id = impl->get_type ()->get_mappings ().get_hirid ();
    TyTy::BaseType *impl_type = nullptr;
    bool ok = context->lookup_type (impl_type_id, &impl_type);
    rust_assert (ok);

    std::string impl_item_name;
    ok = InherentImplItemToName::resolve (impl_item, impl_item_name);
    rust_assert (ok);

    std::pair<HIR::InherentImplItem *, std::string> elem (impl_item,
							  impl_item_name);
    impl_mappings[impl_type].insert (std::move (elem));
  }

  void scan ()
  {
    // we can now brute force the map looking for can_eq on each of the
    // impl_items_types to look for possible colliding impl blocks;
    for (auto it = impl_mappings.begin (); it != impl_mappings.end (); it++)
      {
	TyTy::BaseType *query = it->first;

	for (auto iy = impl_mappings.begin (); iy != impl_mappings.end (); iy++)
	  {
	    TyTy::BaseType *candidate = iy->first;
	    if (query == candidate)
	      continue;

	    if (query->can_eq (candidate))
	      possible_collision (it->second, iy->second);
	  }
      }
  }

  void possible_collision (
    std::set<std::pair<HIR::InherentImplItem *, std::string> > query,
    std::set<std::pair<HIR::InherentImplItem *, std::string> > candidate)
  {
    for (auto &q : query)
      {
	HIR::InherentImplItem *query_impl_item = q.first;
	std::string query_impl_item_name = q.second;

	for (auto &c : candidate)
	  {
	    HIR::InherentImplItem *candidate_impl_item = c.first;
	    std::string candidate_impl_item_name = c.second;

	    if (query_impl_item_name.compare (candidate_impl_item_name) == 0)
	      collision_detected (query_impl_item, candidate_impl_item,
				  candidate_impl_item_name);
	  }
      }
  }

  void collision_detected (HIR::InherentImplItem *query,
			   HIR::InherentImplItem *dup, const std::string &name)
  {
    Location qlocus;
    bool ok = GetLocusFromImplItem::Resolve (query, qlocus);
    rust_assert (ok);

    Location dlocus;
    ok = GetLocusFromImplItem::Resolve (dup, dlocus);
    rust_assert (ok);

    // this needs GCC Rich locations see
    // https://github.com/Rust-GCC/gccrs/issues/97
    rust_error_at (qlocus, "duplicate definitions with name %s", name.c_str ());
    rust_error_at (dlocus, "duplicate def associated with");
  }

private:
  OverlappingImplItemPass () : TypeCheckBase () {}

  std::map<TyTy::BaseType *,
	   std::set<std::pair<HIR::InherentImplItem *, std::string> > >
    impl_mappings;

  std::map<TyTy::BaseType *, std::set<TyTy::BaseType *> >
    possible_colliding_impls;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_HIR_INHERENT_IMPL_ITEM_OVERLAP_H
