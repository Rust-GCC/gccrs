// Copyright (C) 2020-2023 Free Software Foundation, Inc.

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

#include "rust-cfg-strip.h"
#include "rust-ast-full.h"
#include "rust-session-manager.h"
#include "rust-attribute-values.h"

namespace Rust {

/**
 * Determines whether any cfg predicate is false and hence item with attributes
 * should be stripped. Note that attributes must be expanded before calling.
 */
bool
fails_cfg (const AST::AttrVec &attrs)
{
  auto &session = Session::get_instance ();

  for (const auto &attr : attrs)
    {
      if (attr.get_path () == Values::Attributes::CFG
	  && !attr.check_cfg_predicate (session))
	return true;
    }
  return false;
}

/**
 * Determines whether any cfg predicate is false and hence item with attributes
 * should be stripped. Will expand attributes as well.
 */
bool
fails_cfg_with_expand (AST::AttrVec &attrs)
{
  auto &session = Session::get_instance ();

  // TODO: maybe have something that strips cfg attributes that evaluate true?
  for (auto &attr : attrs)
    {
      if (attr.get_path () == Values::Attributes::CFG)
	{
	  if (!attr.is_parsed_to_meta_item ())
	    attr.parse_attr_to_meta_item ();

	  // DEBUG
	  if (!attr.is_parsed_to_meta_item ())
	    rust_debug ("failed to parse attr to meta item, right before "
			"cfg predicate check");
	  else
	    rust_debug ("attr has been successfully parsed to meta item, "
			"right before cfg predicate check");

	  if (!attr.check_cfg_predicate (session))
	    {
	      // DEBUG
	      rust_debug (
		"cfg predicate failed for attribute: \033[0;31m'%s'\033[0m",
		attr.as_string ().c_str ());

	      return true;
	    }
	  else
	    {
	      // DEBUG
	      rust_debug ("cfg predicate succeeded for attribute: "
			  "\033[0;31m'%s'\033[0m",
			  attr.as_string ().c_str ());
	    }
	}
    }
  return false;
}

/**
 * Expands cfg_attr attributes.
 */
void
expand_cfg_attrs (AST::AttrVec &attrs)
{
  auto &session = Session::get_instance ();

  for (std::size_t i = 0; i < attrs.size (); i++)
    {
      auto &attr = attrs[i];
      if (attr.get_path () == Values::Attributes::CFG_ATTR)
	{
	  if (!attr.is_parsed_to_meta_item ())
	    attr.parse_attr_to_meta_item ();

	  if (attr.check_cfg_predicate (session))
	    {
	      // split off cfg_attr
	      AST::AttrVec new_attrs = attr.separate_cfg_attrs ();

	      // remove attr from vector
	      attrs.erase (attrs.begin () + i);

	      // add new attrs to vector
	      attrs.insert (attrs.begin () + i,
			    std::make_move_iterator (new_attrs.begin ()),
			    std::make_move_iterator (new_attrs.end ()));
	    }

	  /* do something - if feature (first token in tree) is in fact enabled,
	   * make tokens listed afterwards into attributes. i.e.: for
	   * [cfg_attr(feature = "wow", wow1, wow2)], if "wow" is true, then add
	   * attributes [wow1] and [wow2] to attribute list. This can also be
	   * recursive, so check for expanded attributes being recursive and
	   * possibly recursively call the expand_attrs? */
	}
      else
	{
	  i++;
	}
    }
  attrs.shrink_to_fit ();
}

void
CfgStrip::go (AST::Crate &crate)
{
  // expand crate cfg_attr attributes
  expand_cfg_attrs (crate.inner_attrs);

  if (fails_cfg_with_expand (crate.inner_attrs))
    {
      // basically, delete whole crate
      crate.strip_crate ();
      // TODO: maybe create warning here? probably not desired behaviour
    }

  auto &items = crate.items;

  for (auto it = items.begin (); it != items.end ();)
    {
      auto &item = *it;
      item->accept_vis (*this);

      if (item->is_marked_for_strip ())
	it = items.erase (it);
      else
	it++;
    }

  // expand module attributes?
}

// Visitor used to expand attributes.
void
CfgStrip::maybe_strip_struct_fields (std::vector<AST::StructField> &fields)
{
  for (auto it = fields.begin (); it != fields.end ();)
    {
      auto &field = *it;

      auto &field_attrs = field.get_outer_attrs ();
      expand_cfg_attrs (field_attrs);
      if (fails_cfg_with_expand (field_attrs))
	{
	  it = fields.erase (it);
	  continue;
	}

      // expand sub-types of type, but can't strip type itself
      auto &type = field.get_field_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");

      // if nothing else happens, increment
      ++it;
    }
}

void
CfgStrip::maybe_strip_tuple_fields (std::vector<AST::TupleField> &fields)
{
  for (auto it = fields.begin (); it != fields.end ();)
    {
      auto &field = *it;

      auto &field_attrs = field.get_outer_attrs ();
      expand_cfg_attrs (field_attrs);
      if (fails_cfg_with_expand (field_attrs))
	{
	  it = fields.erase (it);
	  continue;
	}

      // expand sub-types of type, but can't strip type itself
      auto &type = field.get_field_type ();
      type->accept_vis (*this);
      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");

      // if nothing else happens, increment
      ++it;
    }
}

void
CfgStrip::maybe_strip_function_params (std::vector<AST::FunctionParam> &params)
{
  for (auto it = params.begin (); it != params.end ();)
    {
      auto &param = *it;

      auto &param_attrs = param.get_outer_attrs ();
      expand_cfg_attrs (param_attrs);
      if (fails_cfg_with_expand (param_attrs))
	{
	  it = params.erase (it);
	  continue;
	}

      // TODO: should an unwanted strip lead to break out of loop?
      auto &pattern = param.get_pattern ();
      pattern->accept_vis (*this);
      if (pattern->is_marked_for_strip ())
	rust_error_at (pattern->get_locus (),
		       "cannot strip pattern in this position");

      auto &type = param.get_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");

      // increment
      ++it;
    }
}

void
CfgStrip::maybe_strip_generic_args (AST::GenericArgs &args)
{
  // lifetime args can't be expanded
  // FIXME: Can we have macro invocations for lifetimes?

  // expand type args - strip sub-types only
  for (auto &arg : args.get_generic_args ())
    {
      switch (arg.get_kind ())
	{
	  case AST::GenericArg::Kind::Type: {
	    auto &type = arg.get_type ();
	    type->accept_vis (*this);

	    if (type->is_marked_for_strip ())
	      rust_error_at (type->get_locus (),
			     "cannot strip type in this position");
	    break;
	  }
	  case AST::GenericArg::Kind::Const: {
	    auto &expr = arg.get_expression ();
	    expr->accept_vis (*this);

	    if (expr->is_marked_for_strip ())
	      rust_error_at (expr->get_locus (),
			     "cannot strip expression in this position");
	    break;
	  }
	default:
	  break;
	  // FIXME: Figure out what to do here if there is ambiguity. Since the
	  // resolver comes after the expansion, we need to figure out a way to
	  // strip ambiguous values here
	  // TODO: Arthur: Probably add a `mark_as_strip` method to `GenericArg`
	  // or something. This would clean up this whole thing
	}
    }

  // FIXME: Can we have macro invocations in generic type bindings?
  // expand binding args - strip sub-types only
  for (auto &binding : args.get_binding_args ())
    {
      auto &type = binding.get_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");
    }
}

void
CfgStrip::maybe_strip_qualified_path_type (AST::QualifiedPathType &path_type)
{
  auto &type = path_type.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  if (path_type.has_as_clause ())
    {
      auto &type_path = path_type.get_as_type_path ();
      visit (type_path);
      if (type_path.is_marked_for_strip ())
	rust_error_at (type_path.get_locus (),
		       "cannot strip type path in this position");
    }
}

void
CfgStrip::CfgStrip::maybe_strip_closure_params (
  std::vector<AST::ClosureParam> &params)
{
  for (auto it = params.begin (); it != params.end ();)
    {
      auto &param = *it;

      auto &param_attrs = param.get_outer_attrs ();
      expand_cfg_attrs (param_attrs);
      if (fails_cfg_with_expand (param_attrs))
	{
	  it = params.erase (it);
	  continue;
	}

      auto &pattern = param.get_pattern ();
      pattern->accept_vis (*this);
      if (pattern->is_marked_for_strip ())
	rust_error_at (pattern->get_locus (),
		       "cannot strip pattern in this position");

      if (param.has_type_given ())
	{
	  auto &type = param.get_type ();
	  type->accept_vis (*this);

	  if (type->is_marked_for_strip ())
	    rust_error_at (type->get_locus (),
			   "cannot strip type in this position");
	}

      // increment if found nothing else so far
      ++it;
    }
}

void
CfgStrip::maybe_strip_self_param (AST::SelfParam &self_param)
{
  if (self_param.has_type ())
    {
      auto &type = self_param.get_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");
    }
  /* TODO: maybe check for invariants being violated - e.g. both type and
   * lifetime? */
}

void
CfgStrip::maybe_strip_where_clause (AST::WhereClause &where_clause)
{
  // items cannot be stripped conceptually, so just accept visitor
  for (auto &item : where_clause.get_items ())
    item->accept_vis (*this);
}

void
CfgStrip::maybe_strip_trait_function_decl (AST::TraitFunctionDecl &decl)
{
  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : decl.get_generic_params ())
    param->accept_vis (*this);

