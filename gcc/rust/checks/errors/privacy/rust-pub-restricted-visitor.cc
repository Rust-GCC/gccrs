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

#include "rust-pub-restricted-visitor.h"
#include "rust-hir.h"
#include "rust-hir-item.h"

/**
 * This visitor takes care of reporting `pub(restricted)` violations:
 * A `pub(restricted)` violation is defined as the usage of a path restriction
 * on an item which does not restrict the item's visibility to one of its parent
 * modules. What this means is that an user is allowed to specify that an item
 * should be public for any of its parent modules, going all the way to the
 * `crate` module, but not for any of its children module.
 *
 * ```rust
 * mod a {
 * 	mod b {
 * 		pub (in a) struct A0;
 *
 * 		mod c {
 * 			mod d {
 * 				pub (in a) struct A1;
 * 			}
 * 		}
 *
 * 		pub (in c::d) struct A2;
 * 	}
 * }
 * ```
 *
 * The above `A0`'s visibility is valid: It is restricted to a path, `a`,
 * which is a parent of the current module, `b`.
 * Likewise, `A1` is also defined properly: `a` is a parent of `d`, albeit
 * a great-great-great-grandparant of it.
 *
 * `A2` visibility, however, is invalid: Where the struct is defined, the
 * current module is `b`. `c::d` (which refers to the `d` module) is a child of
 * `b`, and not one of its ancestors.
 *
 * Note that these resolution rules are also the ones of the 2015 rust edition:
 * All the `pub(restricted)` visibilities above would be invalid in the 2018
 * edition, as the paths there must be absolute and not relative (`c::d` would
 * become `crate::a::b::c::d` etc). Nonetheless, the logic stays the same.
 */

namespace Rust {
namespace Privacy {

/**
 * Check if an item's restricted visibility (`pub (crate)`, `pub (self)`,
 * `pub(super)`, `pub (in <path>)`) is valid in the current context.
 * `pub restricted` visibilities are only allowed when the restriction path
 * is a parent module of the item being visited.
 *
 * In case of error, this function will emit the errors and return.
 *
 * @param item_id NodeId of the item to check the restriction of
 * @param locus Location of the item being checked
 *
 * @return true if the visibility restriction is valid, false otherwise.
 */
bool
PubRestrictedVisitor::is_restriction_valid (NodeId item_id,
					    const Location &locus)
{
  ModuleVisibility visibility;

  // If there is no visibility in the mappings, then the item is private and
  // does not contain any restriction
  // FIXME: Is that correct?
  if (!mappings.lookup_visibility (item_id, visibility))
    return true;

  for (auto mod = module_stack.rbegin (); mod != module_stack.rend (); mod++)
    if (*mod == visibility.get_module_id ())
      return true;

  rust_error_at (locus, "restricted path is not an ancestor of the "
			"current module");
  return false;
}

PubRestrictedVisitor::PubRestrictedVisitor (Analysis::Mappings &mappings)
  : mappings (mappings)
{}

void
PubRestrictedVisitor::go (HIR::Crate &crate)
{
  // The `crate` module will always be present
  module_stack.emplace_back (crate.get_mappings ().get_defid ());

  // FIXME: When do we insert `super`? `self`?
  // We need wrapper function for these

  for (auto &item : crate.items)
    {
      if (item->get_hir_kind () == HIR::Node::VIS_ITEM)
	{
	  auto vis_item = static_cast<HIR::VisItem *> (item.get ());
	  vis_item->accept_vis (*this);
	}
    }
}

void
PubRestrictedVisitor::visit (HIR::Module &mod)
{
  // FIXME: We need to update `super` and `self` here
  module_stack.push_back (mod.get_mappings ().get_defid ());

  is_restriction_valid (mod.get_mappings ().get_nodeid (), mod.get_locus ());

  for (auto &item : mod.get_items ())
    {
      if (item->get_hir_kind () == HIR::Node::VIS_ITEM)
	{
	  auto vis_item = static_cast<HIR::VisItem *> (item.get ());
	  vis_item->accept_vis (*this);
	}
    }

  module_stack.pop_back ();
}

void
PubRestrictedVisitor::visit (HIR::ExternCrate &crate)
{
  is_restriction_valid (crate.get_mappings ().get_nodeid (),
			crate.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::UseDeclaration &use_decl)
{
  is_restriction_valid (use_decl.get_mappings ().get_nodeid (),
			use_decl.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::Function &func)
{
  is_restriction_valid (func.get_mappings ().get_nodeid (), func.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::TypeAlias &type_alias)
{
  is_restriction_valid (type_alias.get_mappings ().get_nodeid (),
			type_alias.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::StructStruct &struct_item)
{
  is_restriction_valid (struct_item.get_mappings ().get_nodeid (),
			struct_item.get_locus ());
  // FIXME: Check fields here as well
}

void
PubRestrictedVisitor::visit (HIR::TupleStruct &tuple_struct)
{
  is_restriction_valid (tuple_struct.get_mappings ().get_nodeid (),
			tuple_struct.get_locus ());
  // FIXME: Check fields here as well
}

void
PubRestrictedVisitor::visit (HIR::Enum &enum_item)
{
  is_restriction_valid (enum_item.get_mappings ().get_nodeid (),
			enum_item.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::Union &union_item)
{
  is_restriction_valid (union_item.get_mappings ().get_nodeid (),
			union_item.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::ConstantItem &const_item)
{
  is_restriction_valid (const_item.get_mappings ().get_nodeid (),
			const_item.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::StaticItem &static_item)
{
  is_restriction_valid (static_item.get_mappings ().get_nodeid (),
			static_item.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::Trait &trait)
{
  is_restriction_valid (trait.get_mappings ().get_nodeid (),
			trait.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::ImplBlock &impl)
{
  is_restriction_valid (impl.get_mappings ().get_nodeid (), impl.get_locus ());
}

void
PubRestrictedVisitor::visit (HIR::ExternBlock &block)
{
  is_restriction_valid (block.get_mappings ().get_nodeid (),
			block.get_locus ());
}

} // namespace Privacy
} // namespace Rust
