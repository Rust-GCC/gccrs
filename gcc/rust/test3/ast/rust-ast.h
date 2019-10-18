#ifndef RUST_AST_BASE_H
#define RUST_AST_BASE_H
// Base for AST used in gccrs, basically required by all specific ast things

// GCC imports
#include "config.h"
#define INCLUDE_UNIQUE_PTR // should allow including the gcc emulation of std::unique_ptr
#include "system.h"
#include "coretypes.h"     // order: config, INCLUDE, system, coretypes

// STL imports
#include <string>
#include <vector>

namespace Rust {
    // TODO: remove
    // typedef int location_t;
    // typedef ::std::string SimplePath;
    typedef ::std::string Identifier;
    typedef int TupleIndex;
    typedef int Literal;

    namespace AST {
        // Base AST node object
        class Node {
          public:
            // Gets node's location_t.
            location_t get_locus() const {
                return loc;
            }

            // Sets node's location_t.
            void set_locus(location_t loc_) {
                loc = loc_;
            }

            // Get node output as a string. Pure virtual.
            virtual ::std::string as_string() const = 0;

            virtual ~Node() {}

          private:
            // The node's location.
            location_t loc;
        };

        // Attribute body - abstract base class
        class AttrInput {
          public:
            virtual ~AttrInput() {}

            // Unique pointer custom clone function
            ::gnu::unique_ptr<AttrInput> clone_attr_input() const {
                return ::gnu::unique_ptr<AttrInput>(clone_attr_input_impl());
            }

          protected:
            // pure virtual clone implementation
            virtual AttrInput* clone_attr_input_impl() const = 0;
        };

        // A tree of tokens (or a single token) - abstract base class
        class TokenTree {
          public:
            virtual ~TokenTree() {}
        };

        // Abstract base class for a macro match
        class MacroMatch {
          public:
            virtual ~MacroMatch() {}
        };

        // A token is a kind of token tree (except delimiter tokens)
        class Token
          : public TokenTree
          , public MacroMatch {
            // A token is a kind of token tree (except delimiter tokens)
            // A token is a kind of MacroMatch (except $ and delimiter tokens)

          public:
            // Unique pointer custom clone function
            ::gnu::unique_ptr<Token> clone_token() const {
                return ::gnu::unique_ptr<Token>(clone_token_impl());
            }

          protected:
            // No virtual for now as not polymorphic but can be in future
            /*virtual*/ Token* clone_token_impl() const {
                return new Token(*this);
            }
        };

        // A token tree with delimiters
        class DelimTokenTree
          : public TokenTree
          , public AttrInput {
            enum DelimType { PARENS, SQUARE, CURLY } delim_type;

            ::std::vector< ::gnu::unique_ptr<TokenTree> > token_trees;

          protected:
            // Use covariance to implement clone function as returning a DelimTokenTree object
            virtual DelimTokenTree* clone_attr_input_impl() const OVERRIDE {
                return new DelimTokenTree(*this);
            }

          public:
            DelimTokenTree(
              DelimType delim_type, ::std::vector< ::gnu::unique_ptr<TokenTree> > token_trees) :
              delim_type(delim_type),
              token_trees(token_trees) {}
        };

        // Forward decl - definition moved to rust-expr.h as it requires LiteralExpr to be defined
        class AttrInputLiteral;

        // TODO: move applicable stuff into here
        // A segment of a path (maybe)
        class PathSegment {
          public:
            virtual ~PathSegment() {}
        };

        // A segment of a simple path without generic or type arguments
        class SimplePathSegment : public PathSegment {
            ::std::string segment_name;

            // only allow identifiers, "super", "self", "crate", or "$crate"
          public:
            // TODO: put checks in constructor to enforce this rule?
            SimplePathSegment(::std::string segment_name) : segment_name(segment_name) {}
        };

        // A simple path without generic or type arguments
        class SimplePath {
            bool has_opening_scope_resolution;
            ::std::vector<SimplePathSegment> segments;

          public:
            // Constructor with no opening scope resolution
            SimplePath(::std::vector<SimplePathSegment> path_segments) :
              segments(path_segments), has_opening_scope_resolution(false) {}

            // Constructor with an opening scope resolution
            SimplePath(
              ::std::vector<SimplePathSegment> path_segments, bool has_opening_scope_resolution) :
              segments(path_segments),
              has_opening_scope_resolution(has_opening_scope_resolution) {}

