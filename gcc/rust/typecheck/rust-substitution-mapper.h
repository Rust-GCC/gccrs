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
				  HIR::GenericArgs *generics = nullptr)
  {
    SubstMapper mapper (base->get_ref (), generics, locus);
    base->accept_vis (mapper);
    rust_assert (mapper.resolved != nullptr);
    return mapper.resolved;
  }

  bool have_generic_args () const { return generics != nullptr; }

  void visit (TyTy::FnType &type) override
  {
    TyTy::FnType *concrete = nullptr;
    if (!have_generic_args ())
      {
	TyTy::BaseType *substs = type.infer_substitions (locus);
	rust_assert (substs->get_kind () == TyTy::TypeKind::FNDEF);
	concrete = static_cast<TyTy::FnType *> (substs);
      }
    else
      {
	TyTy::SubstitutionArgumentMappings mappings
	  = type.get_mappings_from_generic_args (*generics);
	concrete = type.handle_substitions (mappings);
      }

    if (concrete != nullptr)
      resolved = concrete;
  }

  void visit (TyTy::ADTType &type) override
  {
    TyTy::ADTType *concrete = nullptr;
    if (!have_generic_args ())
      {
	TyTy::BaseType *substs = type.infer_substitions (locus);
	rust_assert (substs->get_kind () == TyTy::TypeKind::ADT);
	concrete = static_cast<TyTy::ADTType *> (substs);
      }
    else
      {
	TyTy::SubstitutionArgumentMappings mappings
	  = type.get_mappings_from_generic_args (*generics);
	concrete = type.handle_substitions (mappings);
      }

    if (concrete != nullptr)
      resolved = concrete;
  }

  void visit (TyTy::InferType &) override { gcc_unreachable (); }
  void visit (TyTy::TupleType &) override { gcc_unreachable (); }
  void visit (TyTy::FnPtr &) override { gcc_unreachable (); }
  void visit (TyTy::ArrayType &) override { gcc_unreachable (); }
  void visit (TyTy::BoolType &) override { gcc_unreachable (); }
  void visit (TyTy::IntType &) override { gcc_unreachable (); }
  void visit (TyTy::UintType &) override { gcc_unreachable (); }
  void visit (TyTy::FloatType &) override { gcc_unreachable (); }
  void visit (TyTy::USizeType &) override { gcc_unreachable (); }
  void visit (TyTy::ISizeType &) override { gcc_unreachable (); }
  void visit (TyTy::ErrorType &) override { gcc_unreachable (); }
  void visit (TyTy::CharType &) override { gcc_unreachable (); }
  void visit (TyTy::ReferenceType &) override { gcc_unreachable (); }
  void visit (TyTy::ParamType &) override { gcc_unreachable (); }
  void visit (TyTy::StrType &) override { gcc_unreachable (); }

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
				  TyTy::SubstitutionArgumentMappings &mappings)
  {
    SubstMapperInternal mapper (base->get_ref (), mappings);
    base->accept_vis (mapper);
    rust_assert (mapper.resolved != nullptr);
    return mapper.resolved;
  }

  void visit (TyTy::FnType &type) override
  {
    TyTy::SubstitutionArgumentMappings adjusted
      = type.adjust_mappings_for_this (mappings);

    TyTy::BaseType *concrete = type.handle_substitions (adjusted);
    if (concrete != nullptr)
      resolved = concrete;
  }

  void visit (TyTy::ADTType &type) override
  {
    TyTy::SubstitutionArgumentMappings adjusted
      = type.adjust_mappings_for_this (mappings);

    TyTy::BaseType *concrete = type.handle_substitions (adjusted);
    if (concrete != nullptr)
      resolved = concrete;
  }

  void visit (TyTy::InferType &) override { gcc_unreachable (); }
  void visit (TyTy::TupleType &) override { gcc_unreachable (); }
  void visit (TyTy::FnPtr &) override { gcc_unreachable (); }
  void visit (TyTy::ArrayType &) override { gcc_unreachable (); }
  void visit (TyTy::BoolType &) override { gcc_unreachable (); }
  void visit (TyTy::IntType &) override { gcc_unreachable (); }
  void visit (TyTy::UintType &) override { gcc_unreachable (); }
  void visit (TyTy::FloatType &) override { gcc_unreachable (); }
  void visit (TyTy::USizeType &) override { gcc_unreachable (); }
  void visit (TyTy::ISizeType &) override { gcc_unreachable (); }
  void visit (TyTy::ErrorType &) override { gcc_unreachable (); }
  void visit (TyTy::CharType &) override { gcc_unreachable (); }
  void visit (TyTy::ReferenceType &) override { gcc_unreachable (); }
  void visit (TyTy::ParamType &) override { gcc_unreachable (); }
  void visit (TyTy::StrType &) override { gcc_unreachable (); }

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
				  TyTy::BaseType *receiver)
  {
    rust_assert (concrete->get_kind () == receiver->get_kind ());

    SubstMapperFromExisting mapper (concrete, receiver);
    concrete->accept_vis (mapper);
    return mapper.resolved;
  }

  void visit (TyTy::FnType &type) override
  {
    rust_assert (type.was_substituted ());

    TyTy::FnType *to_sub = static_cast<TyTy::FnType *> (receiver);
    resolved = to_sub->handle_substitions (type.get_substitution_arguments ());
  }

  void visit (TyTy::ADTType &type) override
  {
    rust_assert (type.was_substituted ());

    TyTy::ADTType *to_sub = static_cast<TyTy::ADTType *> (receiver);
    resolved = to_sub->handle_substitions (type.get_substitution_arguments ());
  }

  void visit (TyTy::InferType &) override { gcc_unreachable (); }
  void visit (TyTy::TupleType &) override { gcc_unreachable (); }
  void visit (TyTy::FnPtr &) override { gcc_unreachable (); }
  void visit (TyTy::ArrayType &) override { gcc_unreachable (); }
  void visit (TyTy::BoolType &) override { gcc_unreachable (); }
  void visit (TyTy::IntType &) override { gcc_unreachable (); }
  void visit (TyTy::UintType &) override { gcc_unreachable (); }
  void visit (TyTy::FloatType &) override { gcc_unreachable (); }
  void visit (TyTy::USizeType &) override { gcc_unreachable (); }
  void visit (TyTy::ISizeType &) override { gcc_unreachable (); }
  void visit (TyTy::ErrorType &) override { gcc_unreachable (); }
  void visit (TyTy::CharType &) override { gcc_unreachable (); }
  void visit (TyTy::ReferenceType &) override { gcc_unreachable (); }
  void visit (TyTy::ParamType &) override { gcc_unreachable (); }
  void visit (TyTy::StrType &) override { gcc_unreachable (); }

private:
  SubstMapperFromExisting (TyTy::BaseType *concrete, TyTy::BaseType *receiver)
    : concrete (concrete), receiver (receiver), resolved (nullptr)
  {}

  TyTy::BaseType *concrete;
  TyTy::BaseType *receiver;

  TyTy::BaseType *resolved;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_SUBSTITUTION_MAPPER_H
