// Copyright (C) 2020-2021 Free Software Foundation, Inc.

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

#ifndef RUST_AST_RESOLVE_STMT_H
#define RUST_AST_RESOLVE_STMT_H

#include "rust-ast-resolve-base.h"
#include "rust-ast-full.h"
#include "rust-ast-resolve-type.h"
#include "rust-ast-resolve-pattern.h"
#include "rust-ast-resolve-expr.h"

namespace Rust {
namespace Resolver {

class ResolveStmt : public ResolverBase
{
  using Rust::Resolver::ResolverBase::visit;

public:
  static void go (AST::Stmt *stmt, NodeId parent,
		  const CanonicalPath &enum_prefix
		  = CanonicalPath::create_empty ())
  {
    ResolveStmt resolver (parent, enum_prefix);
    stmt->accept_vis (resolver);
  };

  void visit (AST::ExprStmtWithBlock &stmt) override
  {
    ResolveExpr::go (stmt.get_expr ().get (), stmt.get_node_id ());
  }

  void visit (AST::ExprStmtWithoutBlock &stmt) override
  {
    ResolveExpr::go (stmt.get_expr ().get (), stmt.get_node_id ());
  }

  void visit (AST::ConstantItem &constant) override
  {
    auto path = ResolveConstantItemToCanonicalPath::resolve (constant);
    resolver->get_name_scope ().insert (
      path, constant.get_node_id (), constant.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (constant.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });
    resolver->insert_new_definition (constant.get_node_id (),
				     Definition{constant.get_node_id (),
						constant.get_node_id ()});

    ResolveType::go (constant.get_type ().get (), constant.get_node_id ());
    ResolveExpr::go (constant.get_expr ().get (), constant.get_node_id ());

    // the mutability checker needs to verify for immutable decls the number
    // of assignments are <1. This marks an implicit assignment
    resolver->mark_decl_mutability (constant.get_node_id (), false);
    resolver->mark_assignment_to_decl (constant.get_node_id (),
				       constant.get_node_id ());
  }

  void visit (AST::LetStmt &stmt) override
  {
    if (stmt.has_init_expr ())
      {
	ResolveExpr::go (stmt.get_init_expr ().get (), stmt.get_node_id ());

	// mark the assignment
	resolver->mark_assignment_to_decl (
	  stmt.get_pattern ()->get_pattern_node_id (), stmt.get_node_id ());
      }

    PatternDeclaration::go (stmt.get_pattern ().get (), stmt.get_node_id ());
    if (stmt.has_type ())
      ResolveType::go (stmt.get_type ().get (), stmt.get_node_id ());
  }

