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
#include "rust-hir-const-fold-ctx.h"
#include "rust-mangle.h"

namespace Rust {
namespace Compile {

struct fncontext
{
  tree fndecl;
  ::Bvariable *ret_addr;
};

class Context
{
public:
  Context (::Backend *backend)
    : backend (backend), resolver (Resolver::Resolver::get ()),
      tyctx (Resolver::TypeCheckContext::get ()),
      mappings (Analysis::Mappings::get ()),
      const_ctx (ConstFold::Context::get ()), mangler (Mangler ())
  {
    // insert the builtins
    auto builtins = resolver->get_builtin_types ();
    for (auto it = builtins.begin (); it != builtins.end (); it++)
      {
	HirId ref;
	bool ok = tyctx->lookup_type_by_node_id ((*it)->get_node_id (), &ref);
	rust_assert (ok);

	TyTy::BaseType *lookup;
	ok = tyctx->lookup_type (ref, &lookup);
	rust_assert (ok);

	tree compiled = TyTyCompile::compile (backend, lookup);
	compiled_type_map.insert (std::pair<HirId, tree> (ref, compiled));
	builtin_range.insert (ref);
      }
  }

  bool lookup_compiled_types (HirId id, tree *type,
			      const TyTy::BaseType *ref = nullptr)
  {
    if (ref != nullptr)
      {
	for (auto it = mono.begin (); it != mono.end (); it++)
	  {
	    std::pair<HirId, tree> &val = it->second;
	    const TyTy::BaseType *r = it->first;

	    if (ref->is_equal (*r))
	      {
		*type = val.second;

		return true;
	      }
	  }
	return false;
      }

    auto it = compiled_type_map.find (id);
    if (it == compiled_type_map.end ())
      return false;

    *type = it->second;
    return true;
  }

  void insert_compiled_type (HirId id, tree type,
			     const TyTy::BaseType *ref = nullptr)
  {
    rust_assert (builtin_range.find (id) == builtin_range.end ());
    compiled_type_map.insert (std::pair<HirId, tree> (id, type));
    if (ref != nullptr)
      {
	std::pair<HirId, tree> elem (id, type);
	mono[ref] = std::move (elem);
      }
  }

  ::Backend *get_backend () { return backend; }
  Resolver::Resolver *get_resolver () { return resolver; }
  Resolver::TypeCheckContext *get_tyctx () { return tyctx; }
  Analysis::Mappings *get_mappings () { return mappings; }
  ConstFold::Context *get_const_ctx () { return const_ctx; }

  void push_block (tree scope)
  {
    scope_stack.push_back (scope);
    statements.push_back ({});
  }

  tree pop_block ()
  {
    auto block = scope_stack.back ();
    scope_stack.pop_back ();

    auto stmts = statements.back ();
    statements.pop_back ();

    backend->block_add_statements (block, stmts);

    return block;
  }

  tree peek_enclosing_scope ()
  {
    if (scope_stack.size () == 0)
      return nullptr;

    return scope_stack.back ();
  }

  void add_statement_to_enclosing_scope (tree stmt)
  {
    statements.at (statements.size () - 2).push_back (stmt);
  }

  void add_statement (tree stmt) { statements.back ().push_back (stmt); }

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

  void insert_function_decl (const TyTy::FnType *ref, tree fn)
  {
    auto id = ref->get_ty_ref ();
    auto dId = ref->get_id ();

    rust_assert (compiled_fn_map.find (id) == compiled_fn_map.end ());
    compiled_fn_map[id] = fn;

    auto it = mono_fns.find (dId);
    if (it == mono_fns.end ())
      mono_fns[dId] = {};

    mono_fns[dId].push_back ({ref, fn});
  }

