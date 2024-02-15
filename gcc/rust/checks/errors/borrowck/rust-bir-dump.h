// Copyright (C) 2020-2024 Free Software Foundation, Inc.

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

#ifndef RUST_BIR_DUMP_H
#define RUST_BIR_DUMP_H

#include <ostream>
#include <utility>
#include "rust-bir-place.h"
#include "rust-bir-visitor.h"
#include "rust-bir.h"

namespace Rust {
namespace BIR {

/** Prints the BIR to a stream in a format resembling rustc's MIR. */
class Dump : public Visitor
{
  std::ostream &stream;
  Function &func;
  const std::string &name;

  std::vector<BasicBlockId> bb_fold_map;
  std::vector<PlaceId> place_map;

  PlaceId node_place = INVALID_PLACE;
  BasicBlockId node_bb = INVALID_BB;
  bool bb_terminated = false;

public:
  Dump (std::ostream &os, Function &func, const std::string &name)
    : stream (os), func (func), name (name),
      bb_fold_map (func.basic_blocks.size ()), place_map (func.place_db.size ())
  {}
  void go (bool enable_simplify_cfg = false);

protected:
  void visit (Node &node) override;
  void visit_place (PlaceId place_id);
  void visit_move_place (PlaceId place_id);
  void visit (BorrowExpr &expr) override;
  void visit_lifetime (PlaceId place_id);
  void visit (InitializerExpr &expr) override;
  void visit (CallExpr &expr) override;
  void visit (Operator<1> &expr) override;
  void visit (Operator<2> &expr) override;
  void visit (Assignment &expr) override;
};

} // namespace BIR
} // namespace Rust

#endif // RUST_BIR_DUMP_H
