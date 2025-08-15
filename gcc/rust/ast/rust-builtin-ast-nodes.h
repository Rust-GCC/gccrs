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

#ifndef RUST_AST_BUILTIN_NODES_H
#define RUST_AST_BUILTIN_NODES_H

#include "rust-system.h"
#include "line-map.h"
#include "optional.h"
#include "rust-ast.h"
#include "rust-fmt.h"

namespace Rust {
namespace AST {

// Definitions, from rustc's `FormatArgs` AST struct
// https://github.com/rust-lang/rust/blob/1be468815c/compiler/rustc_ast/src/format.rs
//
// format_args!("hello {abc:.xyz$}!!", abc="world");
// └──────────────────────────────────────────────┘
//                     FormatArgs
//
// format_args!("hello {abc:.xyz$}!!", abc="world");
//                                     └─────────┘
//                                      argument
//
// format_args!("hello {abc:.xyz$}!!", abc="world");
//              └───────────────────┘
//                     template
//
// format_args!("hello {abc:.xyz$}!!", abc="world");
//               └────┘└─────────┘└┘
//                      pieces
//
// format_args!("hello {abc:.xyz$}!!", abc="world");
//               └────┘           └┘
//                   literal pieces
//
// format_args!("hello {abc:.xyz$}!!", abc="world");
//                     └─────────┘
//                     placeholder
//
// format_args!("hello {abc:.xyz$}!!", abc="world");
//                      └─┘  └─┘
//                      positions (could be names, numbers, empty, or `*`)

// FIXME: Merge with the class below this one?
class FormatArgumentKind
{
public:
  enum class Kind
  {
    Normal,
    Named,
    Captured,
  } kind;

  Identifier &get_ident ()
  {
    rust_assert (kind == Kind::Captured || kind == Kind::Named);

    return ident.value ();
  }

  FormatArgumentKind (Kind kind, tl::optional<Identifier> ident)
    : kind (kind), ident (ident)
  {}

  FormatArgumentKind (const FormatArgumentKind &other)
  {
    kind = other.kind;
    ident = other.ident;
  }

  FormatArgumentKind operator= (const FormatArgumentKind &other)
  {
    kind = other.kind;
    ident = other.ident;

    return *this;
  }

private:
  tl::optional<Identifier> ident;
};

class FormatArgument
{
public:
  static FormatArgument normal (std::unique_ptr<Expr> expr)
  {
    return FormatArgument (FormatArgumentKind::Kind::Normal, tl::nullopt,
			   std::move (expr));
  }

  static FormatArgument named (Identifier ident, std::unique_ptr<Expr> expr)
  {
    return FormatArgument (FormatArgumentKind::Kind::Named, ident,
			   std::move (expr));
  }

  static FormatArgument captured (Identifier ident, std::unique_ptr<Expr> expr)
  {
    return FormatArgument (FormatArgumentKind::Kind::Captured, ident,
			   std::move (expr));
  }

  FormatArgument (const FormatArgument &other)
    : kind (other.kind), expr (other.expr->clone_expr ())
  {}

  FormatArgument operator= (const FormatArgument &other)
  {
    kind = other.kind;
    expr = other.expr->clone_expr ();

    return *this;
  }

  FormatArgumentKind get_kind () const { return kind; }
  const Expr &get_expr () const { return *expr; }

private:
  FormatArgument (FormatArgumentKind::Kind kind, tl::optional<Identifier> ident,
		  std::unique_ptr<Expr> expr)
    : kind (FormatArgumentKind (kind, ident)), expr (std::move (expr))
  {}

  FormatArgumentKind kind;
  std::unique_ptr<Expr> expr;
};

class FormatArguments
{
public:
  FormatArguments () {}
  FormatArguments (FormatArguments &&) = default;
  FormatArguments (const FormatArguments &other)
  {
    args = std::vector<FormatArgument> ();
    args.reserve (other.args.size ());

    for (const auto &arg : other.args)
      args.emplace_back (arg);
  };

  FormatArguments &operator= (const FormatArguments &other) = default;

  void push (FormatArgument &&elt) { args.emplace_back (std::move (elt)); }
  const FormatArgument at (size_t idx) const { return args.at (idx); }

private:
  std::vector<FormatArgument> args;
};

enum class FormatArgsNewline
{
  Yes,
  No
};

// can be either an expression or Fmt::Pieces
// used to handle `format_args!(some_macro!(...), ...)`
class FormatArgsPieces final
{
public:
  using Newline = FormatArgsNewline;

  enum class Kind
  {
    EXPR,
    PIECES
  };

  FormatArgsPieces (std::unique_ptr<Expr> expr, Newline nl)
    : kind (Kind::EXPR), expr_nl (std::move (expr), nl)
  {}

  FormatArgsPieces (Fmt::Pieces template_str)
    : kind (Kind::PIECES), template_str (std::move (template_str))
  {}

  FormatArgsPieces (const FormatArgsPieces &other) noexcept : kind (other.kind)
  {
    switch (kind)
      {
      case Kind::EXPR:
	new (&expr_nl) std::pair<std::unique_ptr<Expr>, Newline> (
	  other.expr_nl.first->clone_expr (), other.expr_nl.second);
	break;
      case Kind::PIECES:
	new (&template_str) Fmt::Pieces (other.template_str);
	break;
      default:
	rust_unreachable ();
      }
  }

  FormatArgsPieces (FormatArgsPieces &&other) noexcept : kind (other.kind)
  {
    switch (kind)
      {
      case Kind::EXPR:
	new (&expr_nl) std::pair<std::unique_ptr<Expr>, Newline> (
	  std::move (other.expr_nl.first), other.expr_nl.second);
	break;
      case Kind::PIECES:
	new (&template_str) Fmt::Pieces (std::move (other.template_str));
	break;
      default:
	rust_unreachable ();
      }
  }

