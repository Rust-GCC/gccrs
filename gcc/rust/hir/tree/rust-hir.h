// Copyright (C) 2020-2021 Free Software Foundation, Inc.

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

#ifndef RUST_HIR_BASE_H
#define RUST_HIR_BASE_H

#include "rust-ast.h"
#include "rust-system.h"
#include "rust-token.h"
#include "rust-location.h"
#include "rust-hir-map.h"
#include "rust-diagnostics.h"

namespace Rust {
typedef std::string Identifier;
typedef int TupleIndex;

namespace HIR {
// foward decl: ast visitor
class HIRVisitor;

// forward decl for use in token tree method
class Token;

// A literal - value with a type. Used in LiteralExpr and LiteralPattern.
struct Literal
{
public:
  enum LitType
  {
    CHAR,
    STRING,
    BYTE,
    BYTE_STRING,
    INT,
    FLOAT,
    BOOL
  };

private:
  std::string value_as_string;
  LitType type;
  PrimitiveCoreType type_hint;

public:
  std::string as_string () const { return value_as_string; }

  LitType get_lit_type () const { return type; }

  PrimitiveCoreType get_type_hint () const { return type_hint; }

  Literal (std::string value_as_string, LitType type,
	   PrimitiveCoreType type_hint)
    : value_as_string (std::move (value_as_string)), type (type),
      type_hint (type_hint)
  {}

  static Literal create_error ()
  {
    return Literal ("", CHAR, PrimitiveCoreType::CORETYPE_UNKNOWN);
  }

  void set_lit_type (LitType lt) { type = lt; }

  // Returns whether literal is in an invalid state.
  bool is_error () const { return value_as_string == ""; }
};

/* Base statement abstract class. Note that most "statements" are not allowed in
 * top-level module scope - only a subclass of statements called "items" are. */
class Stmt
{
public:
  // Unique pointer custom clone function
  std::unique_ptr<Stmt> clone_stmt () const
  {
    return std::unique_ptr<Stmt> (clone_stmt_impl ());
  }

  virtual ~Stmt () {}

  virtual std::string as_string () const = 0;

  virtual void accept_vis (HIRVisitor &vis) = 0;

  virtual Location get_locus () const = 0;

  virtual bool is_unit_check_needed () const { return false; }

  const Analysis::NodeMapping &get_mappings () const { return mappings; }

  virtual bool is_item () const = 0;

protected:
  Stmt (Analysis::NodeMapping mappings) : mappings (std::move (mappings)) {}

  // Clone function implementation as pure virtual method
  virtual Stmt *clone_stmt_impl () const = 0;

  Analysis::NodeMapping mappings;
};

// Rust "item" HIR node (declaration of top-level/module-level allowed stuff)
class Item : public Stmt
{
  AST::AttrVec outer_attrs;

  // TODO: should outer attrs be defined here or in each derived class?

public:
  // Unique pointer custom clone function
  std::unique_ptr<Item> clone_item () const
  {
    return std::unique_ptr<Item> (clone_item_impl ());
  }

  std::string as_string () const override;

  /* Adds crate names to the vector passed by reference, if it can
   * (polymorphism). */
  virtual void
  add_crate_name (std::vector<std::string> &names ATTRIBUTE_UNUSED) const
  {}

  virtual void accept_vis (HIRVisitor &vis ATTRIBUTE_UNUSED) {}

  AST::AttrVec &get_outer_attrs () { return outer_attrs; }
  const AST::AttrVec &get_outer_attrs () const { return outer_attrs; }

  bool is_item () const override final { return true; }

protected:
  // Constructor
  Item (Analysis::NodeMapping mappings,
	AST::AttrVec outer_attribs = AST::AttrVec ())
    : Stmt (std::move (mappings)), outer_attrs (std::move (outer_attribs))
  {}

  // Clone function implementation as pure virtual method
  virtual Item *clone_item_impl () const = 0;

  /* Save having to specify two clone methods in derived classes by making
   * statement clone return item clone. Hopefully won't affect performance too
   * much. */
  Item *clone_stmt_impl () const override { return clone_item_impl (); }
};

// forward decl of ExprWithoutBlock
class ExprWithoutBlock;

// Base expression HIR node - abstract
class Expr
{
  // TODO: move outer attribute data to derived classes?
  AST::AttrVec outer_attrs;

