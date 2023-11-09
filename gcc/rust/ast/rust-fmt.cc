// Copyright (C) 2020-2023 Free Software Foundation, Inc.

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

#include "rust-fmt.h"

namespace Rust {
tl::expected<Fmt, Fmt::Error>
Fmt::parse_fmt_string (Fmt::Input input)
{
  return Fmt ();
}

Fmt::ParseResult<tl::optional<Fmt::Format>>
Fmt::maybe_format (Fmt::Input input)
{
  tl::optional<Fmt::Format> none = tl::nullopt;

  return Fmt::Result (input, none);
}

Fmt::ParseResult<Fmt::Format>
Fmt::format (Input input)
{
  return Fmt::Result (input, Format ());
}

Fmt::ParseResult<Fmt::Argument>
Fmt::argument (Input input)
{
  return Fmt::Result (input, Argument ());
}

Fmt::ParseResult<Fmt::FormatSpec>
Fmt::format_spec (Input input)
{
  return Fmt::Result (input, FormatSpec ());
}

Fmt::ParseResult<Fmt::Fill>
Fmt::fill (Input input)
{
  return Fmt::Result (input, Fill ());
}

Fmt::ParseResult<Fmt::Align>
Fmt::align (Input input)
{
  switch (input[0])
    {
    case '<':
      return Fmt::Result (input.substr (1), Align::Left);
    case '^':
      return Fmt::Result (input.substr (1), Align::Top);
    case '>':
      return Fmt::Result (input.substr (1), Align::Right);
    default:
      // TODO: Store the character here
      // TODO: Can we have proper error locations?
      // TODO: Maybe we should use a Rust::Literal string instead of a string
      return tl::make_unexpected (Error::Align);
    }
}

Fmt::ParseResult<Fmt::Sign>
Fmt::sign (Input input)
{
  switch (input[0])
    {
    case '+':
      return Fmt::Result (input.substr (1), Sign::Plus);
    case '-':
      return Fmt::Result (input.substr (1), Sign::Minus);
    default:
      // TODO: Store the character here
      // TODO: Can we have proper error locations?
      // TODO: Maybe we should use a Rust::Literal string instead of a string
      return tl::make_unexpected (Error::Sign);
    }
}

} // namespace Rust
