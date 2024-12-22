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
#include "rust-stmt.h"
#include "rust-diagnostics.h"
#include "rust-expr.h"

namespace Rust {
namespace AST {

DesugarForLoops::DesugarForLoops () {}

void
DesugarForLoops::go (AST::Crate &crate)
{
  DefaultASTVisitor::visit (crate);
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
		rust_error_at (loop.get_locus (), "found a for-loop - yuck");
	    }
	}
    }
}

} // namespace AST
} // namespace Rust
