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

#ifndef RUST_COMPILE_ITEM
#define RUST_COMPILE_ITEM

#include "rust-compile-base.h"
#include "rust-compile-tyty.h"
#include "rust-compile-implitem.h"
#include "rust-compile-var-decl.h"
#include "rust-compile-stmt.h"
#include "rust-compile-expr.h"
#include "rust-compile-fnparam.h"

namespace Rust {
namespace Compile {

class CompileItem : public HIRCompileBase
{
public:
  static void compile (HIR::Item *item, Context *ctx, bool compile_fns = true)
  {
    CompileItem compiler (ctx, compile_fns);
    item->accept_vis (compiler);
  }

  void visit (HIR::StaticItem &var)
  {
    TyTy::BaseType *resolved_type = nullptr;
    bool ok = ctx->get_tyctx ()->lookup_type (var.get_mappings ().get_hirid (),
					      &resolved_type);
    rust_assert (ok);

    Btype *type = TyTyResolveCompile::compile (ctx, resolved_type);
    Bexpression *value = CompileExpr::Compile (var.get_expr (), ctx);

    std::string name = var.get_identifier ();
    // FIXME need name mangling
    std::string asm_name = "__" + var.get_identifier ();

    bool is_external = false;
    bool is_hidden = false;
    bool in_unique_section = true;

    Bvariable *static_global
      = ctx->get_backend ()->global_variable (name, asm_name, type, is_external,
					      is_hidden, in_unique_section,
					      var.get_locus ());
    ctx->get_backend ()->global_variable_set_init (static_global, value);

    ctx->insert_var_decl (var.get_mappings ().get_hirid (), static_global);
    ctx->push_var (static_global);
  }

  void visit (HIR::ConstantItem &constant)
  {
    TyTy::BaseType *resolved_type = nullptr;
    bool ok
      = ctx->get_tyctx ()->lookup_type (constant.get_mappings ().get_hirid (),
					&resolved_type);
    rust_assert (ok);

    ::Btype *type = TyTyResolveCompile::compile (ctx, resolved_type);
    Bexpression *value = CompileExpr::Compile (constant.get_expr (), ctx);

    Bexpression *const_expr = ctx->get_backend ()->named_constant_expression (
      type, constant.get_identifier (), value, constant.get_locus ());

    ctx->push_const (const_expr);
    ctx->insert_const_decl (constant.get_mappings ().get_hirid (), const_expr);
  }

