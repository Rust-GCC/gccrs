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

#ifndef RUST_HIR_MAP_H
#define RUST_HIR_MAP_H

#include "rust-system.h"
#include "rust-location.h"
#include "rust-mapping-common.h"
#include "rust-canonical-path.h"
#include "rust-ast-full-decls.h"
#include "rust-hir-full-decls.h"
#include "operator.h"

namespace Rust {
namespace Analysis {

// https://github.com/rust-lang/rust/blob/master/library/core/src/ops/arith.rs
class RustLangItem
{
public:
  enum ItemType
  {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    REMAINDER,

    NEGATION,
    NOT,

    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    REM_ASSIGN,

    DEREF,

    UNKNOWN,
  };

  static ItemType Parse (const std::string &item)
  {
    if (item.compare ("add") == 0)
      {
	return ItemType::ADD;
      }
    else if (item.compare ("sub") == 0)
      {
	return ItemType::SUBTRACT;
      }
    else if (item.compare ("mul") == 0)
      {
	return ItemType::MULTIPLY;
      }
    else if (item.compare ("div") == 0)
      {
	return ItemType::DIVIDE;
      }
    else if (item.compare ("rem") == 0)
      {
	return ItemType::REMAINDER;
      }
    else if (item.compare ("neg") == 0)
      {
	return ItemType::NEGATION;
      }
    else if (item.compare ("not") == 0)
      {
	return ItemType::NOT;
      }
    else if (item.compare ("add_assign") == 0)
      {
	return ItemType::ADD_ASSIGN;
      }
    else if (item.compare ("sub_assign") == 0)
      {
	return ItemType::SUB_ASSIGN;
      }
    else if (item.compare ("mul_assign") == 0)
      {
	return ItemType::MUL_ASSIGN;
      }
    else if (item.compare ("div_assign") == 0)
      {
	return ItemType::DIV_ASSIGN;
      }
    else if (item.compare ("rem_assign") == 0)
      {
	return ItemType::REM_ASSIGN;
      }
    else if (item.compare ("deref") == 0)
      {
	return ItemType::DEREF;
      }

    return ItemType::UNKNOWN;
  }

  static std::string ToString (ItemType type)
  {
    switch (type)
      {
      case ADD:
	return "add";
      case SUBTRACT:
	return "sub";
      case MULTIPLY:
	return "mul";
      case DIVIDE:
	return "div";
      case REMAINDER:
	return "rem";
      case NEGATION:
	return "neg";
      case NOT:
	return "not";
      case ADD_ASSIGN:
	return "add_assign";
      case SUB_ASSIGN:
	return "sub_assign";
      case MUL_ASSIGN:
	return "mul_assign";
      case DIV_ASSIGN:
	return "div_assign";
      case REM_ASSIGN:
	return "rem_assign";
      case DEREF:
	return "deref";

      case UNKNOWN:
	break;
      }
    return "<UNKNOWN>";
  }

  static ItemType OperatorToLangItem (ArithmeticOrLogicalOperator op)
  {
    switch (op)
      {
      case ArithmeticOrLogicalOperator::ADD:
	return ItemType::ADD;
      case ArithmeticOrLogicalOperator::SUBTRACT:
	return ItemType::SUBTRACT;
      case ArithmeticOrLogicalOperator::MULTIPLY:
	return ItemType::MULTIPLY;
      case ArithmeticOrLogicalOperator::DIVIDE:
	return ItemType::DIVIDE;
      case ArithmeticOrLogicalOperator::MODULUS:
	return ItemType::REMAINDER;

      default:
	return ItemType::UNKNOWN;
      }
    return ItemType::UNKNOWN;
  }

  static ItemType
  CompoundAssignmentOperatorToLangItem (ArithmeticOrLogicalOperator op)
  {
    switch (op)
      {
      case ArithmeticOrLogicalOperator::ADD:
	return ItemType::ADD_ASSIGN;
      case ArithmeticOrLogicalOperator::SUBTRACT:
	return ItemType::SUB_ASSIGN;
      case ArithmeticOrLogicalOperator::MULTIPLY:
	return ItemType::MUL_ASSIGN;
      case ArithmeticOrLogicalOperator::DIVIDE:
	return ItemType::DIV_ASSIGN;
      case ArithmeticOrLogicalOperator::MODULUS:
	return ItemType::REM_ASSIGN;

      default:
	return ItemType::UNKNOWN;
      }
    return ItemType::UNKNOWN;
  }

