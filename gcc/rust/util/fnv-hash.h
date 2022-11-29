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

#ifndef RUST_FNV_HASH_H
#define RUST_FNV_HASH_H

#include "rust-system.h"

namespace Rust {
namespace Hash {

const uint64_t offset128Lower = 0x62b821756295c58d;
const uint64_t offset128Higher = 0x6c62272e07bb0142;
const uint64_t prime128Lower = 0x13b;
const uint64_t prime128Shift = 24;

// ported from https://github.com/golang/go/blob/master/src/hash/fnv/fnv.go
class FNV128
{
public:
  FNV128 ();

  void reset ();
  void write (const unsigned char *in, size_t len);
  void sum (uint64_t *hi, uint64_t *lo) const;

private:
  uint64_t buf[2];
};

} // namespace Hash
} // namespace Rust

#endif // RUST_FNV_HASH_H
