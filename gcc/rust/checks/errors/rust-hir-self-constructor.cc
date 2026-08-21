// Copyright (C) 2025-2026 Free Software Foundation, Inc.

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

#include "rust-hir-self-constructor.h"
#include "rust-hir-item.h"
#include "rust-hir-path.h"
#include "rust-keyword-values.h"
#include "rust-diagnostics.h"

namespace Rust {
namespace HIR {

void
SelfConstructorChecker::go (Crate &crate)
{
  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}

void
SelfConstructorChecker::visit (Function &fct)
{
  bool is_associated = in_associated_scope;
  bool saved_associated = in_associated_scope;
  bool saved_self_ctor = self_ctor_from_outer;
  in_associated_scope = false;
  self_ctor_from_outer = impl_trait_nesting > 0 && !is_associated;
  walk (fct);
  in_associated_scope = saved_associated;
  self_ctor_from_outer = saved_self_ctor;
}

void
SelfConstructorChecker::visit (ImplBlock &impl)
{
  impl_trait_nesting++;
  bool saved_associated = in_associated_scope;
  in_associated_scope = true;
  walk (impl);
  in_associated_scope = saved_associated;
  impl_trait_nesting--;
}

void
SelfConstructorChecker::visit (Trait &trait)
{
  impl_trait_nesting++;
  bool saved_associated = in_associated_scope;
  in_associated_scope = true;
  walk (trait);
  in_associated_scope = saved_associated;
  impl_trait_nesting--;
}

void
SelfConstructorChecker::visit (PathInExpression &path)
{
  if (self_ctor_from_outer && path.get_segments ().size () == 1
      && path.get_segments ()[0].get_segment ().to_string ()
	   == Values::Keywords::SELF_ALIAS)
    rust_error_at (path.get_locus (),
		   "cannot reference %<Self%> constructor from outer item");
  walk (path);
}

} // namespace HIR
} // namespace Rust