  static ItemType NegationOperatorToLangItem (NegationOperator op)
  {
    switch (op)
      {
      case NegationOperator::NEGATE:
	return ItemType::NEGATION;
      case NegationOperator::NOT:
	return ItemType::NOT;

      default:
	return ItemType::UNKNOWN;
      }
    return ItemType::UNKNOWN;
  }
};

class NodeMapping
{
public:
  NodeMapping (CrateNum crateNum, NodeId nodeId, HirId hirId,
	       LocalDefId localDefId)
    : crateNum (crateNum), nodeId (nodeId), hirId (hirId),
      localDefId (localDefId)
  {}

  static NodeMapping get_error ();

  CrateNum get_crate_num () const;
  NodeId get_nodeid () const;
  HirId get_hirid () const;
  LocalDefId get_local_defid () const;
  DefId get_defid () const;

  static DefId get_defid (CrateNum crate_num, LocalDefId local_defid);

  std::string as_string () const;

  bool is_equal (const NodeMapping &other) const
  {
    return get_crate_num () == other.get_crate_num ()
	   && get_nodeid () == other.get_nodeid ()
	   && get_hirid () == other.get_hirid ()
	   && get_local_defid () == other.get_local_defid ();
  }

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
  CrateNum get_current_crate () const;
  CrateNum setup_crate_mappings (std::string crate_name);

  bool get_crate_name (CrateNum crate_num, std::string &name) const
  {
    auto it = crate_names.find (crate_num);
    if (it == crate_names.end ())
      return false;

    name.assign (it->second);
    return true;
  }

  std::string get_current_crate_name () const
  {
    std::string name;
    bool ok = get_crate_name (get_current_crate (), name);
    rust_assert (ok);
    return name;
  }

  NodeId get_next_node_id () { return get_next_node_id (get_current_crate ()); }
  NodeId get_next_node_id (CrateNum crateNum);

  HirId get_next_hir_id () { return get_next_hir_id (get_current_crate ()); }
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

  void insert_hir_trait_item (CrateNum crateNum, HirId id,
			      HIR::TraitItem *item);
  HIR::TraitItem *lookup_hir_trait_item (CrateNum crateNum, HirId id);

  void insert_hir_extern_item (CrateNum crateNum, HirId id,
			       HIR::ExternalItem *item);
  HIR::ExternalItem *lookup_hir_extern_item (CrateNum crateNum, HirId id);

  void insert_hir_impl_block (CrateNum crateNum, HirId id,
			      HIR::ImplBlock *item);
  HIR::ImplBlock *lookup_hir_impl_block (CrateNum crateNum, HirId id);

  void insert_module (CrateNum crateNum, HirId id, HIR::Module *module);
  HIR::Module *lookup_module (CrateNum crateNum, HirId id);

  void insert_hir_implitem (CrateNum crateNum, HirId id, HirId parent_impl_id,
			    HIR::ImplItem *item);
  HIR::ImplItem *lookup_hir_implitem (CrateNum crateNum, HirId id,
				      HirId *parent_impl_id);

  void insert_hir_expr (CrateNum crateNum, HirId id, HIR::Expr *expr);
  HIR::Expr *lookup_hir_expr (CrateNum crateNum, HirId id);

  void insert_hir_path_expr_seg (CrateNum crateNum, HirId id,
				 HIR::PathExprSegment *expr);
  HIR::PathExprSegment *lookup_hir_path_expr_seg (CrateNum crateNum, HirId id);

  void insert_hir_generic_param (CrateNum crateNum, HirId id,
				 HIR::GenericParam *expr);
  HIR::GenericParam *lookup_hir_generic_param (CrateNum crateNum, HirId id);

  void insert_hir_type (CrateNum crateNum, HirId id, HIR::Type *type);
  HIR::Type *lookup_hir_type (CrateNum crateNum, HirId id);

