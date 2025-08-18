// Copyright (C) 2020-2025 Free Software Foundation, Inc.

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

#ifndef RUST_NAME_RESOLVER_H
#define RUST_NAME_RESOLVER_H

#include "rust-system.h"
#include "rust-canonical-path.h"
#include "rust-hir-map.h"
#include "rust-hir-type-check.h"

namespace Rust {
namespace Resolver {

class Rib
{
public:
  enum ItemType
  {
    Var,
    Param,
    Function,
    Type,
    Module,
    Static,
    Const,
    Trait,
    Impl,
    TraitImpl,
    ExternCrate,
    MacroDecl,
    Label,
    Unknown
  };

  // FIXME
  // Rust uses local_def_ids assigned by def_collector on the AST. Consider
  // moving to a local-def-id
  Rib (CrateNum crateNum, NodeId node_id);

  // this takes the relative paths of items within a compilation unit for lookup
  void insert_name (
    const CanonicalPath &path, NodeId id, location_t locus, bool shadow,
    ItemType type,
    std::function<void (const CanonicalPath &, NodeId, location_t)> dup_cb);

  bool lookup_canonical_path (const NodeId &id, CanonicalPath *ident);
  bool lookup_name (const CanonicalPath &ident, NodeId *id);
  void clear_name (const CanonicalPath &ident, NodeId id);
  void append_reference_for_def (NodeId def, NodeId ref);
  bool have_references_for_node (NodeId def) const;
  bool decl_was_declared_here (NodeId def) const;
  bool lookup_decl_type (NodeId def, ItemType *type) const;
  void debug () const;
  std::string debug_str () const;

  CrateNum get_crate_num () const { return crate_num; }
  NodeId get_node_id () const { return node_id; }
  std::map<NodeId, location_t> &get_declarations () { return decls_within_rib; }

private:
  CrateNum crate_num;
  NodeId node_id;
  std::map<CanonicalPath, NodeId> path_mappings;
  std::map<NodeId, CanonicalPath> reverse_path_mappings;
  std::map<NodeId, location_t> decls_within_rib;
  std::map<NodeId, std::set<NodeId>> references;
  std::map<NodeId, ItemType> decl_type_mappings;
};

class Scope
{
public:
  Scope (CrateNum crate_num);

  void insert (
    const CanonicalPath &ident, NodeId id, location_t locus, bool shadow,
    Rib::ItemType type,
    std::function<void (const CanonicalPath &, NodeId, location_t)> dup_cb);

  void insert (const CanonicalPath &ident, NodeId id, location_t locus,
	       Rib::ItemType type = Rib::ItemType::Unknown);
  bool lookup (const CanonicalPath &ident, NodeId *id);
  bool lookup_decl_type (NodeId id, Rib::ItemType *type);
  bool lookup_rib_for_decl (NodeId id, const Rib **rib);

  void iterate (std::function<bool (Rib *)> cb);
  void iterate (std::function<bool (const Rib *)> cb) const;

  Rib *peek ();
  void push (NodeId id);
  Rib *pop ();

  bool decl_was_declared_here (NodeId def) const;
  void append_reference_for_def (NodeId refId, NodeId defId);

  CrateNum get_crate_num () const { return crate_num; }

  const std::vector<Rib *> &get_context () const { return stack; };

private:
  CrateNum crate_num;
  std::vector<Rib *> stack;
};

} // namespace Resolver
} // namespace Rust

#endif // RUST_NAME_RESOLVER_H
