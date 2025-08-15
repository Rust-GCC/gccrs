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
#include "rust-ast-fragment.h"
#include "rust-fmt.h"
#include "rust-macro-builtins-helpers.h"
#include "rust-expand-format-args.h"

namespace Rust {

struct FormatArgsInput
{
  std::unique_ptr<AST::Expr> format_expr;
  AST::FormatArguments args;
  // bool is_literal?
};

struct FormatArgsParseError
{
  enum class Kind
  {
    MissingArguments
  } kind;
};

static tl::expected<FormatArgsInput, FormatArgsParseError>
format_args_parse_arguments (AST::MacroInvocData &invoc)
{
  MacroInvocLexer lex (invoc.get_delim_tok_tree ().to_token_stream ());
  Parser<MacroInvocLexer> parser (lex);

  // TODO: check if EOF - return that format_args!() requires at least one
  // argument

  auto args = AST::FormatArguments ();
  auto last_token_id = macro_end_token (invoc.get_delim_tok_tree (), parser);

  auto format_expr = parser.parse_expr ();
  rust_assert (format_expr);

  // TODO: Allow implicit captures ONLY if the the first arg is a string literal
  // and not a macro invocation

  // TODO: How to consume all of the arguments until the delimiter?

  // TODO: What we then want to do is as follows:
  // for each token, check if it is an identifier
  //     yes? is the next token an equal sign (=)
  //          yes?
  //              -> if that identifier is already present in our map, error
  //              out
  //              -> parse an expression, return a FormatArgument::Named
  //     no?
  //         -> if there have been named arguments before, error out
  //         (positional after named error)
  //         -> parse an expression, return a FormatArgument::Normal
  while (parser.peek_current_token ()->get_id () != last_token_id)
    {
      parser.skip_token (COMMA);

      // Check in case of an extraneous comma in the args list, which is
      // allowed - format_args!("fmt", arg, arg2,)
      if (parser.peek_current_token ()->get_id () == last_token_id)
	break;

      if (parser.peek_current_token ()->get_id () == IDENTIFIER
	  && parser.peek (1)->get_id () == EQUAL)
	{
	  // FIXME: This is ugly - just add a parser.parse_identifier()?
	  auto ident_tok = parser.peek_current_token ();
	  auto ident = Identifier (ident_tok);

	  parser.skip_token (IDENTIFIER);
	  parser.skip_token (EQUAL);

	  auto expr = parser.parse_expr ();

	  // TODO: Handle graciously
	  if (!expr)
	    rust_unreachable ();

	  args.push (AST::FormatArgument::named (ident, std::move (expr)));
	}
      else
	{
	  auto expr = parser.parse_expr ();

	  // TODO: Handle graciously
	  if (!expr)
	    rust_unreachable ();

	  args.push (AST::FormatArgument::normal (std::move (expr)));
	}
      // we need to skip commas, don't we?
    }

  return FormatArgsInput{std::move (format_expr), std::move (args)};
}

tl::optional<AST::Fragment>
MacroBuiltin::format_args_handler (location_t invoc_locus,
				   AST::MacroInvocData &invoc,
				   AST::InvocKind semicolon,
				   AST::FormatArgs::Newline nl)
{
  auto input = format_args_parse_arguments (invoc);

  if (!input)
    {
      rust_error_at (invoc_locus,
		     "could not parse arguments to %<format_args!()%>");
      return tl::nullopt;
    }

  auto node = AST::SingleASTNode (std::make_unique<AST::FormatArgs> (
    invoc_locus, AST::FormatArgsPieces (std::move (input->format_expr), nl),
    std::move (input->args)));

  return AST::Fragment ({std::move (node)},
			invoc.get_delim_tok_tree ().to_token_stream ());
}

} // namespace Rust
