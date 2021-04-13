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

#ifndef RUST_AST_RESOLVE_TYPE_H
#define RUST_AST_RESOLVE_TYPE_H

#include "rust-ast-resolve-base.h"
#include "rust-ast-full.h"

namespace Rust {
namespace Resolver {

class ResolveConstantItemToCanonicalPath
{
public:
  static CanonicalPath resolve (AST::ConstantItem &constant)
  {
    return CanonicalPath (constant.get_identifier ());
  }
};

class ResolveFunctionItemToCanonicalPath
{
public:
  static CanonicalPath resolve (AST::Function &function)
  {
    return CanonicalPath (function.get_function_name ());
  }
};

class ResolveMethodItemToCanonicalPath
{
public:
  static CanonicalPath resolve (AST::Method &method)
  {
    return CanonicalPath (method.get_method_name ());
  }
};

class ResolveTypeToCanonicalPath : public ResolverBase
{
  using Rust::Resolver::ResolverBase::visit;

public:
  static CanonicalPath resolve (AST::Type &type,
				bool include_generic_args = true,
				bool type_resolve_generic_args = true)
  {
    ResolveTypeToCanonicalPath resolver (include_generic_args,
					 type_resolve_generic_args);
    type.accept_vis (resolver);
    return resolver.result;
  }

  void visit (AST::TypePath &path) override
  {
    for (auto &seg : path.get_segments ())
      {
	seg->accept_vis (*this);
	if (failure_flag)
	  return;
      }
  }

  void visit (AST::TypePathSegmentGeneric &seg) override;

  void visit (AST::TypePathSegment &seg) override;

  static CanonicalPath canonicalize_generic_args (AST::GenericArgs &args);

  static bool type_resolve_generic_args (AST::GenericArgs &args);

private:
  ResolveTypeToCanonicalPath (bool include_generic_args,
			      bool type_resolve_generic_args)
    : ResolverBase (UNKNOWN_NODEID), result (CanonicalPath::create_empty ()),
      include_generic_args_flag (include_generic_args),
      type_resolve_generic_args_flag (type_resolve_generic_args),
      failure_flag (false)
  {}

  CanonicalPath result;
  bool include_generic_args_flag;
  bool type_resolve_generic_args_flag;
  bool failure_flag;
};

class ResolvePathSegmentToCanonicalPath
{
public:
  static CanonicalPath resolve (AST::PathExprSegment &seg)
  {
    CanonicalPath path = CanonicalPath (seg.get_ident_segment ().as_string ());
    if (seg.has_generic_args ())
      {
	bool ok = ResolveTypeToCanonicalPath::type_resolve_generic_args (
	  seg.get_generic_args ());
	if (!ok)
	  {
	    rust_error_at (seg.get_locus (),
			   "failed to resolve all generic args");
	    return CanonicalPath::create_empty ();
	  }

	path
	  = path.append (ResolveTypeToCanonicalPath::canonicalize_generic_args (
	    seg.get_generic_args ()));
      }
    return path;
  }
};

// FIXME: as part of imports and visibility we need to be able to keep a context
// for handling PathInExpressions segments as they can be local to a particular
// lexical scope requiring a context to be maintained for resolution
class ResolveRelativeTypePath
{
public:
  static NodeId go (AST::TypePath &path, NodeId parent,
		    const CanonicalPath &prefix,
		    bool canonicalize_type_with_generics)
  {
    CanonicalPath canonical_path
      = ResolveTypeToCanonicalPath::resolve (path,
					     canonicalize_type_with_generics,
					     true);
    if (canonical_path.is_error ())
      {
	rust_error_at (path.get_locus (), "Failed to resolve canonical path");
	return UNKNOWN_NODEID;
      }

    CanonicalPath lookup = canonical_path;
    if (!prefix.is_error ())
      lookup = prefix.append (canonical_path);

    auto resolver = Resolver::get ();
    NodeId resolved_node = UNKNOWN_NODEID;
    if (!resolver->get_type_scope ().lookup (canonical_path, &resolved_node))
      {
	rust_error_at (path.get_locus_slow (), "failed to resolve TypePath: %s",
		       canonical_path.get ().c_str ());
	return UNKNOWN_NODEID;
      }

    return resolved_node;
  }
};

class ResolveType : public ResolverBase
{
  using Rust::Resolver::ResolverBase::visit;

public:
  static NodeId go (AST::Type *type, NodeId parent,
		    bool canonicalize_type_with_generics = false)
  {
    ResolveType resolver (parent, canonicalize_type_with_generics);
    type->accept_vis (resolver);
    if (!resolver.ok)
      rust_error_at (type->get_locus_slow (), "unresolved type");

    return resolver.resolved_node;
  };

  void visit (AST::BareFunctionType &fntype) override
  {
    ok = true;
    for (auto &param : fntype.get_function_params ())
      ResolveType::go (param.get_type ().get (), fntype.get_node_id ());

    if (fntype.has_return_type ())
      ResolveType::go (fntype.get_return_type ().get (), fntype.get_node_id ());
  }

  void visit (AST::TupleType &tuple) override
  {
    ok = true;
    if (tuple.is_unit_type ())
      {
	resolved_node = resolver->get_unit_type_node_id ();
	return;
      }

    for (auto &elem : tuple.get_elems ())
      ResolveType::go (elem.get (), tuple.get_node_id ());
  }

  void visit (AST::TypePath &path) override
  {
    resolved_node
      = ResolveRelativeTypePath::go (path, parent,
				     CanonicalPath::create_empty (),
				     canonicalize_type_with_generics);
    ok = resolved_node != UNKNOWN_NODEID;
    if (ok)
      {
	resolver->insert_resolved_type (path.get_node_id (), resolved_node);
	resolver->insert_new_definition (path.get_node_id (),
					 Definition{path.get_node_id (),
						    parent});
      }
  }

  void visit (AST::ArrayType &type) override
  {
    type.get_elem_type ()->accept_vis (*this);
  }

  void visit (AST::ReferenceType &type) override
  {
    type.get_type_referenced ()->accept_vis (*this);
  }

  void visit (AST::InferredType &type) override { ok = true; }

private:
  ResolveType (NodeId parent, bool canonicalize_type_with_generics)
    : ResolverBase (parent),
      canonicalize_type_with_generics (canonicalize_type_with_generics),
      ok (false)
  {}

  bool canonicalize_type_with_generics;
  bool ok;
};

class ResolveGenericParam : public ResolverBase
{
  using Rust::Resolver::ResolverBase::visit;

public:
  static NodeId go (AST::GenericParam *param, NodeId parent)
  {
    ResolveGenericParam resolver (parent);
    param->accept_vis (resolver);
    if (!resolver.ok)
      rust_error_at (param->get_locus_slow (), "unresolved generic parameter");

    return resolver.resolved_node;
  };

  void visit (AST::TypeParam &param) override
  {
    ok = true;

    // for now lets focus on handling the basics: like struct<T> { a:T, ....}
    resolver->get_type_scope ().insert (
      CanonicalPath (param.get_type_representation ()), param.get_node_id (),
      param.get_locus (), false,
      [&] (const CanonicalPath &, NodeId, Location locus) -> void {
	rust_error_at (param.get_locus (),
		       "generic param redefined multiple times");
	rust_error_at (locus, "was defined here");
      });
  }

private:
  ResolveGenericParam (NodeId parent)
    : ResolverBase (parent),

      ok (false)
  {}

  bool ok;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_AST_RESOLVE_TYPE_H
