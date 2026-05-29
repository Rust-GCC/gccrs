// Copyright (C) 2020-2026 Free Software Foundation, Inc.

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

#include "rust-compile-block.h"
#include "rust-compile-stmt.h"
#include "rust-compile-expr.h"
#include "rust-compile-implitem.h"
#include "rust-hir-expr.h"
#include "rust-hir-path-probe.h"
#include "rust-lang-item.h"

namespace Rust {
namespace Compile {

static tree
compile_drop_call (Context *ctx, Bvariable *var, TyTy::BaseType *ty,
		   location_t locus)
{
  auto drop_lang = ctx->get_mappings ().lookup_lang_item (LangItem::Kind::DROP);
  if (!drop_lang.has_value ())
    return NULL_TREE;

  Resolver::TraitReference *drop_ref = nullptr;
  bool ok
    = ctx->get_tyctx ()->lookup_trait_reference (drop_lang.value (), &drop_ref);
  if (!ok)
    return NULL_TREE;

  HIR::PathIdentSegment segment ("drop");
  auto candidates
    = Resolver::PathProbeImplTrait::Probe (ty->get_root (), segment, drop_ref);

  for (auto &candidate : candidates)
    {
      if (!candidate.is_impl_candidate ()
	  || candidate.ty->get_kind () != TyTy::TypeKind::FNDEF)
	continue;

      auto *fn_type = static_cast<TyTy::FnType *> (candidate.ty);
      tree fn_addr
	= CompileInherentImplItem::Compile (candidate.item.impl.impl_item, ctx,
					    fn_type, locus);

      tree var_expr = Backend::var_expression (var, locus);
      tree var_addr = HIRCompileBase::address_expression (var_expr, locus);

      return Backend::call_expression (fn_addr, {var_addr}, nullptr, locus);
    }
  return NULL_TREE;
}

CompileBlock::CompileBlock (Context *ctx, Bvariable *result)
  : HIRCompileBase (ctx), translated (nullptr), result (result)
{}

tree
CompileBlock::compile (HIR::BlockExpr &expr, Context *ctx, Bvariable *result)
{
  CompileBlock compiler (ctx, result);
  compiler.visit (expr);
  return compiler.translated;
}

void
CompileBlock::visit (HIR::BlockExpr &expr)
{
  fncontext fnctx = ctx->peek_fn ();
  tree fndecl = fnctx.fndecl;
  location_t start_location = expr.get_locus ();
  location_t end_location = expr.get_end_locus ();

  tree enclosing_scope = ctx->peek_enclosing_scope ();
  tree new_block = Backend::block (fndecl, enclosing_scope, {} /*locals*/,
				   start_location, end_location);
  ctx->push_block (new_block);

  for (auto &s : expr.get_statements ())
    {
      auto compiled_expr = CompileStmt::Compile (s.get (), ctx);
      if (compiled_expr != nullptr)
	{
	  tree s = convert_to_void (compiled_expr, ICV_STATEMENT);
	  ctx->add_statement (s);
	}
    }

  if (expr.has_expr ())
    {
      tree compiled_expr = CompileExpr::Compile (expr.get_final_expr (), ctx);
      if (result != nullptr)
	{
	  location_t locus = expr.get_final_expr ().get_locus ();
	  tree result_reference = Backend::var_expression (result, locus);

	  tree assignment
	    = Backend::assignment_statement (result_reference, compiled_expr,
					     expr.get_locus ());
	  ctx->add_statement (assignment);
	}
    }
  else if (result != nullptr)
    {
      location_t locus = expr.get_locus ();
      tree compiled_expr = unit_expression (expr.get_locus ());
      tree result_reference = Backend::var_expression (result, locus);

      tree assignment
	= Backend::assignment_statement (result_reference, compiled_expr,
					 expr.get_locus ());
      ctx->add_statement (assignment);
    }

  auto &drop_candidates = ctx->peek_block_drop_candidates ();

  for (auto it = drop_candidates.rbegin (); it != drop_candidates.rend (); ++it)
    {
      TyTy::BaseType *ty = nullptr;
      Bvariable *var = nullptr;

      bool ok = ctx->get_tyctx ()->lookup_type (it->hirid, &ty);
      rust_assert (ok);

      ok = ctx->lookup_var_decl (it->hirid, &var);
      rust_assert (ok);

      tree drop_call = compile_drop_call (ctx, var, ty, it->locus);
      if (drop_call != NULL_TREE)
	ctx->add_statement (convert_to_void (drop_call, ICV_STATEMENT));
    }

  ctx->pop_block ();
  translated = new_block;
}

void
CompileConditionalBlocks::visit (HIR::IfExpr &expr)
{
  fncontext fnctx = ctx->peek_fn ();
  tree fndecl = fnctx.fndecl;
  tree condition_expr = CompileExpr::Compile (expr.get_if_condition (), ctx);
  tree then_block = CompileBlock::compile (expr.get_if_block (), ctx, result);

  translated = Backend::if_statement (fndecl, condition_expr, then_block, NULL,
				      expr.get_locus ());
}

void
CompileConditionalBlocks::visit (HIR::IfExprConseqElse &expr)
{
  fncontext fnctx = ctx->peek_fn ();
  tree fndecl = fnctx.fndecl;
  tree condition_expr = CompileExpr::Compile (expr.get_if_condition (), ctx);
  tree then_block = CompileBlock::compile (expr.get_if_block (), ctx, result);

  // else block
  std::vector<Bvariable *> locals;
  location_t start_location = expr.get_else_block ().get_locus ();
  location_t end_location = expr.get_else_block ().get_locus (); // FIXME
  tree enclosing_scope = ctx->peek_enclosing_scope ();
  tree else_block = Backend::block (fndecl, enclosing_scope, locals,
				    start_location, end_location);
  ctx->push_block (else_block);

  tree else_stmt_decl
    = CompileExprWithBlock::compile (&expr.get_else_block (), ctx, result);

  ctx->add_statement (else_stmt_decl);

  ctx->pop_block ();

  translated = Backend::if_statement (fndecl, condition_expr, then_block,
				      else_block, expr.get_locus ());
}

} // namespace Compile
} // namespace Rust
