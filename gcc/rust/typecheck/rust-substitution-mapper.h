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

#ifndef RUST_SUBSTITUTION_MAPPER_H
#define RUST_SUBSTITUTION_MAPPER_H

#include "rust-tyty.h"
#include "rust-tyty-visitor.h"

namespace Rust {
namespace Resolver {

class SubstMapper : public TyTy::TyVisitor
{
public:
  static TyTy::BaseType *Resolve (TyTy::BaseType *base, Location locus,
				  HIR::GenericArgs *generics = nullptr);

  static TyTy::BaseType *InferSubst (TyTy::BaseType *base, Location locus);

  bool have_generic_args () const;

  void visit (TyTy::FnType &type) override;
  void visit (TyTy::ADTType &type) override;
  void visit (TyTy::PlaceholderType &type) override;
  void visit (TyTy::ProjectionType &type) override;
  void visit (TyTy::InferType &) override;
  void visit (TyTy::TupleType &) override;
  void visit (TyTy::FnPtr &) override;
  void visit (TyTy::ArrayType &) override;
  void visit (TyTy::SliceType &) override;
  void visit (TyTy::BoolType &) override;
  void visit (TyTy::IntType &) override;
  void visit (TyTy::UintType &) override;
  void visit (TyTy::FloatType &) override;
  void visit (TyTy::USizeType &) override;
  void visit (TyTy::ISizeType &) override;
  void visit (TyTy::ErrorType &) override;
  void visit (TyTy::CharType &) override;
  void visit (TyTy::ReferenceType &) override;
  void visit (TyTy::PointerType &) override;
  void visit (TyTy::ParamType &) override;
  void visit (TyTy::StrType &) override;
  void visit (TyTy::NeverType &) override;
  void visit (TyTy::DynamicObjectType &) override;
  void visit (TyTy::ClosureType &) override;

private:
  SubstMapper (HirId ref, HIR::GenericArgs *generics, Location locus)
    : resolved (new TyTy::ErrorType (ref)), generics (generics), locus (locus)
  {}

  TyTy::BaseType *resolved;
  HIR::GenericArgs *generics;
  Location locus;
};

class SubstMapperInternal : public TyTy::TyVisitor
{
public:
  static TyTy::BaseType *Resolve (TyTy::BaseType *base,
				  TyTy::SubstitutionArgumentMappings &mappings);

  static bool mappings_are_bound (TyTy::BaseType *ty,
				  TyTy::SubstitutionArgumentMappings &mappings);

  void visit (TyTy::FnType &type) override;
  void visit (TyTy::ADTType &type) override;

  // these don't support generic arguments but might contain a type param
  void visit (TyTy::TupleType &type) override;
  void visit (TyTy::ReferenceType &type) override;
  void visit (TyTy::PointerType &type) override;
  void visit (TyTy::ParamType &type) override;
  void visit (TyTy::PlaceholderType &type) override;
  void visit (TyTy::ProjectionType &type) override;
  void visit (TyTy::ClosureType &type) override;
  void visit (TyTy::ArrayType &type) override;
  void visit (TyTy::SliceType &type) override;

  // nothing to do for these
  void visit (TyTy::InferType &type) override;
  void visit (TyTy::FnPtr &type) override;
  void visit (TyTy::BoolType &type) override;
  void visit (TyTy::IntType &type) override;
  void visit (TyTy::UintType &type) override;
  void visit (TyTy::FloatType &type) override;
  void visit (TyTy::USizeType &type) override;
  void visit (TyTy::ISizeType &type) override;
  void visit (TyTy::ErrorType &type) override;
  void visit (TyTy::CharType &type) override;
  void visit (TyTy::StrType &type) override;
  void visit (TyTy::NeverType &type) override;
  void visit (TyTy::DynamicObjectType &type) override;

private:
  SubstMapperInternal (HirId ref, TyTy::SubstitutionArgumentMappings &mappings)
    : resolved (new TyTy::ErrorType (ref)), mappings (mappings)
  {}

  TyTy::BaseType *resolved;
  TyTy::SubstitutionArgumentMappings &mappings;
};

class SubstMapperFromExisting : public TyTy::TyVisitor
{
public:
  static TyTy::BaseType *Resolve (TyTy::BaseType *concrete,
				  TyTy::BaseType *receiver);

  void visit (TyTy::FnType &type) override;
  void visit (TyTy::ADTType &type) override;
  void visit (TyTy::ClosureType &type) override;

  void visit (TyTy::InferType &) override;
  void visit (TyTy::TupleType &) override;
  void visit (TyTy::FnPtr &) override;
  void visit (TyTy::ArrayType &) override;
  void visit (TyTy::SliceType &) override;
  void visit (TyTy::BoolType &) override;
  void visit (TyTy::IntType &) override;
  void visit (TyTy::UintType &) override;
  void visit (TyTy::FloatType &) override;
  void visit (TyTy::USizeType &) override;
  void visit (TyTy::ISizeType &) override;
  void visit (TyTy::ErrorType &) override;
  void visit (TyTy::CharType &) override;
  void visit (TyTy::ReferenceType &) override;
  void visit (TyTy::PointerType &) override;
  void visit (TyTy::ParamType &) override;
  void visit (TyTy::StrType &) override;
  void visit (TyTy::NeverType &) override;
  void visit (TyTy::PlaceholderType &) override;
  void visit (TyTy::ProjectionType &) override;
  void visit (TyTy::DynamicObjectType &) override;

private:
  SubstMapperFromExisting (TyTy::BaseType *concrete, TyTy::BaseType *receiver)
    : concrete (concrete), receiver (receiver), resolved (nullptr)
  {}

  TyTy::BaseType *concrete;
  TyTy::BaseType *receiver;

  TyTy::BaseType *resolved;
};

class GetUsedSubstArgs : public TyTy::TyConstVisitor
{
public:
  static TyTy::SubstitutionArgumentMappings From (const TyTy::BaseType *from);

  void visit (const TyTy::FnType &type) override;
  void visit (const TyTy::ADTType &type) override;
  void visit (const TyTy::ClosureType &type) override;

  void visit (const TyTy::InferType &) override;
  void visit (const TyTy::TupleType &) override;
  void visit (const TyTy::FnPtr &) override;
  void visit (const TyTy::ArrayType &) override;
  void visit (const TyTy::SliceType &) override;
  void visit (const TyTy::BoolType &) override;
  void visit (const TyTy::IntType &) override;
  void visit (const TyTy::UintType &) override;
  void visit (const TyTy::FloatType &) override;
  void visit (const TyTy::USizeType &) override;
  void visit (const TyTy::ISizeType &) override;
  void visit (const TyTy::ErrorType &) override;
  void visit (const TyTy::CharType &) override;
  void visit (const TyTy::ReferenceType &) override;
  void visit (const TyTy::PointerType &) override;
  void visit (const TyTy::ParamType &) override;
  void visit (const TyTy::StrType &) override;
  void visit (const TyTy::NeverType &) override;
  void visit (const TyTy::PlaceholderType &) override;
  void visit (const TyTy::ProjectionType &) override;
  void visit (const TyTy::DynamicObjectType &) override;

private:
  GetUsedSubstArgs () : args (TyTy::SubstitutionArgumentMappings::error ()) {}

  TyTy::SubstitutionArgumentMappings args;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_SUBSTITUTION_MAPPER_H
