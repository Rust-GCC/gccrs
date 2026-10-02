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

#include "rust-derive-coerce-pointee.h"
#include "rust-diagnostics.h"
#include "rust-feature-store.h"
#include "rust-feature.h"
#include "rust-session-manager.h"

namespace Rust {
namespace AST {

DeriveCoercePointee::DeriveCoercePointee (location_t loc,
					  Builder::Source item_source)
  : DeriveVisitor (loc, item_source)
{}

std::unique_ptr<AST::Item>
DeriveCoercePointee::go (Item &item)
{
  Features::EarlyFeatureGateStore::get ().add (
    Feature::Name::DERIVE_COERCE_POINTEE,
    Error (loc, "use of unstable library feature %<derive_coerce_pointee%>"));

  // NOTE: This is technically a library feature, so I don't think this is
  // how it should be gated. But as far as I can tell at the moment we have
  // no mechanisms for handling these, and furthermore this will go through
  // our compatibility layer.
  //
  // What this means is that the `CoercePointee` macro will *not* be defined
  // in the `core` that we will be using to compile the kernel at first.
  //
  // The basic example for `CoercePointee` is something like this:
  //
  // ```rust
  // use std::marker::CoercePointee;
  //
  // #[derive(CoercePointee)]
  // struct Flip<Flop>(*const Flop);
  // ```
  //
  // but the marker will not be present in the `core` we will be using at
  // first, so we will need to fake its existence with
  // -frust-compat-version, and then the compiler can assume that it is a
  // built-in derive. At least that's my expectation.

  if (!Session::get_instance ().should_support_coerce_pointee ())
    {
      rust_error_at (loc, "derive(CoercePointee) requires a compatibility mode "
			  "greater or equal to 1.84");
      return {};
    }

  rust_warning_at (
    loc, 0,
    "derive(CoercePointee) is currently unimplemented and has no effect");

  return {};
}

// CoercePointee requires the Struct or Tuple to have transparent representation
bool DeriveCoercePointee::validate_repr_transparent(
    const Rust::AST::Item &item) {
  const auto attrs = item.get_outer_attrs();
  for (const auto &attr : attrs) {
    if (attr.get_path().as_string() != "repr")
      continue;
    if (attr.empty_input())
      continue;

    const auto &attr_input = attr.get_attr_input();
    if (attr_input.get_attr_input_type() !=
        AST::AttrInput::AttrInputType::TOKEN_TREE)
      continue;

    std::unique_ptr<AST::AttrInputMetaItemContainer> meta_item(
        static_cast<const AST::DelimTokenTree &>(attr_input)
            .parse_to_meta_item());

    for (const auto &item : meta_item->get_items())
      if (item->as_string() == "transparent")
        return true;
  }
  rust_error_at(item.get_locus(), ErrorCode::E0802,
                "%<CoercePointee%> is only applicable to struct/tuple with "
                "repr(transparent) layout");
  return false;
}

// Structs or Tuples with CoercePointee must have minimum of one field else
// compile time error
bool DeriveCoercePointee::validate_number_of_fields(
    const Rust::AST::Item &item) {
  size_t field_count = 0;

  if (item.get_item_kind() == Item::Kind::Struct) {
    auto *struct_item = static_cast<const StructStruct *>(&item);
    field_count = struct_item->get_fields().size();
  } else
    rust_unreachable();

  if (field_count == 0) {
    rust_error_at(item.get_locus(), ErrorCode::E0802,
                  "%<CoercePointee%> can only be derived on a struct "
                  "with at least one field");
    return false;
  }
  return true;
}

// The `pointee` must be of non-generic type. If there are more than 2 traits
// defined, then one of them must be specifically marked as pointee else compile
// error
bool DeriveCoercePointee::validate_non_generic_pointee(
    const Rust::AST::Item &item) {
  const std::vector<std::unique_ptr<GenericParam>> *generic_params = nullptr;

  if (item.get_item_kind() == Item::Kind::Struct) {
    const StructStruct *struct_item = static_cast<const StructStruct *>(&item);
    generic_params = &struct_item->get_generic_params();
  } else
    rust_unreachable();

  std::vector<size_t> type_param_indices;
  std::vector<size_t> pointee_indices;

  for (size_t i = 0; i < generic_params->size(); ++i) {
    auto *type_param = static_cast<TypeParam *>((*generic_params)[i].get());
    if (!type_param)
      continue;

    type_param_indices.push_back(i);

    for (const auto &attr : type_param->get_outer_attrs())
      if (attr.get_path().as_string() == "pointee") {
        pointee_indices.push_back(i);
        break;
      }
  }
  if (type_param_indices.empty()) {
    rust_error_at(item.get_locus(), ErrorCode::E0802,
                  "%<CoercePointee%> can only be derived on a struct "
                  "with at least one generic type");
    return false;
  }

  if (type_param_indices.size() == 1)
    return true; // the sole type param is implicitly the pointee

  if (pointee_indices.empty()) {
    rust_error_at(item.get_locus(), ErrorCode::E0802,
                  "exactly one generic type parameter must be marked "
                  "%<#[pointee]%> when deriving %<CoercePointee%> on a "
                  "struct with multiple type parameters");
    return false;
  }

  if (pointee_indices.size() > 1) {
    rust_error_at(item.get_locus(), ErrorCode::E0802,
                  "only one type parameter can be marked "
                  "%<#[pointee]%>");
    return false;
  }

  return true;
}
} // namespace AST
} // namespace Rust
