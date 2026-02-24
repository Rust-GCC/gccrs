/* Copyright (C) 2026 Free Software Foundation, Inc.
   Contributed by David Malcolm <dmalcolm@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_PRETTY_PRINT_MARKUP_JSON_H
#define GCC_PRETTY_PRINT_MARKUP_JSON_H

#include "pretty-print-markup.h"

namespace pp_markup {

/* Print the JSON Pointer of a given json::value in quotes.  */

class quoted_json_pointer : public pp_element
{
public:
  quoted_json_pointer (const json::value &js_val)
  : m_js_val (js_val)
  {
  }

  void
  add_to_phase_2 (context &ctxt) final override
  {
   ctxt.begin_quote ();
   m_js_val.print_pointer (&ctxt.m_pp);
   ctxt.end_quote ();
  }

private:
  const json::value &m_js_val;
};

} // namespace pp_markup

#endif /* GCC_PRETTY_PRINT_MARKUP_JSON_H */