            // Creates an empty SimplePath.
            static SimplePath create_empty() {
                ::std::vector<SimplePathSegment> empty_path_segments;

                return SimplePath(empty_path_segments);
            }

            // Returns whether the SimplePath is empty, i.e. has path segments.
            inline bool is_empty() const {
                return segments.empty();
            }
        };

        // aka Attr
        // Attribute AST representation
        struct Attribute {
            SimplePath path;

            // bool has_attr_input;
            // AttrInput* attr_input;
            ::gnu::unique_ptr<AttrInput> attr_input;

          public:
            // Returns whether Attribute has AttrInput
            inline bool has_attr_input() const {
                return attr_input != NULL;
            }

            // Constructor has pointer AttrInput for polymorphism reasons
            Attribute(SimplePath path_, AttrInput* input) : path(path_), attr_input(input) {}

            // TODO: remove and also remove has_attr_input as pointer is nullable
            // Attribute(SimplePath path_) : path(path_), has_attr_input(false), attr_input(NULL) {}
            // this "NULL" is ambiguous

            // Copy constructor must deep copy attr_input as unique pointer
            Attribute(Attribute const& other) :
              path(other.path), attr_input(other.attr_input->clone_attr_input()) {}

            // default destructor
            ~Attribute() = default;

            // overload assignment operator to use custom clone method
            Attribute& operator=(Attribute const& other) {
                path = other.path;
                attr_input = other.attr_input->clone_attr_input();

                return *this;
            }

            // default move semantics but no move in c++03
            /*Attribute(Attribute&& other) = default;
            Attribute& operator=(Attribute&& other) = default;*/

            // Unique pointer custom clone function
            ::gnu::unique_ptr<Attribute> clone_attribute() const {
                return ::gnu::unique_ptr<Attribute>(clone_attribute_impl());
            }

            /*~Attribute() {
                delete attr_input;
            }*/

            /* e.g.:
                #![crate_type = "lib"]
                #[test]
                #[cfg(target_os = "linux")]
                #[allow(non_camel_case_types)]
                #![allow(unused_variables)]
            */

            // Full built-in attribute list:
            /*   cfg
             *   cfg_attr
             *   test
             *   ignore
             *   should_panic
             *   derive
             *   macro_export
             *   macro_use
             *   proc_macro
             *   proc_macro_derive
             *   proc_macro_attribute
             *   allow
             *   warn
             *   deny
             *   forbid
             *   deprecated
             *   must_use
             *   link
             *   link_name
             *   no_link
             *   repr
             *   crate_type
             *   no_main
             *   export_name
             *   link_section
             *   no_mangle
             *   used
             *   crate_name
             *   inline
             *   cold
             *   no_builtins
             *   target_feature
             *   doc
             *   no_std
             *   no_implicit_prelude
             *   path
             *   recursion_limit
             *   type_length_limit
             *   panic_handler
             *   global_allocator
             *   windows_subsystem
             *   feature     */
          protected:
            // not virtual as currently no subclasses of Attribute, but could be in future
            /*virtual*/ Attribute* clone_attribute_impl() const {
                return new Attribute(*this);
            }
        };

        // Syntax used for Attribute by most built-in attributes and the meta fragment spec
        class MetaItem {
          protected:
            SimplePath path;

            MetaItem(SimplePath path) : path(path) {}

            // pure virtual as MetaItem is abstract?
            virtual MetaItem* clone_meta_item_impl() const = 0;

          public:
            // Unique pointer custom clone function
            ::gnu::unique_ptr<MetaItem> clone_meta_item() const {
                return ::gnu::unique_ptr<MetaItem>(clone_meta_item_impl());
            }
        };

        // Forward decl - defined in rust-expr.h
        class MetaItemLit;

        // Forward decl - defined in rust-expr.h
        struct MetaItemInner;

        // Forward decl - defined in rust-expr.h
        class MetaItemSeq;

        // Forward decl - defined in rust-expr.h
        struct MetaWord;

        // Forward decl - defined in rust-expr.h
        struct MetaNameValueStr;

        // Forward decl - defined in rust-expr.h
        struct MetaListPaths;

        // Forward decl - defined in rust-expr.h
        struct MetaListIdents;

        // Forward decl - defined in rust-expr.h
        struct MetaListNameValueStr;

