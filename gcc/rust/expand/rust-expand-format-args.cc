// Copyright (C) 2024-2025 Free Software Foundation, Inc.

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

#include "rust-expand-format-args.h"
#include "rust-ast-fragment.h"
#include "rust-ast.h"
#include "rust-builtin-ast-nodes.h"
#include "rust-ast-builder.h"
#include "rust-diagnostics.h"
#include "rust-expr.h"
#include "rust-fmt.h"
#include "rust-path.h"
#include "rust-system.h"
#include "rust-token.h"
#include "rust-expand-visitor.h"

namespace Rust {
namespace Fmt {

static std::unique_ptr<AST::Expr>
format_arg (const AST::Builder &builder, std::unique_ptr<AST::Expr> &&to_format,
	    const std::string &trait)
{
  auto formatter_fn = std::unique_ptr<AST::Expr> (new AST::PathInExpression (
    builder.path_in_expression ({"core", "fmt", trait, "fmt"})));

  auto path = std::unique_ptr<AST::Expr> (new AST::PathInExpression (
    builder.path_in_expression ({"core", "fmt", "ArgumentV1", "new"})));

  auto args = std::vector<std::unique_ptr<AST::Expr>> ();
  args.emplace_back (std::move (to_format));
  args.emplace_back (std::move (formatter_fn));

  return builder.call (std::move (path), std::move (args));
}

const std::string &
get_trait_name (ffi::FormatSpec format_specifier)
{
  static const std::unordered_map<std::string, std::string> spec_map = {
    {"", "Display"},   {"?", "Debug"},	  {"e", "LowerExp"},
    {"E", "UpperExp"}, {"o", "Octal"},	  {"p", "Pointer"},
    {"b", "Binary"},   {"x", "LowerHex"}, {"X", "UpperHex"},
  };

  auto it = spec_map.find (format_specifier.ty.to_string ());

  if (it == spec_map.end ())
    rust_unreachable ();

  return it->second;
}

static bool
expand_format_first_arg (ExpandVisitor &expand_visitor,
			 AST::FormatArgsPieces &fmt_arg)
{
  if (fmt_arg.has_pieces ())
    return true;

  expand_visitor.maybe_expand_expr (fmt_arg.get_expr_ptr ());

  AST::Expr &expr = fmt_arg.get_expr ();

  auto emit_err = [&] () {
    rust_error_at (expr.get_locus (),
		   "format argument must be a string literal");
  };

  if (!expr.is_literal ())
    {
      if (expr.get_expr_kind () != AST::Expr::Kind::MacroInvocation)
	emit_err ();
      return false;
    }

  auto &lit_expr = static_cast<AST::LiteralExpr &> (expr);

  if (lit_expr.get_lit_type () != AST::Literal::STRING
      && lit_expr.get_lit_type () != AST::Literal::RAW_STRING)
    {
      emit_err ();
      return false;
    }

  auto fmt_str = lit_expr.get_literal ().as_string ();

  // TODO: does Fmt::Pieces::collect do this?
  if (fmt_arg.should_add_newline ())
    fmt_str += '\n';

  auto pieces = Fmt::Pieces::collect (fmt_str, fmt_arg.should_add_newline (),
				      Fmt::ffi::ParseMode::Format);

  fmt_arg = pieces;
  return true;
}

void
expand_format_args (ExpandVisitor &expand_visitor, MacroExpander &expander,
		    AST::FormatArgs &fmt)
{
  if (!expand_format_first_arg (expand_visitor, fmt.get_template_arg ()))
    return;

  auto loc = fmt.get_locus ();
  auto builder = AST::Builder (loc);
  auto &arguments = fmt.get_arguments ();

  auto static_pieces = std::vector<std::unique_ptr<AST::Expr>> ();
  auto args
    = std::vector<std::pair<std::unique_ptr<AST::Expr>, ffi::FormatSpec>> ();

  for (const auto &node : fmt.get_template ().get_pieces ())
    {
      switch (node.tag)
	{
	case ffi::Piece::Tag::String:
	  static_pieces.emplace_back (
	    builder.literal_string (node.string._0.to_string ()));
	  break;
	case ffi::Piece::Tag::NextArgument:
	  {
	    auto next_argument = node.next_argument._0;
	    switch (node.next_argument._0.position.tag)
	      {
	      case ffi::Position::Tag::ArgumentImplicitlyIs:
		{
		  auto idx = next_argument.position.argument_implicitly_is._0;
		  auto trait = next_argument.format;
		  auto arg = arguments.at (idx);

		  // FIXME(Arthur): This API sucks
		  rust_assert (arg.get_kind ().kind
			       == AST::FormatArgumentKind::Kind::Normal);

		  args.push_back ({arg.get_expr ().clone_expr (), trait});
		}
		break;
	      case ffi::Position::Tag::ArgumentIs:
	      case ffi::Position::Tag::ArgumentNamed:
		rust_sorry_at (loc, "unhandled argument position specifier");
		break;
	      }
	  }
	  break;
	}
    }

  auto args_array = std::vector<std::unique_ptr<AST::Expr>> ();
  for (auto &&arg : args)
    args_array.emplace_back (format_arg (builder,
					 builder.ref (std::move (arg.first)),
					 get_trait_name (arg.second)));

  auto pieces = builder.ref (builder.array (std::move (static_pieces)));
  auto args_slice = builder.ref (builder.array (std::move (args_array)));

  auto final_path = std::make_unique<AST::PathInExpression> (
    builder.path_in_expression ({"core", "fmt", "Arguments", "new_v1"}));
  auto final_args = std::vector<std::unique_ptr<AST::Expr>> ();
  final_args.emplace_back (std::move (pieces));
  final_args.emplace_back (std::move (args_slice));

  auto final_call
    = builder.call (std::move (final_path), std::move (final_args));

  // TODO: is it alright to have an empty token list?
  std::vector<std::unique_ptr<AST::Token>> fragment_tokens;

  expander.set_expanded_fragment (
    AST::Fragment ({AST::SingleASTNode (std::move (final_call))},
		   std::move (fragment_tokens)));
}

} // namespace Fmt
} // namespace Rust
