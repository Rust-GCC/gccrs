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
#include "rust-ast-tokenstream.h"

namespace Rust {
namespace AST {

TokenStream::TokenStream (std::vector<TokenPtr> &container) : tokens (container)
{}

std::vector<TokenPtr>
TokenStream::collect_tokens () const
{
  return tokens;
}

void
TokenStream::visit (AST::Crate &crate)
{
  visit_items_as_lines (crate.inner_attrs);
  visit_items_as_lines (crate.items);
}

void
TokenStream::visit (AST::Item &item)
{
  item.accept_vis (*this);
}

template <typename T>
void
TokenStream::visit (std::unique_ptr<T> &node)
{
  node->accept_vis (*this);
}

template <typename T>
void
TokenStream::visit (T &node)
{
  node.accept_vis (*this);
}

template <typename T>
void
TokenStream::visit_items_joined_by_separator (T &collection, TokenId separator,
					      size_t start_offset,
					      size_t end_offset)
{
  if (collection.size () > start_offset)
    {
      visit (collection.at (start_offset));
      auto size = collection.size () - end_offset;
      for (size_t i = start_offset + 1; i < size; i++)
	{
	  tokens.push_back (Rust::Token::make (separator, Location ()));
	  visit (collection.at (i));
	}
    }
}

template <typename T>
void
TokenStream::visit_as_line (T &item, std::vector<TokenPtr> trailing)
{
  indentation ();
  visit (item);
  for (auto &token : trailing)
    tokens.push_back (token);
  newline ();
}

template <typename T>
void
TokenStream::visit_items_as_lines (T &collection,
				   std::vector<TokenPtr> trailing)
{
  for (auto &item : collection)
    visit_as_line (item, trailing);
}

template <typename T>
void
TokenStream::visit_items_as_block (T &collection,
				   std::vector<TokenPtr> trailing,
				   TokenId left_brace, TokenId right_brace)
{
  tokens.push_back (Rust::Token::make (left_brace, Location ()));
  if (collection.empty ())
    {
      tokens.push_back (Rust::Token::make (right_brace, Location ()));
      newline ();
    }
  else
    {
      newline ();
      increment_indentation ();
      visit_items_as_lines (collection, trailing);
      decrement_indentation ();
      indentation ();
      tokens.push_back (Rust::Token::make (right_brace, Location ()));
      newline ();
    }
}

void
TokenStream::trailing_comma ()
{
  if (output_trailing_commas)
    {
      tokens.push_back (Rust::Token::make (COMMA, Location ()));
    }
}

void
TokenStream::newline ()
{}

void
TokenStream::indentation ()
{}

void
TokenStream::increment_indentation ()
{}

void
TokenStream::decrement_indentation ()
{}

void
TokenStream::visit (FunctionParam &param)
{
  visit (param.get_pattern ());
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (param.get_type ());
}

void
TokenStream::visit (Attribute &attrib)
{
  tokens.push_back (Rust::Token::make (HASH, attrib.get_locus ()));
  if (attrib.is_inner_attribute ())
    tokens.push_back (Rust::Token::make (EXCLAM, Location ()));
  tokens.push_back (Rust::Token::make (LEFT_SQUARE, Location ()));
  visit (attrib.get_path ());

  if (attrib.has_attr_input ())
    {
      switch (attrib.get_attr_input ().get_attr_input_type ())
	{
	  case AST::AttrInput::AttrInputType::LITERAL: {
	    visit (static_cast<AttrInputLiteral &> (attrib.get_attr_input ()));
	    break;
	  }
	  case AST::AttrInput::AttrInputType::META_ITEM: {
	    visit (static_cast<AttrInputMetaItemContainer &> (
	      attrib.get_attr_input ()));
	    break;
	  }
	  case AST::AttrInput::AttrInputType::TOKEN_TREE: {
	    visit (static_cast<DelimTokenTree &> (attrib.get_attr_input ()));
	    break;
	  }
	default:
	  gcc_unreachable ();
	}
    }
  tokens.push_back (Rust::Token::make (RIGHT_SQUARE, Location ()));
}

void
TokenStream::visit (SimplePath &path)
{
  if (path.has_opening_scope_resolution ())
    {
      tokens.push_back (
	Rust::Token::make (SCOPE_RESOLUTION, path.get_locus ()));
    }
  visit_items_joined_by_separator (path.get_segments (), SCOPE_RESOLUTION);
}

void
TokenStream::visit (SimplePathSegment &segment)
{
  auto name = segment.get_segment_name ();
  if (segment.is_crate_path_seg ())
    {
      tokens.push_back (Rust::Token::make (CRATE, segment.get_locus ()));
    }
  else if (segment.is_super_path_seg ())
    {
      tokens.push_back (Rust::Token::make (SUPER, segment.get_locus ()));
    }
  else if (segment.is_lower_self ())
    {
      tokens.push_back (Rust::Token::make (SELF, segment.get_locus ()));
    }
  else if (segment.is_big_self ())
    {
      tokens.push_back (Rust::Token::make (SELF_ALIAS, segment.get_locus ()));
    }
  else
    {
      tokens.push_back (
	Rust::Token::make_identifier (segment.get_locus (), std::move (name)));
    }
}

void
TokenStream::visit (Visibility &vis)
{
  switch (vis.get_vis_type ())
    {
    case Visibility::PUB:
      tokens.push_back (Rust::Token::make (PUB, vis.get_locus ()));
      break;
    case Visibility::PUB_CRATE:
      tokens.push_back (Rust::Token::make (PUB, vis.get_locus ()));
      tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
      tokens.push_back (Rust::Token::make (CRATE, Location ()));
      tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
      break;
    case Visibility::PUB_SELF:
      tokens.push_back (Rust::Token::make (PUB, vis.get_locus ()));
      tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
      tokens.push_back (Rust::Token::make (SELF, Location ()));
      tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
      break;
    case Visibility::PUB_SUPER:
      tokens.push_back (Rust::Token::make (PUB, vis.get_locus ()));
      tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
      tokens.push_back (Rust::Token::make (SUPER, Location ()));
      tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
      break;
    case Visibility::PUB_IN_PATH:
      tokens.push_back (Rust::Token::make (PUB, vis.get_locus ()));
      tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
      tokens.push_back (Rust::Token::make (IN, Location ()));
      visit (vis.get_path ());
      tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
      break;
    case Visibility::PRIV:
      break;
    }
}

void
TokenStream::visit (NamedFunctionParam &param)
{
  auto name = param.get_name ();
  tokens.push_back (
    Rust::Token::make_identifier (param.get_locus (), std::move (name)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (param.get_type ());
}

void
TokenStream::visit (std::vector<std::unique_ptr<GenericParam>> &params)
{
  tokens.push_back (Rust::Token::make (LEFT_ANGLE, Location ()));
  visit_items_joined_by_separator (params, COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_ANGLE, Location ()));
}

void
TokenStream::visit (TupleField &field)
{
  for (auto attr : field.get_outer_attrs ())
    {
      visit (attr);
    }
  visit (field.get_visibility ());
  visit (field.get_field_type ());
}

void
TokenStream::visit (StructField &field)
{
  for (auto attr : field.get_outer_attrs ())
    {
      visit (attr);
    }
  visit (field.get_visibility ());
  auto name = field.get_field_name ();
  tokens.push_back (
    Rust::Token::make_identifier (field.get_locus (), std::move (name)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (field.get_field_type ());
}

void
TokenStream::visit (std::vector<LifetimeParam> &for_lifetimes)
{
  tokens.push_back (Rust::Token::make (FOR, Location ()));
  tokens.push_back (Rust::Token::make (LEFT_ANGLE, Location ()));
  visit_items_joined_by_separator (for_lifetimes, COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_ANGLE, Location ()));
}

void
TokenStream::visit (FunctionQualifiers &qualifiers)
{
  // Syntax:
  //    `const`? `async`? `unsafe`? (`extern` Abi?)?
  //    unsafe? (extern Abi?)?

  switch (qualifiers.get_const_status ())
    {
    case NONE:
      break;
    case CONST_FN:
      tokens.push_back (Rust::Token::make (CONST, qualifiers.get_locus ()));
      break;
    case ASYNC_FN:
      tokens.push_back (Rust::Token::make (ASYNC, qualifiers.get_locus ()));
      break;
    }

  if (qualifiers.is_unsafe ())
    tokens.push_back (Rust::Token::make (UNSAFE, qualifiers.get_locus ()));
  if (qualifiers.is_extern ())
    {
      tokens.push_back (
	Rust::Token::make (EXTERN_TOK, qualifiers.get_locus ()));
      if (qualifiers.has_abi ())
	{
	  tokens.push_back (Rust::Token::make (DOUBLE_QUOTE, Location ()));
	  auto abi = qualifiers.get_extern_abi ();
	  tokens.push_back (
	    Rust::Token::make_identifier (Location (), std::move (abi)));
	  tokens.push_back (Rust::Token::make (DOUBLE_QUOTE, Location ()));
	}
    }
}

void
TokenStream::visit (MaybeNamedParam &param)
{
  // Syntax:
  //     OuterAttribute* ( ( IDENTIFIER | _ ) : )? Type

  for (auto attr : param.get_outer_attrs ())
    {
      visit (attr);
    }
  auto param_name = param.get_name ();
  switch (param.get_param_kind ())
    {
    case MaybeNamedParam::UNNAMED:
      break;
    case MaybeNamedParam::IDENTIFIER:
      tokens.push_back (
	Rust::Token::make_identifier (Location (), std::move (param_name)));
      tokens.push_back (Rust::Token::make (COLON, Location ()));
      break;
    case MaybeNamedParam::WILDCARD:
      tokens.push_back (Rust::Token::make (UNDERSCORE, Location ()));
      tokens.push_back (Rust::Token::make (COLON, Location ()));
      break;
    }
  visit (param.get_type ());
}

void
TokenStream::visit (Token &tok)
{
  std::string data = tok.get_tok_ptr ()->has_str () ? tok.get_str () : "";
  switch (tok.get_id ())
    {
    case IDENTIFIER:
      tokens.push_back (
	Rust::Token::make_identifier (tok.get_locus (), std::move (data)));
      break;
    case INT_LITERAL:
      tokens.push_back (Rust::Token::make_int (tok.get_locus (),
					       std::move (data),
					       tok.get_type_hint ()));
      break;
    case FLOAT_LITERAL:
      tokens.push_back (Rust::Token::make_float (tok.get_locus (),
						 std::move (data),
						 tok.get_type_hint ()));
      break;
    case STRING_LITERAL:
      tokens.push_back (
	Rust::Token::make_string (tok.get_locus (), std::move (data)));
      break;
    case CHAR_LITERAL:
      tokens.push_back (Rust::Token::make_char (
	tok.get_locus (),
	// FIXME: This need to be fixed to properly support UTF-8
	static_cast<uint32_t> (data[0])));
      break;
    case BYTE_CHAR_LITERAL:
      tokens.push_back (
	Rust::Token::make_byte_char (tok.get_locus (), data[0]));
      break;
    case BYTE_STRING_LITERAL:
      tokens.push_back (
	Rust::Token::make_byte_string (tok.get_locus (), std::move (data)));
      break;
    case INNER_DOC_COMMENT:
      tokens.push_back (Rust::Token::make_inner_doc_comment (tok.get_locus (),
							     std::move (data)));
      break;
    case OUTER_DOC_COMMENT:
      tokens.push_back (Rust::Token::make_outer_doc_comment (tok.get_locus (),
							     std::move (data)));
      break;
    case LIFETIME:
      tokens.push_back (
	Rust::Token::make_lifetime (tok.get_locus (), std::move (data)));
      break;
    default:
      tokens.push_back (Rust::Token::make (tok.get_id (), tok.get_locus ()));
    }
}

void
TokenStream::visit (DelimTokenTree &delim_tok_tree)
{
  increment_indentation ();
  newline ();
  indentation ();
  for (auto &token : delim_tok_tree.to_token_stream ())
    {
      visit (token);
    }
  decrement_indentation ();
  newline ();
  indentation ();
}

void
TokenStream::visit (AttrInputMetaItemContainer &container)
{
  for (auto &item : container.get_items ())
    {
      visit (item);
    }
}

void
TokenStream::visit (IdentifierExpr &ident_expr)
{
  auto ident = ident_expr.get_ident ();
  tokens.push_back (
    Rust::Token::make_identifier (ident_expr.get_locus (), std::move (ident)));
}

void
TokenStream::visit (Lifetime &lifetime)
{
  // Syntax:
  // Lifetime :
  // 	LIFETIME_OR_LABEL
  // 	| 'static
  // 	| '_

  auto name = lifetime.get_lifetime_name ();
  switch (lifetime.get_lifetime_type ())
    {
    case Lifetime::LifetimeType::NAMED:
      tokens.push_back (
	Rust::Token::make_lifetime (lifetime.get_locus (), std::move (name)));
      break;
    case Lifetime::LifetimeType::STATIC:
      tokens.push_back (Rust::Token::make_lifetime (lifetime.get_locus (),
						    std::move ("static")));
      break;
    case Lifetime::LifetimeType::WILDCARD:
      tokens.push_back (
	Rust::Token::make_lifetime (lifetime.get_locus (), std::move ("_")));
      break;
    }
}

void
TokenStream::visit (LifetimeParam &lifetime_param)
{
  // Syntax:
  //   LIFETIME_OR_LABEL ( : LifetimeBounds )?
  // LifetimeBounds :
  //   ( Lifetime + )* Lifetime?

  // TODO what to do with outer attr? They are not mentioned in the reference.

  auto lifetime = lifetime_param.get_lifetime ();
  visit (lifetime);

  if (lifetime_param.has_lifetime_bounds ())
    {
      tokens.push_back (Rust::Token::make (COLON, Location ()));
      for (auto &bound : lifetime_param.get_lifetime_bounds ())
	{
	  visit (bound);
	}
    }
}

void
TokenStream::visit (ConstGenericParam &param)
{
  // Syntax:
  // const IDENTIFIER : Type ( = Block | IDENTIFIER | -?LITERAL )?

  tokens.push_back (Rust::Token::make (CONST, param.get_locus ()));
  auto id = param.get_name ();
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (param.get_type ());
  if (param.has_default_value ())
    {
      tokens.push_back (Rust::Token::make (EQUAL, Location ()));
      visit (param.get_type ());
    }
}

void
TokenStream::visit (PathExprSegment &segment)
{
  visit (segment.get_ident_segment ());
  if (segment.has_generic_args ())
    {
      auto generics = segment.get_generic_args ();
      tokens.push_back (
	Rust::Token::make (SCOPE_RESOLUTION, segment.get_locus ()));
      tokens.push_back (Rust::Token::make (LEFT_ANGLE, generics.get_locus ()));

      auto &lifetime_args = generics.get_lifetime_args ();
      auto &generic_args = generics.get_generic_args ();
      auto &binding_args = generics.get_binding_args ();

      visit_items_joined_by_separator (generic_args, COMMA);

      if (!lifetime_args.empty ()
	  && (!generic_args.empty () || !binding_args.empty ()))
	{
	  tokens.push_back (Rust::Token::make (COMMA, Location ()));
	}

      visit_items_joined_by_separator (binding_args, COMMA);

      if (!generic_args.empty () && !binding_args.empty ())
	{
	  tokens.push_back (Rust::Token::make (COMMA, Location ()));
	}

      visit_items_joined_by_separator (lifetime_args, COMMA);

      tokens.push_back (Rust::Token::make (RIGHT_ANGLE, Location ()));
    }
}

void
TokenStream::visit (PathInExpression &path)
{
  if (path.opening_scope_resolution ())
    {
      tokens.push_back (
	Rust::Token::make (SCOPE_RESOLUTION, path.get_locus ()));
    }

  visit_items_joined_by_separator (path.get_segments (), SCOPE_RESOLUTION);
}

void
TokenStream::visit (TypePathSegment &segment)
{
  // Syntax:
  //    PathIdentSegment
  auto ident_segment = segment.get_ident_segment ();
  auto id = ident_segment.as_string ();
  tokens.push_back (
    Rust::Token::make_identifier (ident_segment.get_locus (), std::move (id)));
}

void
TokenStream::visit (TypePathSegmentGeneric &segment)
{
  // Syntax:
  //    PathIdentSegment `::`? (GenericArgs)?
  // GenericArgs :
  //    `<` `>`
  //    | `<` ( GenericArg `,` )* GenericArg `,`? `>`

  auto ident_segment = segment.get_ident_segment ();
  auto id = ident_segment.as_string ();
  tokens.push_back (
    Rust::Token::make_identifier (ident_segment.get_locus (), std::move (id)));

  if (segment.get_separating_scope_resolution ())
    tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));

  tokens.push_back (Rust::Token::make (LEFT_ANGLE, Location ()));

  {
    auto &lifetime_args = segment.get_generic_args ().get_lifetime_args ();
    auto &generic_args = segment.get_generic_args ().get_generic_args ();
    auto &binding_args = segment.get_generic_args ().get_binding_args ();

    visit_items_joined_by_separator (lifetime_args, COMMA);
    if (!lifetime_args.empty ()
	&& (!generic_args.empty () || !binding_args.empty ()))
      tokens.push_back (Rust::Token::make (COMMA, Location ()));
    visit_items_joined_by_separator (generic_args, COMMA);
    if (!generic_args.empty () && !binding_args.empty ())
      tokens.push_back (Rust::Token::make (COMMA, Location ()));
    visit_items_joined_by_separator (generic_args, COMMA);
  }

  tokens.push_back (Rust::Token::make (RIGHT_ANGLE, Location ()));
}

void
TokenStream::visit (GenericArgsBinding &binding)
{
  // Syntax:
  //    IDENTIFIER `=` Type
  auto identifier = binding.get_identifier ();
  tokens.push_back (Rust::Token::make_identifier (binding.get_locus (),
						  std::move (identifier)));

  tokens.push_back (Rust::Token::make (EQUAL, Location ()));
  visit (binding.get_type ());
}

void
TokenStream::visit (GenericArg &arg)
{
  // `GenericArg` implements `accept_vis` but it is not useful for this case as
  // it ignores unresolved cases (`Kind::Either`).
  switch (arg.get_kind ())
    {
    case GenericArg::Kind::Const:
      visit (arg.get_expression ());
      break;
    case GenericArg::Kind::Type:
      visit (arg.get_type ());
      break;
      case GenericArg::Kind::Either: {
	auto path = arg.get_path ();
	tokens.push_back (
	  Rust::Token::make_identifier (Location (), std::move (path)));
      }
      break;
    case GenericArg::Kind::Error:
      gcc_unreachable ();
    }
}

void
TokenStream::visit (TypePathSegmentFunction &segment)
{
  // Syntax:
  //   PathIdentSegment `::`? (TypePathFn)?

  auto ident_segment = segment.get_ident_segment ();
  auto id = ident_segment.as_string ();
  tokens.push_back (
    Rust::Token::make_identifier (ident_segment.get_locus (), std::move (id)));

  if (segment.get_separating_scope_resolution ())
    tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));