  void insert_hir_stmt (CrateNum crateNum, HirId id, HIR::Stmt *type);
  HIR::Stmt *lookup_hir_stmt (CrateNum crateNum, HirId id);

  void insert_hir_param (CrateNum crateNum, HirId id, HIR::FunctionParam *type);
  HIR::FunctionParam *lookup_hir_param (CrateNum crateNum, HirId id);

  void insert_hir_self_param (CrateNum crateNum, HirId id,
			      HIR::SelfParam *type);
  HIR::SelfParam *lookup_hir_self_param (CrateNum crateNum, HirId id);

  void insert_hir_struct_field (CrateNum crateNum, HirId id,
				HIR::StructExprField *type);
  HIR::StructExprField *lookup_hir_struct_field (CrateNum crateNum, HirId id);

  void insert_hir_pattern (CrateNum crateNum, HirId id, HIR::Pattern *pattern);
  HIR::Pattern *lookup_hir_pattern (CrateNum crateNum, HirId id);

  void walk_local_defids_for_crate (CrateNum crateNum,
				    std::function<bool (HIR::Item *)> cb);

  void insert_node_to_hir (CrateNum crate, NodeId id, HirId ref);
  bool lookup_node_to_hir (CrateNum crate, NodeId id, HirId *ref);

  void insert_location (CrateNum crate, HirId id, Location locus);
  Location lookup_location (CrateNum crate, HirId id);
  Location lookup_location (HirId id)
  {
    return lookup_location (get_current_crate (), id);
  }

  bool resolve_nodeid_to_stmt (CrateNum crate, NodeId id, HIR::Stmt **stmt);
  bool resolve_nodeid_to_stmt (NodeId id, HIR::Stmt **stmt)
  {
    return resolve_nodeid_to_stmt (get_current_crate (), id, stmt);
  }

  std::set<HirId> &get_hirids_within_crate (CrateNum crate)
  {
    return hirNodesWithinCrate[crate];
  }

  void insert_impl_item_mapping (HirId impl_item_id, HIR::ImplBlock *impl)
  {
    rust_assert (hirImplItemsToImplMappings.find (impl_item_id)
		 == hirImplItemsToImplMappings.end ());
    hirImplItemsToImplMappings[impl_item_id] = impl;
  }

  HIR::ImplBlock *lookup_associated_impl (HirId impl_item_id)
  {
    auto lookup = hirImplItemsToImplMappings.find (impl_item_id);
    rust_assert (lookup != hirImplItemsToImplMappings.end ());
    return lookup->second;
  }

  void iterate_impl_items (
    std::function<bool (HirId, HIR::ImplItem *, HIR::ImplBlock *)> cb);

  void iterate_impl_blocks (std::function<bool (HirId, HIR::ImplBlock *)> cb);

  bool is_impl_item (HirId id)
  {
    HirId parent_impl_block_id = UNKNOWN_HIRID;
    return lookup_hir_implitem (get_current_crate (), id, &parent_impl_block_id)
	   != nullptr;
  }

  void insert_trait_item_mapping (HirId trait_item_id, HIR::Trait *trait)
  {
    rust_assert (hirTraitItemsToTraitMappings.find (trait_item_id)
		 == hirTraitItemsToTraitMappings.end ());
    hirTraitItemsToTraitMappings[trait_item_id] = trait;
  }

  HIR::Trait *lookup_trait_item_mapping (HirId trait_item_id)
  {
    auto lookup = hirTraitItemsToTraitMappings.find (trait_item_id);
    rust_assert (lookup != hirTraitItemsToTraitMappings.end ());
    return lookup->second;
  }

  void insert_canonical_path (CrateNum crate, NodeId id,
			      const Resolver::CanonicalPath path)
  {
    const Resolver::CanonicalPath *p = nullptr;
    if (lookup_canonical_path (crate, id, &p))
      {
	// if we have already stored a canonical path this is ok so long as
	// this new path is equal or is smaller that the existing one but in
	// that case we ignore it.
	if (p->is_equal (path))
	  return;
	else
	  {
	    rust_assert (p->size () >= path.size ());
	    return;
	  }
      }

    paths[crate].emplace (id, std::move (path));
  }

