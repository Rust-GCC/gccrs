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

#ifndef RUST_AST_RESOLVE_EXPR_H
#define RUST_AST_RESOLVE_EXPR_H

#include "rust-ast-resolve-base.h"
#include "rust-ast-full.h"

namespace Rust {
namespace Resolver {

class ResolveExpr : public ResolverBase
{
public:
  static void go (AST::Expr *expr, NodeId parent)
  {
    ResolveExpr resolver (parent);
    expr->accept_vis (resolver);
  };

  ~ResolveExpr () {}

  void visit (AST::PathInExpression &expr)
  {
    if (!resolver->get_name_scope ().lookup (expr.as_string (), &resolved_node))
      {
	rust_error_at (expr.get_locus (), "unknown path %s",
		       expr.as_string ().c_str ());
	return;
      }
    else
      {
	resolver->insert_resolved_name (expr.get_node_id (), resolved_node);
	resolver->insert_new_definition (expr.get_node_id (),
					 Definition{expr.get_node_id (),
						    parent});
      }
  }

  void visit (AST::ReturnExpr &expr)
  {
    if (expr.has_returned_expr ())
      ResolveExpr::go (expr.get_returned_expr ().get (), expr.get_node_id ());
  }

  void visit (AST::CallExpr &expr)
  {
    ResolveExpr::go (expr.get_function_expr ().get (), expr.get_node_id ());
    expr.iterate_params ([&] (AST::Expr *p) mutable -> bool {
      ResolveExpr::go (p, expr.get_node_id ());
      return true;
    });
    // resolver->insert_resolved_name(NodeId refId,NodeId defId)
  }

  void visit (AST::AssignmentExpr &expr)
  {
    ResolveExpr::go (expr.get_left_expr ().get (), expr.get_node_id ());
    ResolveExpr::go (expr.get_right_expr ().get (), expr.get_node_id ());
  }

  void visit (AST::IdentifierExpr &expr)
  {
    if (!resolver->get_name_scope ().lookup (expr.as_string (), &resolved_node))
      {
	rust_error_at (expr.get_locus (), "failed to find name: %s",
		       expr.as_string ().c_str ());
	return;
      }

    resolver->insert_resolved_name (expr.get_node_id (), resolved_node);
    resolver->insert_new_definition (expr.get_node_id (),
				     Definition{expr.get_node_id (), parent});
  }

  void visit (AST::ArithmeticOrLogicalExpr &expr)
  {
    ResolveExpr::go (expr.get_left_expr ().get (), expr.get_node_id ());
    ResolveExpr::go (expr.get_right_expr ().get (), expr.get_node_id ());
  }

private:
  ResolveExpr (NodeId parent) : ResolverBase (parent) {}
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_AST_RESOLVE_EXPR_H
