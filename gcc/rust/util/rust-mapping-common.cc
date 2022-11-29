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

#include "rust-mapping-common.h"

namespace Rust {

bool
DefId::operator== (const DefId &other) const
{
  return this->crateNum == other.crateNum
	 && this->localDefId == other.localDefId;
}

bool
DefId::operator!= (const DefId &other) const
{
  return !(*this == other);
}

bool
DefId::operator< (const DefId &other) const
{
  return ((uint64_t) this->crateNum << 32 | this->localDefId)
	 < ((uint64_t) other.crateNum << 32 | other.localDefId);
}

std::string
DefId::as_string () const
{
  std::string buf;
  buf += std::to_string (crateNum);
  buf += " "; // or anything else
  buf += std::to_string (localDefId);
  return buf;
}

} // namespace Rust
