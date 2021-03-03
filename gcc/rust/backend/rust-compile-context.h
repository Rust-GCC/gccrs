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

#ifndef RUST_COMPILE_CONTEXT
#define RUST_COMPILE_CONTEXT

#include "rust-system.h"
#include "rust-hir-map.h"
#include "rust-name-resolver.h"
#include "rust-hir-type-check.h"
#include "rust-backend.h"
#include "rust-compile-tyty.h"
#include "rust-ast-full.h"
#include "rust-hir-full.h"

namespace Rust {
namespace Compile {

struct fncontext
{
  ::Bfunction *fndecl;
  ::Bvariable *ret_addr;
};

class Context
{
public:
  Context (::Backend *backend)
    : backend (backend), resolver (Resolver::Resolver::get ()),
      tyctx (Resolver::TypeCheckContext::get ()),
      mappings (Analysis::Mappings::get ())
  {
    // insert the builtins
    auto builtins = resolver->get_builtin_types ();
    for (auto it = builtins.begin (); it != builtins.end (); it++)
      {
	HirId ref;
	rust_assert (
	  tyctx->lookup_type_by_node_id ((*it)->get_node_id (), &ref));

	TyTy::BaseType *lookup;
	rust_assert (tyctx->lookup_type (ref, &lookup));

	auto compiled = TyTyCompile::compile (backend, lookup);
	compiled_type_map[ref] = compiled;
      }
  }

  ~Context () {}

  bool lookup_compiled_types (HirId id, ::Btype **type)
  {
    auto it = compiled_type_map.find (id);
    if (it == compiled_type_map.end ())
      return false;

    *type = it->second;
    return true;
  }

  void insert_compiled_type (HirId id, ::Btype *type)
  {
    compiled_type_map[id] = type;
  }

  ::Backend *get_backend () { return backend; }
  Resolver::Resolver *get_resolver () { return resolver; }
  Resolver::TypeCheckContext *get_tyctx () { return tyctx; }
  Analysis::Mappings *get_mappings () { return mappings; }

  void push_block (Bblock *scope)
  {
    scope_stack.push_back (scope);
    statements.push_back ({});
  }

  Bblock *pop_block ()
  {
    auto block = scope_stack.back ();
    scope_stack.pop_back ();

    auto stmts = statements.back ();
    statements.pop_back ();

    backend->block_add_statements (block, stmts);

    return block;
  }

  Bblock *peek_enclosing_scope ()
  {
    if (scope_stack.size () == 0)
      return nullptr;

    return scope_stack.back ();
  }

  void add_statement_to_enclosing_scope (Bstatement *stmt)
  {
    statements.at (statements.size () - 2).push_back (stmt);
  }

  void add_statement (Bstatement *stmt) { statements.back ().push_back (stmt); }

  void insert_var_decl (HirId id, ::Bvariable *decl)
  {
    compiled_var_decls[id] = decl;
  }

  bool lookup_var_decl (HirId id, ::Bvariable **decl)
  {
    auto it = compiled_var_decls.find (id);
    if (it == compiled_var_decls.end ())
      return false;

    *decl = it->second;
    return true;
  }

  void insert_function_decl (HirId id, ::Bfunction *fn)
  {
    compiled_fn_map[id] = fn;
  }

  bool lookup_function_decl (HirId id, ::Bfunction **fn)
  {
    auto it = compiled_fn_map.find (id);
    if (it == compiled_fn_map.end ())
      return false;

    *fn = it->second;
    return true;
  }

  void insert_const_decl (HirId id, ::Bexpression *expr)
  {
    compiled_consts[id] = expr;
  }

  bool lookup_const_decl (HirId id, ::Bexpression **expr)
  {
    auto it = compiled_consts.find (id);
    if (it == compiled_consts.end ())
      return false;

    *expr = it->second;
    return true;
  }

  void insert_label_decl (HirId id, ::Blabel *label)
  {
    compiled_labels[id] = label;
  }

  bool lookup_label_decl (HirId id, ::Blabel **label)
  {
    auto it = compiled_labels.find (id);
    if (it == compiled_labels.end ())
      return false;

    *label = it->second;
    return true;
  }

