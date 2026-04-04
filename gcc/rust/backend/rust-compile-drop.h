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

#ifndef RUST_COMPILE_DROP_H
#define RUST_COMPILE_DROP_H

#define check_node(node) (node != NULL_TREE && !error_operand_p (node))
#define assert_node(node, message) assert (check_node (node) && message)
#define die(message) assert (false && message)
#define todo_die(message) assert (false && message)

#include "rust-hir-map.h"
#include "rust-hir-type-check.h"
#include "rust-lang-item.h"
#include "rust-mapping-common.h"
#include "rust-tyty.h"
#include <cassert>
#include <memory>

namespace Rust {
namespace Compile {

class Context;
class TyTyResolveCompile;

struct DropPath
{
  HirId root;
  std::vector<size_t> path;
  std::string var_name;
  std::vector<std::string> field_names;

  DropPath () : root (UNKNOWN_HIRID), path () {}

  bool operator< (const DropPath &o) const
  {
    if (root != o.root)
      return root < o.root;
    return path < o.path;
  }
  bool operator== (const DropPath &o) const
  {
    return root == o.root && path == o.path;
  }

  DropPath child (size_t field_index, const std::string &fname) const
  {
    DropPath c;
    c.root = root;
    c.var_name = var_name;
    c.path = path;
    c.path.push_back (field_index);
    c.field_names = field_names;
    c.field_names.push_back (fname);
    return c;
  }

  DropPath child (size_t field_index) const
  {
    return child (field_index, std::to_string (field_index));
  }

  std::string flag_name () const
  {
    std::string name = "__drop_flag_";
    name += var_name.empty () ? "tmp" : var_name;
    for (size_t i = 0; i < field_names.size (); ++i)
      {
	name += "__";
	name += field_names[i];
	name += "_";
	name += std::to_string (path[i]);
      }
    return name;
  }
  void dump () const { todo_die ("dump for path is not implemented yet bro"); }
};

struct DropPlace
{
  tree type;
  tree drop_flag;
  const TyTy::BaseType *ty;
  DropPath path;
  std::vector<DropPlace *> children;

  DropPlace (tree type, tree drop_flag, const TyTy::BaseType *ty, DropPath path,
	     std::vector<DropPlace *> children)
    : type (type), drop_flag (drop_flag), ty (ty), path (path),
      children (children)
  {}

  void dump () const { die ("dump is not implemented"); }
  bool is_leaf () const { return children.empty (); }
};

struct DropContext
{
  static DropContext *get (Context *compile_ctx);
  DropContext (Context *compile_ctx);

  bool has_drop_impl (const TyTy::BaseType *ty, HirId *mapping);
  bool has_drop_impl (const TyTy::BaseType *ty);

  DropPlace *make_drop_place (tree var_expr, DropPath var_path,
			      const TyTy::BaseType *var_ty);
  DropPlace *make_drop_place_struct (tree var_expr, DropPath var_path,
				     const TyTy::ADTType *var_ty);
  DropPlace *make_drop_place_union (tree var_expr, DropPath var_path,
				    const TyTy::ADTType *var_ty);
  DropPlace *make_drop_place_tuple (tree var_expr, DropPath var_path,
				    const TyTy::TupleType *var_ty);

  DropPlace *finish_make_drop_place (DropPath var_path, tree var_expr,
				     tree drop_flag,
				     const TyTy::BaseType *var_ty,
				     std::vector<DropPlace *> &&children
				     = std::vector<DropPlace *> ())
  {
    drop_places[var_path]
      = std::make_unique<DropPlace> (var_expr, drop_flag, var_ty, var_path,
				     std::move (children));
    setup_drop_flag (drop_places[var_path].get ());
    return drop_places[var_path].get ();
  }

  void setup_drop_flag (DropPlace *drop_place);
  void dump_all_drop_places ();
  DropPlace *lookup_drop_place (DropPath path);
  void mark_drop_flags (DropPlace *place, tree val, std::vector<tree> &stmts);
  tree init (DropPlace *drop_place);
  tree uninit (DropPlace *drop_place);
  tree drop (DropPlace *drop_place);
  tree build_drop_in_place (const TyTy::BaseType *ty, tree self);
  tree build_drop_in_place_struct (const TyTy::ADTType *ty, tree self);
  tree build_drop_in_place_union (const TyTy::ADTType *ty, tree self);
  tree build_drop_in_place_tuple (const TyTy::TupleType *ty, tree self);
  tree build_drop (const TyTy::BaseType *ty);
  tree call_drop (const TyTy::BaseType *ty, tree self);
  tree call_drop_in_place (const TyTy::BaseType *ty, tree self);
  bool needs_drop (const TyTy::BaseType *ty);

  Context *compile_ctx;
  Resolver::TypeCheckContext *tyctx;
  Analysis::Mappings &mappings;
  std::map<DropPath, std::unique_ptr<DropPlace>> drop_places;
};
} // namespace Compile
} // namespace Rust
#endif // RUST_COMPILE_DROP_H
