/* Capturing the results of pretty_print for later playback.
   Copyright (C) 2023-2026 Free Software Foundation, Inc.
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

#ifndef GCC_PRETTY_PRINT_TOKEN_BUFFER_H
#define GCC_PRETTY_PRINT_TOKEN_BUFFER_H

#include "pretty-print-format-impl.h"
#include "auto-obstack.h"
#include "pretty-print-markup.h"

/* A class for capturing the results of pretty-printing as tokens,
   potentially for playback into a different pretty-printer.  */

class pretty_print_token_buffer
{
public:
  pretty_print_token_buffer ();
  pretty_print_token_buffer (const char *gmsgid,
			     va_list *args);

  pretty_print_token_buffer (const pretty_print_token_buffer &) = delete;
  pretty_print_token_buffer (pretty_print_token_buffer &&);

  ~pretty_print_token_buffer () = default;

  std::string to_string () const;

  void dump (FILE *out) const { m_tokens.dump (out); }
  void DEBUG_FUNCTION dump () const { dump (stderr); }

  std::unique_ptr<auto_obstack> m_obstack;
  pp_token_list m_tokens;
};

/* A pp_element subclass for use with "%e" that replays the buffered tokens
   from TOKEN_BUF in another formatting call.  */

class pp_token_buffer_element : public pp_element
{
public:
  pp_token_buffer_element (const pretty_print_token_buffer &token_buf)
  : m_token_buf (token_buf)
  {
  }

  void
  add_to_phase_2 (pp_markup::context &ctxt) final override;

private:
  const pretty_print_token_buffer &m_token_buf;
};

#endif /* GCC_PRETTY_PRINT_TOKEN_BUFFER_H */