        // Rust "item" AST node (declaration of top-level/module-level allowed stuff)
        class Item : public Node {
          protected:
            ::std::vector<Attribute> outer_attrs;

            // Outer attribute constructor
            Item(::std::vector<Attribute> outer_attribs) : outer_attrs(outer_attribs) {}

            // No outer attributes constructor
            Item() {}
        };

        // Base expression AST node - abstract
        class Expr : public Node {
          public:
            inline ::std::vector<Attribute> get_outer_attrs() const {
                return outer_attrs;
            }

            // Unique pointer custom clone function
            ::gnu::unique_ptr<Expr> clone_expr() const {
                return ::gnu::unique_ptr<Expr>(clone_expr_impl());
            }

          protected:
            ::std::vector<Attribute> outer_attrs;

            // Outer attribute constructor
            Expr(::std::vector<Attribute> outer_attribs) : outer_attrs(outer_attribs) {}

            // No outer attributes constructor
            Expr() {}

            // Clone function implementation as pure virtual method
            virtual Expr* clone_expr_impl() const = 0;
        };

        // AST node for an expression without an accompanying block - abstract
        class ExprWithoutBlock : public Expr {
          protected:
            // Outer attribute constructor
            ExprWithoutBlock(::std::vector<Attribute> outer_attribs) : Expr(outer_attribs) {}

            // No outer attribute constructor
            ExprWithoutBlock() : Expr() {}

            // pure virtual clone implementation
            virtual ExprWithoutBlock* clone_expr_without_block_impl() const = 0;

          public:
            // Unique pointer custom clone function
            ::gnu::unique_ptr<ExprWithoutBlock> clone_expr_without_block() const {
                return ::gnu::unique_ptr<ExprWithoutBlock>(clone_expr_without_block_impl());
            }
        };

        /* Base statement abstract class. Note that statements in Rust are only called as such if they
         * are inside a block inside a function or outer expression, i.e. standalone, module-scope
         * declarations are not statements. */
        class Statement : public Node {};

        // Pattern base AST node
        class Pattern : public Node {
          public:
            // Unique pointer custom clone function
            ::gnu::unique_ptr<Pattern> clone_pattern() const {
                return ::gnu::unique_ptr<Pattern>(clone_pattern_impl());
            }

          protected:
            // Clone pattern implementation as pure virtual method
            virtual Pattern* clone_pattern_impl() const = 0;
        };

        // Base class for types as represented in AST - abstract
        class Type {
          public:
            // Unique pointer custom clone function
            ::gnu::unique_ptr<Type> clone_type() const {
                return ::gnu::unique_ptr<Type>(clone_type_impl());
            }

            // virtual destructor
            virtual ~Type() {}

          protected:
            // Clone function implementation as pure virtual method
            virtual Type* clone_type_impl() const = 0;
        };

        // A type without parentheses? - abstract
        class TypeNoBounds : public Type {
          public:
            // Unique pointer custom clone function
            ::gnu::unique_ptr<TypeNoBounds> clone_type_no_bounds() const {
                return ::gnu::unique_ptr<TypeNoBounds>(clone_type_no_bounds_impl());
            }

          protected:
            // Clone function implementation as pure virtual method
            virtual TypeNoBounds* clone_type_no_bounds_impl() const = 0;
        };

        // Abstract base class representing a type param bound - Lifetime and TraitBound extends it
        class TypeParamBound {
          public:
            virtual ~TypeParamBound() {}
        };

        // Represents a lifetime (and is also a kind of type param bound)
        class Lifetime : public TypeParamBound {
            enum LifetimeType {
                NAMED,   // corresponds to LIFETIME_OR_LABEL
                STATIC,  // corresponds to 'static
                WILDCARD // corresponds to '_
            } lifetime_type;

            // TODO: LIFETIME_OR_LABEL (aka lifetime token) is only field
            // find way of enclosing token or something
            ::std::string lifetime_name;
            // only applies for NAMED lifetime_type

          public:
            Lifetime(LifetimeType type, ::std::string name) :
              lifetime_type(type), lifetime_name(name) {}

            // Creates an "error" lifetime.
            static Lifetime error() {
                return Lifetime(NAMED, ::std::string(""));
            }

            // Returns true if the lifetime is in an error state.
            inline bool is_error() const {
                return lifetime_type == NAMED && lifetime_name.empty();
            }
        };

