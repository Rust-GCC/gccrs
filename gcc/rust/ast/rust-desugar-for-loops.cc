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

std::unique_ptr<Expr>
desugar (AST::ForLoopExpr &expr)
{
  auto builder = AST::Builder (expr.get_locus ());

  auto into_iter
    = builder.call (builder.lang_item_path (LangItem::Kind::INTOITER_INTOITER),
		    expr.get_iterator_expr ().clone_expr ());

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