  if (!segment.is_ident_only ())
    visit (segment.get_type_path_function ());
}

void
TokenStream::visit (TypePathFunction &type_path_fn)
{
  // Syntax:
  //   `(` TypePathFnInputs? `)` (`->` Type)?
  // TypePathFnInputs :
  //   Type (`,` Type)* `,`?

  tokens.push_back (Rust::Token::make (LEFT_PAREN, type_path_fn.get_locus ()));
  if (type_path_fn.has_inputs ())
    visit_items_joined_by_separator (type_path_fn.get_params (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));

  if (type_path_fn.has_return_type ())
    {
      tokens.push_back (Rust::Token::make (RETURN_TYPE, Location ()));
      visit (type_path_fn.get_return_type ());
    }
}

void
TokenStream::visit (TypePath &path)
{
  // Syntax:
  //    `::`? TypePathSegment (`::` TypePathSegment)*

  if (path.has_opening_scope_resolution_op ())
    tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, path.get_locus ()));

  visit_items_joined_by_separator (path.get_segments (), SCOPE_RESOLUTION);
}

void
TokenStream::visit (PathIdentSegment &segment)
{
  if (segment.is_super_segment ())
    {
      tokens.push_back (Rust::Token::make (SUPER, segment.get_locus ()));
    }
  else if (segment.is_crate_segment ())
    {
      tokens.push_back (Rust::Token::make (CRATE, segment.get_locus ()));
    }
  else if (segment.is_lower_self ())
    {
      tokens.push_back (Rust::Token::make (SELF, segment.get_locus ()));
    }
  else if (segment.is_big_self ())
    {
      tokens.push_back (Rust::Token::make (SELF_ALIAS, segment.get_locus ()));
    }
  else
    {
      auto id = segment.as_string ();
      tokens.push_back (
	Rust::Token::make_identifier (segment.get_locus (), std::move (id)));
    }
}

