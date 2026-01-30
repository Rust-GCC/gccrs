// Copyright (C) 2026 Free Software Foundation, Inc.

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
#ifndef RUST_FEATURE_COLLECTOR_H
#define RUST_FEATURE_COLLECTOR_H

#include "rust-feature.h"
#include "rust-ast-visitor.h"

namespace Rust {
namespace Features {

/** Helper structure gathering all features enabled within a given crate
 * using the #![feature(XXXXX)] syntax.
 **/
struct CrateFeatures
{
  // The node id identifying the crate those features belong to.
  NodeId crate_id;
  // Language features that have been declared within the crate.
  std::set<Feature::Name> valid_lang_features;
  // Library features that have been declared within the crate.
  std::map<std::string, location_t> valid_lib_features;

  CrateFeatures (NodeId crate_id) : crate_id (crate_id) {}
};

class FeatureCollector : public AST::DefaultASTVisitor
{
public:
  FeatureCollector ();

  CrateFeatures collect (AST::Crate &crate);

private:
  CrateFeatures features;

  using AST::DefaultASTVisitor::visit;
  void visit (AST::Crate &crate) override;
  void identify_feature (const AST::Attribute &attribute);
  void add_features_from_token_tree (const AST::DelimTokenTree &delim_ttree);
  void add_features_from_meta_item_container (
    const AST::AttrInputMetaItemContainer &meta_item_container);
};
} // namespace Features
} // namespace Rust

#endif /* ! RUST_FEATURE_COLLECTOR_H */
