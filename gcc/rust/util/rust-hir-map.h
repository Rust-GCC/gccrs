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

#ifndef RUST_HIR_MAP_H
#define RUST_HIR_MAP_H

#include "rust-system.h"

#include "rust-ast-full-decls.h"
#include "rust-hir-full-decls.h"

namespace Rust {

// refers to a Crate
typedef uint32_t CrateNum;
// refers to any node in the AST in current Crate
typedef uint32_t NodeId;
// refers to any node in the HIR for the current crate
typedef uint32_t HirId;
// refers to any top-level decl in HIR
typedef uint32_t LocalDefId;
// refers to <Crate><DefId>
typedef uint64_t DefId;

#define DEF_ID_CRATE_MASK 0xFFFFFFFF00000000
#define DEF_ID_LOCAL_DEF_MASK 0x00000000FFFFFFFF

#define UNKNOWN_CREATENUM ((uint32_t) (-1))
#define UNKNOWN_NODEID ((uint32_t) (-1))
#define UNKNOWN_HIRID ((uint32_t) (-1))
#define UNKNOWN_LOCAL_DEFID ((uint32_t) (-1))
#define UNKNOWN_DEFID ((uint64_t) (-1))

namespace Analysis {

class NodeMapping
{
public:
  NodeMapping (CrateNum crateNum, NodeId nodeId, HirId hirId,
	       LocalDefId localDefId);
  ~NodeMapping ();

  static NodeMapping get_error ();

  CrateNum get_crate_num () const;
  NodeId get_nodeid () const;
  HirId get_hirid () const;
  LocalDefId get_local_defid () const;
  DefId get_defid () const;

  std::string as_string () const;

private:
  CrateNum crateNum;
  NodeId nodeId;
  HirId hirId;
  LocalDefId localDefId;
};

class Mappings
{
public:
  static Mappings *get ();
  ~Mappings ();

  CrateNum get_next_crate_num ();
  void set_current_crate (CrateNum crateNum);
  CrateNum get_current_crate ();

  NodeId get_next_node_id () { return get_next_node_id (get_current_crate ()); }
  NodeId get_next_node_id (CrateNum crateNum);
  HirId get_next_hir_id (CrateNum crateNum);
  LocalDefId get_next_localdef_id (CrateNum crateNum);

  AST::Crate *get_ast_crate (CrateNum crateNum);
  void insert_ast_crate (AST::Crate *crate);

  HIR::Crate *get_hir_crate (CrateNum crateNum);
  void insert_hir_crate (HIR::Crate *crate);

  void insert_defid_mapping (DefId id, HIR::Item *item);
  HIR::Item *lookup_defid (DefId id);

  void insert_local_defid_mapping (CrateNum crateNum, LocalDefId id,
				   HIR::Item *item);
  HIR::Item *lookup_local_defid (CrateNum crateNum, LocalDefId id);

  void insert_hir_item (CrateNum crateNum, HirId id, HIR::Item *item);
  HIR::Item *lookup_hir_item (CrateNum crateNum, HirId id);

  void insert_hir_expr (CrateNum crateNum, HirId id, HIR::Expr *expr);
  HIR::Expr *lookup_hir_expr (CrateNum crateNum, HirId id);

  void walk_local_defids_for_crate (CrateNum crateNum,
				    std::function<bool (HIR::Item *)> cb);

private:
  Mappings ();

  CrateNum crateNumItr;
  CrateNum currentCrateNum;

  std::map<CrateNum, HirId> hirIdIter;
  std::map<CrateNum, NodeId> nodeIdIter;
  std::map<CrateNum, LocalDefId> localIdIter;

  std::map<CrateNum, AST::Crate *> astCrateMappings;
  std::map<CrateNum, HIR::Crate *> hirCrateMappings;

  std::map<DefId, HIR::Item *> defIdMappings;
  std::map<CrateNum, std::map<LocalDefId, HIR::Item *> > localDefIdMappings;
  std::map<CrateNum, std::map<HirId, HIR::Item *> > hirItemMappings;
  std::map<CrateNum, std::map<HirId, HIR::Expr *> > hirExprMappings;
};

} // namespace Analysis
} // namespace Rust

#endif // RUST_HIR_MAP_H
