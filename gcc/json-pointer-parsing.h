/* JSON Pointer parsing (RFC 6901).
   Copyright (C) 2026 Free Software Foundation, Inc.
   Contributed by David Malcolm <dmalcolm@redhat.com>.

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

#ifndef GCC_JSON_POINTER_PARSING_H
#define GCC_JSON_POINTER_PARSING_H

#include "json.h"
#include "json-parsing.h"
#include "pretty-print-token-buffer.h"

namespace json {
namespace pointer {

class error
{
public:
  error (pretty_print_token_buffer tokens)
  : m_tokens (std::move (tokens))
  {
  }

  pretty_print_token_buffer m_tokens;
};

/* Typedef for the result of parsing JSON pointer: borrowed json::value *
   or of a json::pointer::error *.  */
typedef result<const json::value*,
	       std::unique_ptr<error>> parser_result_t;

/* Function for parsing JSON pointer.  */

extern parser_result_t
parse_utf8_string (const char *utf8_json_pointer,
		   const json::value *root_val);

} // namespace pointer
} // namespace json

#endif  /* GCC_JSON_POINTER_PARSING_H  */