  FormatArgsPieces &operator= (const FormatArgsPieces &other)
  {
    this->~FormatArgsPieces ();
    new (this) FormatArgsPieces (other);
    return *this;
  }

  FormatArgsPieces &operator= (FormatArgsPieces &&other)
  {
    this->~FormatArgsPieces ();
    new (this) FormatArgsPieces (std::move (other));
    return *this;
  }

  bool has_expr () const { return kind == Kind::EXPR; }

  Expr &get_expr ()
  {
    rust_assert (has_expr ());
    return *expr_nl.first;
  }

  const Expr &get_expr () const
  {
    rust_assert (has_expr ());
    return *expr_nl.first;
  }

  std::unique_ptr<Expr> &get_expr_ptr ()
  {
    rust_assert (has_expr ());
    return expr_nl.first;
  }

  bool should_add_newline () const
  {
    rust_assert (has_expr ());
    return expr_nl.second == Newline::Yes;
  }

  bool has_pieces () const { return kind == Kind::PIECES; }

  const Fmt::Pieces &get_pieces () const
  {
    rust_assert (has_pieces ());
    return template_str;
  }

  void try_simplify ();

  Kind get_kind () const { return kind; }

  ~FormatArgsPieces ()
  {
    switch (kind)
      {
      case Kind::EXPR:
	expr_nl.~pair ();
	break;
      case Kind::PIECES:
	template_str.~Pieces ();
	break;
      default:
	rust_unreachable ();
      }
  }

private:
  Kind kind;
  union
  {
    std::pair<std::unique_ptr<Expr>, Newline> expr_nl;
    Fmt::Pieces template_str;
  };
};

// TODO: Format documentation better
// Having a separate AST node for `format_args!()` expansion allows some
// important optimizations which help reduce generated code a lot. For example,
// turning `format_args!("a {} {} {}", 15, "hey", 'a')` directly into
// `format_args!("a 15 hey a")`, since all arguments are literals. Or,
// flattening imbricated `format_args!()` calls: `format_args!("heyo {}",
// format_args!("result: {}", some_result))` -> `format_args!("heyo result: {}",
// some_result)`
// FIXME: Move to rust-macro.h
class FormatArgs : public Expr
{
public:
  using Newline = FormatArgsNewline;

  FormatArgs (location_t loc, FormatArgsPieces template_arg,
	      FormatArguments &&arguments)
    : loc (loc), template_arg (std::move (template_arg)),
      arguments (std::move (arguments))
  {}

  FormatArgs (FormatArgs &&other) = default;
  FormatArgs (const FormatArgs &other) = default;
  FormatArgs &operator= (const FormatArgs &other) = default;

  void accept_vis (AST::ASTVisitor &vis) override;

  FormatArgsPieces &get_template_arg () { return template_arg; }
  const Fmt::Pieces &get_template () const
  {
    rust_assert (template_arg.has_pieces ());
    return template_arg.get_pieces ();
  }
  const FormatArguments &get_arguments () const { return arguments; }
  virtual location_t get_locus () const override;

  Expr::Kind get_expr_kind () const override { return Expr::Kind::FormatArgs; }

private:
  location_t loc;
  // FIXME: This probably needs to be a separate type - it is one in rustc's
  // expansion of format_args!(). There is extra handling associated with it.
  // we can maybe do that in rust-fmt.cc? in collect_pieces()? like do the
  // transformation into something we can handle better
  FormatArgsPieces template_arg;
  FormatArguments arguments;

  bool marked_for_strip = false;

protected:
  virtual std::string as_string () const override;
  virtual bool is_expr_without_block () const override;
  virtual void mark_for_strip () override;
  virtual bool is_marked_for_strip () const override;
  virtual std::vector<Attribute> &get_outer_attrs () override;
  virtual void set_outer_attrs (std::vector<Attribute>) override;
  virtual Expr *clone_expr_impl () const override;
};

/**
 * The node associated with the builtin offset_of!() macro
 */
class OffsetOf : public Expr
{
public:
  OffsetOf (std::unique_ptr<Type> &&type, Identifier field, location_t loc)
    : type (std::move (type)), field (field), loc (loc)
  {}

  OffsetOf (const OffsetOf &other)
    : type (other.type->clone_type ()), field (other.field), loc (other.loc),
      marked_for_strip (other.marked_for_strip)
  {}

  OffsetOf &operator= (const OffsetOf &other)
  {
    type = other.type->clone_type ();
    field = other.field;
    loc = other.loc;
    marked_for_strip = other.marked_for_strip;

    return *this;
  }

  void accept_vis (AST::ASTVisitor &vis) override;

  virtual location_t get_locus () const override { return loc; }
  const Type &get_type () const { return *type; }
  Type &get_type () { return *type; }
  const Identifier &get_field () const { return field; }

  bool is_expr_without_block () const override { return false; }

  void mark_for_strip () override { marked_for_strip = true; }
  bool is_marked_for_strip () const override { return marked_for_strip; }

  std::string as_string () const override;

  std::vector<Attribute> &get_outer_attrs () override;
  void set_outer_attrs (std::vector<Attribute>) override;
  Expr *clone_expr_impl () const override;

  Expr::Kind get_expr_kind () const override { return Expr::Kind::OffsetOf; }

private:
  std::unique_ptr<Type> type;
  Identifier field;

  location_t loc;
  bool marked_for_strip = false;
};

} // namespace AST
} // namespace Rust

#endif // ! RUST_AST_BUILTIN_NODES_H
