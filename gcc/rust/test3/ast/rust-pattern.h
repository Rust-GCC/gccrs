#ifndef RUST_AST_PATTERN_H
#define RUST_AST_PATTERN_H

#include "rust-ast.h"

namespace Rust {
    namespace AST {
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