        // Base generic parameter in AST. Abstract - can be represented by a Lifetime or Type param
        class GenericParam {
          public:
            virtual ~GenericParam() {}
        };

        // A lifetime generic parameter (as opposed to a type generic parameter)
        class LifetimeParam : public GenericParam {
            // bool has_outer_attribute;
            ::gnu::unique_ptr<Attribute> outer_attr;

            Lifetime lifetime;

            // bool has_lifetime_bounds;
            // LifetimeBounds lifetime_bounds;
            ::std::vector<Lifetime> lifetime_bounds; // inlined LifetimeBounds

          public:
            // Returns whether the lifetime param has any lifetime bounds.
            inline bool has_lifetime_bounds() const {
                return !lifetime_bounds.empty();
            }

            // Returns whether the lifetime param has an outer attribute.
            inline bool has_outer_attribute() const {
                return outer_attr != NULL;
            }

            // Constructor for a lifetime param with no attribute.
            LifetimeParam(Lifetime lifetime, ::std::vector<Lifetime> lifetime_bounds) :
              lifetime(lifetime), lifetime_bounds(lifetime_bounds)/*, outer_attr(NULL)*/ {}

            // Constructor for a lifetime param with an attribute.
            LifetimeParam(
              Lifetime lifetime, ::std::vector<Lifetime> lifetime_bounds, Attribute* outer_attr) :
              lifetime(lifetime),
              lifetime_bounds(lifetime_bounds), outer_attr(outer_attr) {}

            // Copy constructor with clone
            LifetimeParam(LifetimeParam const& other) :
              lifetime(other.lifetime), lifetime_bounds(other.lifetime_bounds),
              outer_attr(other.outer_attr->clone_attribute()) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone attribute
            LifetimeParam& operator=(LifetimeParam const& other) {
                lifetime = other.lifetime;
                lifetime_bounds = other.lifetime_bounds;
                outer_attr = other.outer_attr->clone_attribute();

                return *this;
            }

            // no move constructors as not supported in c++03
            /*LifetimeParam(LifetimeParam&& other) = default;
            LifetimeParam& operator=(LifetimeParam&& other) = default;*/
        };

        // A macro item AST node - potentially abstract base class
        class MacroItem : public Item {
            /*public:
            ::std::string as_string() const;*/
          protected:
            MacroItem(::std::vector<Attribute> outer_attribs) : Item(outer_attribs) {}
        };

        // A macro invocation item (or statement) AST node
        class MacroInvocationSemi
          : public MacroItem
          , public Statement {
            SimplePath path;
            enum DelimType {
                PARENS,
                SQUARE,
                CURLY // all delim types except curly must have invocation end with a semicolon
            } delim_type;
            //::std::vector<TokenTree> token_trees;
            ::std::vector< ::gnu::unique_ptr<TokenTree> > token_trees;

          public:
            ::std::string as_string() const;

            MacroInvocationSemi(SimplePath macro_path, DelimType delim_type,
              ::std::vector< ::gnu::unique_ptr<TokenTree> > token_trees,
              ::std::vector<Attribute> outer_attribs) :
              path(macro_path),
              delim_type(delim_type), token_trees(token_trees), MacroItem(outer_attribs) {}
        };

        // A crate AST object - holds all the data for a single compilation unit
        struct Crate {
            bool has_utf8bom;
            bool has_shebang;

            ::std::vector<Attribute> inner_attrs;
            //::std::vector<Item> items;
            // dodgy spacing required here
            ::std::vector< ::gnu::unique_ptr<Item> > items;

          public:
            // Constructor for crate without shebang or utf8bom
            Crate(
              ::std::vector< ::gnu::unique_ptr<Item> > items, ::std::vector<Attribute> inner_attrs) :
              items(items),
              inner_attrs(inner_attrs), has_shebang(false), has_utf8bom(false) {}

            // Constructor with potentially a shebang and/or utf8bom
            Crate(::std::vector< ::gnu::unique_ptr<Item> > items,
              ::std::vector<Attribute> inner_attrs, bool has_utf8bom, bool has_shebang) :
              items(items),
              inner_attrs(inner_attrs), has_shebang(has_shebang), has_utf8bom(has_utf8bom) {}

            ~Crate() = default;
        };
    }
}

#endif