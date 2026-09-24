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

#include "rust-lint-naming.h"
#include "options.h"

namespace Rust {
namespace Analysis {

static bool
is_camel_case (Identifier identifier)
{
  auto s = identifier.as_string ();
  return ISUPPER (s.front ())
	 && std::all_of (s.begin (), s.end (),
			 [] (unsigned char c) { return ISALNUM (c); });
}

void
NamingConventionCheck::go (HIR::Crate &crate)
{
  for (auto &item : crate.get_items ())
    item->accept_vis (*this);
}

void
NamingConventionCheck::visit (HIR::Trait &trait)
{
  if (!is_camel_case (trait.get_name ()))
    rust_warning_at (trait.get_locus (), OPT_Wunused,
		     "trait %qs should have an upper camel case name",
		     trait.get_name ().as_string ().c_str ());
  walk (trait);
}

void
NamingConventionCheck::visit (HIR::StructStruct &strct)
{
  if (!is_camel_case (strct.get_identifier ()))
    rust_warning_at (strct.get_locus (), OPT_Wunused,
		     "struct %qs should have an upper camel case name",
		     strct.get_identifier ().as_string ().c_str ());
  walk (strct);
}

void
NamingConventionCheck::visit (HIR::TupleStruct &strct)
{
  if (!is_camel_case (strct.get_identifier ()))
    rust_warning_at (strct.get_locus (), OPT_Wunused,
		     "struct %qs should have an upper camel case name",
		     strct.get_identifier ().as_string ().c_str ());
  walk (strct);
}

void
NamingConventionCheck::visit (HIR::Enum &enm)
{
  if (!is_camel_case (enm.get_identifier ()))
    rust_warning_at (enm.get_locus (), OPT_Wunused,
		     "enum %qs should have an upper camel case name",
		     enm.get_identifier ().as_string ().c_str ());
  walk (enm);
}

} // namespace Analysis
} // namespace Rust
