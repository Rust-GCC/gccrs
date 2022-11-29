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

#ifndef RUST_TYTY_CALL
#define RUST_TYTY_CALL

#include "rust-diagnostics.h"
#include "rust-hir-full.h"
#include "rust-tyty-visitor.h"
#include "rust-tyty.h"
#include "rust-hir-type-check.h"

namespace Rust {
namespace TyTy {

class TypeCheckCallExpr : private TyVisitor
{
public:
  static BaseType *go (BaseType *ref, HIR::CallExpr &call,
		       TyTy::VariantDef &variant,
		       Resolver::TypeCheckContext *context);

  void visit (InferType &) override;
  void visit (TupleType &) override;
  void visit (ArrayType &) override;
  void visit (SliceType &) override;
  void visit (BoolType &) override;
  void visit (IntType &) override;
  void visit (UintType &) override;
  void visit (FloatType &) override;
  void visit (USizeType &) override;
  void visit (ISizeType &) override;
  void visit (ErrorType &) override;
  void visit (CharType &) override;
  void visit (ReferenceType &) override;
  void visit (PointerType &) override;
  void visit (ParamType &) override;
  void visit (StrType &) override;
  void visit (NeverType &) override;
  void visit (PlaceholderType &) override;
  void visit (ProjectionType &) override;
  void visit (DynamicObjectType &) override;
  void visit (ClosureType &type) override;

  // tuple-structs
  void visit (ADTType &type) override;

  // call fns
  void visit (FnType &type) override;
  void visit (FnPtr &type) override;

private:
  TypeCheckCallExpr (HIR::CallExpr &c, TyTy::VariantDef &variant,
		     Resolver::TypeCheckContext *context)
    : resolved (new TyTy::ErrorType (c.get_mappings ().get_hirid ())), call (c),
      variant (variant), context (context),
      mappings (Analysis::Mappings::get ())
  {}

  BaseType *resolved;
  HIR::CallExpr &call;
  TyTy::VariantDef &variant;
  Resolver::TypeCheckContext *context;
  Analysis::Mappings *mappings;
};

class TypeCheckMethodCallExpr : private TyVisitor
{
public:
  // Resolve the Method parameters and return back the return type
  static BaseType *go (BaseType *ref, HIR::MethodCallExpr &call,
		       TyTy::BaseType *adjusted_self,
		       Resolver::TypeCheckContext *context);

  void visit (InferType &) override;
  void visit (TupleType &) override;
  void visit (ArrayType &) override;
  void visit (SliceType &) override;
  void visit (BoolType &) override;
  void visit (IntType &) override;
  void visit (UintType &) override;
  void visit (FloatType &) override;
  void visit (USizeType &) override;
  void visit (ISizeType &) override;
  void visit (ErrorType &) override;
  void visit (ADTType &) override;

  void visit (CharType &) override;
  void visit (ReferenceType &) override;
  void visit (PointerType &) override;
  void visit (ParamType &) override;
  void visit (StrType &) override;
  void visit (NeverType &) override;
  void visit (PlaceholderType &) override;
  void visit (ProjectionType &) override;
  void visit (DynamicObjectType &) override;

  // FIXME
  void visit (FnPtr &type) override;

  // call fns
  void visit (FnType &type) override;
  void visit (ClosureType &type) override;

private:
  TypeCheckMethodCallExpr (HIR::MethodCallExpr &c,
			   TyTy::BaseType *adjusted_self,
			   Resolver::TypeCheckContext *context)
    : resolved (nullptr), call (c), adjusted_self (adjusted_self),
      context (context), mappings (Analysis::Mappings::get ())
  {}

  BaseType *resolved;
  HIR::MethodCallExpr &call;
  TyTy::BaseType *adjusted_self;
  Resolver::TypeCheckContext *context;
  Analysis::Mappings *mappings;
};

} // namespace TyTy
} // namespace Rust

#endif // RUST_TYTY_CALL
