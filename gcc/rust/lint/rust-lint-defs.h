// Copyright (C) 2021 Free Software Foundation, Inc.

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

#ifndef RUST_LINT_DEFS_H
#define RUST_LINT_DEFS_H

#include <string>

namespace Rust {
namespace Lint {

enum Level
{
  Allow,
  Warn,
  Deny,
  Forbid
};

// TODO:
// edition_lint_opts
// future_incompatible
// feature_gate
class Lint
{
public:
  std::string name;
  Level default_level;
  std::string description;
  bool report_in_external_macro;
  bool is_plugin;
  bool crate_level_only;

  Lint (std::string name, Level default_level, std::string description)
    : name (name), default_level (default_level), description (description),
      report_in_external_macro (false), is_plugin (false),
      crate_level_only (false)
  {}
};

} // namespace Lint
} // namespace Rust

#endif