  bool lookup_canonical_path (CrateNum crate, NodeId id,
			      const Resolver::CanonicalPath **path)
  {
    auto it = paths.find (crate);
    if (it == paths.end ())
      return false;

    auto iy = it->second.find (id);
    if (iy == it->second.end ())
      return false;

    *path = &iy->second;
    return true;
  }

  void insert_lang_item (RustLangItem::ItemType item_type, DefId id)
  {
    auto it = lang_item_mappings.find (item_type);
    rust_assert (it == lang_item_mappings.end ());

    lang_item_mappings[item_type] = id;
  }

  bool lookup_lang_item (RustLangItem::ItemType item_type, DefId *id)
  {
    auto it = lang_item_mappings.find (item_type);
    if (it == lang_item_mappings.end ())
      return false;

    *id = it->second;
    return true;
  }

private:
  Mappings ();

  CrateNum crateNumItr = 0;
  CrateNum currentCrateNum;

  std::map<CrateNum, HirId> hirIdIter;
  std::map<CrateNum, NodeId> nodeIdIter;
  std::map<CrateNum, LocalDefId> localIdIter;

  std::map<CrateNum, AST::Crate *> astCrateMappings;
  std::map<CrateNum, HIR::Crate *> hirCrateMappings;

  std::map<DefId, HIR::Item *> defIdMappings;
  std::map<CrateNum, std::map<LocalDefId, HIR::Item *>> localDefIdMappings;
  std::map<CrateNum, std::map<HirId, HIR::Module *>> hirModuleMappings;
  std::map<CrateNum, std::map<HirId, HIR::Item *>> hirItemMappings;
  std::map<CrateNum, std::map<HirId, HIR::Type *>> hirTypeMappings;
  std::map<CrateNum, std::map<HirId, HIR::Expr *>> hirExprMappings;
  std::map<CrateNum, std::map<HirId, HIR::Stmt *>> hirStmtMappings;
  std::map<CrateNum, std::map<HirId, HIR::FunctionParam *>> hirParamMappings;
  std::map<CrateNum, std::map<HirId, HIR::StructExprField *>>
    hirStructFieldMappings;
  std::map<CrateNum, std::map<HirId, std::pair<HirId, HIR::ImplItem *>>>
    hirImplItemMappings;
  std::map<CrateNum, std::map<HirId, HIR::SelfParam *>> hirSelfParamMappings;
  std::map<HirId, HIR::ImplBlock *> hirImplItemsToImplMappings;
  std::map<CrateNum, std::map<HirId, HIR::ImplBlock *>> hirImplBlockMappings;
  std::map<CrateNum, std::map<HirId, HIR::TraitItem *>> hirTraitItemMappings;
  std::map<CrateNum, std::map<HirId, HIR::ExternalItem *>>
    hirExternItemMappings;
  std::map<CrateNum, std::map<HirId, HIR::PathExprSegment *>>
    hirPathSegMappings;
  std::map<CrateNum, std::map<HirId, HIR::GenericParam *>>
    hirGenericParamMappings;
  std::map<HirId, HIR::Trait *> hirTraitItemsToTraitMappings;
  std::map<CrateNum, std::map<HirId, HIR::Pattern *>> hirPatternMappings;

  // this maps the lang=<item_type> to DefId mappings
  std::map<RustLangItem::ItemType, DefId> lang_item_mappings;

  // canonical paths
  std::map<CrateNum, std::map<NodeId, const Resolver::CanonicalPath>> paths;

  // location info
  std::map<CrateNum, std::map<NodeId, Location>> locations;

  // reverse mappings
  std::map<CrateNum, std::map<NodeId, HirId>> nodeIdToHirMappings;

  // all hirid nodes
  std::map<CrateNum, std::set<HirId>> hirNodesWithinCrate;

  // crate names
  std::map<CrateNum, std::string> crate_names;
};

} // namespace Analysis
} // namespace Rust

#endif // RUST_HIR_MAP_H
