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

#ifndef RUST_COMPILE_TYTY
#define RUST_COMPILE_TYTY

#include "rust-system.h"
#include "rust-location.h"
#include "rust-diagnostics.h"
#include "rust-backend.h"
#include "rust-tyty.h"
#include "rust-tyty-visitor.h"
#include "rust-hir-map.h"

namespace Rust {
namespace Compile {

class TyTyCompile : public TyTy::TyVisitor
{
public:
  static ::Btype *compile (::Backend *backend, TyTy::TyBase *ty)
  {
    TyTyCompile compiler (backend);
    ty->accept_vis (compiler);
    rust_assert (compiler.translated != nullptr);
    return compiler.translated;
  }

  ~TyTyCompile () {}

  void visit (TyTy::InferType &type) override
  {
    // there shouldn't be any of these left
    gcc_unreachable ();
  }

  void visit (TyTy::UnitType &type) override {}

  void visit (TyTy::FnType &type) override
  {
    Backend::Btyped_identifier receiver;
    std::vector<Backend::Btyped_identifier> parameters;
    std::vector<Backend::Btyped_identifier> results;

    if (!type.get_return_type ()->is_unit ())
      {
	auto hir_type = type.get_return_type ();
	auto ret = TyTyCompile::compile (backend, hir_type);
	results.push_back (Backend::Btyped_identifier (
	  "_", ret, mappings->lookup_location (hir_type->get_ref ())));
      }

    for (size_t i = 0; i < type.num_params (); i++)
      {
	auto param_tyty = type.param_at (i);
	auto compiled_param_type
	  = TyTyCompile::compile (backend, param_tyty->get_base_type ());
	auto compiled_param = Backend::Btyped_identifier (
	  param_tyty->get_identifier (), compiled_param_type,
	  mappings->lookup_location (param_tyty->get_ref ()));

	parameters.push_back (compiled_param);
      }

    translated
      = backend->function_type (receiver, parameters, results, NULL,
				mappings->lookup_location (type.get_ref ()));
  }

  void visit (TyTy::ParamType &type) override {}

  void visit (TyTy::BoolType &type) override
  {
    translated = backend->named_type ("bool", backend->bool_type (),
				      Linemap::predeclared_location ());
  }

  void visit (TyTy::IntType &type) override
  {
    switch (type.get_kind ())
      {
      case TyTy::IntType::I8:
	translated
	  = backend->named_type ("i8", backend->integer_type (false, 8),
				 Linemap::predeclared_location ());
	return;

      case TyTy::IntType::I16:
	translated
	  = backend->named_type ("i16", backend->integer_type (false, 16),
				 Linemap::predeclared_location ());
	return;

      case TyTy::IntType::I32:
	translated
	  = backend->named_type ("i32", backend->integer_type (false, 32),
				 Linemap::predeclared_location ());
	return;
      }
    gcc_unreachable ();
  }

  void visit (TyTy::UintType &type) override
  {
    switch (type.get_kind ())
      {
      case TyTy::UintType::U8:
	translated = backend->named_type ("i8", backend->integer_type (true, 8),
					  Linemap::predeclared_location ());
	return;

      case TyTy::UintType::U16:
	translated
	  = backend->named_type ("i16", backend->integer_type (true, 16),
				 Linemap::predeclared_location ());
	return;

      case TyTy::UintType::U32:
	translated
	  = backend->named_type ("i32", backend->integer_type (true, 32),
				 Linemap::predeclared_location ());
	return;
      }
    gcc_unreachable ();
  }

private:
  TyTyCompile (::Backend *backend)
    : backend (backend), translated (nullptr),
      mappings (Analysis::Mappings::get ())
  {}

  ::Backend *backend;
  ::Btype *translated;
  Analysis::Mappings *mappings;
};

class TyTyExtractParamsFromFnType : public TyTy::TyVisitor
{
public:
  static std::vector<TyTy::ParamType *> compile (TyTy::TyBase *ty)
  {
    TyTyExtractParamsFromFnType compiler;
    ty->accept_vis (compiler);
    rust_assert (compiler.ok);
    return compiler.translated;
  }

  ~TyTyExtractParamsFromFnType () {}

  void visit (TyTy::FnType &type) override
  {
    ok = true;
    for (size_t i = 0; i < type.num_params (); i++)
      {
	translated.push_back (type.param_at (i));
      }
  }

private:
  TyTyExtractParamsFromFnType () : ok (false) {}

  bool ok;
  std::vector<TyTy::ParamType *> translated;
};

class TyTyExtractRetFromFnType : public TyTy::TyVisitor
{
public:
  static TyTy::TyBase *compile (TyTy::TyBase *ty)
  {
    TyTyExtractRetFromFnType compiler;
    ty->accept_vis (compiler);
    rust_assert (compiler.ok);
    return compiler.translated;
  }

  ~TyTyExtractRetFromFnType () {}

  void visit (TyTy::FnType &type) override
  {
    ok = true;
    translated = type.get_return_type ();
  }

private:
  TyTyExtractRetFromFnType () : ok (false), translated (nullptr) {}

  bool ok;
  TyTy::TyBase *translated;
};

class TyTyCompileParam : public TyTy::TyVisitor
{
public:
  static ::Bvariable *compile (::Backend *backend, Bfunction *fndecl,
			       TyTy::TyBase *ty)
  {
    TyTyCompileParam compiler (backend, fndecl);
    ty->accept_vis (compiler);
    rust_assert (compiler.translated != nullptr);
    return compiler.translated;
  }

  ~TyTyCompileParam () {}

  void visit (TyTy::ParamType &type) override
  {
    auto btype = TyTyCompile::compile (backend, type.get_base_type ());
    bool tree_addressable = false;
    translated = backend->parameter_variable (fndecl, type.get_identifier (),
					      btype, tree_addressable,
					      mappings->lookup_location (
						type.get_ref ()));
  }

private:
  TyTyCompileParam (::Backend *backend, ::Bfunction *fndecl)
    : backend (backend), translated (nullptr), fndecl (fndecl),
      mappings (Analysis::Mappings::get ())
  {}

  ::Backend *backend;
  ::Bvariable *translated;
  ::Bfunction *fndecl;
  Analysis::Mappings *mappings;
};

} // namespace Compile
} // namespace Rust

#endif // RUST_COMPILE_TYTY