  Analysis::NodeMapping mappings;

public:
  const AST::AttrVec &get_outer_attrs () const { return outer_attrs; }

  // Unique pointer custom clone function
  std::unique_ptr<Expr> clone_expr () const
  {
    return std::unique_ptr<Expr> (clone_expr_impl ());
  }

  /* TODO: public methods that could be useful:
   *  - get_type() - returns type of expression. set_type() may also be useful
   * for some?
   *  - evaluate() - evaluates expression if constant? can_evaluate()? */

  /* HACK: downcasting without dynamic_cast (if possible) via polymorphism -
   * overrided in subclasses of ExprWithoutBlock */
  virtual ExprWithoutBlock *as_expr_without_block () const { return nullptr; }

  // TODO: make pure virtual if move out outer attributes to derived classes
  virtual std::string as_string () const;

  virtual ~Expr () {}

  virtual Location get_locus () const = 0;

  // HACK: strictly not needed, but faster than full downcast clone
  virtual bool is_expr_without_block () const = 0;

  virtual void accept_vis (HIRVisitor &vis) = 0;

  const Analysis::NodeMapping &get_mappings () const { return mappings; }

  // Clone function implementation as pure virtual method
  virtual Expr *clone_expr_impl () const = 0;

protected:
  // Constructor
  Expr (Analysis::NodeMapping mappings,
	AST::AttrVec outer_attribs = AST::AttrVec ())
    : outer_attrs (std::move (outer_attribs)), mappings (std::move (mappings))
  {}

  // TODO: think of less hacky way to implement this kind of thing
  // Sets outer attributes.
  void set_outer_attrs (AST::AttrVec outer_attrs_to_set)
  {
    outer_attrs = std::move (outer_attrs_to_set);
  }
};

// HIR node for an expression without an accompanying block - abstract
class ExprWithoutBlock : public Expr
{
protected:
  // Constructor
  ExprWithoutBlock (Analysis::NodeMapping mappings,
		    AST::AttrVec outer_attribs = AST::AttrVec ())
    : Expr (std::move (mappings), std::move (outer_attribs))
  {}

  // pure virtual clone implementation
  virtual ExprWithoutBlock *clone_expr_without_block_impl () const = 0;

  /* Save having to specify two clone methods in derived classes by making expr
   * clone return exprwithoutblock clone. Hopefully won't affect performance too
   * much. */
  ExprWithoutBlock *clone_expr_impl () const override
  {
    return clone_expr_without_block_impl ();
  }

  bool is_expr_without_block () const final override { return true; };

public:
  // Unique pointer custom clone function
  std::unique_ptr<ExprWithoutBlock> clone_expr_without_block () const
  {
    return std::unique_ptr<ExprWithoutBlock> (clone_expr_without_block_impl ());
  }

  /* downcasting hack from expr to use pratt parsing with
   * parse_expr_without_block */
  ExprWithoutBlock *as_expr_without_block () const override
  {
    return clone_expr_without_block_impl ();
  }
};

/* HACK: IdentifierExpr, delete when figure out identifier vs expr problem in
 * Pratt parser */
/* Alternatively, identifiers could just be represented as single-segment paths
 */
class IdentifierExpr : public ExprWithoutBlock
{
  Identifier ident;

public:
  Location locus;

  IdentifierExpr (Analysis::NodeMapping mappings, Identifier ident,
		  Location locus = Location (),
		  AST::AttrVec outer_attrs = AST::AttrVec ())
    : ExprWithoutBlock (std::move (mappings), std::move (outer_attrs)),
      ident (std::move (ident)), locus (locus)
  {}

  std::string as_string () const override
  {
    return "( " + ident + " (" + get_mappings ().as_string () + "))";
  }

  Location get_locus () const override final { return locus; }

  void accept_vis (HIRVisitor &vis) override;

  // Clones this object.
  std::unique_ptr<IdentifierExpr> clone_identifier_expr () const
  {
    return std::unique_ptr<IdentifierExpr> (clone_identifier_expr_impl ());
  }

  Identifier get_identifier () const { return ident; }

protected:
  // Clone method implementation
  IdentifierExpr *clone_expr_without_block_impl () const override
  {
    return clone_identifier_expr_impl ();
  }