void
TokenStream::visit (QualifiedPathInExpression &path)
{
  visit (path.get_qualified_path_type ());
  for (auto &segment : path.get_segments ())
    {
      tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));
      visit (segment);
    }
}

void
TokenStream::visit (QualifiedPathType &path)
{
  tokens.push_back (Rust::Token::make (LEFT_ANGLE, path.get_locus ()));
  visit (path.get_type ());
  if (path.has_as_clause ())
    {
      tokens.push_back (Rust::Token::make (AS, Location ()));
      visit (path.get_as_type_path ());
    }
  tokens.push_back (Rust::Token::make (RIGHT_ANGLE, Location ()));
}

void
TokenStream::visit (QualifiedPathInType &path)
{
  visit (path.get_qualified_path_type ());

  tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));
  visit (path.get_associated_segment ());
  for (auto &segment : path.get_segments ())
    {
      tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));
      visit (segment);
    }
}

void
TokenStream::visit (Literal &lit, Location locus)
{
  auto value = lit.as_string ();
  switch (lit.get_lit_type ())
    {
    case Literal::LitType::CHAR:
      tokens.push_back (
	Rust::Token::make_char (locus,
				// TODO: Change this to support utf-8 properly
				Codepoint (static_cast<uint32_t> (value[0]))));
      break;
    case Literal::LitType::STRING:
      tokens.push_back (Rust::Token::make_string (locus, std::move (value)));
      break;
    case Literal::LitType::BYTE:
      tokens.push_back (Rust::Token::make_byte_char (locus, value[0]));
      break;
    case Literal::LitType::BYTE_STRING:
      tokens.push_back (
	Rust::Token::make_byte_string (locus, std::move (value)));
      break;
    case Literal::LitType::INT:
      tokens.push_back (Rust::Token::make_int (locus, std::move (value)));
      break;
    case Literal::LitType::FLOAT:
      tokens.push_back (Rust::Token::make_float (locus, std::move (value)));
      break;
      case Literal::LitType::BOOL: {
	if (value == "false")
	  tokens.push_back (Rust::Token::make (FALSE_LITERAL, locus));
	else if (value == "true")
	  tokens.push_back (Rust::Token::make (TRUE_LITERAL, locus));
	else
	  gcc_unreachable (); // Not a boolean
	break;
      }
    case Literal::LitType::ERROR:
      gcc_unreachable ();
      break;
    }
}

void
TokenStream::visit (LiteralExpr &expr)
{
  auto lit = expr.get_literal ();
  visit (lit, expr.get_locus ());
}

void
TokenStream::visit (AttrInputLiteral &literal)
{
  tokens.push_back (Rust::Token::make (EQUAL, Location ()));
  visit (literal.get_literal ());
}

void
TokenStream::visit (MetaItemLitExpr &item)
{
  auto lit = item.get_literal ();
  visit (lit);
}

void
TokenStream::visit (MetaItemPathLit &item)
{
  auto path = item.get_path ();
  auto lit = item.get_literal ();
  visit (path);
  tokens.push_back (Rust::Token::make (COLON, item.get_locus ()));
  visit (lit);
}

void
TokenStream::visit (BorrowExpr &expr)
{
  tokens.push_back (Rust::Token::make (AMP, expr.get_locus ()));
  if (expr.get_is_double_borrow ())
    tokens.push_back (Rust::Token::make (AMP, Location ()));
  if (expr.get_is_mut ())
    tokens.push_back (Rust::Token::make (MUT, Location ()));

  visit (expr.get_borrowed_expr ());
}

void
TokenStream::visit (DereferenceExpr &expr)
{
  tokens.push_back (Rust::Token::make (ASTERISK, expr.get_locus ()));
  visit (expr.get_dereferenced_expr ());
}

void
TokenStream::visit (ErrorPropagationExpr &expr)
{
  visit (expr.get_propagating_expr ());
  tokens.push_back (Rust::Token::make (QUESTION_MARK, expr.get_locus ()));
}

void
TokenStream::visit (NegationExpr &expr)
{
  switch (expr.get_expr_type ())
    {
    case NegationOperator::NEGATE:
      tokens.push_back (Rust::Token::make (MINUS, expr.get_locus ()));
      break;
    case NegationOperator::NOT:
      tokens.push_back (Rust::Token::make (EXCLAM, expr.get_locus ()));
      break;
    }
  visit (expr.get_negated_expr ());
}

