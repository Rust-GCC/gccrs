// Copyright (C) 2025-2026 Free Software Foundation, Inc.

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

#ifndef RUST_GGC_H
#define RUST_GGC_H

#include "rust-system.h"
#include "tree.h"

namespace Rust {

// forward declare
class Identifier;

namespace GGC {

class Ident
{
  tree inner;

public:
  Ident (const char *str);
  Ident (const std::string &str);
  Ident (const Rust::Identifier &ident);

  bool operator== (const Ident &other) const { return inner == other.inner; }
  bool operator== (const std::string &other) const;

  const char *c_str () const { return IDENTIFIER_POINTER (inner); }
  size_t size () const { return IDENTIFIER_LENGTH (inner); }

  bool empty () const { return !size (); }

  std::string as_string () const
  {
    return std::string (c_str (), c_str () + size ());
  }

  tree as_tree () const { return inner; }
};

static inline bool
operator== (const std::string &a, const Ident &b)
{
  return b == a;
}

class ChainList
{
  tree head;
  tree *tail_cdr;

public:
  ChainList () : head (NULL_TREE), tail_cdr (&head) {}

  ChainList (ChainList &&oth)
  {
    head = oth.head;
    if (oth.tail_cdr == &oth.head)
      tail_cdr = &oth.head;
    else
      tail_cdr = oth.tail_cdr;
    oth.head = NULL_TREE;
    oth.tail_cdr = &oth.head;
  }

  ChainList &operator= (ChainList &&oth)
  {
    this->~ChainList ();
    new (this) ChainList (std::move (oth));
    return *this;
  }

  tree get_head () const { return head; }

  // TREE_CHAIN (ent) will be modified
  // making ent a node in this list
  // do not push a single tree to multiple ChainList objects
  void push_back (tree ent)
  {
    *tail_cdr = ent;
    tail_cdr = &TREE_CHAIN (ent);
  }
};

} // namespace GGC

} // namespace Rust

#endif // RUST_GGC_H