  IdentifierExpr *clone_identifier_expr_impl () const
  {
    return new IdentifierExpr (*this);
  }

  IdentifierExpr (IdentifierExpr const &other) = default;
  IdentifierExpr &operator= (IdentifierExpr const &other) = default;
};

// Pattern base HIR node
class Pattern
{
public:
  // Unique pointer custom clone function
  std::unique_ptr<Pattern> clone_pattern () const
  {
    return std::unique_ptr<Pattern> (clone_pattern_impl ());
  }

  // possible virtual methods: is_refutable()

  virtual ~Pattern () {}

  virtual std::string as_string () const = 0;

  virtual void accept_vis (HIRVisitor &vis) = 0;

  virtual Analysis::NodeMapping get_pattern_mappings () const = 0;

protected:
  // Clone pattern implementation as pure virtual method
  virtual Pattern *clone_pattern_impl () const = 0;
};

// forward decl for Type
class TraitBound;

// Base class for types as represented in HIR - abstract
class Type
{
public:
  // Unique pointer custom clone function
  std::unique_ptr<Type> clone_type () const
  {
    return std::unique_ptr<Type> (clone_type_impl ());
  }

  // virtual destructor
  virtual ~Type () {}

  virtual std::string as_string () const = 0;

  /* HACK: convert to trait bound. Virtual method overriden by classes that
   * enable this. */
  virtual TraitBound *to_trait_bound (bool in_parens ATTRIBUTE_UNUSED) const
  {
    return nullptr;
  }
  /* as pointer, shouldn't require definition beforehand, only forward
   * declaration. */

  virtual void accept_vis (HIRVisitor &vis) = 0;

  virtual Analysis::NodeMapping get_mappings () const { return mappings; }

protected:
  Type (Analysis::NodeMapping mappings) : mappings (mappings) {}

  // Clone function implementation as pure virtual method
  virtual Type *clone_type_impl () const = 0;

  Analysis::NodeMapping mappings;
};

// A type without parentheses? - abstract
class TypeNoBounds : public Type
{
public:
  // Unique pointer custom clone function
  std::unique_ptr<TypeNoBounds> clone_type_no_bounds () const
  {
    return std::unique_ptr<TypeNoBounds> (clone_type_no_bounds_impl ());
  }

protected:
  TypeNoBounds (Analysis::NodeMapping mappings) : Type (mappings) {}

  // Clone function implementation as pure virtual method
  virtual TypeNoBounds *clone_type_no_bounds_impl () const = 0;

  /* Save having to specify two clone methods in derived classes by making type
   * clone return typenobounds clone. Hopefully won't affect performance too
   * much. */
  TypeNoBounds *clone_type_impl () const override
  {
    return clone_type_no_bounds_impl ();
  }
};

/* Abstract base class representing a type param bound - Lifetime and TraitBound
 * extends it */
class TypeParamBound
{
public:
  enum BoundType
  {
    LIFETIME,
    TRAITBOUND
  };

  virtual ~TypeParamBound () {}

  // Unique pointer custom clone function
  std::unique_ptr<TypeParamBound> clone_type_param_bound () const
  {
    return std::unique_ptr<TypeParamBound> (clone_type_param_bound_impl ());
  }

  virtual std::string as_string () const = 0;

  virtual void accept_vis (HIRVisitor &vis) = 0;

  virtual Analysis::NodeMapping get_mappings () const = 0;

  virtual Location get_locus () const = 0;

  virtual BoundType get_bound_type () const = 0;

protected:
  // Clone function implementation as pure virtual method
  virtual TypeParamBound *clone_type_param_bound_impl () const = 0;
};

// Represents a lifetime (and is also a kind of type param bound)
class Lifetime : public TypeParamBound
{
private:
  AST::Lifetime::LifetimeType lifetime_type;
  std::string lifetime_name;
  Location locus;
  Analysis::NodeMapping mappings;

public:
  // Constructor
  Lifetime (Analysis::NodeMapping mapping, AST::Lifetime::LifetimeType type,
	    std::string name, Location locus)
    : lifetime_type (type), lifetime_name (std::move (name)), locus (locus),
      mappings (mapping)
  {}

