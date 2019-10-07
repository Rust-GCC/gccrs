#ifndef RUST_AST_BASE_H
#define RUST_AST_BASE_H
// Base for AST used in gccrs

#include <string>
#include <vector>

//#include <tr1/memory> // as memory not available in c++03

// std::tr1::shared_ptr<int> no;
// TODO: no unique_ptr<T> in C++03, so have to use regular pointers with delete and whatever

namespace Rust {
    // TODO: remove
    typedef int location_t;
    //typedef ::std::string SimplePath;
    typedef ::std::string Identifier;

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
            virtual ::std::string as_string() = 0;

            virtual ~Node() {}

          private:
            // The node's location.
            location_t loc;
        };

        // Attribute body - abstract base class
        class AttrInput {};

        // Macro attribute body? May use multiple inheritance here.
        struct DelimTokenTree : public AttrInput {};

        // forward decl LiteralExpr: must have rust-expr.h also included to use this
        class LiteralExpr;

        // Literal expression attribute body (non-macro attribute)
        class AttrInputLiteral : public AttrInput {
            // Literal expression WITHOUT SUFFIX
            LiteralExpr* literal_expr;

          public:
            ~AttrInputLiteral() {
                delete literal_expr;
            }
        };

        // forward decl for SimplePath - defined in rust-path.h
        class SimplePath;

        // Attribute
        struct Attribute {
            SimplePath path;

            bool has_attr_input;
            AttrInput attr_input;

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

        // The rule for the DelimTokenTree used for AttrInput?
        class MetaItem {
            SimplePath path;
        };

        class MetaItemLit : public MetaItem {
            LiteralExpr* expr;

          public:
            ~MetaItemLit() {
                delete expr;
            }
        };

        struct MetaItemInner {
            // Allows EITHER MetaItem or LiteralExpression (without suffix)
            bool lit_active;
            MetaItem item;
            LiteralExpr* expr;

          public:
            ~MetaItemInner() {
                if (lit_active) {
                    delete expr;
                }
            }
        };

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
    }
}

#endif