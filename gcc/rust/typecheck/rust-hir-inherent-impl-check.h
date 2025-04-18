// Copyright (C) 2020-2025 Free Software Foundation, Inc.

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
#ifndef RUST_HIR_INHERENT_IMPL_ITEM_CHECK_H
#define RUST_HIR_INHERENT_IMPL_ITEM_CHECK_H

#include "rust-diagnostics.h"
#include "rust-hir-item.h"
#include "rust-hir-type-check-base.h"
#include "rust-mapping-common.h"
#include "rust-type-util.h"

namespace Rust {
namespace Resolver {

class PrimitiveImplCheck : public TypeCheckBase
{
public:
  static void go ()
  {
    PrimitiveImplCheck pass;

    pass.scan ();
  }

private:
  void scan ()

  {
    std::vector<HIR::ImplBlock *> possible_primitive_impl;
    mappings.iterate_impl_blocks ([&] (HirId id, HIR::ImplBlock *impl) -> bool {
      // filtering  trait-impl-blocks
      if (impl->has_trait_ref ())
	return true;
      HirId impl_ty_id = impl->get_type ().get_mappings ().get_hirid ();
      TyTy::BaseType *impl_type = nullptr;
      if (!query_type (impl_ty_id, &impl_type))
	return true;
      DefId defid = impl->get_mappings ().get_defid ();
      // ignore lang item
      if (mappings.lookup_lang_item (defid))
	return true;
      if (is_primitive_type_kind (impl_type->get_kind ()))
	{
	  possible_primitive_impl.push_back (impl);
	}
      return true;
    });

    for (auto impl : possible_primitive_impl)
      {
	report_error (impl);
      }
  }

  void report_error (HIR::ImplBlock *impl)
  {
    rich_location r (line_table, impl->get_locus ());
    std::string msg = "consider using an extension trait instead";
    r.add_fixit_replace (impl->get_locus (), msg.c_str ());
    r.add_range (impl->get_locus ());
    std::string err = "impl";
    err = "cannot define inherent `" + err + "` for primitive types";
    rust_error_at (r, ErrorCode::E0390, "%s", err.c_str ());
  }
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_HIR_INHERENT_IMPL_ITEM_CHECK_H
