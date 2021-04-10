// Copyright (C) 2020 Free Software Foundation, Inc.

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

#ifndef RUST_TYTY_VISITOR
#define RUST_TYTY_VISITOR

#include "rust-tyty.h"

namespace Rust {
namespace TyTy {

class TyVisitor
{
public:
  virtual void visit (InferType &type) = 0;
  virtual void visit (ADTType &type) = 0;
  virtual void visit (TupleType &type) = 0;
  virtual void visit (FnType &type) = 0;
  virtual void visit (FnPtr &type) = 0;
  virtual void visit (ArrayType &type) = 0;
  virtual void visit (BoolType &type) = 0;
  virtual void visit (IntType &type) = 0;
  virtual void visit (UintType &type) = 0;
  virtual void visit (FloatType &type) = 0;
  virtual void visit (USizeType &type) = 0;
  virtual void visit (ISizeType &type) = 0;
  virtual void visit (ErrorType &type) = 0;
  virtual void visit (CharType &type) = 0;
  virtual void visit (ReferenceType &type) = 0;
  virtual void visit (ParamType &type) = 0;
  virtual void visit (StrType &type) = 0;
  virtual void visit (NeverType &type) = 0;
};

} // namespace TyTy
} // namespace Rust

#endif // RUST_TYTY_VISITOR