  bool lookup_function_decl (HirId id, tree *fn, DefId dId = UNKNOWN_DEFID,
			     const TyTy::BaseType *ref = nullptr)
  {
    // for for any monomorphized fns
    if (ref != nullptr)
      {
	rust_assert (dId != UNKNOWN_DEFID);

	auto it = mono_fns.find (dId);
	if (it == mono_fns.end ())
	  return false;

	for (auto &e : mono_fns[dId])
	  {
	    const TyTy::BaseType *r = e.first;
	    tree f = e.second;
	    if (ref->is_equal (*r))
	      {
		*fn = f;
		return true;
	      }
	  }
	return false;
      }

    auto it = compiled_fn_map.find (id);
    if (it == compiled_fn_map.end ())
      return false;

    *fn = it->second;
    return true;
  }

  void insert_const_decl (HirId id, tree expr) { compiled_consts[id] = expr; }

  bool lookup_const_decl (HirId id, tree *expr)
  {
    auto it = compiled_consts.find (id);
    if (it == compiled_consts.end ())
      return false;

    *expr = it->second;
    return true;
  }

  void insert_label_decl (HirId id, tree label) { compiled_labels[id] = label; }

  bool lookup_label_decl (HirId id, tree *label)
  {
    auto it = compiled_labels.find (id);
    if (it == compiled_labels.end ())
      return false;

    *label = it->second;
    return true;
  }

  void push_fn (tree fn, ::Bvariable *ret_addr)
  {
    fn_stack.push_back (fncontext{fn, ret_addr});
  }
  void pop_fn () { fn_stack.pop_back (); }
  fncontext peek_fn () { return fn_stack.back (); }

  void push_type (tree t) { type_decls.push_back (t); }
  void push_var (::Bvariable *v) { var_decls.push_back (v); }
  void push_const (tree c) { const_decls.push_back (c); }
  void push_function (tree f) { func_decls.push_back (f); }

  void write_to_backend ()
  {
    backend->write_global_definitions (type_decls, const_decls, func_decls,
				       var_decls);
  }

  bool function_completed (tree fn)
  {
    for (auto it = func_decls.begin (); it != func_decls.end (); it++)
      {
	tree i = (*it);
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

  void push_loop_begin_label (tree label)
  {
    loop_begin_labels.push_back (label);
  }

  tree peek_loop_begin_label () { return loop_begin_labels.back (); }

  tree pop_loop_begin_label ()
  {
    tree pop = loop_begin_labels.back ();
    loop_begin_labels.pop_back ();
    return pop;
  }

  std::string mangle_item (const TyTy::BaseType *ty,
			   const Resolver::CanonicalPath &path) const
  {
    return mangler.mangle_item (ty, path, mappings->get_current_crate_name ());
  }

  std::string mangle_impl_item (const TyTy::BaseType *self,
				const TyTy::BaseType *ty,
				const std::string &name) const
  {
    return mangler.mangle_impl_item (self, ty, name,
				     mappings->get_current_crate_name ());
  }

private:
  ::Backend *backend;
  Resolver::Resolver *resolver;
  Resolver::TypeCheckContext *tyctx;
  Analysis::Mappings *mappings;
  ConstFold::Context *const_ctx;
  std::set<HirId> builtin_range;
  Mangler mangler;

  // state
  std::vector<fncontext> fn_stack;
  std::map<HirId, ::Bvariable *> compiled_var_decls;
  std::map<HirId, tree> compiled_type_map;
  std::map<HirId, tree> compiled_fn_map;
  std::map<HirId, tree> compiled_consts;
  std::map<HirId, tree> compiled_labels;
  std::vector<::std::vector<tree>> statements;
  std::vector<tree> scope_stack;
  std::vector<::Bvariable *> loop_value_stack;
  std::vector<tree> loop_begin_labels;
  std::map<const TyTy::BaseType *, std::pair<HirId, tree>> mono;
  std::map<DefId, std::vector<std::pair<const TyTy::BaseType *, tree>>>
    mono_fns;

  // To GCC middle-end
  std::vector<tree> type_decls;
  std::vector<::Bvariable *> var_decls;
  std::vector<tree> const_decls;
  std::vector<tree> func_decls;
};

} // namespace Compile
} // namespace Rust

#endif // RUST_COMPILE_CONTEXT