  /* strip function parameters if required - this is specifically
   * allowed by spec */
  maybe_strip_function_params (decl.get_function_params ());

  if (decl.has_return_type ())
    {
      auto &return_type = decl.get_return_type ();
      return_type->accept_vis (*this);

      if (return_type->is_marked_for_strip ())
	rust_error_at (return_type->get_locus (),
		       "cannot strip type in this position");
    }

  if (decl.has_where_clause ())
    maybe_strip_where_clause (decl.get_where_clause ());
}

void
CfgStrip::maybe_strip_trait_method_decl (AST::TraitMethodDecl &decl)
{
  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : decl.get_generic_params ())
    param->accept_vis (*this);

  /* assuming you can't strip self param - wouldn't be a method
   * anymore. spec allows outer attrs on self param, but doesn't
   * specify whether cfg is used. */
  maybe_strip_self_param (decl.get_self_param ());

  /* strip function parameters if required - this is specifically
   * allowed by spec */
  maybe_strip_function_params (decl.get_function_params ());

  if (decl.has_return_type ())
    {
      auto &return_type = decl.get_return_type ();
      return_type->accept_vis (*this);

      if (return_type->is_marked_for_strip ())
	rust_error_at (return_type->get_locus (),
		       "cannot strip type in this position");
    }

  if (decl.has_where_clause ())
    maybe_strip_where_clause (decl.get_where_clause ());
}

void
CfgStrip::visit (AST::Token &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::DelimTokenTree &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::AttrInputMetaItemContainer &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::IdentifierExpr &ident_expr)
{
  // strip test based on outer attrs
  expand_cfg_attrs (ident_expr.get_outer_attrs ());
  if (fails_cfg_with_expand (ident_expr.get_outer_attrs ()))
    {
      ident_expr.mark_for_strip ();
      return;
    }
}
void
CfgStrip::visit (AST::Lifetime &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::LifetimeParam &)
{
  // supposedly does not require - cfg does nothing
}
void
CfgStrip::visit (AST::ConstGenericParam &)
{
  // likewise
}

void
CfgStrip::visit (AST::MacroInvocation &macro_invoc)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (macro_invoc.get_outer_attrs ());
  if (fails_cfg_with_expand (macro_invoc.get_outer_attrs ()))
    {
      macro_invoc.mark_for_strip ();
      return;
    }

  // can't strip simple path

  // I don't think any macro token trees can be stripped in any way

  // TODO: maybe have cfg! macro stripping behaviour here?
}

void
CfgStrip::visit (AST::PathInExpression &path)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (path.get_outer_attrs ());
  if (fails_cfg_with_expand (path.get_outer_attrs ()))
    {
      path.mark_for_strip ();
      return;
    }

  for (auto &segment : path.get_segments ())
    {
      if (segment.has_generic_args ())
	maybe_strip_generic_args (segment.get_generic_args ());
    }
}
void
CfgStrip::visit (AST::TypePathSegment &)
{
  // shouldn't require
}
void
CfgStrip::visit (AST::TypePathSegmentGeneric &segment)
{
  // TODO: strip inside generic args

  if (!segment.has_generic_args ())
    return;

  maybe_strip_generic_args (segment.get_generic_args ());
}
void
CfgStrip::visit (AST::TypePathSegmentFunction &segment)
{
  auto &type_path_function = segment.get_type_path_function ();

  for (auto &type : type_path_function.get_params ())
    {
      type->accept_vis (*this);
      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");
    }

  if (type_path_function.has_return_type ())
    {
      auto &return_type = type_path_function.get_return_type ();
      return_type->accept_vis (*this);

      if (return_type->is_marked_for_strip ())
	rust_error_at (return_type->get_locus (),
		       "cannot strip type in this position");
    }
}
void
CfgStrip::visit (AST::TypePath &path)
{
  // this shouldn't strip any segments, but can strip inside them
  for (auto &segment : path.get_segments ())
    segment->accept_vis (*this);
}
void
CfgStrip::visit (AST::QualifiedPathInExpression &path)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (path.get_outer_attrs ());
  if (fails_cfg_with_expand (path.get_outer_attrs ()))
    {
      path.mark_for_strip ();
      return;
    }

  maybe_strip_qualified_path_type (path.get_qualified_path_type ());

  for (auto &segment : path.get_segments ())
    {
      if (segment.has_generic_args ())
	maybe_strip_generic_args (segment.get_generic_args ());
    }
}
void
CfgStrip::visit (AST::QualifiedPathInType &path)
{
  maybe_strip_qualified_path_type (path.get_qualified_path_type ());

  // this shouldn't strip any segments, but can strip inside them
  for (auto &segment : path.get_segments ())
    segment->accept_vis (*this);
}

