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

#ifndef AST_BUILDER_H
#define AST_BUILDER_H

#include "rust-ast-full.h"
#include "rust-expr.h"
#include "rust-ast.h"
#include "rust-item.h"

namespace Rust {
namespace AST {

template <typename T>
std::vector<std::unique_ptr<T>>
vec (std::unique_ptr<T> &&t)
{
  auto v = std::vector<std::unique_ptr<T>> ();

  v.emplace_back (std::move (t));

  return v;
}

template <typename T>
std::vector<std::unique_ptr<T>>
vec (std::unique_ptr<T> &&t1, std::unique_ptr<T> &&t2)
{
  auto v = std::vector<std::unique_ptr<T>> ();

  v.emplace_back (std::move (t1));
  v.emplace_back (std::move (t2));

  return v;
}

// TODO: Use this builder when expanding regular macros
/* Builder class with helper methods to create AST nodes. This builder is
 * tailored towards generating multiple AST nodes from a single location, and
 * may not be suitable to other purposes */
class Builder
{
public:
  Builder (location_t loc) : loc (loc) {}

  /* Create a string literal expression ("content") */
  std::unique_ptr<Expr> literal_string (std::string &&content) const;

  /* Create an identifier expression (`variable`) */
  std::unique_ptr<Expr> identifier (std::string name) const;
  std::unique_ptr<Pattern> identifier_pattern (std::string name,
					       bool mut = false) const;

  /* Create a tuple index expression (`receiver.0`) */
  std::unique_ptr<Expr> tuple_idx (std::string receiver, int idx) const;

  /* Create a reference to an expression (`&of`) */
  std::unique_ptr<Expr> ref (std::unique_ptr<Expr> &&of,
			     bool mut = false) const;

  /* Create a dereference of an expression (`*of`) */
  std::unique_ptr<Expr> deref (std::unique_ptr<Expr> &&of) const;

  /* Create a block with an optional tail expression */
  std::unique_ptr<Expr> block (std::vector<std::unique_ptr<Stmt>> &&stmts,
			       std::unique_ptr<Expr> &&tail_expr
			       = nullptr) const;
  std::unique_ptr<Expr> block (std::unique_ptr<Stmt> &&stmt,
			       std::unique_ptr<Expr> &&tail_expr
			       = nullptr) const;

  /* Create a let binding with an optional type and initializer (`let <name> :
   * <type> = <init>`) */
  std::unique_ptr<Stmt> let (std::unique_ptr<Pattern> pattern,
			     std::unique_ptr<Type> type = nullptr,
			     std::unique_ptr<Expr> init = nullptr) const;

  /**
   * Create a call expression to a function, struct or enum variant, given its
   * arguments (`path(arg0, arg1, arg2)`)
   */
  std::unique_ptr<Expr> call (std::unique_ptr<Expr> &&path,
			      std::vector<std::unique_ptr<Expr>> &&args) const;
  std::unique_ptr<Expr> call (std::unique_ptr<Expr> &&path,
			      std::unique_ptr<Expr> &&arg) const;

  /**
   * Create an array expression (`[member0, member1, member2]`)
   */
  std::unique_ptr<Expr>
  array (std::vector<std::unique_ptr<Expr>> &&members) const;

  /* Empty function qualifiers, with no specific qualifiers */
  FunctionQualifiers fn_qualifiers () const;

  /* Create a single path segment from one string */
  PathExprSegment path_segment (std::string seg) const;

  /* And similarly for type path segments */
  std::unique_ptr<TypePathSegment> type_path_segment (std::string seg) const;
  std::unique_ptr<TypePathSegment>
  type_path_segment (LangItem::Kind lang_item) const;

  std::unique_ptr<TypePathSegment>
  type_path_segment_generic (std::string seg, GenericArgs args) const;
  std::unique_ptr<TypePathSegment>
  type_path_segment_generic (LangItem::Kind lang_item, GenericArgs args) const;

  /* Create a Type from a single string - the most basic kind of type in our AST
   */
  std::unique_ptr<Type> single_type_path (std::string type) const;
  std::unique_ptr<Type> single_type_path (LangItem::Kind lang_item) const;

  std::unique_ptr<Type> single_generic_type_path (std::string type,
						  GenericArgs args) const;
  std::unique_ptr<Type> single_generic_type_path (LangItem::Kind lang_item,
						  GenericArgs args) const;

  TypePath type_path (std::unique_ptr<TypePathSegment> &&segment) const;
  TypePath type_path (std::string type) const;
  TypePath type_path (LangItem::Kind lang_item) const;

  /**
   * Create a path in expression from multiple segments (`Clone::clone`). You
   * do not need to separate the segments using `::`, you can simply provide a
   * vector of strings to the functions which will get turned into path segments
   */
  PathInExpression
  path_in_expression (std::vector<std::string> &&segments) const;

  /**
   * Create a path in expression from a lang item.
   */
  PathInExpression path_in_expression (LangItem::Kind lang_item) const;

  /* Create a new struct */
  std::unique_ptr<Stmt>
  struct_struct (std::string struct_name,
		 std::vector<std::unique_ptr<GenericParam>> &&generics,
		 std::vector<StructField> &&fields);

  /* Create a struct expression for unit structs (`S`) */
  std::unique_ptr<Expr> struct_expr_struct (std::string struct_name) const;

  /**
   * Create an expression for struct instantiation with fields (`S { a, b: c }`)
   * Tuple expressions are call expressions and can thus be constructed with
   * `call`
   */
  std::unique_ptr<Expr>
  struct_expr (std::string struct_name,
	       std::vector<std::unique_ptr<StructExprField>> &&fields) const;
  std::unique_ptr<Expr>
  struct_expr (PathInExpression struct_name,
	       std::vector<std::unique_ptr<StructExprField>> &&fields) const;

  /* Create a field expression for struct instantiation (`field_name: value`) */
  std::unique_ptr<StructExprField>
  struct_expr_field (std::string field_name,
		     std::unique_ptr<Expr> &&value) const;

  /* Create a field access expression (`instance.field`) */
  std::unique_ptr<Expr> field_access (std::unique_ptr<Expr> &&instance,
				      std::string field) const;

  /* Create a wildcard pattern (`_`) */
  std::unique_ptr<Pattern> wildcard () const;

  /* Create a lang item path usable as a general path */
  std::unique_ptr<Path> lang_item_path (LangItem::Kind) const;

  /* Create match expressions and their components */
  std::unique_ptr<Expr> match (std::unique_ptr<Expr> &&scrutinee,
			       std::vector<MatchCase> &&cases);
  MatchArm match_arm (std::unique_ptr<Pattern> &&pattern);
  MatchCase match_case (std::unique_ptr<Pattern> &&pattern,
			std::unique_ptr<Expr> &&expr);

  /* Create a loop expression */
  std::unique_ptr<Expr> loop (std::vector<std::unique_ptr<Stmt>> &&stmts);

  static std::unique_ptr<Type> new_type (Type &type);

  static std::unique_ptr<GenericParam>
  new_lifetime_param (LifetimeParam &param);

  static std::unique_ptr<GenericParam> new_type_param (TypeParam &param);

  static Lifetime new_lifetime (const Lifetime &lifetime);

  static GenericArgs new_generic_args (GenericArgs &args);

private:
  /**
   * Location of the generated AST nodes
   */
  location_t loc;
};

} // namespace AST
} // namespace Rust

#endif // AST_BUILDER_H