  void visit (AST::TupleStruct &struct_decl) override
  {
    auto path = CanonicalPath::new_seg (struct_decl.get_node_id (),
					struct_decl.get_identifier ());
    resolver->get_type_scope ().insert (
      path, struct_decl.get_node_id (), struct_decl.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (struct_decl.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    NodeId scope_node_id = struct_decl.get_node_id ();
    resolver->get_type_scope ().push (scope_node_id);

    if (struct_decl.has_generics ())
      {
	for (auto &generic : struct_decl.get_generic_params ())
	  {
	    ResolveGenericParam::go (generic.get (),
				     struct_decl.get_node_id ());
	  }
      }

    for (AST::TupleField &field : struct_decl.get_fields ())
      ResolveType::go (field.get_field_type ().get (),
		       struct_decl.get_node_id ());

    resolver->get_type_scope ().pop ();
  }

  void visit (AST::Enum &enum_decl) override
  {
    auto enum_path = CanonicalPath::new_seg (enum_decl.get_node_id (),
					     enum_decl.get_identifier ());
    resolver->get_type_scope ().insert (
      enum_path, enum_decl.get_node_id (), enum_decl.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (enum_decl.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    NodeId scope_node_id = enum_decl.get_node_id ();
    resolver->get_type_scope ().push (scope_node_id);

    if (enum_decl.has_generics ())
      {
	for (auto &generic : enum_decl.get_generic_params ())
	  {
	    ResolveGenericParam::go (generic.get (), enum_decl.get_node_id ());
	  }
      }

    for (auto &variant : enum_decl.get_variants ())
      ResolveStmt::go (variant.get (), parent, enum_path);

    resolver->get_type_scope ().pop ();
  }

  void visit (AST::EnumItem &item) override
  {
    auto path = enum_prefix.append (
      CanonicalPath::new_seg (item.get_node_id (), item.get_identifier ()));
    resolver->get_type_scope ().insert (
      path, item.get_node_id (), item.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (item.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    // Done, no fields.
  }

  void visit (AST::EnumItemTuple &item) override
  {
    auto path = enum_prefix.append (
      CanonicalPath::new_seg (item.get_node_id (), item.get_identifier ()));
    resolver->get_type_scope ().insert (
      path, item.get_node_id (), item.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (item.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    for (auto &field : item.get_tuple_fields ())
      ResolveType::go (field.get_field_type ().get (), item.get_node_id ());
  }

  void visit (AST::EnumItemStruct &item) override
  {
    auto path = enum_prefix.append (
      CanonicalPath::new_seg (item.get_node_id (), item.get_identifier ()));
    resolver->get_type_scope ().insert (
      path, item.get_node_id (), item.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (item.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    for (auto &field : item.get_struct_fields ())
      ResolveType::go (field.get_field_type ().get (), item.get_node_id ());
  }

  void visit (AST::EnumItemDiscriminant &item) override
  {
    auto path = enum_prefix.append (
      CanonicalPath::new_seg (item.get_node_id (), item.get_identifier ()));
    resolver->get_type_scope ().insert (
      path, item.get_node_id (), item.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (item.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    // Done, no fields.
  }

  void visit (AST::StructStruct &struct_decl) override
  {
    auto path = CanonicalPath::new_seg (struct_decl.get_node_id (),
					struct_decl.get_identifier ());
    resolver->get_type_scope ().insert (
      path, struct_decl.get_node_id (), struct_decl.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (struct_decl.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    NodeId scope_node_id = struct_decl.get_node_id ();
    resolver->get_type_scope ().push (scope_node_id);

    if (struct_decl.has_generics ())
      {
	for (auto &generic : struct_decl.get_generic_params ())
	  {
	    ResolveGenericParam::go (generic.get (),
				     struct_decl.get_node_id ());
	  }
      }

    for (AST::StructField &field : struct_decl.get_fields ())
      ResolveType::go (field.get_field_type ().get (),
		       struct_decl.get_node_id ());

    resolver->get_type_scope ().pop ();
  }

  void visit (AST::Union &union_decl) override
  {
    auto path = CanonicalPath::new_seg (union_decl.get_node_id (),
					union_decl.get_identifier ());
    resolver->get_type_scope ().insert (
      path, union_decl.get_node_id (), union_decl.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (union_decl.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });

    NodeId scope_node_id = union_decl.get_node_id ();
    resolver->get_type_scope ().push (scope_node_id);

    if (union_decl.has_generics ())
      {
	for (auto &generic : union_decl.get_generic_params ())
	  {
	    ResolveGenericParam::go (generic.get (), union_decl.get_node_id ());
	  }
      }

    for (AST::StructField &field : union_decl.get_variants ())
      ResolveType::go (field.get_field_type ().get (),
		       union_decl.get_node_id ());

    resolver->get_type_scope ().pop ();
  }

  void visit (AST::Function &function) override
  {
    auto path = ResolveFunctionItemToCanonicalPath::resolve (function);
    resolver->get_name_scope ().insert (
      path, function.get_node_id (), function.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	RichLocation r (function.get_locus ());
	r.add_range (locus);
	rust_error_at (r, "redefined multiple times");
      });
    resolver->insert_new_definition (function.get_node_id (),
				     Definition{function.get_node_id (),
						function.get_node_id ()});

    NodeId scope_node_id = function.get_node_id ();
    resolver->get_name_scope ().push (scope_node_id);
    resolver->get_type_scope ().push (scope_node_id);
    resolver->get_label_scope ().push (scope_node_id);
    resolver->push_new_name_rib (resolver->get_name_scope ().peek ());
    resolver->push_new_type_rib (resolver->get_type_scope ().peek ());
    resolver->push_new_label_rib (resolver->get_type_scope ().peek ());

    if (function.has_generics ())
      {
	for (auto &generic : function.get_generic_params ())
	  ResolveGenericParam::go (generic.get (), function.get_node_id ());
      }

    if (function.has_return_type ())
      ResolveType::go (function.get_return_type ().get (),
		       function.get_node_id ());

    // we make a new scope so the names of parameters are resolved and shadowed
    // correctly
    for (auto &param : function.get_function_params ())
      {
	ResolveType::go (param.get_type ().get (), param.get_node_id ());
	PatternDeclaration::go (param.get_pattern ().get (),
				param.get_node_id ());

	// the mutability checker needs to verify for immutable decls the number
	// of assignments are <1. This marks an implicit assignment
	resolver->mark_assignment_to_decl (
	  param.get_pattern ()->get_pattern_node_id (), param.get_node_id ());
      }

    // resolve the function body
    ResolveExpr::go (function.get_definition ().get (),
		     function.get_node_id ());

    resolver->get_name_scope ().pop ();
    resolver->get_type_scope ().pop ();
    resolver->get_label_scope ().pop ();
  }

private:
  ResolveStmt (NodeId parent, const CanonicalPath &enum_prefix)
    : ResolverBase (parent), enum_prefix (enum_prefix)
  {}

  /* item declaration statements are not given a canonical path, but enum items
   * (variants) do inherit the enum path/identifier name.  */
  const CanonicalPath &enum_prefix;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_AST_RESOLVE_STMT_H