  void push_fn (::Bfunction *fn, ::Bvariable *ret_addr)
  {
    fn_stack.push_back (fncontext{fn, ret_addr});
  }
  void pop_fn () { fn_stack.pop_back (); }
  fncontext peek_fn () { return fn_stack.back (); }

  void push_type (::Btype *t) { type_decls.push_back (t); }
  void push_var (::Bvariable *v) { var_decls.push_back (v); }
  void push_const (::Bexpression *c) { const_decls.push_back (c); }
  void push_function (::Bfunction *f) { func_decls.push_back (f); }

  void write_to_backend ()
  {
    backend->write_global_definitions (type_decls, const_decls, func_decls,
				       var_decls);
  }

  bool function_completed (Bfunction *fn)
  {
    for (auto it = func_decls.begin (); it != func_decls.end (); it++)
      {
	Bfunction *i = (*it);
	if (i == fn)
	  {
	    return true;
	  }
      }
    return false;
  }

  void push_loop_context (Bvariable *var) { loop_value_stack.push_back (var); }

  Bvariable *peek_loop_context () { return loop_value_stack.back (); }

  Bvariable *pop_loop_context ()
  {
    auto back = loop_value_stack.back ();
    loop_value_stack.pop_back ();
    return back;
  }

  void push_loop_begin_label (Blabel *label)
  {
    loop_begin_labels.push_back (label);
  }

  Blabel *peek_loop_begin_label () { return loop_begin_labels.back (); }

  Blabel *pop_loop_begin_label ()
  {
    Blabel *pop = loop_begin_labels.back ();
    loop_begin_labels.pop_back ();
    return pop;
  }

private:
  ::Backend *backend;
  Resolver::Resolver *resolver;
  Resolver::TypeCheckContext *tyctx;
  Analysis::Mappings *mappings;

  // state
  std::vector<fncontext> fn_stack;
  std::map<HirId, ::Bvariable *> compiled_var_decls;
  std::map<HirId, ::Btype *> compiled_type_map;
  std::map<HirId, ::Bfunction *> compiled_fn_map;
  std::map<HirId, ::Bexpression *> compiled_consts;
  std::map<HirId, ::Blabel *> compiled_labels;
  std::vector< ::std::vector<Bstatement *> > statements;
  std::vector< ::Bblock *> scope_stack;
  std::vector< ::Bvariable *> loop_value_stack;
  std::vector< ::Blabel *> loop_begin_labels;

  // To GCC middle-end
  std::vector< ::Btype *> type_decls;
  std::vector< ::Bvariable *> var_decls;
  std::vector< ::Bexpression *> const_decls;
  std::vector< ::Bfunction *> func_decls;
};

class TyTyResolveCompile : public TyTy::TyVisitor
{
public:
  static ::Btype *compile (Context *ctx, TyTy::BaseType *ty)
  {
    TyTyResolveCompile compiler (ctx);
    ty->accept_vis (compiler);
    return compiler.translated;
  }

  void visit (TyTy::ErrorType &) override { gcc_unreachable (); }

  void visit (TyTy::InferType &) override { gcc_unreachable (); }

  void visit (TyTy::ParamType &param) override
  {
    rust_assert (param.get_ref () != param.get_ty_ref ());

    TyTy::BaseType *lookup = nullptr;
    bool ok = ctx->get_tyctx ()->lookup_type (param.get_ty_ref (), &lookup);
    rust_assert (ok);
    lookup->accept_vis (*this);
  }

  void visit (TyTy::FnType &type) override
  {
    Backend::Btyped_identifier receiver;
    std::vector<Backend::Btyped_identifier> parameters;
    std::vector<Backend::Btyped_identifier> results;

    if (!type.get_return_type ()->is_unit ())
      {
	auto hir_type = type.get_return_type ();
	auto ret = TyTyResolveCompile::compile (ctx, hir_type);
	results.push_back (Backend::Btyped_identifier (
	  "_", ret,
	  ctx->get_mappings ()->lookup_location (hir_type->get_ref ())));
      }

    for (auto &param_pair : type.get_params ())
      {
	auto param_tyty = param_pair.second;
	auto compiled_param_type
	  = TyTyResolveCompile::compile (ctx, param_tyty);

	auto compiled_param = Backend::Btyped_identifier (
	  param_pair.first->as_string (), compiled_param_type,
	  ctx->get_mappings ()->lookup_location (param_tyty->get_ref ()));

	parameters.push_back (compiled_param);
      }

    translated = ctx->get_backend ()->function_type (
      receiver, parameters, results, NULL,
      ctx->get_mappings ()->lookup_location (type.get_ref ()));
  }

