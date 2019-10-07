#ifndef RUST_TYPES_H
#define RUST_TYPES_H
// statically-typed AST nodes and whatever - may move

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

        // forward decl LiteralExpr: must have rust-types-expr.h also included to use this
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

        // Pattern base AST node
        class Pattern : public Node {};

        // Literal pattern AST node (comparing to a literal)
        class LiteralPattern : public Pattern {
            Literal val; // make literal have a type given by enum, etc. rustc uses an extended form
            // of its literal token implementation

            // Minus prefixed to literal (if integer or floating-point)
            bool has_minus;
            // Actually, this might be a good place to use a template.
        };

        // Identifier pattern AST node (bind value matched to a variable)
        class IdentifierPattern : public Pattern {
            Identifier variable_ident;
            bool is_ref;
            bool is_mut;

            bool has_pattern;
            Pattern* to_bind;

          public:
            ~IdentifierPattern() {
                delete to_bind;
            }
        };

        // AST node for using the '_' wildcard "match any value" pattern
        class WildcardPattern : public Pattern {};

        // Base range pattern bound (lower or upper limit) - abstract
        class RangePatternBound {
            /*union {
                CharLiteral char_lit;
                ByteLiteral byte_lit;
                IntLiteral int_lit;
                FloatLiteral float_lit;
                PathInExpression path;
                QualifiedPathInExpression qual_path;
            } pattern;*/
        };

        // Literal-based pattern bound
        class RangePatternBoundLiteral : public RangePatternBound {
            Literal literal;
            // Can only be a char, byte, int, or float literal - same impl here as previously

            // Minus prefixed to literal (if integer or floating-point)
            bool has_minus;
        };

        // Path-based pattern bound
        class RangePatternBoundPath : public RangePatternBound {
            PathInExpression path;
        };

        // Qualified path-based pattern bound
        class RangePatternBoundQualPath : public RangePatternBound {
            QualifiedPathInExpression path;
        };

        // AST node for matching within a certain range (range pattern)
        class RangePattern : public Pattern {
            RangePatternBound lower;
            RangePatternBound upper;

            bool has_ellipsis_synax;
        };

        // AST node for pattern based on dereferencing the pointers given
        class ReferencePattern : public Pattern {
            bool has_two_amps;
            bool is_mut;
            Pattern* pattern;

          public:
            ~ReferencePattern() {
                delete pattern;
            }
        };

        // aka StructPatternEtCetera; potential element in struct pattern
        struct StructPatternEtc {
            ::std::vector<Attribute> outer_attrs;
        };

        // Base class for a single field in a struct pattern - abstract
        class StructPatternField {
            ::std::vector<Attribute> outer_attrs;
            /*union {
                struct {
                    //TupleIndex index;
                    Pattern tuple_pattern;
                } tuple_pattern;
                struct {
                    //Identifier ident;
                    Pattern ident_pattern;
                } ident_pattern;
                struct {
                    bool has_ref;
                    bool has_mut;
                    //Identifier ident;
                } ident;
            } pattern;*/
        };

        // Tuple pattern single field in a struct pattern
        class StructPatternFieldTuplePat : public StructPatternField {
            TupleIndex index;
            Pattern* tuple_pattern;

          public:
            ~StructPatternFieldTuplePat() {
                delete tuple_pattern;
            }
        };

        // Identifier pattern single field in a struct pattern
        class StructPatternFieldIdentPat : public StructPatternField {
            Identifier ident;
            Pattern* ident_pattern;

          public:
            ~StructPatternFieldIdentPat() {
                delete ident_pattern;
            }
        };

        // Identifier only (with no pattern) single field in a struct pattern
        class StructPatternFieldIdent : public StructPatternField {
            bool has_ref;
            bool has_mut;

            Identifier ident;
        };

        // Elements of a struct pattern
        struct StructPatternElements {
            bool has_struct_pattern_fields;
            ::std::vector<StructPatternField> fields;

            bool has_struct_pattern_etc;
            StructPatternEtc etc;

            // must have at least one of the two and maybe both
        };

        // Struct pattern AST node representation
        class StructPattern : public Pattern {
            PathInExpression path;

            bool has_struct_pattern_elements;
            StructPatternElements elems;
        };

        // Base abstract class for patterns used in TupleStructPattern
        class TupleStructItems {};

        // Class for non-ranged tuple struct pattern patterns
        class TupleStructItemsNoRange : public TupleStructItems {
            ::std::vector<Pattern> patterns;
        };

        // Class for ranged tuple struct pattern patterns
        class TupleStructItemsRange : public TupleStructItems {
            ::std::vector<Pattern> lower_patterns;
            ::std::vector<Pattern> upper_patterns;
        };

        // AST node representing a tuple struct pattern
        class TupleStructPattern : public Pattern {
            PathInExpression path;
            TupleStructItems items;
        };

        // Base abstract class representing TuplePattern patterns
        class TuplePatternItems {};

        // Class representing TuplePattern patterns where there is only a single pattern
        class TuplePatternItemsSingle {
            Pattern pattern;
        };

        // Class representing TuplePattern patterns where there are multiple patterns
        class TuplePatternItemsMultiple {
            ::std::vector<Pattern> patterns;
        };

        // Class representing TuplePattern patterns where there are a range of patterns
        class TuplePatternItemsRanged {
            ::std::vector<Pattern> lower_patterns;
            ::std::vector<Pattern> upper_patterns;
        };

        // AST node representing a tuple pattern
        class TuplePattern : public Pattern {
            bool has_tuple_pattern_items;
            TuplePatternItems items;
        };

        // AST node representing a pattern in parentheses, used to control precedence
        class GroupedPattern : public Pattern {
            Pattern pattern_in_parens;
        };

        // AST node representing patterns that can match slices and arrays
        class SlicePattern : public Pattern {
            ::std::vector<Pattern> items;
        };

        // AST node representing a pattern that involves a "path" - abstract base class
        class PathPattern : public Pattern {};

        // Forward decls for paths (defined in rust-path.h)
        class PathInExpression;
        class QualifiedPathInExpression;

        // AST node representing a macro invocation
        class MacroInvocation : public Pattern {};
    }
}

#endif