void
TokenStream::visit (ArithmeticOrLogicalExpr &expr)
{
  visit (expr.get_left_expr ());
  switch (expr.get_expr_type ())
    {
    case ArithmeticOrLogicalOperator::ADD:
      tokens.push_back (Rust::Token::make (PLUS, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::SUBTRACT:
      tokens.push_back (Rust::Token::make (MINUS, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::MULTIPLY:
      tokens.push_back (Rust::Token::make (ASTERISK, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::DIVIDE:
      tokens.push_back (Rust::Token::make (DIV, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::MODULUS:
      tokens.push_back (Rust::Token::make (PERCENT, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::BITWISE_AND:
      tokens.push_back (Rust::Token::make (AMP, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::BITWISE_OR:
      tokens.push_back (Rust::Token::make (PIPE, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::BITWISE_XOR:
      tokens.push_back (Rust::Token::make (CARET, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::LEFT_SHIFT:
      tokens.push_back (Rust::Token::make (LEFT_SHIFT, expr.get_locus ()));
      break;

    case ArithmeticOrLogicalOperator::RIGHT_SHIFT:
      tokens.push_back (Rust::Token::make (RIGHT_SHIFT, expr.get_locus ()));
      break;
    }

  visit (expr.get_right_expr ());
}

void
TokenStream::visit (ComparisonExpr &expr)
{
  visit (expr.get_left_expr ());

  switch (expr.get_expr_type ())
    {
    case ComparisonOperator::EQUAL:
      tokens.push_back (Rust::Token::make (EQUAL_EQUAL, expr.get_locus ()));
      break;
    case ComparisonOperator::NOT_EQUAL:
      tokens.push_back (Rust::Token::make (NOT_EQUAL, expr.get_locus ()));
      break;
    case ComparisonOperator::GREATER_THAN:
      tokens.push_back (Rust::Token::make (RIGHT_ANGLE, expr.get_locus ()));
      break;
    case ComparisonOperator::LESS_THAN:
      tokens.push_back (Rust::Token::make (LEFT_ANGLE, expr.get_locus ()));
      break;
    case ComparisonOperator::GREATER_OR_EQUAL:
      tokens.push_back (
	Rust::Token::make (GREATER_OR_EQUAL, expr.get_locus ()));
      break;

    case ComparisonOperator::LESS_OR_EQUAL:
      tokens.push_back (Rust::Token::make (LESS_OR_EQUAL, expr.get_locus ()));
      break;
    }
  visit (expr.get_right_expr ());
}

void
TokenStream::visit (LazyBooleanExpr &expr)
{
  visit (expr.get_left_expr ());

  switch (expr.get_expr_type ())
    {
    case LazyBooleanOperator::LOGICAL_AND:
      tokens.push_back (Rust::Token::make (LOGICAL_AND, expr.get_locus ()));
      break;
    case LazyBooleanOperator::LOGICAL_OR:
      tokens.push_back (Rust::Token::make (OR, expr.get_locus ()));
      break;
    }

  visit (expr.get_right_expr ());
}

void
TokenStream::visit (TypeCastExpr &expr)
{
  visit (expr.get_casted_expr ());
  tokens.push_back (Rust::Token::make (AS, expr.get_locus ()));
  visit (expr.get_type_to_cast_to ());
}

void
TokenStream::visit (AssignmentExpr &expr)
{
  expr.visit_lhs (*this);
  tokens.push_back (Rust::Token::make (EQUAL, expr.get_locus ()));
  expr.visit_rhs (*this);
}

void
TokenStream::visit (CompoundAssignmentExpr &expr)
{
  visit (expr.get_left_expr ());

  switch (expr.get_expr_type ())
    {
    case CompoundAssignmentOperator::ADD:
      tokens.push_back (Rust::Token::make (PLUS_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::SUBTRACT:
      tokens.push_back (Rust::Token::make (MINUS_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::MULTIPLY:
      tokens.push_back (Rust::Token::make (ASTERISK_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::DIVIDE:
      tokens.push_back (Rust::Token::make (DIV_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::MODULUS:
      tokens.push_back (Rust::Token::make (PERCENT_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::BITWISE_AND:
      tokens.push_back (Rust::Token::make (AMP_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::BITWISE_OR:
      tokens.push_back (Rust::Token::make (PIPE_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::BITWISE_XOR:
      tokens.push_back (Rust::Token::make (CARET_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::LEFT_SHIFT:
      tokens.push_back (Rust::Token::make (LEFT_SHIFT_EQ, expr.get_locus ()));
      break;
    case CompoundAssignmentOperator::RIGHT_SHIFT:
      tokens.push_back (Rust::Token::make (RIGHT_SHIFT_EQ, expr.get_locus ()));
      break;
    }
  visit (expr.get_right_expr ());
}

void
TokenStream::visit (GroupedExpr &expr)
{
  tokens.push_back (Rust::Token::make (LEFT_PAREN, expr.get_locus ()));
  visit (expr.get_expr_in_parens ());
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, expr.get_locus ()));
}

void
TokenStream::visit (ArrayElemsValues &elems)
{
  visit_items_joined_by_separator (elems.get_values (), COMMA);
}

void
TokenStream::visit (ArrayElemsCopied &elems)
{
  visit (elems.get_elem_to_copy ());
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  visit (elems.get_num_copies ());
}

void
TokenStream::visit (ArrayExpr &expr)
{
  tokens.push_back (Rust::Token::make (LEFT_SQUARE, expr.get_locus ()));
  visit (expr.get_array_elems ());
  tokens.push_back (Rust::Token::make (RIGHT_SQUARE, Location ()));
}

void
TokenStream::visit (ArrayIndexExpr &expr)
{
  visit (expr.get_array_expr ());
  tokens.push_back (Rust::Token::make (LEFT_SQUARE, expr.get_locus ()));
  visit (expr.get_index_expr ());
  tokens.push_back (Rust::Token::make (RIGHT_SQUARE, Location ()));
}

void
TokenStream::visit (TupleExpr &expr)
{
  visit_items_as_lines (expr.get_outer_attrs ());
  tokens.push_back (Rust::Token::make (LEFT_PAREN, expr.get_locus ()));
  visit_items_joined_by_separator (expr.get_tuple_elems (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (TupleIndexExpr &expr)
{
  visit (expr.get_tuple_expr ());
  tokens.push_back (Rust::Token::make (DOT, expr.get_locus ()));
  tokens.push_back (
    Rust::Token::make_int (Location (),
			   std::to_string (expr.get_tuple_index ())));
}

void
TokenStream::visit (StructExprStruct &expr)
{
  visit (expr.get_struct_name ());
}

void
TokenStream::visit (StructExprFieldIdentifier &expr)
{
  // TODO: Add attributes
  // visit_items_as_lines (expr.get_attrs ());
  auto id = expr.get_field_name ();
  tokens.push_back (
    Rust::Token::make_identifier (expr.get_locus (), std::move (id)));
}

void
TokenStream::visit (StructExprFieldIdentifierValue &expr)
{
  // TODO: Add attributes
  // visit_items_as_lines (expr.get_attrs ());
  auto id = expr.get_field_name ();
  tokens.push_back (
    Rust::Token::make_identifier (expr.get_locus (), std::move (id)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (expr.get_value ());
}

void
TokenStream::visit (StructExprFieldIndexValue &expr)
{
  // TODO: Add attributes
  // visit_items_as_lines (expr.get_attrs ());
  tokens.push_back (Rust::Token::make_int (expr.get_locus (),
					   std::to_string (expr.get_index ())));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (expr.get_value ());
}

void
TokenStream::visit (StructBase &base)
{
  tokens.push_back (Rust::Token::make (DOT_DOT, Location ()));
  visit (base.get_base_struct ());
}

void
TokenStream::visit (StructExprStructFields &expr)
{
  visit (expr.get_struct_name ());
  tokens.push_back (Rust::Token::make (LEFT_CURLY, expr.get_locus ()));
  visit_items_joined_by_separator (expr.get_fields (), COMMA);
  if (expr.has_struct_base ())
    {
      tokens.push_back (Rust::Token::make (COMMA, Location ()));
      visit (expr.get_struct_base ());
    }
  else
    {
      trailing_comma ();
    }
  tokens.push_back (Rust::Token::make (RIGHT_CURLY, expr.get_locus ()));
}

void
TokenStream::visit (StructExprStructBase &)
{
  // FIXME: Implement this node
  gcc_unreachable ();
}

void
TokenStream::visit (CallExpr &expr)
{
  visit (expr.get_function_expr ());

  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));

  visit_items_joined_by_separator (expr.get_params (), COMMA);

  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (MethodCallExpr &expr)
{
  visit (expr.get_receiver_expr ());
  tokens.push_back (Rust::Token::make (DOT, expr.get_locus ()));
  visit (expr.get_method_name ());
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
  visit_items_joined_by_separator (expr.get_params (), COMMA);
  trailing_comma ();
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (FieldAccessExpr &expr)
{
  visit (expr.get_receiver_expr ());
  tokens.push_back (Rust::Token::make (DOT, expr.get_locus ()));
  auto field_name = expr.get_field_name ();
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (field_name)));
}

void
TokenStream::visit (ClosureParam &param)
{
  visit_items_as_lines (param.get_outer_attrs ());
  visit (param.get_pattern ());
  if (param.has_type_given ())
    {
      tokens.push_back (Rust::Token::make (COLON, param.get_locus ()));
      visit (param.get_type ());
    }
}

void
TokenStream::visit_closure_common (ClosureExpr &expr)
{
  if (expr.get_has_move ())
    {
      tokens.push_back (Rust::Token::make (MOVE, expr.get_locus ()));
    }
  tokens.push_back (Rust::Token::make (PIPE, Location ()));
  visit_items_joined_by_separator (expr.get_params (), COMMA);
  tokens.push_back (Rust::Token::make (PIPE, Location ()));
}

void
TokenStream::visit (ClosureExprInner &expr)
{
  visit_closure_common (expr);
  visit (expr.get_definition_expr ());
}

void
TokenStream::visit (BlockExpr &expr)
{
  tokens.push_back (Rust::Token::make (LEFT_CURLY, expr.get_locus ()));
  newline ();
  increment_indentation ();
  visit_items_as_lines (expr.get_inner_attrs ());

  visit_items_as_lines (expr.get_statements (),
			{Rust::Token::make (SEMICOLON, Location ())});

  if (expr.has_tail_expr ())
    {
      visit (expr.get_tail_expr ());
      newline ();
    }

  decrement_indentation ();
  tokens.push_back (Rust::Token::make (RIGHT_CURLY, expr.get_locus ()));
  newline ();
}

void
TokenStream::visit (ClosureExprInnerTyped &expr)
{
  visit_closure_common (expr);
  tokens.push_back (Rust::Token::make (RETURN_TYPE, expr.get_locus ()));
  visit (expr.get_return_type ());
  visit (expr.get_definition_block ());
}

void
TokenStream::visit (ContinueExpr &expr)
{
  tokens.push_back (Rust::Token::make (CONTINUE, expr.get_locus ()));
  if (expr.has_label ())
    visit (expr.get_label ());
}

void
TokenStream::visit (BreakExpr &expr)
{
  tokens.push_back (Rust::Token::make (BREAK, expr.get_locus ()));
  if (expr.has_label ())
    visit (expr.get_label ());
  if (expr.has_break_expr ())
    visit (expr.get_break_expr ());
}

void
TokenStream::visit (RangeFromToExpr &expr)
{
  visit (expr.get_from_expr ());
  tokens.push_back (Rust::Token::make (DOT_DOT, expr.get_locus ()));
  visit (expr.get_to_expr ());
}

void
TokenStream::visit (RangeFromExpr &expr)
{
  visit (expr.get_from_expr ());
  tokens.push_back (Rust::Token::make (DOT_DOT, expr.get_locus ()));
}

void
TokenStream::visit (RangeToExpr &expr)
{
  tokens.push_back (Rust::Token::make (DOT_DOT, expr.get_locus ()));
  visit (expr.get_to_expr ());
}

void
TokenStream::visit (RangeFullExpr &expr)
{
  tokens.push_back (Rust::Token::make (DOT_DOT, expr.get_locus ()));
}

void
TokenStream::visit (RangeFromToInclExpr &expr)
{
  visit (expr.get_from_expr ());
  tokens.push_back (Rust::Token::make (DOT_DOT_EQ, expr.get_locus ()));
  visit (expr.get_to_expr ());
}

void
TokenStream::visit (RangeToInclExpr &expr)
{
  tokens.push_back (Rust::Token::make (DOT_DOT_EQ, expr.get_locus ()));
  visit (expr.get_to_expr ());
}

void
TokenStream::visit (ReturnExpr &expr)
{
  tokens.push_back (Rust::Token::make (RETURN_TOK, expr.get_locus ()));
  if (expr.has_returned_expr ())
    visit (expr.get_returned_expr ());
}

void
TokenStream::visit (UnsafeBlockExpr &expr)
{
  tokens.push_back (Rust::Token::make (UNSAFE, expr.get_locus ()));
  visit (expr.get_block_expr ());
}

void
TokenStream::visit (LoopLabel &label)
{
  visit (label.get_lifetime ());
  tokens.push_back (Rust::Token::make (COLON, label.get_locus ()));
}

void
TokenStream::visit_loop_common (BaseLoopExpr &expr)
{
  if (expr.has_loop_label ())
    visit (expr.get_loop_label ());
}

void
TokenStream::visit (LoopExpr &expr)
{
  visit_loop_common (expr);
  tokens.push_back (Rust::Token::make (LOOP, expr.get_locus ()));
  visit (expr.get_loop_block ());
}

void
TokenStream::visit (WhileLoopExpr &expr)
{
  visit_loop_common (expr);
  tokens.push_back (Rust::Token::make (WHILE, expr.get_locus ()));
  visit (expr.get_predicate_expr ());
  visit (expr.get_loop_block ());
}

void
TokenStream::visit (WhileLetLoopExpr &expr)
{
  visit_loop_common (expr);
  tokens.push_back (Rust::Token::make (WHILE, expr.get_locus ()));
  tokens.push_back (Rust::Token::make (LET, Location ()));
  // TODO: The reference mention only one Pattern
  for (auto &item : expr.get_patterns ())
    {
      visit (item);
    }
  tokens.push_back (Rust::Token::make (EQUAL, Location ()));
  visit (expr.get_scrutinee_expr ());
  visit (expr.get_loop_block ());
}

void
TokenStream::visit (ForLoopExpr &expr)
{
  visit_loop_common (expr);
  tokens.push_back (Rust::Token::make (FOR, expr.get_locus ()));
  visit (expr.get_pattern ());
  tokens.push_back (Rust::Token::make (IN, Location ()));
  visit (expr.get_iterator_expr ());
  visit (expr.get_loop_block ());
}

void
TokenStream::visit (IfExpr &expr)
{
  tokens.push_back (Rust::Token::make (IF, expr.get_locus ()));
  visit (expr.get_condition_expr ());
  visit (expr.get_if_block ());
}

void
TokenStream::visit (IfExprConseqElse &expr)
{
  visit (static_cast<IfExpr &> (expr));
  indentation ();
  tokens.push_back (Rust::Token::make (ELSE, expr.get_locus ()));
  visit (expr.get_else_block ());
}

void
TokenStream::visit (IfLetExpr &expr)
{
  tokens.push_back (Rust::Token::make (IF, expr.get_locus ()));
  tokens.push_back (Rust::Token::make (LET, Location ()));
  for (auto &pattern : expr.get_patterns ())
    {
      visit (pattern);
    }
  tokens.push_back (Rust::Token::make (EQUAL, Location ()));
  visit (expr.get_value_expr ());
  visit (expr.get_if_block ());
}

void
TokenStream::visit (IfLetExprConseqElse &expr)
{
  visit (static_cast<IfLetExpr &> (expr));
  indentation ();
  tokens.push_back (Rust::Token::make (ELSE, expr.get_locus ()));
  visit (expr.get_else_block ());
}

void
TokenStream::visit (MatchArm &arm)
{
  visit_items_as_lines (arm.get_outer_attrs ());
  for (auto &pattern : arm.get_patterns ())
    {
      visit (pattern);
    }
  if (arm.has_match_arm_guard ())
    {
      tokens.push_back (Rust::Token::make (IF, Location ()));
      visit (arm.get_guard_expr ());
    }
}

void
TokenStream::visit (MatchCase &match_case)
{
  visit (match_case.get_arm ());
  tokens.push_back (Rust::Token::make (MATCH_ARROW, Location ()));
  visit (match_case.get_expr ());
  tokens.push_back (Rust::Token::make (COMMA, Location ()));
}

void
TokenStream::visit (MatchExpr &expr)
{
  tokens.push_back (Rust::Token::make (MATCH_TOK, expr.get_locus ()));
  visit (expr.get_scrutinee_expr ());
  tokens.push_back (Rust::Token::make (LEFT_CURLY, Location ()));
  visit_items_as_lines (expr.get_inner_attrs ());
  for (auto &arm : expr.get_match_cases ())
    {
      visit (arm);
    }
  tokens.push_back (Rust::Token::make (RIGHT_CURLY, Location ()));
}

void
TokenStream::visit (AwaitExpr &expr)
{
  visit (expr.get_awaited_expr ());
  tokens.push_back (Rust::Token::make (DOT, expr.get_locus ()));
  // TODO: Check status of await keyword (Context dependant ?)
  tokens.push_back (Rust::Token::make_identifier (Location (), "await"));
}

void
TokenStream::visit (AsyncBlockExpr &expr)
{
  tokens.push_back (Rust::Token::make (ASYNC, expr.get_locus ()));
  if (expr.get_has_move ())
    tokens.push_back (Rust::Token::make (MOVE, Location ()));
  visit (expr.get_block_expr ());
}

// rust-item.h

void
TokenStream::visit (TypeParam &param)
{
  // Syntax:
  //    IDENTIFIER( : TypeParamBounds? )? ( = Type )?
  // TypeParamBounds :
  //    TypeParamBound ( + TypeParamBound )* +?

  auto id = param.get_type_representation ();
  tokens.push_back (
    Rust::Token::make_identifier (param.get_locus (), std::move (id)));
  if (param.has_type_param_bounds ())
    {
      tokens.push_back (Rust::Token::make (COLON, Location ()));
      visit_items_joined_by_separator (param.get_type_param_bounds (), PLUS);
    }
  if (param.has_type ())
    {
      tokens.push_back (Rust::Token::make (EQUAL, Location ()));
      visit (param.get_type ());
    }
}

void
TokenStream::visit (WhereClause &rule)
{
  // Syntax:
  // 	where ( WhereClauseItem , )* WhereClauseItem ?
  // WhereClauseItem :
  // 	LifetimeWhereClauseItem
  //  	| TypeBoundWhereClauseItem

  tokens.push_back (Rust::Token::make (WHERE, Location ()));
  newline ();
  increment_indentation ();
  visit_items_joined_by_separator (rule.get_items (), COMMA);
  decrement_indentation ();
}

void
TokenStream::visit (LifetimeWhereClauseItem &item)
{
  // Syntax:
  // 	Lifetime : LifetimeBounds
  // LifetimeBounds :
  //   ( Lifetime + )* Lifetime?

  visit (item.get_lifetime ());
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit_items_joined_by_separator (item.get_lifetime_bounds (), PLUS);
}

void
TokenStream::visit (TypeBoundWhereClauseItem &item)
{
  // Syntax:
  // 	ForLifetimes? Type : TypeParamBounds?
  // TypeParamBounds :
  // 	TypeParamBound ( + TypeParamBound )* +?
  // TypeParamBound :
  //    Lifetime | TraitBound

  if (item.has_for_lifetimes ())
    visit (item.get_for_lifetimes ());

  visit (item.get_type ());

  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit_items_joined_by_separator (item.get_type_param_bounds (), PLUS);
}

void
TokenStream::visit (Method &method)
{
  visit (method.get_visibility ());
  auto method_name = method.get_method_name ();
  tokens.push_back (Rust::Token::make (FN_TOK, method.get_locus ()));
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (method_name)));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));

  tokens.push_back (Rust::Token::make (SELF, Location ()));
  if (!method.get_function_params ().empty ())
    {
      tokens.push_back (Rust::Token::make (COMMA, Location ()));
      visit_items_joined_by_separator (method.get_function_params (), COMMA);
    }

  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));

  if (method.has_return_type ())
    {
      tokens.push_back (Rust::Token::make (RETURN_TYPE, Location ()));
      visit (method.get_return_type ());
    }

  auto &block = method.get_definition ();
  if (!block)
    tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  else
    visit (block);
  newline ();
}

void
TokenStream::visit (Module &module)
{
  //  Syntax:
  //	mod IDENTIFIER ;
  //     | mod IDENTIFIER {
  //	  InnerAttribute*
  //	  Item*
  //	}

  visit (module.get_visibility ());
  auto name = module.get_name ();
  tokens.push_back (Rust::Token::make (MOD, module.get_locus ()));
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (name)));

  if (module.get_kind () == Module::UNLOADED)
    {
      tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
      newline ();
    }
  else /* Module::LOADED */
    {
      tokens.push_back (Rust::Token::make (LEFT_CURLY, Location ()));
      newline ();
      increment_indentation ();

      visit_items_as_lines (module.get_inner_attrs ());
      visit_items_as_lines (module.get_items ());

      decrement_indentation ();

      tokens.push_back (Rust::Token::make (RIGHT_CURLY, Location ()));
      newline ();
    }
}

void
TokenStream::visit (ExternCrate &crate)
{
  tokens.push_back (Rust::Token::make (EXTERN_TOK, crate.get_locus ()));
  tokens.push_back (Rust::Token::make (CRATE, Location ()));
  auto ref = crate.get_referenced_crate ();
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (ref)));
  if (crate.has_as_clause ())
    {
      auto as_clause = crate.get_as_clause ();
      tokens.push_back (Rust::Token::make (AS, Location ()));
      tokens.push_back (
	Rust::Token::make_identifier (Location (), std::move (as_clause)));
    }
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  newline ();
}

void
TokenStream::visit (UseTreeGlob &use_tree)
{
  switch (use_tree.get_glob_type ())
    {
      case UseTreeGlob::PathType::PATH_PREFIXED: {
	auto path = use_tree.get_path ();
	visit (path);
	tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));
      }
      break;
    case UseTreeGlob::PathType::NO_PATH:
      tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));
      break;
    case UseTreeGlob::PathType::GLOBAL:
      break;
    }
  tokens.push_back (Rust::Token::make (ASTERISK, Location ()));
}

void
TokenStream::visit (UseTreeList &use_tree)
{
  switch (use_tree.get_path_type ())
    {
      case UseTreeList::PathType::PATH_PREFIXED: {
	auto path = use_tree.get_path ();
	visit (path);
	tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));
      }
      break;
    case UseTreeList::PathType::NO_PATH:
      tokens.push_back (Rust::Token::make (SCOPE_RESOLUTION, Location ()));
      break;
    case UseTreeList::PathType::GLOBAL:
      break;
    }

  tokens.push_back (Rust::Token::make (LEFT_CURLY, Location ()));
  if (use_tree.has_trees ())
    {
      visit_items_joined_by_separator (use_tree.get_trees (), COMMA);
    }
  tokens.push_back (Rust::Token::make (RIGHT_CURLY, Location ()));
}

void
TokenStream::visit (UseTreeRebind &use_tree)
{
  auto path = use_tree.get_path ();
  visit (path);
  switch (use_tree.get_new_bind_type ())
    {
      case UseTreeRebind::NewBindType::IDENTIFIER: {
	tokens.push_back (Rust::Token::make (AS, Location ()));
	auto id = use_tree.get_identifier ();
	tokens.push_back (
	  Rust::Token::make_identifier (use_tree.get_locus (), std::move (id)));
      }
      break;
    case UseTreeRebind::NewBindType::WILDCARD:
      tokens.push_back (Rust::Token::make (AS, Location ()));
      tokens.push_back (Rust::Token::make (UNDERSCORE, use_tree.get_locus ()));
      break;
    case UseTreeRebind::NewBindType::NONE:
      break;
    }
}

void
TokenStream::visit (UseDeclaration &decl)
{
  tokens.push_back (Rust::Token::make (USE, decl.get_locus ()));
  visit (*decl.get_tree ());
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  newline ();
}

void
TokenStream::visit (Function &function)
{
  // Syntax:
  //   FunctionQualifiers fn IDENTIFIER GenericParams?
  //      ( FunctionParameters? )
  //      FunctionReturnType? WhereClause?
  //      ( BlockExpression | ; )

  visit (function.get_visibility ());

  tokens.push_back (Rust::Token::make (FN_TOK, function.get_locus ()));
  auto name = function.get_function_name ();
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (name)));
  if (function.has_generics ())
    visit (function.get_generic_params ());

  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
  visit_items_joined_by_separator (function.get_function_params ());
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));

  if (function.has_return_type ())
    {
      tokens.push_back (Rust::Token::make (RETURN_TYPE, Location ()));
      visit (function.get_return_type ());
    }

  if (function.has_where_clause ())
    visit (function.get_where_clause ());

  auto &block = function.get_definition ();
  if (!block)
    tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  else
    visit (block);
  newline ();
}

void
TokenStream::visit (TypeAlias &type_alias)
{
  // Syntax:
  // Visibility? type IDENTIFIER GenericParams? WhereClause? = Type;

  // Note: Associated types are handled by `AST::TraitItemType`.

  if (type_alias.has_visibility ())
    visit (type_alias.get_visibility ());
  auto alias_name = type_alias.get_new_type_name ();
  tokens.push_back (Rust::Token::make (TYPE, type_alias.get_locus ()));
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (alias_name)));
  if (type_alias.has_generics ())
    visit (type_alias.get_generic_params ());
  if (type_alias.has_where_clause ())
    visit (type_alias.get_where_clause ());
  tokens.push_back (Rust::Token::make (EQUAL, Location ()));
  visit (type_alias.get_type_aliased ());
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
}

void
TokenStream::visit (StructStruct &struct_item)
{
  if (struct_item.has_visibility ())
    visit (struct_item.get_visibility ());
  auto struct_name = struct_item.get_identifier ();
  tokens.push_back (Rust::Token::make (STRUCT_TOK, struct_item.get_locus ()));
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (struct_name)));

  if (struct_item.has_generics ())
    visit (struct_item.get_generic_params ());
  if (struct_item.has_where_clause ())
    visit (struct_item.get_where_clause ());
  if (struct_item.is_unit_struct ())
    {
      tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
      newline ();
    }
  else
    visit_items_as_block (struct_item.get_fields (),
			  {Rust::Token::make (COMMA, Location ())});
}

void
TokenStream::visit (TupleStruct &tuple_struct)
{
  auto struct_name = tuple_struct.get_identifier ();
  tokens.push_back (Rust::Token::make (STRUCT_TOK, tuple_struct.get_locus ()));
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (struct_name)));
  if (tuple_struct.has_generics ())
    visit (tuple_struct.get_generic_params ());
  if (tuple_struct.has_where_clause ())
    visit (tuple_struct.get_where_clause ());

  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
  visit_items_joined_by_separator (tuple_struct.get_fields (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  newline ();
}

void
TokenStream::visit (EnumItem &item)
{
  auto id = item.get_identifier ();
  tokens.push_back (
    Rust::Token::make_identifier (item.get_locus (), std::move (id)));
}

void
TokenStream::visit (EnumItemTuple &item)
{
  auto id = item.get_identifier ();
  tokens.push_back (
    Rust::Token::make_identifier (item.get_locus (), std::move (id)));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
  visit_items_joined_by_separator (item.get_tuple_fields (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (EnumItemStruct &item)
{
  auto id = item.get_identifier ();
  tokens.push_back (
    Rust::Token::make_identifier (item.get_locus (), std::move (id)));
  visit_items_as_block (item.get_struct_fields (),
			{Rust::Token::make (COMMA, Location ())});
}

void
TokenStream::visit (EnumItemDiscriminant &item)
{
  auto id = item.get_identifier ();
  tokens.push_back (
    Rust::Token::make_identifier (item.get_locus (), std::move (id)));
  tokens.push_back (Rust::Token::make (EQUAL, Location ()));
  visit (item.get_expr ());
}

void
TokenStream::visit (Enum &enumeration)
{
  if (enumeration.has_visibility ())
    visit (enumeration.get_visibility ());
  tokens.push_back (Rust::Token::make (ENUM_TOK, enumeration.get_locus ()));
  auto id = enumeration.get_identifier ();
  tokens.push_back (
    Rust::Token::make_identifier (enumeration.get_locus (), std::move (id)));
  if (enumeration.has_generics ())
    visit (enumeration.get_generic_params ());
  if (enumeration.has_where_clause ())
    visit (enumeration.get_where_clause ());

  visit_items_as_block (enumeration.get_variants (),
			{Rust::Token::make (COMMA, Location ())});
}

void
TokenStream::visit (Union &union_item)
{
  visit_items_as_lines (union_item.get_outer_attrs ());
  auto id = union_item.get_identifier ();
  tokens.push_back (
    Rust::Token::make_identifier (union_item.get_locus (), "union"));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  if (union_item.has_generics ())
    visit (union_item.get_generic_params ());
  if (union_item.has_where_clause ())
    visit (union_item.get_where_clause ());

  visit_items_as_block (union_item.get_variants (),
			{Rust::Token::make (COMMA, Location ())});
}

void
TokenStream::visit (ConstantItem &item)
{
  tokens.push_back (Rust::Token::make (CONST, item.get_locus ()));
  if (item.is_unnamed ())
    {
      tokens.push_back (Rust::Token::make (UNDERSCORE, Location ()));
    }
  else
    {
      auto id = item.get_identifier ();
      tokens.push_back (
	Rust::Token::make_identifier (Location (), std::move (id)));
    }
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (item.get_type ());
  if (item.has_expr ())
    {
      tokens.push_back (Rust::Token::make (EQUAL, Location ()));
      visit (item.get_expr ());
    }
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
}

void
TokenStream::visit (StaticItem &item)
{
  tokens.push_back (Rust::Token::make (STATIC_TOK, item.get_locus ()));
  if (item.is_mutable ())
    tokens.push_back (Rust::Token::make (MUT, Location ()));
  auto id = item.get_identifier ();
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (item.get_type ());
  if (item.has_expr ())
    {
      tokens.push_back (Rust::Token::make (EQUAL, Location ()));
      visit (item.get_expr ());
    }
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
}

void
TokenStream::visit_function_common (std::unique_ptr<Type> &return_type,
				    std::unique_ptr<BlockExpr> &block)
{
  // FIXME: This should format the `<vis> fn <name> ( [args] )` as well
  if (return_type)
    {
      tokens.push_back (Rust::Token::make (RETURN_TYPE, Location ()));
      visit (return_type);
    }

  if (block)
    {
      visit (block);
    }
  else
    {
      tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
      newline ();
    }
}

void
TokenStream::visit (TraitItemFunc &item)
{
  auto func = item.get_trait_function_decl ();
  auto id = func.get_identifier ();
  tokens.push_back (Rust::Token::make (FN_TOK, item.get_locus ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));

  visit_items_joined_by_separator (func.get_function_params ());

  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));

  visit_function_common (func.get_return_type (), item.get_definition ());
}

void
TokenStream::visit (SelfParam &param)
{
  if (param.get_has_ref ())
    {
      tokens.push_back (Rust::Token::make (AMP, param.get_locus ()));
      if (param.has_lifetime ())
	{
	  auto lifetime = param.get_lifetime ();
	  visit (lifetime);
	}
    }

  if (param.get_is_mut ())
    {
      tokens.push_back (Rust::Token::make (MUT, Location ()));
    }

  tokens.push_back (Rust::Token::make (SELF, Location ()));
}

void
TokenStream::visit (TraitItemMethod &item)
{
  auto method = item.get_trait_method_decl ();
  auto id = method.get_identifier ();

  tokens.push_back (Rust::Token::make (FN_TOK, item.get_locus ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));

  visit (method.get_self_param ());

  if (!method.get_function_params ().empty ())
    {
      tokens.push_back (Rust::Token::make (COMMA, Location ()));
      visit_items_joined_by_separator (method.get_function_params (), COMMA);
    }

  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));

  visit_function_common (method.get_return_type (), item.get_definition ());
}

void
TokenStream::visit (TraitItemConst &item)
{
  auto id = item.get_identifier ();
  indentation ();
  tokens.push_back (Rust::Token::make (CONST, item.get_locus ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (item.get_type ());
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  newline ();
}

void
TokenStream::visit (TraitItemType &item)
{
  auto id = item.get_identifier ();
  indentation ();
  tokens.push_back (Rust::Token::make (TYPE, item.get_locus ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  newline ();
}

void
TokenStream::visit (Trait &trait)
{
  for (auto &attr : trait.get_outer_attrs ())
    {
      visit (attr);
      newline ();
      indentation ();
    }

  visit (trait.get_visibility ());

  auto id = trait.get_identifier ();
  tokens.push_back (Rust::Token::make (TRAIT, trait.get_locus ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));

  // Traits actually have an implicit Self thrown at the start, so we must
  // expect the number of generic params to be > 1
  if (trait.get_generic_params ().size () > 1)
    {
      tokens.push_back (Rust::Token::make (LEFT_ANGLE, Location ()));
      visit_items_joined_by_separator (trait.get_generic_params (), COMMA, 1);
      tokens.push_back (Rust::Token::make (RIGHT_ANGLE, Location ()));
    }

  visit_items_as_block (trait.get_trait_items (), {});
}

void
TokenStream::visit (InherentImpl &impl)
{
  tokens.push_back (Rust::Token::make (IMPL, impl.get_locus ()));
  // FIXME: Handle generics

  visit (impl.get_type ());

  if (impl.has_where_clause ())
    visit (impl.get_where_clause ());

  // FIXME: Handle inner attributes

  visit_items_as_block (impl.get_impl_items (), {});
}

void
TokenStream::visit (TraitImpl &impl)
{
  tokens.push_back (Rust::Token::make (IMPL, impl.get_locus ()));
  visit (impl.get_trait_path ());
  tokens.push_back (Rust::Token::make (FOR, Location ()));
  visit (impl.get_type ());
  tokens.push_back (Rust::Token::make (LEFT_CURLY, Location ()));
  newline ();

  increment_indentation ();

  for (auto &item : impl.get_impl_items ())
    {
      indentation ();
      visit (item);
    }

  decrement_indentation ();
  newline ();

  tokens.push_back (Rust::Token::make (RIGHT_CURLY, Location ()));
  newline ();
}

void
TokenStream::visit (ExternalTypeItem &type)
{
  visit (type.get_visibility ());

  auto id = type.get_identifier ();
  tokens.push_back (Rust::Token::make (TYPE, Location ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
}

void
TokenStream::visit (ExternalStaticItem &item)
{
  auto id = item.get_identifier ();
  visit_items_as_lines (item.get_outer_attrs ());
  if (item.has_visibility ())
    visit (item.get_visibility ());
  tokens.push_back (Rust::Token::make (STATIC_TOK, item.get_locus ()));
  if (item.is_mut ())
    tokens.push_back (Rust::Token::make (MUT, Location ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  visit (item.get_type ());
  // TODO: No expr ? The "(= Expression)?" part from the reference seems missing
  // in the ast.
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
}

void
TokenStream::visit (ExternalFunctionItem &function)
{
  visit (function.get_visibility ());

  auto id = function.get_identifier ();
  tokens.push_back (Rust::Token::make (FN_TOK, function.get_locus ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));

  visit_items_joined_by_separator (function.get_function_params ());

  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
  if (function.has_return_type ())
    {
      tokens.push_back (Rust::Token::make (RETURN_TYPE, Location ()));
      visit (function.get_return_type ());
    }
}

void
TokenStream::visit (ExternBlock &block)
{
  tokens.push_back (Rust::Token::make (EXTERN_TOK, block.get_locus ()));

  if (block.has_abi ())
    {
      auto abi = block.get_abi ();
      tokens.push_back (
	Rust::Token::make_string (Location (), std::move (abi)));
    }

  visit_items_as_block (block.get_extern_items ());
}

static std::pair<TokenId, TokenId>
get_delimiters (DelimType delim)
{
  switch (delim)
    {
    case PARENS:
      return {LEFT_PAREN, RIGHT_PAREN};
    case SQUARE:
      return {LEFT_SQUARE, RIGHT_SQUARE};
    case CURLY:
      return {LEFT_CURLY, RIGHT_CURLY};
    default:
      gcc_unreachable ();
    }
}

void
TokenStream::visit (MacroMatchFragment &match)
{
  auto id = match.get_ident ();
  auto frag_spec = match.get_frag_spec ().as_string ();
  tokens.push_back (Rust::Token::make (DOLLAR_SIGN, Location ()));
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (COLON, Location ()));
  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (frag_spec)));
}

void
TokenStream::visit (MacroMatchRepetition &repetition)
{
  tokens.push_back (Rust::Token::make (DOLLAR_SIGN, Location ()));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));

  visit_items_joined_by_separator (repetition.get_matches (), {});

  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));

  if (repetition.has_sep ())
    {
      tokens.push_back (
	Rust::Token::make (repetition.get_sep ()->get_id (),
			   repetition.get_sep ()->get_locus ()));
    }
  switch (repetition.get_op ())
    {
    case MacroMatchRepetition::ANY:
      tokens.push_back (Rust::Token::make (ASTERISK, Location ()));
      break;
    case MacroMatchRepetition::ONE_OR_MORE:
      tokens.push_back (Rust::Token::make (PLUS, Location ()));
      break;
    case MacroMatchRepetition::ZERO_OR_ONE:
      tokens.push_back (Rust::Token::make (QUESTION_MARK, Location ()));
      break;
    case MacroMatchRepetition::NONE:
      break;
    }
}

void
TokenStream::visit (MacroMatcher &matcher)
{
  auto delimiters = get_delimiters (matcher.get_delim_type ());

  tokens.push_back (Rust::Token::make (delimiters.first, Location ()));

  for (auto &item : matcher.get_matches ())
    {
      visit (item);
    }

  tokens.push_back (Rust::Token::make (delimiters.second, Location ()));
}

void
TokenStream::visit (MacroRule &rule)
{
  visit (rule.get_matcher ());
  tokens.push_back (Rust::Token::make (MATCH_ARROW, rule.get_locus ()));
  visit (rule.get_transcriber ().get_token_tree ());
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
}

void
TokenStream::visit (MacroRulesDefinition &rules_def)
{
  for (auto &outer_attr : rules_def.get_outer_attrs ())
    visit (outer_attr);

  auto rule_name = rules_def.get_rule_name ();
  tokens.push_back (
    Rust::Token::make_identifier (rules_def.get_locus (), "macro_rules"));
  tokens.push_back (Rust::Token::make (EXCLAM, Location ()));

  tokens.push_back (
    Rust::Token::make_identifier (Location (), std::move (rule_name)));

  visit_items_as_block (rules_def.get_rules (),
			{Rust::Token::make (SEMICOLON, Location ())});
}

void
TokenStream::visit (MacroInvocation &invocation)
{
  auto data = invocation.get_invoc_data ();
  visit (data.get_path ());
  tokens.push_back (Rust::Token::make (EXCLAM, Location ()));
  visit (data.get_delim_tok_tree ());
  if (invocation.has_semicolon ())
    tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
}

void
TokenStream::visit (MetaItemPath &item)
{
  auto path = item.to_path_item ();
  visit (path);
}

void
TokenStream::visit (MetaItemSeq &item)
{
  visit (item.get_path ());
  // TODO: Double check this, there is probably a mistake.
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
  visit_items_joined_by_separator (item.get_seq (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (MetaWord &word)
{
  auto id = word.get_ident ();
  tokens.push_back (
    Rust::Token::make_identifier (word.get_locus (), std::move (id)));
}

void
TokenStream::visit (MetaNameValueStr &name)
{
  auto pair = name.get_name_value_pair ();
  auto id = std::get<0> (pair);
  auto value = std::get<1> (pair);
  tokens.push_back (
    Rust::Token::make_identifier (name.get_locus (), std::move (id)));
  tokens.push_back (Rust::Token::make (EQUAL, name.get_locus ()));
  tokens.push_back (Rust::Token::make (DOUBLE_QUOTE, Location ()));
  tokens.push_back (
    Rust::Token::make_identifier (name.get_locus (), std::move (value)));
  tokens.push_back (Rust::Token::make (DOUBLE_QUOTE, Location ()));
}

void
TokenStream::visit (MetaListPaths &list)
{
  auto id = list.get_ident ();
  tokens.push_back (
    Rust::Token::make_identifier (list.get_locus (), std::move (id)));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
  visit_items_joined_by_separator (list.get_paths (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (MetaListNameValueStr &list)
{
  auto id = list.get_ident ();
  tokens.push_back (
    Rust::Token::make_identifier (list.get_locus (), std::move (id)));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));
  visit_items_joined_by_separator (list.get_values (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

// rust-pattern.h
void
TokenStream::visit (LiteralPattern &pattern)
{
  visit (pattern.get_literal (), pattern.get_locus ());
}

void
TokenStream::visit (IdentifierPattern &pattern)
{
  if (pattern.get_is_ref ())
    {
      tokens.push_back (Rust::Token::make (REF, pattern.get_locus ()));
    }
  if (pattern.get_is_mut ())
    {
      tokens.push_back (Rust::Token::make (MUT, Location ()));
    }
  auto id = pattern.get_ident ();
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  if (pattern.has_pattern_to_bind ())
    {
      tokens.push_back (Rust::Token::make (PATTERN_BIND, Location ()));
      visit (pattern.get_pattern_to_bind ());
    }
}

void
TokenStream::visit (WildcardPattern &pattern)
{
  tokens.push_back (Rust::Token::make (UNDERSCORE, pattern.get_locus ()));
}

void
TokenStream::visit (RestPattern &pattern)
{
  tokens.push_back (Rust::Token::make (DOT_DOT, pattern.get_locus ()));
}

// void TokenStream::visit(RangePatternBound& ){}

void
TokenStream::visit (RangePatternBoundLiteral &pattern)
{
  if (pattern.get_has_minus ())
    {
      tokens.push_back (Rust::Token::make (MINUS, pattern.get_locus ()));
    }
  auto literal = pattern.get_literal ();
  visit (literal);
}

void
TokenStream::visit (RangePatternBoundPath &pattern)
{
  visit (pattern.get_path ());
}

void
TokenStream::visit (RangePatternBoundQualPath &pattern)
{
  visit (pattern.get_qualified_path ());
}

void
TokenStream::visit (RangePattern &pattern)
{
  if (pattern.get_has_lower_bound () && pattern.get_has_upper_bound ())
    {
      visit (pattern.get_lower_bound ());
      if (pattern.get_has_ellipsis_syntax ())
	tokens.push_back (Rust::Token::make (ELLIPSIS, pattern.get_locus ()));
      else
	tokens.push_back (Rust::Token::make (DOT_DOT_EQ, pattern.get_locus ()));
      visit (pattern.get_upper_bound ());
    }
  else if (pattern.get_has_lower_bound ())
    {
      visit (pattern.get_lower_bound ());
      tokens.push_back (Rust::Token::make (DOT_DOT, pattern.get_locus ()));
    }
  else
    {
      tokens.push_back (Rust::Token::make (DOT_DOT_EQ, pattern.get_locus ()));
      visit (pattern.get_upper_bound ());
    }
}

void
TokenStream::visit (ReferencePattern &pattern)
{
  if (pattern.is_double_reference ())
    {
      tokens.push_back (Rust::Token::make (LOGICAL_AND, pattern.get_locus ()));
    }
  else
    {
      tokens.push_back (Rust::Token::make (AMP, pattern.get_locus ()));
    }

  if (pattern.get_is_mut ())
    {
      tokens.push_back (Rust::Token::make (MUT, Location ()));
    }

  visit (pattern.get_referenced_pattern ());
}

// void TokenStream::visit(StructPatternField& ){}

void
TokenStream::visit (StructPatternFieldTuplePat &pattern)
{
  visit_items_as_lines (pattern.get_outer_attrs ());
  tokens.push_back (
    Rust::Token::make_int (pattern.get_locus (),
			   std::to_string (pattern.get_index ())));
  tokens.push_back (Rust::Token::make (COLON, pattern.get_locus ()));
  visit (pattern.get_index_pattern ());
}

void
TokenStream::visit (StructPatternFieldIdentPat &pattern)
{
  visit_items_as_lines (pattern.get_outer_attrs ());
  auto id = pattern.get_identifier ();
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
  tokens.push_back (Rust::Token::make (COLON, pattern.get_locus ()));
  visit (pattern.get_ident_pattern ());
}

void
TokenStream::visit (StructPatternFieldIdent &pattern)
{
  visit_items_as_lines (pattern.get_outer_attrs ());
  if (pattern.is_ref ())
    tokens.push_back (Rust::Token::make (REF, Location ()));
  if (pattern.is_mut ())
    tokens.push_back (Rust::Token::make (MUT, Location ()));

  auto id = pattern.get_identifier ();
  tokens.push_back (Rust::Token::make_identifier (Location (), std::move (id)));
}

void
TokenStream::visit (StructPattern &pattern)
{
  visit (pattern.get_path ());
  tokens.push_back (Rust::Token::make (LEFT_CURLY, pattern.get_locus ()));
  auto elems = pattern.get_struct_pattern_elems ();
  if (elems.has_struct_pattern_fields ())
    {
      visit_items_joined_by_separator (elems.get_struct_pattern_fields ());
      if (elems.has_etc ())
	{
	  tokens.push_back (Rust::Token::make (COMMA, Location ()));
	  visit_items_as_lines (elems.get_etc_outer_attrs ());
	}
    }
  else
    {
      visit_items_as_lines (elems.get_etc_outer_attrs ());
    }

  tokens.push_back (Rust::Token::make (RIGHT_CURLY, Location ()));
}

// void TokenStream::visit(TupleStructItems& ){}

void
TokenStream::visit (TupleStructItemsNoRange &pattern)
{
  for (auto &pat : pattern.get_patterns ())
    {
      visit (pat);
    }
}

void
TokenStream::visit (TupleStructItemsRange &pattern)
{
  for (auto &lower : pattern.get_lower_patterns ())
    {
      visit (lower);
    }
  tokens.push_back (Rust::Token::make (DOT_DOT, Location ()));
  for (auto &upper : pattern.get_lower_patterns ())
    {
      visit (upper);
    }
}

void
TokenStream::visit (TupleStructPattern &pattern)
{
  visit (pattern.get_path ());
  tokens.push_back (Rust::Token::make (LEFT_PAREN, pattern.get_locus ()));
  if (pattern.has_items ())
    visit (pattern.get_items ());
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

// void
// TokenStream::visit (TuplePatternItems &)
// {}

void
TokenStream::visit (TuplePatternItemsMultiple &pattern)
{
  visit_items_joined_by_separator (pattern.get_patterns (), COMMA);
}

void
TokenStream::visit (TuplePatternItemsRanged &pattern)
{
  for (auto &lower : pattern.get_lower_patterns ())
    {
      visit (lower);
    }
  tokens.push_back (Rust::Token::make (DOT_DOT, Location ()));
  for (auto &upper : pattern.get_lower_patterns ())
    {
      visit (upper);
    }
}

void
TokenStream::visit (TuplePattern &pattern)
{
  tokens.push_back (Rust::Token::make (LEFT_PAREN, pattern.get_locus ()));
  visit (pattern.get_items ());
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (GroupedPattern &pattern)
{
  tokens.push_back (Rust::Token::make (LEFT_PAREN, pattern.get_locus ()));
  visit (pattern.get_pattern_in_parens ());
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (SlicePattern &pattern)
{
  tokens.push_back (Rust::Token::make (LEFT_SQUARE, pattern.get_locus ()));
  visit_items_joined_by_separator (pattern.get_items (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_SQUARE, Location ()));
}

void
TokenStream::visit (AltPattern &pattern)
{
  visit_items_joined_by_separator (pattern.get_alts (), PIPE);
}

// rust-stmt.h
void
TokenStream::visit (EmptyStmt &)
{}

void
TokenStream::visit (LetStmt &stmt)
{
  tokens.push_back (Rust::Token::make (LET, stmt.get_locus ()));
  auto &pattern = stmt.get_pattern ();
  if (pattern)
    visit (pattern);

  if (stmt.has_type ())
    {
      tokens.push_back (Rust::Token::make (COLON, Location ()));
      visit (stmt.get_type ());
    }

  if (stmt.has_init_expr ())
    {
      tokens.push_back (Rust::Token::make (EQUAL, Location ()));
      visit (stmt.get_init_expr ());
    }
}

void
TokenStream::visit (ExprStmtWithoutBlock &stmt)
{
  visit (stmt.get_expr ());
}

void
TokenStream::visit (ExprStmtWithBlock &stmt)
{
  visit (stmt.get_expr ());
}

// rust-type.h
void
TokenStream::visit (TraitBound &bound)
{
  // Syntax:
  //      ?? ForLifetimes? TypePath
  //   | ( ?? ForLifetimes? TypePath )

  if (bound.has_opening_question_mark ())
    tokens.push_back (Rust::Token::make (QUESTION_MARK, bound.get_locus ()));

  if (bound.has_for_lifetimes ())
    visit (bound.get_for_lifetimes ());

  visit (bound.get_type_path ());
}

void
TokenStream::visit (ImplTraitType &type)
{
  // Syntax:
  //    impl TypeParamBounds
  // TypeParamBounds :
  //    TypeParamBound ( + TypeParamBound )* +?

  tokens.push_back (Rust::Token::make (IMPL, type.get_locus ()));
  visit_items_joined_by_separator (type.get_type_param_bounds (), PLUS);
}

void
TokenStream::visit (TraitObjectType &type)
{
  // Syntax:
  //   dyn? TypeParamBounds
  // TypeParamBounds :
  //   TypeParamBound ( + TypeParamBound )* +?

  if (type.is_dyn ())
    tokens.push_back (Rust::Token::make (DYN, type.get_locus ()));
  visit_items_joined_by_separator (type.get_type_param_bounds (), PLUS);
}

void
TokenStream::visit (ParenthesisedType &type)
{
  // Syntax:
  //    ( Type )

  tokens.push_back (Rust::Token::make (LEFT_PAREN, type.get_locus ()));
  visit (type.get_type_in_parens ());
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (ImplTraitTypeOneBound &type)
{
  // Syntax:
  //    impl TraitBound

  tokens.push_back (Rust::Token::make (IMPL, type.get_locus ()));
  visit (type.get_trait_bound ());
}

void
TokenStream::visit (TraitObjectTypeOneBound &type)
{
  // Syntax:
  //    dyn? TraitBound

  if (type.is_dyn ())
    tokens.push_back (Rust::Token::make (DYN, type.get_locus ()));
  visit (type.get_trait_bound ());
}

void
TokenStream::visit (TupleType &type)
{
  // Syntax:
  //   ( )
  //   | ( ( Type , )+ Type? )

  tokens.push_back (Rust::Token::make (LEFT_PAREN, type.get_locus ()));
  visit_items_joined_by_separator (type.get_elems (), COMMA);
  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));
}

void
TokenStream::visit (NeverType &type)
{
  // Syntax:
  //  !

  tokens.push_back (Rust::Token::make (EXCLAM, type.get_locus ()));
}

void
TokenStream::visit (RawPointerType &type)
{
  // Syntax:
  //    * ( mut | const ) TypeNoBounds

  tokens.push_back (Rust::Token::make (ASTERISK, type.get_locus ()));
  if (type.get_pointer_type () == RawPointerType::MUT)
    tokens.push_back (Rust::Token::make (MUT, Location ()));
  else /* RawPointerType::CONST */
    tokens.push_back (Rust::Token::make (CONST, Location ()));

  visit (type.get_type_pointed_to ());
}

void
TokenStream::visit (ReferenceType &type)
{
  // Syntax:
  //    & Lifetime? mut? TypeNoBounds

  tokens.push_back (Rust::Token::make (AMP, type.get_locus ()));

  if (type.has_lifetime ())
    {
      visit (type.get_lifetime ());
    }

  if (type.get_has_mut ())
    tokens.push_back (Rust::Token::make (MUT, Location ()));

  visit (type.get_type_referenced ());
}

void
TokenStream::visit (ArrayType &type)
{
  // Syntax:
  //    [ Type ; Expression ]

  tokens.push_back (Rust::Token::make (LEFT_SQUARE, type.get_locus ()));
  visit (type.get_elem_type ());
  tokens.push_back (Rust::Token::make (SEMICOLON, Location ()));
  visit (type.get_size_expr ());
  tokens.push_back (Rust::Token::make (RIGHT_SQUARE, Location ()));
}

void
TokenStream::visit (SliceType &type)
{
  // Syntax:
  //    [ Type ]

  tokens.push_back (Rust::Token::make (LEFT_SQUARE, type.get_locus ()));
  visit (type.get_elem_type ());
  tokens.push_back (Rust::Token::make (RIGHT_SQUARE, Location ()));
}

void
TokenStream::visit (InferredType &type)
{
  // Syntax:
  //    _

  tokens.push_back (Rust::Token::make (UNDERSCORE, type.get_locus ()));
}

void
TokenStream::visit (BareFunctionType &type)
{
  // Syntax:
  //    ForLifetimes? FunctionTypeQualifiers fn
  //      ( FunctionParametersMaybeNamedVariadic? ) BareFunctionReturnType?
  //
  //    BareFunctionReturnType:
  //      -> TypeNoBounds
  //
  //    FunctionParametersMaybeNamedVariadic :
  //      MaybeNamedFunctionParameters | MaybeNamedFunctionParametersVariadic
  //
  //    MaybeNamedFunctionParameters :
  //      MaybeNamedParam ( , MaybeNamedParam )* ,?
  //
  //    MaybeNamedFunctionParametersVariadic :
  //      ( MaybeNamedParam , )* MaybeNamedParam , OuterAttribute* ...

  if (type.has_for_lifetimes ())
    visit (type.get_for_lifetimes ());

  visit (type.get_function_qualifiers ());

  tokens.push_back (Rust::Token::make (FN_TOK, type.get_locus ()));
  tokens.push_back (Rust::Token::make (LEFT_PAREN, Location ()));

  visit_items_joined_by_separator (type.get_function_params (), COMMA);

  if (type.is_variadic ())
    {
      tokens.push_back (Rust::Token::make (COMMA, Location ()));
      for (auto &item : type.get_variadic_attr ())
	{
	  visit (item);
	}
      tokens.push_back (Rust::Token::make (ELLIPSIS, Location ()));
    }

  tokens.push_back (Rust::Token::make (RIGHT_PAREN, Location ()));

  if (type.has_return_type ())
    {
      tokens.push_back (Rust::Token::make (RETURN_TYPE, Location ()));
      visit (type.get_return_type ());
    }
}

} // namespace AST
} // namespace Rust