  void visit (HIR::Function &function)
  {
    if (!compile_fns)
      return;

    // items can be forward compiled which means we may not need to invoke this
    // code
    Bfunction *lookup = nullptr;
    if (ctx->lookup_function_decl (function.get_mappings ().get_hirid (),
				   &lookup))
      {
	// has this been added to the list then it must be finished
	if (ctx->function_completed (lookup))
	  return;
      }

    TyTy::BaseType *fntype_tyty;
    if (!ctx->get_tyctx ()->lookup_type (function.get_mappings ().get_hirid (),
					 &fntype_tyty))
      {
	rust_fatal_error (function.locus, "failed to lookup function type");
	return;
      }

    if (fntype_tyty->get_kind () != TyTy::TypeKind::FNDEF)
      {
	rust_error_at (function.get_locus (), "invalid TyTy for function item");
	return;
      }

    TyTy::FnType *fntype = (TyTy::FnType *) fntype_tyty;
    // convert to the actual function type
    ::Btype *compiled_fn_type = TyTyResolveCompile::compile (ctx, fntype);

    unsigned int flags = 0;
    bool is_main_fn = function.function_name.compare ("main") == 0;

    // if its the main fn or pub visibility mark its as DECL_PUBLIC
    // please see https://github.com/Rust-GCC/gccrs/pull/137
    if (is_main_fn || function.has_visibility ())
      flags |= Backend::function_is_visible;

    std::string asm_name = function.function_name;
    if (!is_main_fn)
      {
	// FIXME need name mangling
	asm_name = "__" + function.function_name;
      }

    Bfunction *fndecl
      = ctx->get_backend ()->function (compiled_fn_type, function.function_name,
				       asm_name, flags, function.get_locus ());
    ctx->insert_function_decl (function.get_mappings ().get_hirid (), fndecl);

    // setup the params

    TyTy::BaseType *tyret = fntype->get_return_type ();
    std::vector<Bvariable *> param_vars;

    size_t i = 0;
    for (auto &it : fntype->get_params ())
      {
	HIR::FunctionParam &referenced_param = function.function_params.at (i);
	auto param_tyty = it.second;
	auto compiled_param_type
	  = TyTyResolveCompile::compile (ctx, param_tyty);

	Location param_locus
	  = ctx->get_mappings ()->lookup_location (param_tyty->get_ref ());
	Bvariable *compiled_param_var
	  = CompileFnParam::compile (ctx, fndecl, &referenced_param,
				     compiled_param_type, param_locus);
	if (compiled_param_var == nullptr)
	  {
	    rust_error_at (param_locus, "failed to compile parameter variable");
	    return;
	  }

	param_vars.push_back (compiled_param_var);

	ctx->insert_var_decl (referenced_param.get_mappings ().get_hirid (),
			      compiled_param_var);
	i++;
      }

    if (!ctx->get_backend ()->function_set_parameters (fndecl, param_vars))
      {
	rust_fatal_error (function.get_locus (),
			  "failed to setup parameter variables");
	return;
      }

    // lookup locals
    auto block_expr = function.function_body.get ();
    auto body_mappings = block_expr->get_mappings ();

    Resolver::Rib *rib = nullptr;
    if (!ctx->get_resolver ()->find_name_rib (body_mappings.get_nodeid (),
					      &rib))
      {
	rust_fatal_error (function.get_locus (),
			  "failed to setup locals per block");
	return;
      }

    std::vector<Bvariable *> locals;
    rib->iterate_decls ([&] (NodeId n, Location) mutable -> bool {
      Resolver::Definition d;
      bool ok = ctx->get_resolver ()->lookup_definition (n, &d);
      rust_assert (ok);

      HIR::Stmt *decl = nullptr;
      ok = ctx->get_mappings ()->resolve_nodeid_to_stmt (d.parent, &decl);
      rust_assert (ok);

      Bvariable *compiled = CompileVarDecl::compile (fndecl, decl, ctx);
      locals.push_back (compiled);

      return true;
    });

    bool toplevel_item
      = function.get_mappings ().get_local_defid () != UNKNOWN_LOCAL_DEFID;
    Bblock *enclosing_scope
      = toplevel_item ? NULL : ctx->peek_enclosing_scope ();

    HIR::BlockExpr *function_body = function.function_body.get ();
    Location start_location = function_body->get_locus ();
    Location end_location = function_body->get_closing_locus ();

    Bblock *code_block
      = ctx->get_backend ()->block (fndecl, enclosing_scope, locals,
				    start_location, end_location);
    ctx->push_block (code_block);

    Bvariable *return_address = nullptr;
    if (function.has_function_return_type ())
      {
	Btype *return_type = TyTyResolveCompile::compile (ctx, tyret);

	bool address_is_taken = false;
	Bstatement *ret_var_stmt = nullptr;

	return_address = ctx->get_backend ()->temporary_variable (
	  fndecl, code_block, return_type, NULL, address_is_taken,
	  function.get_locus (), &ret_var_stmt);

	ctx->add_statement (ret_var_stmt);
      }

    ctx->push_fn (fndecl, return_address);

    compile_function_body (fndecl, function.function_body,
			   function.has_function_return_type ());

    ctx->pop_block ();
    auto body = ctx->get_backend ()->block_statement (code_block);
    if (!ctx->get_backend ()->function_set_body (fndecl, body))
      {
	rust_error_at (function.get_locus (), "failed to set body to function");
	return;
      }

    ctx->pop_fn ();
    ctx->push_function (fndecl);
  }

  void visit (HIR::InherentImpl &impl_block)
  {
    TyTy::BaseType *self_lookup = nullptr;
    if (!ctx->get_tyctx ()->lookup_type (
	  impl_block.get_type ()->get_mappings ().get_hirid (), &self_lookup))
      {
	rust_error_at (impl_block.get_locus (),
		       "failed to resolve type of impl");
	return;
      }

    for (auto &impl_item : impl_block.get_impl_items ())
      CompileInherentImplItem::Compile (self_lookup, impl_item.get (), ctx,
					compile_fns);
  }

private:
  CompileItem (Context *ctx, bool compile_fns)
    : HIRCompileBase (ctx), compile_fns (compile_fns)
  {}

  bool compile_fns;
};

} // namespace Compile
} // namespace Rust

#endif // RUST_COMPILE_ITEM