void
CfgStrip::visit (AST::LiteralExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }
}
void
CfgStrip::visit (AST::AttrInputLiteral &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::AttrInputMacro &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::MetaItemLitExpr &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::MetaItemPathLit &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::BorrowExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &borrowed_expr = expr.get_borrowed_expr ();
  borrowed_expr->accept_vis (*this);
  if (borrowed_expr->is_marked_for_strip ())
    rust_error_at (borrowed_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::DereferenceExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &dereferenced_expr = expr.get_dereferenced_expr ();
  dereferenced_expr->accept_vis (*this);
  if (dereferenced_expr->is_marked_for_strip ())
    rust_error_at (dereferenced_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ErrorPropagationExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &propagating_expr = expr.get_propagating_expr ();
  propagating_expr->accept_vis (*this);
  if (propagating_expr->is_marked_for_strip ())
    rust_error_at (propagating_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::NegationExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &negated_expr = expr.get_negated_expr ();
  negated_expr->accept_vis (*this);
  if (negated_expr->is_marked_for_strip ())
    rust_error_at (negated_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ArithmeticOrLogicalExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * two direct descendant expressions, can strip ones below that */

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  auto &l_expr = expr.get_left_expr ();
  l_expr->accept_vis (*this);

  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  auto &r_expr = expr.get_right_expr ();
  r_expr->accept_vis (*this);

  // ensure that they are not marked for strip
  if (expr.get_left_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_left_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed "
		   "before binary op exprs");
  if (expr.get_right_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_right_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ComparisonExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * two direct descendant expressions, can strip ones below that */

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  auto &l_expr = expr.get_left_expr ();
  l_expr->accept_vis (*this);

  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  auto &r_expr = expr.get_right_expr ();
  r_expr->accept_vis (*this);

  // ensure that they are not marked for strip
  if (expr.get_left_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_left_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed "
		   "before binary op exprs");
  if (expr.get_right_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_right_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::LazyBooleanExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * two direct descendant expressions, can strip ones below that */

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  auto &l_expr = expr.get_left_expr ();
  l_expr->accept_vis (*this);

  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  auto &r_expr = expr.get_right_expr ();
  r_expr->accept_vis (*this);

  // ensure that they are not marked for strip
  if (expr.get_left_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_left_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed "
		   "before binary op exprs");
  if (expr.get_right_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_right_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::TypeCastExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * direct descendant expression, can strip ones below that */

  auto &casted_expr = expr.get_casted_expr ();
  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  casted_expr->accept_vis (*this);

  // ensure that they are not marked for strip
  if (casted_expr->is_marked_for_strip ())
    rust_error_at (casted_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed before cast exprs");

  // TODO: strip sub-types of type
  auto &type = expr.get_type_to_cast_to ();
  type->accept_vis (*this);
  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");
}
void
CfgStrip::visit (AST::AssignmentExpr &expr)
{
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  auto &l_expr = expr.get_left_expr ();
  l_expr->accept_vis (*this);

  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  auto &r_expr = expr.get_right_expr ();
  r_expr->accept_vis (*this);

  // ensure that they are not marked for strip
  if (expr.get_left_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_left_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed "
		   "before binary op exprs");
  if (expr.get_right_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_right_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::CompoundAssignmentExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * two direct descendant expressions, can strip ones below that */

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  auto &l_expr = expr.get_left_expr ();
  l_expr->accept_vis (*this);

  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  auto &r_expr = expr.get_right_expr ();
  r_expr->accept_vis (*this);

  // ensure that they are not marked for strip
  if (expr.get_left_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_left_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed "
		   "before binary op exprs");
  if (expr.get_right_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_right_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::GroupedExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip test based on inner attrs - spec says these are inner
   * attributes, not outer attributes of inner expr */
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &inner_expr = expr.get_expr_in_parens ();
  inner_expr->accept_vis (*this);
  if (inner_expr->is_marked_for_strip ())
    rust_error_at (inner_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ArrayElemsValues &elems)
{
  /* apparently outer attributes are allowed in "elements of array
   * expressions" according to spec */
  maybe_strip_pointer_allow_strip (elems.get_values ());
}
void
CfgStrip::visit (AST::ArrayElemsCopied &elems)
{
  /* apparently outer attributes are allowed in "elements of array
   * expressions" according to spec. on the other hand, it would not
   * make conceptual sense to be able to remove either expression. As
   * such, not implementing. TODO clear up the ambiguity here */

  // only intend stripping for internal sub-expressions
  auto &copied_expr = elems.get_elem_to_copy ();
  copied_expr->accept_vis (*this);
  if (copied_expr->is_marked_for_strip ())
    rust_error_at (copied_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  auto &copy_count = elems.get_num_copies ();
  copy_count->accept_vis (*this);
  if (copy_count->is_marked_for_strip ())
    rust_error_at (copy_count->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ArrayExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip test based on inner attrs - spec says there are separate
   * inner attributes, not just outer attributes of inner exprs */
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* assuming you can't strip away the ArrayElems type, but can strip
   * internal expressions and whatever */
  expr.get_array_elems ()->accept_vis (*this);
}
void
CfgStrip::visit (AST::ArrayIndexExpr &expr)
{
  /* it is unclear whether outer attributes are supposed to be
   * allowed, but conceptually it wouldn't make much sense, but
   * having expansion code anyway. TODO */
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &array_expr = expr.get_array_expr ();
  array_expr->accept_vis (*this);
  if (array_expr->is_marked_for_strip ())
    rust_error_at (array_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  auto &index_expr = expr.get_index_expr ();
  index_expr->accept_vis (*this);
  if (index_expr->is_marked_for_strip ())
    rust_error_at (index_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::TupleExpr &expr)
{
  /* according to spec, outer attributes are allowed on "elements of
   * tuple expressions" */

  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip test based on inner attrs - spec says these are inner
   * attributes, not outer attributes of inner expr */
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* apparently outer attributes are allowed in "elements of tuple
   * expressions" according to spec */
  maybe_strip_pointer_allow_strip (expr.get_tuple_elems ());
}
void
CfgStrip::visit (AST::TupleIndexExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* wouldn't strip this directly (as outer attrs should be
   * associated with this level), but any sub-expressions would be
   * stripped. Thus, no need to erase when strip check called. */
  auto &tuple_expr = expr.get_tuple_expr ();
  tuple_expr->accept_vis (*this);
  if (tuple_expr->is_marked_for_strip ())
    rust_error_at (tuple_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::StructExprStruct &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip test based on inner attrs - spec says these are inner
   * attributes, not outer attributes of inner expr */
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // strip sub-exprs of path
  auto &struct_name = expr.get_struct_name ();
  visit (struct_name);
  if (struct_name.is_marked_for_strip ())
    rust_error_at (struct_name.get_locus (),
		   "cannot strip path in this position");
}
void
CfgStrip::visit (AST::StructExprFieldIdentifier &)
{
  // as no attrs (at moment, at least), no stripping possible
}
void
CfgStrip::visit (AST::StructExprFieldIdentifierValue &field)
{
  /* as no attrs possible (at moment, at least), only sub-expression
   * stripping is possible */
  auto &value = field.get_value ();
  value->accept_vis (*this);
  if (value->is_marked_for_strip ())
    rust_error_at (value->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::StructExprFieldIndexValue &field)
{
  /* as no attrs possible (at moment, at least), only sub-expression
   * stripping is possible */
  auto &value = field.get_value ();
  value->accept_vis (*this);
  if (value->is_marked_for_strip ())
    rust_error_at (value->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::StructExprStructFields &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip test based on inner attrs - spec says these are inner
   * attributes, not outer attributes of inner expr */
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // strip sub-exprs of path
  auto &struct_name = expr.get_struct_name ();
  visit (struct_name);
  if (struct_name.is_marked_for_strip ())
    rust_error_at (struct_name.get_locus (),
		   "cannot strip path in this position");

  /* spec does not specify whether expressions are allowed to be
   * stripped at top level of struct fields, but I wouldn't think
   * that they would be, so operating under the assumption that only
   * sub-expressions can be stripped. */
  for (auto &field : expr.get_fields ())
    {
      field->accept_vis (*this);
      // shouldn't strip in this
    }

  /* struct base presumably can't be stripped, as the '..' is before
   * the expression. as such, can only strip sub-expressions. */
  if (expr.has_struct_base ())
    {
      auto &base_struct_expr = expr.get_struct_base ().get_base_struct ();
      base_struct_expr->accept_vis (*this);
      if (base_struct_expr->is_marked_for_strip ())
	rust_error_at (base_struct_expr->get_locus (),
		       "cannot strip expression in this position - outer "
		       "attributes not allowed");
    }
}
void
CfgStrip::visit (AST::StructExprStructBase &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip test based on inner attrs - spec says these are inner
   * attributes, not outer attributes of inner expr */
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // strip sub-exprs of path
  auto &struct_name = expr.get_struct_name ();
  visit (struct_name);
  if (struct_name.is_marked_for_strip ())
    rust_error_at (struct_name.get_locus (),
		   "cannot strip path in this position");

  /* struct base presumably can't be stripped, as the '..' is before
   * the expression. as such, can only strip sub-expressions. */
  rust_assert (!expr.get_struct_base ().is_invalid ());
  auto &base_struct_expr = expr.get_struct_base ().get_base_struct ();
  base_struct_expr->accept_vis (*this);
  if (base_struct_expr->is_marked_for_strip ())
    rust_error_at (base_struct_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::CallExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* should not be outer attrs on "function" expression - outer attrs
   * should be associated with call expr as a whole. only sub-expr
   * expansion is possible. */
  auto &function = expr.get_function_expr ();
  function->accept_vis (*this);
  if (function->is_marked_for_strip ())
    rust_error_at (function->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  /* spec says outer attributes are specifically allowed for elements
   * of call expressions, so full stripping possible */
  // FIXME: Arthur: Figure out how to refactor this - This is similar to
  // expanding items in the crate or stmts in blocks
  maybe_strip_pointer_allow_strip (expr.get_params ());
}
void
CfgStrip::visit (AST::MethodCallExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* should not be outer attrs on "receiver" expression - outer attrs
   * should be associated with call expr as a whole. only sub-expr
   * expansion is possible. */
  auto &receiver = expr.get_receiver_expr ();
  receiver->accept_vis (*this);
  if (receiver->is_marked_for_strip ())
    rust_error_at (receiver->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  auto &method_name = expr.get_method_name ();
  if (method_name.has_generic_args ())
    maybe_strip_generic_args (method_name.get_generic_args ());

  /* spec says outer attributes are specifically allowed for elements
   * of method call expressions, so full stripping possible */
  maybe_strip_pointer_allow_strip (expr.get_params ());
}
void
CfgStrip::visit (AST::FieldAccessExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* should not be outer attrs on "receiver" expression - outer attrs
   * should be associated with field expr as a whole. only sub-expr
   * expansion is possible. */
  auto &receiver = expr.get_receiver_expr ();
  receiver->accept_vis (*this);
  if (receiver->is_marked_for_strip ())
    rust_error_at (receiver->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ClosureExprInner &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip closure parameters if required - this is specifically
   * allowed by spec */
  maybe_strip_closure_params (expr.get_params ());

  // can't strip expression itself, but can strip sub-expressions
  auto &definition_expr = expr.get_definition_expr ();
  definition_expr->accept_vis (*this);
  if (definition_expr->is_marked_for_strip ())
    rust_error_at (definition_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}

void
CfgStrip::visit (AST::BlockExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip test based on inner attrs - spec says there are inner
   * attributes, not just outer attributes of inner stmts */
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  maybe_strip_pointer_allow_strip (expr.get_statements ());

  // strip tail expression if exists - can actually fully remove it
  if (expr.has_tail_expr ())
    {
      auto &tail_expr = expr.get_tail_expr ();

      tail_expr->accept_vis (*this);

      if (tail_expr->is_marked_for_strip ())
	expr.strip_tail_expr ();
    }
}

void
CfgStrip::visit (AST::ClosureExprInnerTyped &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* strip closure parameters if required - this is specifically
   * allowed by spec */
  maybe_strip_closure_params (expr.get_params ());

  // can't strip return type, but can strip sub-types
  auto &type = expr.get_return_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  // can't strip expression itself, but can strip sub-expressions
  auto &definition_block = expr.get_definition_block ();
  definition_block->accept_vis (*this);
  if (definition_block->is_marked_for_strip ())
    rust_error_at (definition_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ContinueExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }
}
void
CfgStrip::visit (AST::BreakExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* spec does not say that you can have outer attributes on
   * expression, so assuming you can't. stripping for sub-expressions
   * is the only thing that can be done */
  if (expr.has_break_expr ())
    {
      auto &break_expr = expr.get_break_expr ();

      break_expr->accept_vis (*this);

      if (break_expr->is_marked_for_strip ())
	rust_error_at (break_expr->get_locus (),
		       "cannot strip expression in this position - outer "
		       "attributes not allowed");
    }
}
void
CfgStrip::visit (AST::RangeFromToExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * two direct descendant expressions, can strip ones below that */

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  expr.get_from_expr ()->accept_vis (*this);
  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  expr.get_to_expr ()->accept_vis (*this);

  // ensure that they are not marked for strip
  if (expr.get_from_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_from_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed "
		   "before range exprs");
  if (expr.get_to_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_to_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::RangeFromExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * direct descendant expression, can strip ones below that */

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  auto &from_expr = expr.get_from_expr ();

  from_expr->accept_vis (*this);

  if (from_expr->is_marked_for_strip ())
    rust_error_at (from_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed before range exprs");
}
void
CfgStrip::visit (AST::RangeToExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * direct descendant expression, can strip ones below that */

  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  auto &to_expr = expr.get_to_expr ();

  to_expr->accept_vis (*this);

  if (to_expr->is_marked_for_strip ())
    rust_error_at (to_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::RangeFullExpr &)
{
  // outer attributes never allowed before these, so no stripping
}
void
CfgStrip::visit (AST::RangeFromToInclExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * two direct descendant expressions, can strip ones below that */

  /* should have no possibility for outer attrs as would be parsed
   * with outer expr */
  expr.get_from_expr ()->accept_vis (*this);
  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  expr.get_to_expr ()->accept_vis (*this);

  // ensure that they are not marked for strip
  if (expr.get_from_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_from_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes are never allowed "
		   "before range exprs");
  if (expr.get_to_expr ()->is_marked_for_strip ())
    rust_error_at (expr.get_to_expr ()->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::RangeToInclExpr &expr)
{
  /* outer attributes never allowed before these. while cannot strip
   * direct descendant expression, can strip ones below that */

  /* should syntactically not have outer attributes, though this may
   * not have worked in practice */
  auto &to_expr = expr.get_to_expr ();

  to_expr->accept_vis (*this);

  if (to_expr->is_marked_for_strip ())
    rust_error_at (to_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ReturnExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* spec does not say that you can have outer attributes on
   * expression, so assuming you can't. stripping for sub-expressions
   * is the only thing that can be done */
  if (expr.has_returned_expr ())
    {
      auto &returned_expr = expr.get_returned_expr ();

      returned_expr->accept_vis (*this);

      if (returned_expr->is_marked_for_strip ())
	rust_error_at (returned_expr->get_locus (),
		       "cannot strip expression in this position - outer "
		       "attributes not allowed");
    }
  /* TODO: conceptually, you would maybe be able to remove a returned
   * expr - e.g. if you had conditional compilation returning void or
   * returning a type. On the other hand, I think that function
   * return type cannot be conditionally compiled, so I assumed you
   * can't do this either. */
}
void
CfgStrip::visit (AST::UnsafeBlockExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // can't strip block itself, but can strip sub-expressions
  auto &block_expr = expr.get_block_expr ();
  block_expr->accept_vis (*this);
  if (block_expr->is_marked_for_strip ())
    rust_error_at (block_expr->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::LoopExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // can't strip block itself, but can strip sub-expressions
  auto &loop_block = expr.get_loop_block ();
  loop_block->accept_vis (*this);
  if (loop_block->is_marked_for_strip ())
    rust_error_at (loop_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::WhileLoopExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // can't strip predicate expr itself, but can strip sub-expressions
  auto &predicate_expr = expr.get_predicate_expr ();
  predicate_expr->accept_vis (*this);
  if (predicate_expr->is_marked_for_strip ())
    rust_error_at (predicate_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // can't strip block itself, but can strip sub-expressions
  auto &loop_block = expr.get_loop_block ();
  loop_block->accept_vis (*this);
  if (loop_block->is_marked_for_strip ())
    rust_error_at (loop_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::WhileLetLoopExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  for (auto &pattern : expr.get_patterns ())
    {
      pattern->accept_vis (*this);
      if (pattern->is_marked_for_strip ())
	rust_error_at (pattern->get_locus (),
		       "cannot strip pattern in this position");
    }

  // can't strip scrutinee expr itself, but can strip sub-expressions
  auto &scrutinee_expr = expr.get_scrutinee_expr ();
  scrutinee_expr->accept_vis (*this);
  if (scrutinee_expr->is_marked_for_strip ())
    rust_error_at (scrutinee_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // can't strip block itself, but can strip sub-expressions
  auto &loop_block = expr.get_loop_block ();
  loop_block->accept_vis (*this);
  if (loop_block->is_marked_for_strip ())
    rust_error_at (loop_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::ForLoopExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // strip sub-patterns of pattern
  auto &pattern = expr.get_pattern ();
  pattern->accept_vis (*this);
  if (pattern->is_marked_for_strip ())
    rust_error_at (pattern->get_locus (),
		   "cannot strip pattern in this position");

  // can't strip scrutinee expr itself, but can strip sub-expressions
  auto &iterator_expr = expr.get_iterator_expr ();
  iterator_expr->accept_vis (*this);
  if (iterator_expr->is_marked_for_strip ())
    rust_error_at (iterator_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // can't strip block itself, but can strip sub-expressions
  auto &loop_block = expr.get_loop_block ();
  loop_block->accept_vis (*this);
  if (loop_block->is_marked_for_strip ())
    rust_error_at (loop_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::IfExpr &expr)
{
  // rust playground test shows that IfExpr does support outer attrs, at least
  // when used as statement

  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // can't strip condition expr itself, but can strip sub-expressions
  auto &condition_expr = expr.get_condition_expr ();
  condition_expr->accept_vis (*this);
  if (condition_expr->is_marked_for_strip ())
    rust_error_at (condition_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // can't strip if block itself, but can strip sub-expressions
  auto &if_block = expr.get_if_block ();
  if_block->accept_vis (*this);
  if (if_block->is_marked_for_strip ())
    rust_error_at (if_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::IfExprConseqElse &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // can't strip condition expr itself, but can strip sub-expressions
  auto &condition_expr = expr.get_condition_expr ();
  condition_expr->accept_vis (*this);
  if (condition_expr->is_marked_for_strip ())
    rust_error_at (condition_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // can't strip if block itself, but can strip sub-expressions
  auto &if_block = expr.get_if_block ();
  if_block->accept_vis (*this);
  if (if_block->is_marked_for_strip ())
    rust_error_at (if_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");

  // can't strip else block itself, but can strip sub-expressions
  auto &else_block = expr.get_else_block ();
  else_block->accept_vis (*this);
  if (else_block->is_marked_for_strip ())
    rust_error_at (else_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::IfLetExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  for (auto &pattern : expr.get_patterns ())
    {
      pattern->accept_vis (*this);
      if (pattern->is_marked_for_strip ())
	rust_error_at (pattern->get_locus (),
		       "cannot strip pattern in this position");
    }

  // can't strip value expr itself, but can strip sub-expressions
  auto &value_expr = expr.get_value_expr ();
  value_expr->accept_vis (*this);
  if (value_expr->is_marked_for_strip ())
    rust_error_at (value_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // can't strip if block itself, but can strip sub-expressions
  auto &if_block = expr.get_if_block ();
  if_block->accept_vis (*this);
  if (if_block->is_marked_for_strip ())
    rust_error_at (if_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::IfLetExprConseqElse &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  for (auto &pattern : expr.get_patterns ())
    {
      pattern->accept_vis (*this);
      if (pattern->is_marked_for_strip ())
	rust_error_at (pattern->get_locus (),
		       "cannot strip pattern in this position");
    }

  // can't strip value expr itself, but can strip sub-expressions
  auto &value_expr = expr.get_value_expr ();
  value_expr->accept_vis (*this);
  if (value_expr->is_marked_for_strip ())
    rust_error_at (value_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // can't strip if block itself, but can strip sub-expressions
  auto &if_block = expr.get_if_block ();
  if_block->accept_vis (*this);
  if (if_block->is_marked_for_strip ())
    rust_error_at (if_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");

  // can't strip else block itself, but can strip sub-expressions
  auto &else_block = expr.get_else_block ();
  else_block->accept_vis (*this);
  if (else_block->is_marked_for_strip ())
    rust_error_at (else_block->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::MatchExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // inner attr strip test
  expand_cfg_attrs (expr.get_inner_attrs ());
  if (fails_cfg_with_expand (expr.get_inner_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // can't strip scrutinee expr itself, but can strip sub-expressions
  auto &scrutinee_expr = expr.get_scrutinee_expr ();
  scrutinee_expr->accept_vis (*this);
  if (scrutinee_expr->is_marked_for_strip ())
    rust_error_at (scrutinee_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");

  // strip match cases
  auto &match_cases = expr.get_match_cases ();
  for (auto it = match_cases.begin (); it != match_cases.end ();)
    {
      auto &match_case = *it;

      // strip match case based on outer attributes in match arm
      auto &match_arm = match_case.get_arm ();
      expand_cfg_attrs (match_arm.get_outer_attrs ());
      if (fails_cfg_with_expand (match_arm.get_outer_attrs ()))
	{
	  // strip match case
	  it = match_cases.erase (it);
	  continue;
	}

      for (auto &pattern : match_arm.get_patterns ())
	{
	  pattern->accept_vis (*this);
	  if (pattern->is_marked_for_strip ())
	    rust_error_at (pattern->get_locus (),
			   "cannot strip pattern in this position");
	}

      /* assuming that guard expression cannot be stripped as
       * strictly speaking you would have to strip the whole guard to
       * make syntactical sense, which you can't do. as such, only
       * strip sub-expressions */
      if (match_arm.has_match_arm_guard ())
	{
	  auto &guard_expr = match_arm.get_guard_expr ();
	  guard_expr->accept_vis (*this);
	  if (guard_expr->is_marked_for_strip ())
	    rust_error_at (guard_expr->get_locus (),
			   "cannot strip expression in this position - outer "
			   "attributes not allowed");
	}

      // strip sub-expressions from match cases
      auto &case_expr = match_case.get_expr ();
      case_expr->accept_vis (*this);
      if (case_expr->is_marked_for_strip ())
	rust_error_at (case_expr->get_locus (),
		       "cannot strip expression in this position - outer "
		       "attributes not allowed");

      // increment to next case if haven't continued
      ++it;
    }
}
void
CfgStrip::visit (AST::AwaitExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  /* can't strip awaited expr itself, but can strip sub-expressions
   * - this is because you can't have no expr to await */
  auto &awaited_expr = expr.get_awaited_expr ();
  awaited_expr->accept_vis (*this);
  if (awaited_expr->is_marked_for_strip ())
    rust_error_at (awaited_expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::AsyncBlockExpr &expr)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (expr.get_outer_attrs ());
  if (fails_cfg_with_expand (expr.get_outer_attrs ()))
    {
      expr.mark_for_strip ();
      return;
    }

  // can't strip block itself, but can strip sub-expressions
  auto &block_expr = expr.get_block_expr ();
  block_expr->accept_vis (*this);
  if (block_expr->is_marked_for_strip ())
    rust_error_at (block_expr->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}

void
CfgStrip::visit (AST::TypeParam &param)
{
  // outer attributes don't actually do anything, so ignore them

  if (param.has_type_param_bounds ())
    {
      // don't strip directly, only components of bounds
      for (auto &bound : param.get_type_param_bounds ())
	bound->accept_vis (*this);
    }

  if (param.has_type ())
    {
      auto &type = param.get_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");
    }
}
void
CfgStrip::visit (AST::LifetimeWhereClauseItem &)
{
  // shouldn't require
}
void
CfgStrip::visit (AST::TypeBoundWhereClauseItem &item)
{
  // for lifetimes shouldn't require

  auto &type = item.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  // don't strip directly, only components of bounds
  for (auto &bound : item.get_type_param_bounds ())
    bound->accept_vis (*this);
}
void
CfgStrip::visit (AST::Method &method)
{
  // initial test based on outer attrs
  expand_cfg_attrs (method.get_outer_attrs ());
  if (fails_cfg_with_expand (method.get_outer_attrs ()))
    {
      method.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : method.get_generic_params ())
    param->accept_vis (*this);

  /* assuming you can't strip self param - wouldn't be a method
   * anymore. spec allows outer attrs on self param, but doesn't
   * specify whether cfg is used. */
  maybe_strip_self_param (method.get_self_param ());

  /* strip method parameters if required - this is specifically
   * allowed by spec */
  maybe_strip_function_params (method.get_function_params ());

  if (method.has_return_type ())
    {
      auto &return_type = method.get_return_type ();
      return_type->accept_vis (*this);

      if (return_type->is_marked_for_strip ())
	rust_error_at (return_type->get_locus (),
		       "cannot strip type in this position");
    }

  if (method.has_where_clause ())
    maybe_strip_where_clause (method.get_where_clause ());

  /* body should always exist - if error state, should have returned
   * before now */
  // can't strip block itself, but can strip sub-expressions
  auto &block_expr = method.get_definition ();
  block_expr->accept_vis (*this);
  if (block_expr->is_marked_for_strip ())
    rust_error_at (block_expr->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::Module &module)
{
  // strip test based on outer attrs
  expand_cfg_attrs (module.get_outer_attrs ());
  if (fails_cfg_with_expand (module.get_outer_attrs ()))
    {
      module.mark_for_strip ();
      return;
    }

  // A loaded module might have inner attributes
  if (module.get_kind () == AST::Module::ModuleKind::LOADED)
    {
      // strip test based on inner attrs
      expand_cfg_attrs (module.get_inner_attrs ());
      if (fails_cfg_with_expand (module.get_inner_attrs ()))
	{
	  module.mark_for_strip ();
	  return;
	}
    }

  // strip items if required
  maybe_strip_pointer_allow_strip (module.get_items ());
}
void
CfgStrip::visit (AST::ExternCrate &extern_crate)
{
  // strip test based on outer attrs
  expand_cfg_attrs (extern_crate.get_outer_attrs ());
  if (fails_cfg_with_expand (extern_crate.get_outer_attrs ()))
    {
      extern_crate.mark_for_strip ();
      return;
    }

  if (!extern_crate.references_self ())
    {
      Session &session = Session::get_instance ();
      session.load_extern_crate (extern_crate.get_referenced_crate (),
				 extern_crate.get_locus ());
    }
}
void
CfgStrip::visit (AST::UseTreeGlob &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::UseTreeList &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::UseTreeRebind &)
{
  // shouldn't require?
}
void
CfgStrip::visit (AST::UseDeclaration &use_decl)
{
  // strip test based on outer attrs
  expand_cfg_attrs (use_decl.get_outer_attrs ());
  if (fails_cfg_with_expand (use_decl.get_outer_attrs ()))
    {
      use_decl.mark_for_strip ();
      return;
    }
}
void
CfgStrip::visit (AST::Function &function)
{
  // initial test based on outer attrs
  expand_cfg_attrs (function.get_outer_attrs ());
  if (fails_cfg_with_expand (function.get_outer_attrs ()))
    {
      function.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : function.get_generic_params ())
    param->accept_vis (*this);

  /* strip function parameters if required - this is specifically
   * allowed by spec */
  maybe_strip_function_params (function.get_function_params ());

  if (function.has_return_type ())
    {
      auto &return_type = function.get_return_type ();
      return_type->accept_vis (*this);

      if (return_type->is_marked_for_strip ())
	rust_error_at (return_type->get_locus (),
		       "cannot strip type in this position");
    }

  if (function.has_where_clause ())
    maybe_strip_where_clause (function.get_where_clause ());

  /* body should always exist - if error state, should have returned
   * before now */
  // can't strip block itself, but can strip sub-expressions
  auto &block_expr = function.get_definition ();
  block_expr->accept_vis (*this);
  if (block_expr->is_marked_for_strip ())
    rust_error_at (block_expr->get_locus (),
		   "cannot strip block expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::TypeAlias &type_alias)
{
  // initial test based on outer attrs
  expand_cfg_attrs (type_alias.get_outer_attrs ());
  if (fails_cfg_with_expand (type_alias.get_outer_attrs ()))
    {
      type_alias.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : type_alias.get_generic_params ())
    param->accept_vis (*this);

  if (type_alias.has_where_clause ())
    maybe_strip_where_clause (type_alias.get_where_clause ());

  auto &type = type_alias.get_type_aliased ();
  type->accept_vis (*this);
  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");
}
void
CfgStrip::visit (AST::StructStruct &struct_item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (struct_item.get_outer_attrs ());
  if (fails_cfg_with_expand (struct_item.get_outer_attrs ()))
    {
      struct_item.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : struct_item.get_generic_params ())
    param->accept_vis (*this);

  if (struct_item.has_where_clause ())
    maybe_strip_where_clause (struct_item.get_where_clause ());
}
void
CfgStrip::visit (AST::TupleStruct &tuple_struct)
{
  // initial test based on outer attrs
  expand_cfg_attrs (tuple_struct.get_outer_attrs ());
  if (fails_cfg_with_expand (tuple_struct.get_outer_attrs ()))
    {
      tuple_struct.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : tuple_struct.get_generic_params ())
    param->accept_vis (*this);

  /* strip struct fields if required - this is presumably
   * allowed by spec */
  maybe_strip_tuple_fields (tuple_struct.get_fields ());

  if (tuple_struct.has_where_clause ())
    maybe_strip_where_clause (tuple_struct.get_where_clause ());
}
void
CfgStrip::visit (AST::EnumItem &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }
}
void
CfgStrip::visit (AST::EnumItemTuple &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  /* strip item fields if required - this is presumably
   * allowed by spec */
  maybe_strip_tuple_fields (item.get_tuple_fields ());
}
void
CfgStrip::visit (AST::EnumItemStruct &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  /* strip item fields if required - this is presumably
   * allowed by spec */
  maybe_strip_struct_fields (item.get_struct_fields ());
}
void
CfgStrip::visit (AST::EnumItemDiscriminant &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &expr = item.get_expr ();
  expr->accept_vis (*this);
  if (expr->is_marked_for_strip ())
    rust_error_at (expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::Enum &enum_item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (enum_item.get_outer_attrs ());
  if (fails_cfg_with_expand (enum_item.get_outer_attrs ()))
    {
      enum_item.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : enum_item.get_generic_params ())
    param->accept_vis (*this);

  if (enum_item.has_where_clause ())
    maybe_strip_where_clause (enum_item.get_where_clause ());

  /* strip enum fields if required - this is presumably
   * allowed by spec */
  maybe_strip_pointer_allow_strip (enum_item.get_variants ());
}
void
CfgStrip::visit (AST::Union &union_item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (union_item.get_outer_attrs ());
  if (fails_cfg_with_expand (union_item.get_outer_attrs ()))
    {
      union_item.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : union_item.get_generic_params ())
    param->accept_vis (*this);

  if (union_item.has_where_clause ())
    maybe_strip_where_clause (union_item.get_where_clause ());

  /* strip union fields if required - this is presumably
   * allowed by spec */
  maybe_strip_struct_fields (union_item.get_variants ());
}
void
CfgStrip::visit (AST::ConstantItem &const_item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (const_item.get_outer_attrs ());
  if (fails_cfg_with_expand (const_item.get_outer_attrs ()))
    {
      const_item.mark_for_strip ();
      return;
    }

  // strip any sub-types
  auto &type = const_item.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &expr = const_item.get_expr ();
  expr->accept_vis (*this);
  if (expr->is_marked_for_strip ())
    rust_error_at (expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::StaticItem &static_item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (static_item.get_outer_attrs ());
  if (fails_cfg_with_expand (static_item.get_outer_attrs ()))
    {
      static_item.mark_for_strip ();
      return;
    }

  // strip any sub-types
  auto &type = static_item.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped. */
  auto &expr = static_item.get_expr ();
  expr->accept_vis (*this);
  if (expr->is_marked_for_strip ())
    rust_error_at (expr->get_locus (),
		   "cannot strip expression in this position - outer "
		   "attributes not allowed");
}
void
CfgStrip::visit (AST::TraitItemFunc &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  maybe_strip_trait_function_decl (item.get_trait_function_decl ());

  if (item.has_definition ())
    {
      /* strip any internal sub-expressions - expression itself isn't
       * allowed to have external attributes in this position so can't be
       * stripped. */
      auto &block = item.get_definition ();
      block->accept_vis (*this);
      if (block->is_marked_for_strip ())
	rust_error_at (block->get_locus (),
		       "cannot strip block expression in this "
		       "position - outer attributes not allowed");
    }
}
void
CfgStrip::visit (AST::TraitItemMethod &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  maybe_strip_trait_method_decl (item.get_trait_method_decl ());

  if (item.has_definition ())
    {
      /* strip any internal sub-expressions - expression itself isn't
       * allowed to have external attributes in this position so can't be
       * stripped. */
      auto &block = item.get_definition ();
      block->accept_vis (*this);
      if (block->is_marked_for_strip ())
	rust_error_at (block->get_locus (),
		       "cannot strip block expression in this "
		       "position - outer attributes not allowed");
    }
}
void
CfgStrip::visit (AST::TraitItemConst &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  // strip any sub-types
  auto &type = item.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped */
  if (item.has_expression ())
    {
      auto &expr = item.get_expr ();
      expr->accept_vis (*this);
      if (expr->is_marked_for_strip ())
	rust_error_at (expr->get_locus (),
		       "cannot strip expression in this position - outer "
		       "attributes not allowed");
    }
}
void
CfgStrip::visit (AST::TraitItemType &item)
{
  // initial test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  if (item.has_type_param_bounds ())
    {
      // don't strip directly, only components of bounds
      for (auto &bound : item.get_type_param_bounds ())
	bound->accept_vis (*this);
    }
}
void
CfgStrip::visit (AST::Trait &trait)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (trait.get_outer_attrs ());
  if (fails_cfg_with_expand (trait.get_outer_attrs ()))
    {
      trait.mark_for_strip ();
      return;
    }

  // strip test based on inner attrs
  expand_cfg_attrs (trait.get_inner_attrs ());
  if (fails_cfg_with_expand (trait.get_inner_attrs ()))
    {
      trait.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : trait.get_generic_params ())
    param->accept_vis (*this);

  if (trait.has_type_param_bounds ())
    {
      // don't strip directly, only components of bounds
      for (auto &bound : trait.get_type_param_bounds ())
	bound->accept_vis (*this);
    }

  if (trait.has_where_clause ())
    maybe_strip_where_clause (trait.get_where_clause ());

  maybe_strip_pointer_allow_strip (trait.get_trait_items ());
}
void
CfgStrip::visit (AST::InherentImpl &impl)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (impl.get_outer_attrs ());
  if (fails_cfg_with_expand (impl.get_outer_attrs ()))
    {
      impl.mark_for_strip ();
      return;
    }

  // strip test based on inner attrs
  expand_cfg_attrs (impl.get_inner_attrs ());
  if (fails_cfg_with_expand (impl.get_inner_attrs ()))
    {
      impl.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : impl.get_generic_params ())
    param->accept_vis (*this);

  auto &type = impl.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  if (impl.has_where_clause ())
    maybe_strip_where_clause (impl.get_where_clause ());

  maybe_strip_pointer_allow_strip (impl.get_impl_items ());
}
void
CfgStrip::visit (AST::TraitImpl &impl)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (impl.get_outer_attrs ());
  if (fails_cfg_with_expand (impl.get_outer_attrs ()))
    {
      impl.mark_for_strip ();
      return;
    }

  // strip test based on inner attrs
  expand_cfg_attrs (impl.get_inner_attrs ());
  if (fails_cfg_with_expand (impl.get_inner_attrs ()))
    {
      impl.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : impl.get_generic_params ())
    param->accept_vis (*this);

  auto &type = impl.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");

  auto &trait_path = impl.get_trait_path ();
  visit (trait_path);
  if (trait_path.is_marked_for_strip ())
    rust_error_at (trait_path.get_locus (),
		   "cannot strip typepath in this position");

  if (impl.has_where_clause ())
    maybe_strip_where_clause (impl.get_where_clause ());

  maybe_strip_pointer_allow_strip (impl.get_impl_items ());
}

void
CfgStrip::visit (AST::ExternalTypeItem &item)
{
  expand_cfg_attrs (item.get_outer_attrs ());

  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    item.mark_for_strip ();

  // TODO: Can we do anything like expand a macro here?
  // extern "C" { type ffi_ty!(); }
  // ?
}

void
CfgStrip::visit (AST::ExternalStaticItem &item)
{
  // strip test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  auto &type = item.get_type ();
  type->accept_vis (*this);

  if (type->is_marked_for_strip ())
    rust_error_at (type->get_locus (), "cannot strip type in this position");
}

void
CfgStrip::visit (AST::ExternalFunctionItem &item)
{
  // strip test based on outer attrs
  expand_cfg_attrs (item.get_outer_attrs ());
  if (fails_cfg_with_expand (item.get_outer_attrs ()))
    {
      item.mark_for_strip ();
      return;
    }

  // just expand sub-stuff - can't actually strip generic params themselves
  for (auto &param : item.get_generic_params ())
    param->accept_vis (*this);

  /* strip function parameters if required - this is specifically
   * allowed by spec */
  auto &params = item.get_function_params ();
  for (auto it = params.begin (); it != params.end ();)
    {
      auto &param = *it;

      auto &param_attrs = param.get_outer_attrs ();
      expand_cfg_attrs (param_attrs);
      if (fails_cfg_with_expand (param_attrs))
	{
	  it = params.erase (it);
	  continue;
	}

      auto &type = param.get_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");

      // increment if nothing else happens
      ++it;
    }
  /* NOTE: these are extern function params, which may have different
   * rules and restrictions to "normal" function params. So expansion
   * handled separately. */

  /* TODO: assuming that variadic nature cannot be stripped. If this
   * is not true, then have code here to do so. */

  if (item.has_return_type ())
    {
      auto &return_type = item.get_return_type ();
      return_type->accept_vis (*this);

      if (return_type->is_marked_for_strip ())
	rust_error_at (return_type->get_locus (),
		       "cannot strip type in this position");
    }

  if (item.has_where_clause ())
    maybe_strip_where_clause (item.get_where_clause ());
}

void
CfgStrip::visit (AST::ExternBlock &block)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (block.get_outer_attrs ());
  if (fails_cfg_with_expand (block.get_outer_attrs ()))
    {
      block.mark_for_strip ();
      return;
    }

  // strip test based on inner attrs
  expand_cfg_attrs (block.get_inner_attrs ());
  if (fails_cfg_with_expand (block.get_inner_attrs ()))
    {
      block.mark_for_strip ();
      return;
    }

  maybe_strip_pointer_allow_strip (block.get_extern_items ());
}

// I don't think it would be possible to strip macros without expansion
void
CfgStrip::visit (AST::MacroMatchFragment &)
{}
void
CfgStrip::visit (AST::MacroMatchRepetition &)
{}
void
CfgStrip::visit (AST::MacroMatcher &)
{}
void
CfgStrip::visit (AST::MacroRulesDefinition &rules_def)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (rules_def.get_outer_attrs ());
  if (fails_cfg_with_expand (rules_def.get_outer_attrs ()))
    {
      rules_def.mark_for_strip ();
      return;
    }
}

void
CfgStrip::visit (AST::MetaItemPath &)
{}
void
CfgStrip::visit (AST::MetaItemSeq &)
{}
void
CfgStrip::visit (AST::MetaWord &)
{}
void
CfgStrip::visit (AST::MetaNameValueStr &)
{}
void
CfgStrip::visit (AST::MetaListPaths &)
{}
void
CfgStrip::visit (AST::MetaListNameValueStr &)
{}

void
CfgStrip::visit (AST::LiteralPattern &)
{
  // not possible
}
void
CfgStrip::visit (AST::IdentifierPattern &pattern)
{
  // can only strip sub-patterns of the inner pattern to bind
  if (!pattern.has_pattern_to_bind ())
    return;

  auto &sub_pattern = pattern.get_pattern_to_bind ();
  sub_pattern->accept_vis (*this);
  if (sub_pattern->is_marked_for_strip ())
    rust_error_at (sub_pattern->get_locus (),
		   "cannot strip pattern in this position");
}
void
CfgStrip::visit (AST::WildcardPattern &)
{
  // not possible
}
void
CfgStrip::visit (AST::RestPattern &)
{
  // not possible
}
void
CfgStrip::visit (AST::RangePatternBoundLiteral &)
{
  // not possible
}
void
CfgStrip::visit (AST::RangePatternBoundPath &bound)
{
  // can expand path, but not strip it directly
  auto &path = bound.get_path ();
  visit (path);
  if (path.is_marked_for_strip ())
    rust_error_at (path.get_locus (), "cannot strip path in this position");
}
void
CfgStrip::visit (AST::RangePatternBoundQualPath &bound)
{
  // can expand path, but not strip it directly
  auto &path = bound.get_qualified_path ();
  visit (path);
  if (path.is_marked_for_strip ())
    rust_error_at (path.get_locus (), "cannot strip path in this position");
}
void
CfgStrip::visit (AST::RangePattern &pattern)
{
  // should have no capability to strip lower or upper bounds, only expand
  pattern.get_lower_bound ()->accept_vis (*this);
  pattern.get_upper_bound ()->accept_vis (*this);
}
void
CfgStrip::visit (AST::ReferencePattern &pattern)
{
  auto &sub_pattern = pattern.get_referenced_pattern ();
  sub_pattern->accept_vis (*this);
  if (sub_pattern->is_marked_for_strip ())
    rust_error_at (sub_pattern->get_locus (),
		   "cannot strip pattern in this position");
}
void
CfgStrip::visit (AST::StructPatternFieldTuplePat &field)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (field.get_outer_attrs ());
  if (fails_cfg_with_expand (field.get_outer_attrs ()))
    {
      field.mark_for_strip ();
      return;
    }

  // strip sub-patterns (can't strip top-level pattern)
  auto &sub_pattern = field.get_index_pattern ();
  sub_pattern->accept_vis (*this);
  if (sub_pattern->is_marked_for_strip ())
    rust_error_at (sub_pattern->get_locus (),
		   "cannot strip pattern in this position");
}
void
CfgStrip::visit (AST::StructPatternFieldIdentPat &field)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (field.get_outer_attrs ());
  if (fails_cfg_with_expand (field.get_outer_attrs ()))
    {
      field.mark_for_strip ();
      return;
    }

  // strip sub-patterns (can't strip top-level pattern)
  auto &sub_pattern = field.get_ident_pattern ();
  sub_pattern->accept_vis (*this);
  if (sub_pattern->is_marked_for_strip ())
    rust_error_at (sub_pattern->get_locus (),
		   "cannot strip pattern in this position");
}
void
CfgStrip::visit (AST::StructPatternFieldIdent &field)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (field.get_outer_attrs ());
  if (fails_cfg_with_expand (field.get_outer_attrs ()))
    {
      field.mark_for_strip ();
      return;
    }
}
void
CfgStrip::visit (AST::StructPattern &pattern)
{
  // expand (but don't strip) path
  auto &path = pattern.get_path ();
  visit (path);
  if (path.is_marked_for_strip ())
    rust_error_at (path.get_locus (), "cannot strip path in this position");

  /* TODO: apparently struct pattern fields can have outer attrs. so can they
   * be stripped? */
  if (!pattern.has_struct_pattern_elems ())
    return;

  auto &elems = pattern.get_struct_pattern_elems ();

  // assuming you can strip struct pattern fields
  maybe_strip_pointer_allow_strip (elems.get_struct_pattern_fields ());

  // assuming you can strip the ".." part
  if (elems.has_etc ())
    {
      expand_cfg_attrs (elems.get_etc_outer_attrs ());
      if (fails_cfg_with_expand (elems.get_etc_outer_attrs ()))
	elems.strip_etc ();
    }
}
void
CfgStrip::visit (AST::TupleStructItemsNoRange &tuple_items)
{
  // can't strip individual patterns, only sub-patterns
  for (auto &pattern : tuple_items.get_patterns ())
    {
      pattern->accept_vis (*this);

      if (pattern->is_marked_for_strip ())
	rust_error_at (pattern->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
}
void
CfgStrip::visit (AST::TupleStructItemsRange &tuple_items)
{
  // can't strip individual patterns, only sub-patterns
  for (auto &lower_pattern : tuple_items.get_lower_patterns ())
    {
      lower_pattern->accept_vis (*this);

      if (lower_pattern->is_marked_for_strip ())
	rust_error_at (lower_pattern->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
  for (auto &upper_pattern : tuple_items.get_upper_patterns ())
    {
      upper_pattern->accept_vis (*this);

      if (upper_pattern->is_marked_for_strip ())
	rust_error_at (upper_pattern->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
}
void
CfgStrip::visit (AST::TupleStructPattern &pattern)
{
  // expand (but don't strip) path
  auto &path = pattern.get_path ();
  visit (path);
  if (path.is_marked_for_strip ())
    rust_error_at (path.get_locus (), "cannot strip path in this position");

  if (pattern.has_items ())
    pattern.get_items ()->accept_vis (*this);
}
void
CfgStrip::visit (AST::TuplePatternItemsMultiple &tuple_items)
{
  // can't strip individual patterns, only sub-patterns
  for (auto &pattern : tuple_items.get_patterns ())
    {
      pattern->accept_vis (*this);

      if (pattern->is_marked_for_strip ())
	rust_error_at (pattern->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
}
void
CfgStrip::visit (AST::TuplePatternItemsRanged &tuple_items)
{
  // can't strip individual patterns, only sub-patterns
  for (auto &lower_pattern : tuple_items.get_lower_patterns ())
    {
      lower_pattern->accept_vis (*this);

      if (lower_pattern->is_marked_for_strip ())
	rust_error_at (lower_pattern->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
  for (auto &upper_pattern : tuple_items.get_upper_patterns ())
    {
      upper_pattern->accept_vis (*this);

      if (upper_pattern->is_marked_for_strip ())
	rust_error_at (upper_pattern->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
}
void
CfgStrip::visit (AST::TuplePattern &pattern)
{
  if (pattern.has_tuple_pattern_items ())
    pattern.get_items ()->accept_vis (*this);
}
void
CfgStrip::visit (AST::GroupedPattern &pattern)
{
  // can't strip inner pattern, only sub-patterns
  auto &pattern_in_parens = pattern.get_pattern_in_parens ();

  pattern_in_parens->accept_vis (*this);

  if (pattern_in_parens->is_marked_for_strip ())
    rust_error_at (pattern_in_parens->get_locus (),
		   "cannot strip pattern in this position");
}
void
CfgStrip::visit (AST::SlicePattern &pattern)
{
  // can't strip individual patterns, only sub-patterns
  for (auto &item : pattern.get_items ())
    {
      item->accept_vis (*this);

      if (item->is_marked_for_strip ())
	rust_error_at (item->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
}
void
CfgStrip::visit (AST::AltPattern &pattern)
{
  // can't strip individual patterns, only sub-patterns
  for (auto &alt : pattern.get_alts ())
    {
      alt->accept_vis (*this);

      if (alt->is_marked_for_strip ())
	rust_error_at (alt->get_locus (),
		       "cannot strip pattern in this position");
      // TODO: quit stripping now? or keep going?
    }
}

void
CfgStrip::visit (AST::EmptyStmt &)
{
  // assuming no outer attributes, so nothing can happen
}
void
CfgStrip::visit (AST::LetStmt &stmt)
{
  // initial strip test based on outer attrs
  expand_cfg_attrs (stmt.get_outer_attrs ());
  if (fails_cfg_with_expand (stmt.get_outer_attrs ()))
    {
      stmt.mark_for_strip ();
      return;
    }

  // can't strip pattern, but call for sub-patterns
  auto &pattern = stmt.get_pattern ();
  pattern->accept_vis (*this);
  if (pattern->is_marked_for_strip ())
    rust_error_at (pattern->get_locus (),
		   "cannot strip pattern in this position");

  // similar for type
  if (stmt.has_type ())
    {
      auto &type = stmt.get_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");
    }

  /* strip any internal sub-expressions - expression itself isn't
   * allowed to have external attributes in this position so can't be
   * stripped */
  if (stmt.has_init_expr ())
    {
      auto &init_expr = stmt.get_init_expr ();
      init_expr->accept_vis (*this);

      if (init_expr->is_marked_for_strip ())
	rust_error_at (init_expr->get_locus (),
		       "cannot strip expression in this position - outer "
		       "attributes not allowed");
    }
}
void
CfgStrip::visit (AST::ExprStmt &stmt)
{
  // outer attributes associated with expr, so rely on expr

  // guard - should prevent null pointer expr
  if (stmt.is_marked_for_strip ())
    return;

  // strip if expr is to be stripped
  auto &expr = stmt.get_expr ();
  expr->accept_vis (*this);
  if (expr->is_marked_for_strip ())
    {
      stmt.mark_for_strip ();
      return;
    }
}

void
CfgStrip::visit (AST::TraitBound &bound)
{
  // nothing in for lifetimes to strip

  // expand but don't strip type path
  auto &path = bound.get_type_path ();
  visit (path);
  if (path.is_marked_for_strip ())
    rust_error_at (path.get_locus (),
		   "cannot strip type path in this position");
}
void
CfgStrip::visit (AST::ImplTraitType &type)
{
  // don't strip directly, only components of bounds
  for (auto &bound : type.get_type_param_bounds ())
    bound->accept_vis (*this);
}
void
CfgStrip::visit (AST::TraitObjectType &type)
{
  // don't strip directly, only components of bounds
  for (auto &bound : type.get_type_param_bounds ())
    bound->accept_vis (*this);
}
void
CfgStrip::visit (AST::ParenthesisedType &type)
{
  // expand but don't strip inner type
  auto &inner_type = type.get_type_in_parens ();
  inner_type->accept_vis (*this);
  if (inner_type->is_marked_for_strip ())
    rust_error_at (inner_type->get_locus (),
		   "cannot strip type in this position");
}
void
CfgStrip::visit (AST::ImplTraitTypeOneBound &type)
{
  // no stripping possible
  visit (type.get_trait_bound ());
}
void
CfgStrip::visit (AST::TraitObjectTypeOneBound &type)
{
  // no stripping possible
  visit (type.get_trait_bound ());
}
void
CfgStrip::visit (AST::TupleType &type)
{
  // TODO: assuming that types can't be stripped as types don't have outer
  // attributes
  for (auto &elem_type : type.get_elems ())
    {
      elem_type->accept_vis (*this);
      if (elem_type->is_marked_for_strip ())
	rust_error_at (elem_type->get_locus (),
		       "cannot strip type in this position");
    }
}
void
CfgStrip::visit (AST::NeverType &)
{
  // no stripping possible
}
void
CfgStrip::visit (AST::RawPointerType &type)
{
  // expand but don't strip type pointed to
  auto &pointed_type = type.get_type_pointed_to ();
  pointed_type->accept_vis (*this);
  if (pointed_type->is_marked_for_strip ())
    rust_error_at (pointed_type->get_locus (),
		   "cannot strip type in this position");
}
void
CfgStrip::visit (AST::ReferenceType &type)
{
  // expand but don't strip type referenced
  auto &referenced_type = type.get_type_referenced ();
  referenced_type->accept_vis (*this);
  if (referenced_type->is_marked_for_strip ())
    rust_error_at (referenced_type->get_locus (),
		   "cannot strip type in this position");
}
void
CfgStrip::visit (AST::ArrayType &type)
{
  // expand but don't strip type referenced
  auto &base_type = type.get_elem_type ();
  base_type->accept_vis (*this);
  if (base_type->is_marked_for_strip ())
    rust_error_at (base_type->get_locus (),
		   "cannot strip type in this position");

  // same for expression
  auto &size_expr = type.get_size_expr ();
  size_expr->accept_vis (*this);
  if (size_expr->is_marked_for_strip ())
    rust_error_at (size_expr->get_locus (),
		   "cannot strip expression in this position");
}
void
CfgStrip::visit (AST::SliceType &type)
{
  // expand but don't strip elem type
  auto &elem_type = type.get_elem_type ();
  elem_type->accept_vis (*this);
  if (elem_type->is_marked_for_strip ())
    rust_error_at (elem_type->get_locus (),
		   "cannot strip type in this position");
}
void
CfgStrip::visit (AST::InferredType &)
{
  // none possible
}
void
CfgStrip::visit (AST::BareFunctionType &type)
{
  // seem to be no generics

  // presumably function params can be stripped
  auto &params = type.get_function_params ();
  for (auto it = params.begin (); it != params.end ();)
    {
      auto &param = *it;

      auto &param_attrs = param.get_outer_attrs ();
      expand_cfg_attrs (param_attrs);
      if (fails_cfg_with_expand (param_attrs))
	{
	  it = params.erase (it);
	  continue;
	}

      auto &type = param.get_type ();
      type->accept_vis (*this);

      if (type->is_marked_for_strip ())
	rust_error_at (type->get_locus (),
		       "cannot strip type in this position");

      // increment if nothing else happens
      ++it;
    }

  /* TODO: assuming that variadic nature cannot be stripped. If this
   * is not true, then have code here to do so. */

  if (type.has_return_type ())
    {
      // FIXME: Can we have type expansion in this position?
      // In that case, we need to handle AST::TypeNoBounds on top of just
      // AST::Types
      auto &return_type = type.get_return_type ();
      return_type->accept_vis (*this);
      if (return_type->is_marked_for_strip ())
	rust_error_at (return_type->get_locus (),
		       "cannot strip type in this position");
    }

  // no where clause, apparently
}

} // namespace Rust