  // Returns true if the lifetime is in an error state.
  bool is_error () const
  {
    return lifetime_type == AST::Lifetime::LifetimeType::NAMED
	   && lifetime_name.empty ();
  }

  static Lifetime error ()
  {
    return Lifetime (Analysis::NodeMapping::get_error (),
		     AST::Lifetime::LifetimeType::NAMED, "", Location ());
  }

  std::string as_string () const override;

  void accept_vis (HIRVisitor &vis) override;

  std::string get_name () const { return lifetime_name; }

  AST::Lifetime::LifetimeType get_lifetime_type () const
  {
    return lifetime_type;
  }

  Location get_locus () const override final { return locus; }

  Analysis::NodeMapping get_mappings () const override final
  {
    return mappings;
  }

  BoundType get_bound_type () const final override { return LIFETIME; }

protected:
  /* Use covariance to implement clone function as returning this object rather
   * than base */
  Lifetime *clone_type_param_bound_impl () const override
  {
    return new Lifetime (*this);
  }
};

/* Base generic parameter in HIR. Abstract - can be represented by a Lifetime or
 * Type param */
class GenericParam
{
public:
  virtual ~GenericParam () {}

  enum GenericKind
  {
    TYPE,
    LIFETIME,

    // CONST generic parameter not yet handled
    // CONST,
  };

  // Unique pointer custom clone function
  std::unique_ptr<GenericParam> clone_generic_param () const
  {
    return std::unique_ptr<GenericParam> (clone_generic_param_impl ());
  }

  virtual std::string as_string () const = 0;

  virtual void accept_vis (HIRVisitor &vis) = 0;

  virtual Location get_locus () const = 0;

  Analysis::NodeMapping get_mappings () const { return mappings; }

  enum GenericKind get_kind () const { return kind; }

protected:
  // Clone function implementation as pure virtual method
  virtual GenericParam *clone_generic_param_impl () const = 0;

  Analysis::NodeMapping mappings;

  enum GenericKind kind;

  GenericParam (Analysis::NodeMapping mapping, enum GenericKind kind = TYPE)
    : mappings (mapping), kind (kind)
  {}
};

// A lifetime generic parameter (as opposed to a type generic parameter)
class LifetimeParam : public GenericParam
{
  Lifetime lifetime;

  // bool has_lifetime_bounds;
  // LifetimeBounds lifetime_bounds;
  std::vector<Lifetime> lifetime_bounds; // inlined LifetimeBounds

  // bool has_outer_attribute;
  // std::unique_ptr<Attribute> outer_attr;
  AST::Attribute outer_attr;

  Location locus;

public:
  Lifetime get_lifetime () { return lifetime; }

  // Returns whether the lifetime param has any lifetime bounds.
  bool has_lifetime_bounds () const { return !lifetime_bounds.empty (); }

  // Returns whether the lifetime param has an outer attribute.
  bool has_outer_attribute () const { return !outer_attr.is_empty (); }

  // Returns whether the lifetime param is in an error state.
  bool is_error () const { return lifetime.is_error (); }

  // Constructor
  LifetimeParam (Analysis::NodeMapping mappings, Lifetime lifetime,
		 Location locus = Location (),
		 std::vector<Lifetime> lifetime_bounds
		 = std::vector<Lifetime> (),
		 AST::Attribute outer_attr = AST::Attribute::create_empty ())
    : GenericParam (mappings, GenericKind::LIFETIME),
      lifetime (std::move (lifetime)),
      lifetime_bounds (std::move (lifetime_bounds)),
      outer_attr (std::move (outer_attr)), locus (locus)
  {}

  // TODO: remove copy and assignment operator definitions - not required

  // Copy constructor with clone
  LifetimeParam (LifetimeParam const &other)
    : GenericParam (other.mappings, GenericKind::LIFETIME),
      lifetime (other.lifetime), lifetime_bounds (other.lifetime_bounds),
      outer_attr (other.outer_attr), locus (other.locus)
  {}

  // Overloaded assignment operator to clone attribute
  LifetimeParam &operator= (LifetimeParam const &other)
  {
    lifetime = other.lifetime;
    lifetime_bounds = other.lifetime_bounds;
    outer_attr = other.outer_attr;
    locus = other.locus;
    mappings = other.mappings;

    return *this;
  }

