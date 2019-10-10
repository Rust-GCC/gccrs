#ifndef RUST_AST_BASE_H
#define RUST_AST_BASE_H
// Base for AST used in gccrs

// GCC imports 
#include "config.h"
#define INCLUDE_UNIQUE_PTR // should allow including the gcc emulation of std::unique_ptr
#include "system.h"
#include "coretypes.h"

// STL imports
#include <string>
#include <vector>

namespace Rust {
    // TODO: remove
    typedef int location_t;
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
        class AttrInput {};

        // A tree of tokens (or a single token) - abstract base class
        class TokenTree {};

        // Forward decl for MacroMatch, defined in rust-macro.h
        class MacroMatch;

        // A token is a kind of token tree (except delimiter tokens)
        class Token : public TokenTree, public MacroMatch {
            // A token is a kind of token tree (except delimiter tokens)
            // A token is a kind of MacroMatch (except $ and delimiter tokens)
        };

        // A token tree with delimiters
        class DelimTokenTree : public TokenTree, public AttrInput {
            enum DelimType {
                PARENS,
                SQUARE,
                CURLY
            } delim_type;

            ::std::vector<TokenTree> token_trees;
        };

        // forward decl LiteralExpr: must have rust-expr.h also included to use this
        class LiteralExpr;

        // Literal expression attribute body (non-macro attribute)
        class AttrInputLiteral : public AttrInput {
            // Literal expression WITHOUT SUFFIX
            //LiteralExpr* literal_expr;
            ::gnu::unique_ptr<LiteralExpr> literal_expr;

          public:
            /*~AttrInputLiteral() {
                delete literal_expr;
            }*/
        };

        // forward decl for SimplePath - defined in rust-path.h
        class SimplePath;

        // aka Attr
        // Attribute AST representation
        struct Attribute {
            SimplePath path;

            bool has_attr_input;
            //AttrInput* attr_input;
            ::gnu::unique_ptr<AttrInput> attr_input;

            public:
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
        };

        // Syntax used for Attribute by most built-in attributes and the meta fragment spec
        class MetaItem {
            SimplePath path;
        };

        // A literal meta item
        class MetaItemLit : public MetaItem {
            //LiteralExpr* expr;
            ::gnu::unique_ptr<LiteralExpr> expr;

          public:
            /*~MetaItemLit() {
                delete expr;
            }*/
        };

        // An inner meta item
        struct MetaItemInner {
            // Allows EITHER MetaItem or LiteralExpression (without suffix)
            bool lit_active;
            /*MetaItem* item;
            LiteralExpr* expr;*/
            ::gnu::unique_ptr<MetaItem> item;
            ::gnu::unique_ptr<LiteralExpr> expr;

            // as no more conditional delete on expr member variable, must initialise it as NULL 
          public:
            /*~MetaItemInner() {
                if (lit_active) {
                    delete expr;
                }
                delete item;
            }*/
        };

        // A sequence meta item
        class MetaItemSeq : public MetaItem {
            bool has_sequence;
            ::std::vector<MetaItemInner> sequence;
        };

        // Something to do with subsets of MetaItem syntax or something
        struct MetaWord {
            Identifier word;
        };

        struct MetaNameValueStr {
            Identifier name;
            ::std::string value;
        };

        struct MetaListPaths {
            Identifier type_thing;
            ::std::vector<SimplePath> paths;
        };

        struct MetaListIdents {
            Identifier directive_thing;
            ::std::vector<Identifier> idents_to_use;
        };

        struct MetaListNameValueStr {
            Identifier macro_name_thing;
            ::std::vector<MetaNameValueStr> list;
        };

        // Forward decl of Item - defined in rust-item.h
        class Item;

        // A crate AST object - holds all the data for a single compilation unit
        struct Crate {
            bool has_utf8bom;
            bool has_shebang;

            ::std::vector<Attribute> inner_attrs;
            //::std::vector<Item> items;
            // dodgy spacing required here
            ::std::vector< ::gnu::unique_ptr<Item> > items;
        };

        // Rust "item" AST node (declaration of top-level/module-level allowed stuff)
        class Item : public Node {
            ::std::vector<Attribute> outer_attrs; // maybe only outer attributes
        };
    }
}

#endif