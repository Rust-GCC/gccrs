// Copyright (C) 2023 Free Software Foundation, Inc.

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

#ifndef RUST_POLONIUS_H
#define RUST_POLONIUS_H

// Interface to the Polonius borrow checker engine.
// See (https://github.com/rust-lang/polonius/blob/master/polonius-engine/)

#include "rust-polonius-ffi.h"

namespace Rust {
namespace Polonius {

struct Facts
{
  // See (https://rust-lang.github.io/polonius/rules/relations.html)
  std::vector<FFI::Triple<Origin, Loan, Point>> loan_issued_at;
  std::vector<Origin> universal_region;
  std::vector<FFI::Pair<Point, Point>> cfg_edge;
  std::vector<FFI::Pair<Loan, Point>> loan_killed_at;
  std::vector<FFI::Triple<Origin, Origin, Point>> subset_base;
  std::vector<FFI::Pair<Point, Loan>> loan_invalidated_at;
  std::vector<FFI::Pair<Variable, Point>> var_used_at;
  std::vector<FFI::Pair<Variable, Point>> var_defined_at;
  std::vector<FFI::Pair<Variable, Point>> var_dropped_at;
  std::vector<FFI::Pair<Variable, Origin>> use_of_var_derefs_origin;
  std::vector<FFI::Pair<Variable, Origin>> drop_of_var_derefs_origin;
  std::vector<FFI::Pair<Path, Path>> child_path;
  std::vector<FFI::Pair<Path, Variable>> path_is_var;
  std::vector<FFI::Pair<Path, Point>> path_assigned_at_base;
  std::vector<FFI::Pair<Path, Point>> path_moved_at_base;
  std::vector<FFI::Pair<Path, Point>> path_accessed_at_base;
  std::vector<FFI::Pair<Origin, Origin>> known_placeholder_subset;
  std::vector<FFI::Pair<Origin, Loan>> placeholder;

  /**
   * Create a const view for the struct for FFI.
   *
   * This view uses the original vector storage.
   * Therefore any resizing operation of Facts member may invalidate the view.
   */
  FFI::FactsView freeze ()
  {
    return FFI::FactsView{loan_issued_at,
			  universal_region,
			  cfg_edge,
			  loan_killed_at,
			  subset_base,
			  loan_invalidated_at,
			  var_used_at,
			  var_defined_at,
			  var_dropped_at,
			  use_of_var_derefs_origin,
			  drop_of_var_derefs_origin,
			  child_path,
			  path_is_var,
			  path_assigned_at_base,
			  path_moved_at_base,
			  path_accessed_at_base,
			  known_placeholder_subset,
			  placeholder};
  }
};

/**
 * Check a single function for borrow errors.
 *
 * Output is not yet implemented and is only dumped to stdout.
 */
extern "C" void
polonius_run (FFI::FactsView input, bool dump_enabled);

} // namespace Polonius
} // namespace Rust

#endif /* !RUST_POLONIUS_H */
