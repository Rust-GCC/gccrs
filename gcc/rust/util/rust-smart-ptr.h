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

#ifndef RUST_SMART_PTR_H
#define RUST_SMART_PTR_H

namespace Rust {

// used to mark classes as clone-able
// should greatly reduce the need for non-default
// copy assignment overloading and copy constructors
// implementation MUST have method:
// 'static std::unique_ptr<T> clone(T &)'
template <typename T> class ClonePtrInfo;

// cloning smart pointer
template <typename T> class ClonePtr
{
  std::unique_ptr<T> inner;

public:
  ClonePtr (std::unique_ptr<T> &&ptr) : inner (ptr) {}

  // default movement
  ClonePtr (ClonePtr &&) = default;
  ClonePtr &operator= (ClonePtr &) = default;

  // copy constructor
  ClonePtr (const ClonePtr &oth)
  {
    inner = oth.inner ? ClonePtrInfo<T>::clone (*oth.inner) : nullptr;
  }

  // copy assignment
  ClonePtr &operator= (const ClonePtr &oth)
  {
    inner = oth.inner ? ClonePtrInfo<T>::clone (*oth.inner) : nullptr;
    return *this;
  }

  // dereference
  T &operator* () { return *inner; }

  // comparison
  bool operator== (std::nullptr_t) const { return inner; }

  std::unique_ptr<T> &get_unique () { return inner; }
};

} // namespace Rust

#endif // RUST_SMART_PTR_H
