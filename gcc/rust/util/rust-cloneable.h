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

#ifndef RUST_CLONEABLE
#define RUST_CLONEABLE

#include "rust-system.h"

namespace Rust {

// used to automatically copy cloneable types

template <typename T> struct CloneableDelegate
{
};

template <typename T> class Cloneable
{
public:
  template <typename... Args>
  Cloneable (Args &&...args) : inner (std::forward<Args> (args)...)
  {}

  Cloneable (const Cloneable &other)
    : Cloneable (CloneableDelegate<T>::clone (other.inner))
  {}

  template <typename Arg> Cloneable &operator= (Arg &&arg)
  {
    inner = std::forward<Arg> (arg);
    return *this;
  }

  Cloneable &operator= (const Cloneable &other)
  {
    inner = CloneableDelegate<T>::clone (other.inner);
    return *this;
  }

  Cloneable (Cloneable &&) = default;
  Cloneable &operator= (Cloneable &&) = default;

  T &get () { return inner; }
  const T &get () const { return inner; }

  bool operator== (decltype (nullptr)) const { return inner == nullptr; }

  bool operator!= (decltype (nullptr)) const { return inner != nullptr; }

private:
  T inner;
};

// general specializations

template <typename T> struct CloneableDelegate<std::vector<T>>
{
  static std::vector<T> clone (const std::vector<T> &other)
  {
    std::vector<T> ret;
    ret.reserve (other.size ());
    for (auto &ent : other)
      ret.push_back (CloneableDelegate<T>::clone (ent));
    return ret;
  }
};

} // namespace Rust

#endif // RUST_CLONEABLE
