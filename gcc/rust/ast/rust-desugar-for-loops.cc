// Copyright (C) 2024 Free Software Foundation, Inc.

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

#include "rust-desugar-for-loops.h"
#include "rust-ast-visitor.h"
#include "rust-ast.h"
#include "rust-path.h"
#include "rust-stmt.h"
#include "rust-diagnostics.h"
#include "rust-expr.h"
#include "rust-ast-builder.h"

namespace Rust {
namespace AST {

DesugarForLoops::DesugarForLoops () {}

void
DesugarForLoops::go (AST::Crate &crate)
{
  DefaultASTVisitor::visit (crate);
}

void
replace_for_loop (std::unique_ptr<Expr> &for_loop,
		  std::unique_ptr<Expr> &&expanded)
{
  for_loop = std::move (expanded);
}

MatchArm
DesugarForLoops::DesugarCtx::make_match_arm (std::unique_ptr<Pattern> &&path)
{
  auto patterns = std::vector<std::unique_ptr<Pattern>> ();
  patterns.emplace_back (std::move (path));

  return MatchArm (std::move (patterns), loc);
}

MatchCase
DesugarForLoops::DesugarCtx::make_break_arm ()
{
  auto arm
    = make_match_arm (builder.lang_item_path (LangItem::Kind::OPTION_NONE));

  auto break_expr = std::unique_ptr<Expr> (
    new BreakExpr (Lifetime::error (), nullptr, {}, loc));

  return MatchCase (arm, std::move (break_expr));
}

MatchCase
DesugarForLoops::DesugarCtx::make_continue_arm ()
{
  // Missing the actual `val` binding
  auto arm
    = make_match_arm (builder.lang_item_path (LangItem::Kind::OPTION_SOME));

  auto next = builder.identifier ("__next");
  auto val = builder.identifier ("val");

  auto assignment = std::unique_ptr<Expr> (
    new AssignmentExpr (std::move (next), std::move (val), {}, loc));

  return MatchCase (arm, std::move (assignment));
}

std::unique_ptr<Stmt>
DesugarForLoops::DesugarCtx::statementify (std::unique_ptr<Expr> &&expr)
{
  return std::unique_ptr<Stmt> (new ExprStmt (std::move (expr), loc, true));
}

std::unique_ptr<Expr>
DesugarForLoops::desugar (AST::ForLoopExpr &expr)
{
  auto ctx = DesugarCtx (expr.get_locus ());

  // IntoIterator::into_iter(<head>)
  auto into_iter = ctx.builder.call (ctx.builder.lang_item_path (
				       LangItem::Kind::INTOITER_INTOITER),
				     expr.get_iterator_expr ().clone_expr ());

  // Iterator::next(iter)
  auto next_call = ctx.builder.call (ctx.builder.lang_item_path (
				       LangItem::Kind::ITERATOR_NEXT),
				     ctx.builder.identifier ("iter"));

  // None => break,
  auto break_arm = ctx.make_break_arm ();
  // Some(val) => { __next = val; },
  auto continue_arm = ctx.make_continue_arm ();

  // match <next_call> {
  //     <continue_arm>
  //     <break_arm>
  // }
  auto match_next
    = ctx.builder.match (std::move (next_call),
			 {std::move (continue_arm), std::move (break_arm)});

  // let mut __next;
  auto let_next = ctx.builder.let (ctx.builder.identifier_pattern ("__next"));
  // let <pattern> = __next;
  auto let_pat = ctx.builder.let (expr.get_pattern ().clone_pattern (), nullptr,
				  ctx.builder.identifier ("__next"));

  auto loop_stmts = std::vector<std::unique_ptr<Stmt>> ();
  loop_stmts.emplace_back (std::move (let_next));
  loop_stmts.emplace_back (ctx.statementify (std::move (match_next)));
  loop_stmts.emplace_back (std::move (let_pat));
  loop_stmts.emplace_back (ctx.statementify (expr.get_loop_block().clone_expr()));

  // loop {
  //     <let_next>;
  //     <match_next>;
  //     <let_pat>;
  //
  //     <body>;
  // }
  ctx.builder.loop(std::move(loop_stmts));

  return into_iter;
}

void
DesugarForLoops::visit (AST::BlockExpr &block)
{
  for (auto &stmt : block.get_statements ())
    {
      if (stmt->get_stmt_kind () == AST::Stmt::Kind::Expr)
	{
	  auto &expr = static_cast<AST::ExprStmt &> (*stmt);

	  if (expr.get_expr ().get_expr_kind () == AST::Expr::Kind::Loop)
	    {
	      auto &loop = static_cast<AST::BaseLoopExpr &> (expr.get_expr ());

	      if (loop.get_loop_kind () == AST::BaseLoopExpr::Kind::For)
		{
		  auto &for_loop = static_cast<AST::ForLoopExpr &> (loop);

		  auto desugared = desugar (for_loop);

		  replace_for_loop (expr.get_expr_ptr (),
				    std::move (desugared));
		}
	    }
	}
    }
}

} // namespace AST
} // namespace Rust
