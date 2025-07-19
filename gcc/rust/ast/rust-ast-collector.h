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

#ifndef RUST_AST_COLLECTOR_H
#define RUST_AST_COLLECTOR_H

#include "rust-token.h"
#include "rust-ast-visitor.h"
#include "rust-ast.h"
#include "rust-ast-full.h"

namespace Rust {
namespace AST {

class CollectItem
{
public:
  enum class Kind
  {
    Comment,
    Newline,
    Indentation,
    Token,
  };

  CollectItem (TokenPtr token) : token (token), kind (Kind::Token) {}
  CollectItem (std::string comment) : comment (comment), kind (Kind::Comment) {}
  CollectItem (Kind kind) : kind (kind) { rust_assert (kind != Kind::Token); }
  CollectItem (size_t level) : indent_level (level), kind (Kind::Indentation) {}

  Kind get_kind () { return kind; }

  TokenPtr get_token ()
  {
    rust_assert (kind == Kind::Token);
    return token;
  }

  std::string get_comment ()
  {
    rust_assert (kind == Kind::Comment);
    return comment;
  }

  size_t get_indent_level ()
  {
    rust_assert (kind == Kind::Indentation);
    return indent_level;
  }

private:
  TokenPtr token;
  std::string comment;
  size_t indent_level;
  Kind kind;
};

class TokenCollector : public ASTVisitor
{
public:
  TokenCollector () : indent_level (0) {}

  bool output_trailing_commas = false;

  void visit (AST::Crate &crate);
  void visit (AST::Item &item);

  std::vector<TokenPtr> collect_tokens () const;
  std::vector<CollectItem> collect () const;

private:
  std::vector<CollectItem> tokens;
  size_t indent_level;

  void push (TokenPtr token) { tokens.push_back ({token}); }

  /**
   * Visit all items in given @collection, placing the separator in between but
   * not at the end.
   */
  template <typename T>
  void visit_items_joined_by_separator (T &collection,
					TokenId separator = COMMA,
					size_t start_offset = 0,
					size_t end_offset = 0)
  {
    if (collection.size () > start_offset)
      {
	visit (collection.at (start_offset));
	auto size = collection.size () - end_offset;
	for (size_t i = start_offset + 1; i < size; i++)
	  {
	    push (Rust::Token::make (separator, UNDEF_LOCATION));
	    visit (collection.at (i));
	  }
      }
  }

  /**
   * Visit item placing end of line after.
   */
  template <typename T>
  void visit_as_line (T &item, std::vector<TokenPtr> trailing = {})
  {
    indentation ();
    visit (item);
    for (auto &token : trailing)
      push (token);
    newline ();
  }

  /**
   * Visit each item in @collection "as line".
   *
   * @see visit_as_line
   */
  template <typename T>
  void visit_items_as_lines (T &collection, std::vector<TokenPtr> trailing = {})
  {
    for (auto &item : collection)
      visit_as_line (item, trailing);
  }

  /**
   * Visit each item in @collection as lines inside a block delimited by braces
   * with increased indentation. Also includes special handling for empty
   * collection to print only the delimiters with no new line inside.
   */
  template <typename T>
  void visit_items_as_block (T &collection, std::vector<TokenPtr> trailing = {},
			     TokenId left_brace = LEFT_CURLY,
			     TokenId right_brace = RIGHT_CURLY)
  {
    push (Rust::Token::make (left_brace, UNDEF_LOCATION));
    if (collection.empty ())
      {
	push (Rust::Token::make (right_brace, UNDEF_LOCATION));
	newline ();
      }
    else
      {
	newline ();
	increment_indentation ();
	visit_items_as_lines (collection, trailing);
	decrement_indentation ();
	indentation ();
	push (Rust::Token::make (right_brace, UNDEF_LOCATION));
	newline ();
      }
  }

  void trailing_comma ();
  void newline ();
  void indentation ();
  void increment_indentation ();
  void decrement_indentation ();
  void comment (std::string comment);
  /**
   * Visit common items of functions: Parameters, return type, block
   */
  void visit_function_common (std::unique_ptr<Type> &return_type,
			      std::unique_ptr<BlockExpr> &block);

  void visit_closure_common (ClosureExpr &expr);

  void visit_loop_common (BaseLoopExpr &expr);

public:
  /**
   * Compatibility layer for using the visitor pattern on polymorphic classes
   * with a unified overload syntax. This allows us to call `visit` both on
   * types implementing `accept_vis` method and for classes for which the
   * `visit` method is directly implemented.
   */
  template <typename T> void visit (std::unique_ptr<T> &node)
  {
    node->accept_vis (*this);
  }

  /**
   * @see visit<std::unique_ptr<T>>
   */
  template <typename T> void visit (T &node) { node.accept_vis (*this); }

  void visit (Visitable &v);
  void visit (LoopLabel &label);

  void visit (Literal &lit, location_t locus = UNDEF_LOCATION);

  void visit (Attribute &attrib);
  void visit (Visibility &vis);
  void visit (std::vector<std::unique_ptr<GenericParam>> &params);
  void visit (TupleField &field);
  void visit (StructField &field);
  void visit (SimplePathSegment &segment);
  void visit (MacroRule &rule);
  void visit (WhereClause &rule);
  void visit (std::vector<LifetimeParam> &for_lifetimes);
  void visit (FunctionQualifiers &qualifiers);
  void visit (MaybeNamedParam &param);
  void visit (TypePathFunction &type_path_fn);
  void visit (GenericArgsBinding &binding);
  void visit (GenericArg &arg);

  void visit (SimplePath &path);
  void visit (PathExprSegment &segment);
  void visit (PathIdentSegment &segment);
  void visit (QualifiedPathType &path);

  void visit (StructBase &base);
  void visit (ClosureParam &param);
  void visit (MatchArm &arm);
  void visit (MatchCase &arm);

  void visit (MacroInvocData &invoc_data);

#define VISIT(X) void visit (X &) override;
#include "rust-ast-visitable.h"
#undef VISIT
};
} // namespace AST

} // namespace Rust

#endif // !RUST_AST_COLLECTOR_H
