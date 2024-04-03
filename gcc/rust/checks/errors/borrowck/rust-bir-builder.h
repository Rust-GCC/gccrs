// Copyright (C) 2020-2024 Free Software Foundation, Inc.

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

#ifndef RUST_BIR_BUILDER_H
#define RUST_BIR_BUILDER_H

#include "rust-bir-builder-internal.h"
#include "rust-bir-builder-pattern.h"
#include "rust-bir-builder-expr-stmt.h"

namespace Rust {
namespace BIR {

/** Top-level builder, which compiles a HIR function into a BIR function. */
class Builder final : public AbstractBuilder
{
  std::vector<std::pair<FreeRegion, FreeRegion>> universal_region_bounds;

public:
  explicit Builder (BuilderContext &ctx) : AbstractBuilder (ctx) {}

  Function build (HIR::Function &function)
  {
    rust_debug ("BIR::Builder::build function={%s}",
		function.get_function_name ().as_string ().c_str ());

    auto fn_ty = lookup_type (function)->as<TyTy::FnType> ();

    handle_lifetime_params (fn_ty->get_num_lifetime_params ());
    handle_lifetime_param_constraints (fn_ty->get_region_constraints ());

    handle_return (fn_ty);

    for (auto &param : function.get_function_params ())
      handle_param (param);

    handle_body (*function.get_definition ());

    return Function{
      std::move (ctx.place_db),
      std::move (ctx.arguments),
      std::move (ctx.basic_blocks),
      std::move (ctx.fn_free_regions),
      std::move (universal_region_bounds),
      function.get_locus (),
    };
  }

private:
  /** Instantiate `num_lifetime_params` free regions. */
  void handle_lifetime_params (size_t num_lifetime_params)
  {
    std::vector<FreeRegion> function_free_regions;
    for (size_t i = 0; i < num_lifetime_params; i++)
      {
	function_free_regions.push_back (ctx.place_db.get_next_free_region ());
      }

    rust_debug ("\tctx.fn_free_region={%s}",
		ctx.fn_free_regions.to_string ().c_str ());
    ctx.fn_free_regions.set_from (std::move (function_free_regions));
  }

  void handle_lifetime_param_constraints (
    const TyTy::RegionConstraints &region_constraints)
  {
    rust_debug ("\thandle_lifetime_param_constraints");

    for (auto bound : region_constraints.region_region)
      {
	rust_assert (bound.first.is_early_bound ());
	rust_assert (bound.second.is_early_bound ());

	universal_region_bounds.emplace_back (
	  ctx.fn_free_regions[bound.first.get_index ()],
	  ctx.fn_free_regions[bound.second.get_index ()]);

	auto last_bound = universal_region_bounds.back ();
	rust_debug ("\t\t %ld: %ld", last_bound.first, last_bound.second);
      }

    // TODO: handle type_region constraints
  }

  void handle_return (TyTy::FnType *fn_ty)
  {
    TyTy::BaseType *return_ty = fn_ty->get_return_type ();

    PlaceId return_place = ctx.place_db.add_temporary (return_ty);
    rust_assert (return_place == RETURN_VALUE_PLACE);

    // Set return place to use functions regions, not the fresh ones.
    ctx.place_db[return_place].regions
      = bind_regions (Resolver::TypeCheckContext::get ()
			->get_variance_analysis_ctx ()
			.query_type_regions (fn_ty->get_return_type ()),
		      ctx.fn_free_regions);
  }

  void handle_param (HIR::FunctionParam &param)
  {
    auto param_type = lookup_type (*param.get_param_name ());

    auto &pattern = param.get_param_name ();
    if (pattern->get_pattern_type () == HIR::Pattern::IDENTIFIER
	&& !static_cast<HIR::IdentifierPattern &> (*pattern).get_is_ref ())
      {
	// Avoid useless temporary variable for parameter to look like MIR.
	translated = declare_variable (pattern->get_mappings ());
	ctx.arguments.push_back (translated);
      }
    else
      {
	translated = ctx.place_db.add_temporary (param_type);
	ctx.arguments.push_back (translated);
	PatternBindingBuilder (ctx, translated, tl::nullopt)
	  .go (*param.get_param_name ());
      }

    rust_assert (param.get_type () != nullptr);

    // Set parameter place to use functions regions, not the fresh ones.
    ctx.place_db[translated].regions
      = bind_regions (Resolver::TypeCheckContext::get ()
			->get_variance_analysis_ctx ()
			.query_type_regions (param_type),
		      ctx.fn_free_regions);
  }

  void handle_body (HIR::BlockExpr &body)
  {
    translated = ExprStmtBuilder (ctx).build (body, RETURN_VALUE_PLACE);
    if (!ctx.get_current_bb ().is_terminated ())
      {
	if (ctx.place_db[RETURN_VALUE_PLACE].tyty->is_unit ())
	  {
	    push_assignment (RETURN_VALUE_PLACE,
			     ctx.place_db.get_constant (
			       ctx.place_db[RETURN_VALUE_PLACE].tyty));
	  }
	ctx.get_current_bb ().statements.emplace_back (Statement::Kind::RETURN);
      }
  }
};

} // namespace BIR
} // namespace Rust

#endif // RUST_BIR_BUILDER_H