  // move constructors
  LifetimeParam (LifetimeParam &&other) = default;
  LifetimeParam &operator= (LifetimeParam &&other) = default;

  std::string as_string () const override;

  void accept_vis (HIRVisitor &vis) override;

  Location get_locus () const override final { return locus; }

protected:
  /* Use covariance to implement clone function as returning this object rather
   * than base */
  LifetimeParam *clone_generic_param_impl () const override
  {
    return new LifetimeParam (*this);
  }
};

// Item used in trait declarations - abstract base class
class TraitItem
{
protected:
  // Constructor
  TraitItem (Analysis::NodeMapping mappings) : mappings (mappings) {}

  // Clone function implementation as pure virtual method
  virtual TraitItem *clone_trait_item_impl () const = 0;

  Analysis::NodeMapping mappings;

public:
  virtual ~TraitItem () {}

  std::unique_ptr<TraitItem> clone_trait_item () const
  {
    return std::unique_ptr<TraitItem> (clone_trait_item_impl ());
  }

  virtual std::string as_string () const = 0;

  virtual void accept_vis (HIRVisitor &vis) = 0;

  virtual const std::string trait_identifier () const = 0;

  const Analysis::NodeMapping get_mappings () const { return mappings; }
};

class ImplItem
{
public:
  enum ImplItemType
  {
    FUNCTION,
    TYPE_ALIAS,
    CONSTANT
  };

  virtual ~ImplItem () {}

  // Unique pointer custom clone function
  std::unique_ptr<ImplItem> clone_inherent_impl_item () const
  {
    return std::unique_ptr<ImplItem> (clone_inherent_impl_item_impl ());
  }

  virtual std::string as_string () const = 0;

  virtual void accept_vis (HIRVisitor &vis) = 0;

  virtual Analysis::NodeMapping get_impl_mappings () const = 0;

  virtual Location get_locus () const = 0;

  virtual ImplItemType get_impl_item_type () const = 0;

protected:
  // Clone function implementation as pure virtual method
  virtual ImplItem *clone_inherent_impl_item_impl () const = 0;
};

// A crate HIR object - holds all the data for a single compilation unit
struct Crate
{
  AST::AttrVec inner_attrs;
  // dodgy spacing required here
  /* TODO: is it better to have a vector of items here or a module (implicit
   * top-level one)? */
  std::vector<std::unique_ptr<Item> > items;

  Analysis::NodeMapping mappings;

public:
  // Constructor
  Crate (std::vector<std::unique_ptr<Item> > items, AST::AttrVec inner_attrs,
	 Analysis::NodeMapping mappings)
    : inner_attrs (std::move (inner_attrs)), items (std::move (items)),
      mappings (mappings)
  {}

  // Copy constructor with vector clone
  Crate (Crate const &other)
    : inner_attrs (other.inner_attrs), mappings (other.mappings)
  {
    items.reserve (other.items.size ());
    for (const auto &e : other.items)
      items.push_back (e->clone_item ());
  }

  ~Crate () = default;

  // Overloaded assignment operator with vector clone
  Crate &operator= (Crate const &other)
  {
    inner_attrs = other.inner_attrs;
    mappings = other.mappings;

    items.reserve (other.items.size ());
    for (const auto &e : other.items)
      items.push_back (e->clone_item ());

    return *this;
  }

  // Move constructors
  Crate (Crate &&other) = default;
  Crate &operator= (Crate &&other) = default;

  // Get crate representation as string (e.g. for debugging).
  std::string as_string () const;

  const Analysis::NodeMapping &get_mappings () const { return mappings; }
};

// Base path expression HIR node - abstract
class PathExpr : public ExprWithoutBlock
{
protected:
  PathExpr (Analysis::NodeMapping mappings, AST::AttrVec outer_attribs)
    : ExprWithoutBlock (std::move (mappings), std::move (outer_attribs))
  {}

public:
  // TODO: think of a better and less hacky way to allow this

  /* Replaces the outer attributes of this path expression with the given outer
   * attributes. */
  void replace_outer_attrs (AST::AttrVec outer_attrs)
  {
    set_outer_attrs (std::move (outer_attrs));
  }
};
} // namespace HIR
} // namespace Rust

#endif