  void visit (TyTy::UnitType &) override
  {
    translated = ctx->get_backend ()->void_type ();
  }

  void visit (TyTy::ADTType &type) override
  {
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &translated);
    if (ok)
      return;

    // create implicit struct
    std::vector<Backend::Btyped_identifier> fields;
    for (size_t i = 0; i < type.num_fields (); i++)
      {
	TyTy::StructFieldType *field = type.get_field (i);
	Btype *compiled_field_ty
	  = TyTyResolveCompile::compile (ctx, field->get_field_type ());

	Backend::Btyped_identifier f (field->get_name (), compiled_field_ty,
				      ctx->get_mappings ()->lookup_location (
					type.get_ty_ref ()));
	fields.push_back (std::move (f));
      }

    Btype *struct_type_record = ctx->get_backend ()->struct_type (fields);
    Btype *named_struct
      = ctx->get_backend ()->named_type (type.get_name (), struct_type_record,
					 ctx->get_mappings ()->lookup_location (
					   type.get_ref ()));

    ctx->push_type (named_struct);
    ctx->insert_compiled_type (type.get_ty_ref (), named_struct);
    translated = named_struct;
  }

  void visit (TyTy::TupleType &type) override
  {
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &translated);
    if (ok)
      return;

    // create implicit struct
    std::vector<Backend::Btyped_identifier> fields;
    for (size_t i = 0; i < type.num_fields (); i++)
      {
	TyTy::BaseType *field = type.get_field (i);
	Btype *compiled_field_ty
	  = TyTyCompile::compile (ctx->get_backend (), field);

	Backend::Btyped_identifier f (std::to_string (i), compiled_field_ty,
				      ctx->get_mappings ()->lookup_location (
					type.get_ty_ref ()));
	fields.push_back (std::move (f));
      }

    Btype *struct_type_record = ctx->get_backend ()->struct_type (fields);
    Btype *named_struct
      = ctx->get_backend ()->named_type (type.as_string (), struct_type_record,
					 ctx->get_mappings ()->lookup_location (
					   type.get_ty_ref ()));

    ctx->push_type (named_struct);
    ctx->insert_compiled_type (type.get_ty_ref (), named_struct);
    translated = named_struct;
  }

  void visit (TyTy::ArrayType &type) override
  {
    mpz_t ival;
    mpz_init_set_ui (ival, type.get_capacity ());

    Btype *capacity_type = ctx->get_backend ()->integer_type (true, 32);
    Bexpression *length
      = ctx->get_backend ()->integer_constant_expression (capacity_type, ival);

    Btype *element_type = TyTyResolveCompile::compile (ctx, type.get_type ());
    translated = ctx->get_backend ()->array_type (element_type, length);
  }

  void visit (TyTy::BoolType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

  void visit (TyTy::IntType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

  void visit (TyTy::UintType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

  void visit (TyTy::FloatType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

  void visit (TyTy::USizeType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

  void visit (TyTy::ISizeType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

  void visit (TyTy::CharType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

  void visit (TyTy::ReferenceType &type) override
  {
    Btype *base_compiled_type
      = TyTyResolveCompile::compile (ctx, type.get_base ());
    translated = ctx->get_backend ()->reference_type (base_compiled_type);
  }

  void visit (TyTy::StrType &type) override
  {
    ::Btype *compiled_type = nullptr;
    bool ok = ctx->lookup_compiled_types (type.get_ty_ref (), &compiled_type);
    rust_assert (ok);
    translated = compiled_type;
  }

private:
  TyTyResolveCompile (Context *ctx) : ctx (ctx) {}

  Context *ctx;
  ::Btype *translated;
};

} // namespace Compile
} // namespace Rust

#endif // RUST_COMPILE_CONTEXT
