#ifndef RUST_AST_EXPR_H
#define RUST_AST_EXPR_H

#include "rust-ast.h"

namespace Rust {
    namespace AST {
        /* TODO: if GCC moves to C++17 or allows boost, replace some boolean "has_whatever" pairs with
         * optional types (std::optional or boost::optional). */

        // Base expression AST node - abstract
        class Expr : public Node {
            ::std::vector<Attribute> outer_attrs;

          public:
            inline ::std::vector<Attribute> get_outer_attrs() const {
                return outer_attrs;
            }
        };

        // AST node for an expression without an accompanying block - abstract
        class ExprWithoutBlock : public Expr {};

        // AST node for an expression with an accompanying block - abstract
        class ExprWithBlock : public Expr {};

        // Literals? Or literal base?
        class LiteralExpr : public ExprWithoutBlock {
            // TODO: maybe make subclasses of each type of literal with their typed values
            ::std::string value_as_string;
            enum LitType {
                CHAR,
                STRING,
                RAW_STRING,
                BYTE,
                BYTE_STRING,
                RAW_BYTE_STRING,
                INT,
                FLOAT,
                BOOL
            } type;

          public:
            ::std::string as_string() const {
                return value_as_string;
            }

            inline LitType get_lit_type() const {
                return type;
            }
        };

        // Base path expression AST node - abstract
        class PathExpr : public ExprWithoutBlock {};

        // AST node for a non-qualified path expression
        class PathExprNonQual : public PathExpr {
            PathInExpression path;

          public:
            ::std::string as_string() const {
                return path.as_string();
            }
        };

        // AST node for a qualified path expression
        class PathExprQual : public PathExpr {
            QualifiedPathInExpression path;

          public:
            ::std::string as_string() const {
                return path.as_string();
            }
        };

        // Represents an expression using unary or binary operators as AST node. Can be overloaded.
        class OperatorExpr : public ExprWithoutBlock {
            Expr* main_or_left_expr;

          public:
            virtual ~OperatorExpr() {
                delete main_or_left_expr;
            }
        };

        // Unary prefix & or &mut (or && and &&mut) borrow operator. Cannot be overloaded.
        class BorrowExpr : public OperatorExpr {
            bool is_mut;
            bool double_borrow;

          public:
            ::std::string as_string() const;
        };

        // Unary prefix * deference operator
        class DereferenceExpr : public OperatorExpr {
          public:
            ::std::string as_string() const;
        };

        // Unary postfix ? error propogation operator. Cannot be overloaded.
        class ErrorPropogationExpr : public OperatorExpr {
          public:
            ::std::string as_string() const;
        };

        // Unary prefix - or ! negation or NOT operators.
        class NegationExpr : public OperatorExpr {
            // Note: overload negation via std::ops::Neg and not via std::ops::Not
            // Negation only works for signed integer and floating-point types, NOT only works for
            // boolean and integer types (via bitwise NOT)
            enum NegationType { NEGATE, NOT } negation_type;

          public:
            ::std::string as_string() const;

            inline NegationType get_negation_type() const {
                return negation_type;
            }
        };

        // Infix binary operators. +, -, *, /, %, &, |, ^, <<, >>
        class ArithmeticOrLogicalExpr : public OperatorExpr {
            // Note: overloading trait specified in comments
            enum ExprType {
                ADD,         // std::ops::Add
                SUBTRACT,    // std::ops::Sub
                MULTIPLY,    // std::ops::Mul
                DIVIDE,      // std::ops::Div
                MODULUS,     // std::ops::Rem
                BITWISE_AND, // std::ops::BitAnd
                BITWISE_OR,  // std::ops::BitOr
                BITWISE_XOR, // std::ops::BitXor
                LEFT_SHIFT,  // std::ops::Shl
                RIGHT_SHIFT  // std::ops::Shr
            } expr_type;

            Expr* right_expr;

          public:
            ~ArithmeticOrLogicalExpr() {
                delete right_expr;
            }

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }
        };

        // Infix binary comparison operators. ==, !=, <, <=, >, >=
        class ComparisonExpr : public OperatorExpr {
            // Note: overloading trait specified in comments
            enum ExprType {
                EQUAL,            // std::cmp::PartialEq::eq
                NOT_EQUAL,        // std::cmp::PartialEq::ne
                GREATER_THAN,     // std::cmp::PartialEq::gt
                LESS_THAN,        // std::cmp::PartialEq::lt
                GREATER_OR_EQUAL, // std::cmp::PartialEq::ge
                LESS_OR_EQUAL     // std::cmp::PartialEq::le
            } expr_type;

            Expr* right_expr;

          public:
            ~ComparisonExpr() {
                delete right_expr;
            }

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }

            // TODO: implement via a function call to std::cmp::PartialEq::eq(&op1, &op2) maybe?
        };

        // Infix binary lazy boolean logical operators && and ||.
        class LazyBooleanExpr : public OperatorExpr {
            enum ExprType { LOGICAL_OR, LOGICAL_AND } expr_type;

            Expr* right_expr;

          public:
            ~LazyBooleanExpr() {
                delete right_expr;
            }

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }
        };

        // Binary infix "as" cast expression.
        class TypeCastExpr : public OperatorExpr {
            TypeNoBounds type_to_convert_to;

            // Note: only certain allowed, outlined in reference
          public:
            ::std::string as_string() const;
        };

        // Binary assignment expression.
        class AssignmentExpr : public OperatorExpr {
            Expr* right_expr;

          public:
            ~AssignmentExpr() {
                delete right_expr;
            }

            ::std::string as_string() const;
        };

        // Binary infix compound assignment (arithmetic or logic then assignment) expressions.
        class CompoundAssignmentExpr : public OperatorExpr {
            // Note: overloading trait specified in comments
            enum ExprType {
                ADD,         // std::ops::AddAssign
                SUBTRACT,    // std::ops::SubAssign
                MULTIPLY,    // std::ops::MulAssign
                DIVIDE,      // std::ops::DivAssign
                MODULUS,     // std::ops::RemAssign
                BITWISE_AND, // std::ops::BitAndAssign
                BITWISE_OR,  // std::ops::BitOrAssign
                BITWISE_XOR, // std::ops::BitXorAssign
                LEFT_SHIFT,  // std::ops::ShlAssign
                RIGHT_SHIFT  // std::ops::ShrAssign
            } expr_type;

            Expr* right_expr;

          public:
            ~CompoundAssignmentExpr() {
                delete right_expr;
            }

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }
        };

        // Expression in parentheses (i.e. like literally just any 3 + (2 * 6))
        class GroupedExpr : public ExprWithoutBlock {
            ::std::vector<Attribute> inner_attrs;
            Expr* expr_in_parens;

          public:
            ~GroupedExpr() {
                delete expr_in_parens;
            }

            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }
        };

        // Base array initialisation internal element representation thing
        // aka ArrayElements
        class ArrayElems {
          public:
            virtual ~ArrayElems() {}
        };

        // Value array elements
        class ArrayElemsValues : public ArrayElems {
            ::std::vector<Expr> values;

          public:
            inline ::std::vector<Expr> get_values() const {
                return values;
            }
        };

        // Copied array element and number of copies
        class ArrayElemsCopied : public ArrayElems {
            Expr* elem_to_copy;
            Expr* num_copies;

          public:
            ~ArrayElemsCopied() {
                delete num_copies;
                delete elem_to_copy;
            }
        };

        // Array definition-ish expression
        class ArrayExpr : public ExprWithoutBlock {
            ::std::vector<Attribute> inner_attrs;
            ArrayElems internal_elements;

          public:
            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }

            inline ArrayElems get_internal_elems() const {
                return internal_elements;
            }
        };

        // Aka IndexExpr (also applies to slices)
        // Apparently a[b] is equivalent to *std::ops::Index::index(&a, b) or
        // *std::ops::Index::index_mut(&mut a, b)
        // Also apparently deref operations on a will be repeatedly applied to find an implementation
        class ArrayIndexExpr : public ExprWithoutBlock {
            Expr* array_expr;
            Expr* index_expr;

          public:
            ~ArrayIndexExpr() {
                delete index_expr;
                delete array_expr;
            }

            ::std::string as_string() const;
        };

        // AST representation of a tuple
        class TupleExpr : public ExprWithoutBlock {
            ::std::vector<Attribute> inner_attrs;

            ::std::vector<Expr> tuple_elems;
            // replaces (inlined version of) TupleElements

          public:
            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }

            inline ::std::vector<Expr> get_tuple_elems() const {
                return tuple_elems;
            }

            // Note: syntactically, can disambiguate single-element tuple from parens with comma, i.e.
            // (0,) rather than (0)
        };

        // aka TupleIndexingExpr
        // AST representation of a tuple indexing expression
        class TupleIndexExpr : public ExprWithoutBlock {
            Expr* tuple_expr;
            // TupleIndex is a decimal int literal with no underscores or suffix
            TupleIndex tuple_index;

            // i.e. pair.0

          public:
            ~TupleIndexExpr() {
                delete tuple_expr;
            }

            ::std::string as_string() const;

            inline TupleIndex get_tuple_index() const {
                return tuple_index;
            }
        };

        // Base struct/tuple/union value creator AST node (abstract)
        class StructExpr : public ExprWithoutBlock {
            PathInExpression struct_name;

          public:
            inline PathInExpression get_struct_name() const {
                return struct_name;
            }
        };

        // Actual AST node of the struct creator (with no fields). Not abstract!
        class StructExprStruct : public StructExpr {
            ::std::vector<Attribute> inner_attrs;

          public:
            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }
        };

        // AST node representing expression used to fill a struct's fields from another struct
        struct StructBase {
            Expr* base_struct;

          public:
            StructBase(Expr* base_struct_ptr) : base_struct(base_struct_ptr) {}

            ~StructBase() {
                delete base_struct;
            }

            // Returns a null expr-ed StructBase - error state
            static StructBase error() {
                return StructBase(NULL);
            }
        };

        // Base AST node for a single struct expression field (in struct instance creation) - abstract
        class StructExprField {};

        // Identifier-only variant of StructExprField AST node
        class StructExprFieldIdentifier : public StructExprField {
            Identifier field_name;
        };

        // Base AST node for a single struct expression field with an assigned value - abstract
        class StructExprFieldWithVal : public StructExprField {
            Expr* value;

          public:
            ~StructExprFieldWithVal() {
                delete value;
            }
        };

        // Identifier and value variant of StructExprField AST node
        class StructExprFieldIdentifierValue : public StructExprFieldWithVal {
            Identifier field_name;
        };

        // Tuple index and value variant of StructExprField AST node
        class StructExprFieldIndexValue : public StructExprFieldWithVal {
            TupleIndex index;
        };

        // AST node of a struct creator with fields
        class StructExprStructFields : public StructExprStruct {
            ::std::vector<StructExprField> fields;

            bool has_struct_base;
            StructBase struct_base;

          public:
            ::std::string as_string() const;

            inline ::std::vector<StructExprField> get_fields() const {
                return fields;
            }

            inline StructBase get_struct_base() const {
                return has_struct_base ? struct_base : StructBase::error();
            }
        };

        // AST node of the functional update struct creator
        class StructExprStructBase : public StructExprStruct {
            StructBase struct_base;

          public:
            ::std::string as_string() const;

            inline StructBase get_struct_base() const {
                return struct_base;
            }
        };

        // AST node of a tuple struct creator
        class StructExprTuple : public StructExpr {
            ::std::vector<Attribute> inner_attrs;
            ::std::vector<Expr> exprs;

          public:
            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }

            inline ::std::vector<Expr> get_exprs() const {
                return exprs;
            }
        };

        // AST node of a "unit" struct creator (no fields and no braces)
        class StructExprUnit : public StructExpr {
          public:
            ::std::string as_string() const {
                return get_struct_name().as_string();
                // return struct_name.as_string();
            }
        };

        // aka EnumerationVariantExpr
        // Base AST node representing creation of an enum variant instance - abstract
        class EnumVariantExpr : public ExprWithoutBlock {
            PathInExpression enum_variant_path;

          public:
            // TODO: maybe remove and have string version gotten here directly
            inline PathInExpression get_enum_variant_path() const {
                return enum_variant_path;
            }
        };

        // Base AST node for a single enum expression field (in enum instance creation) - abstract
        class EnumExprField {};

        // Identifier-only variant of EnumExprField AST node
        class EnumExprFieldIdentifier : public EnumExprField {
            Identifier field_name;
        };

        // Base AST node for a single enum expression field with an assigned value - abstract
        class EnumExprFieldWithVal : public EnumExprField {
            Expr* value;

          public:
            ~EnumExprFieldWithVal() {
                delete value;
            }
        };

        // Identifier and value variant of EnumExprField AST node
        class EnumExprFieldIdentifierValue : public EnumExprFieldWithVal {
            Identifier field_name;
        };

        // Tuple index and value variant of EnumExprField AST node
        class EnumExprFieldIndexValue : public EnumExprFieldWithVal {
            TupleIndex index;
        };

        // Struct-like syntax enum variant instance creation AST node
        class EnumExprStruct : public EnumVariantExpr {
            ::std::vector<EnumExprField> fields;

          public:
            ::std::string as_string() const;

            inline ::std::vector<EnumExprField> get_fields() const {
                return fields;
            }
        };

        // Tuple-like syntax enum variant instance creation AST node
        class EnumExprTuple : public EnumVariantExpr {
            ::std::vector<Expr> values;

          public:
            ::std::string as_string() const;

            inline ::std::vector<Expr> get_values() const {
                return values;
            }
        };

        // No-field enum variant instance creation AST node
        class EnumExprFieldless : public EnumVariantExpr {
          public:
            ::std::string as_string() const {
                // return enum_variant_path.as_string();
                return get_enum_variant_path().as_string();
            }
        };

        // TODO: inline
        struct CallParams {
            ::std::vector<Expr> params;
        };

        // Function call expression AST node
        class CallExpr : public ExprWithoutBlock {
            Expr* function;
            ::std::vector<Expr> params; // inlined form of CallParams

          public:
            ~CallExpr() {
                delete function;
            }

            ::std::string as_string() const;

            inline ::std::vector<Expr> get_params() const {
                return params;
            }
        };

        // Method call expression AST node
        class MethodCallExpr : public ExprWithoutBlock {
            Expr* receiver;
            PathExprSegment method_name;
            ::std::vector<Expr> params; // inlined form of CallParams

          public:
            ~MethodCallExpr() {
                delete receiver;
            }

            ::std::string as_string() const;

            inline ::std::vector<Expr> get_params() const {
                return params;
            }
        };

        // aka FieldExpression
        // Struct or union field access expression AST node
        class FieldAccessExpr : public ExprWithoutBlock {
            Expr* receiver;
            Identifier field;

          public:
            ~FieldAccessExpr() {
                delete receiver;
            }

            ::std::string as_string() const;
        };

        // Closure parameter data structure
        struct ClosureParam {
            Pattern pattern;

            bool has_type_given;
            Type type;
        };

        // Base closure definition expression AST node - abstract
        class ClosureExpr : public ExprWithoutBlock {
            bool has_move;
            ::std::vector<ClosureParam> params; // may be empty
            // also note a double pipe "||" can be used for empty params - does not need a space
        };

        // Represents a non-type-specified closure expression AST node
        class ClosureExprInner : public ClosureExpr {
            Expr* closure_inner;

          public:
            ~ClosureExprInner() {
                delete closure_inner;
            }

            ::std::string as_string() const;
        };

        // Represents a type-specified closure expression AST node
        class ClosureExprInnerTyped : public ClosureExpr {
            Type return_type;
            BlockExpr* expr;

          public:
            ~ClosureExprInnerTyped() {
                delete expr;
            }

            ::std::string as_string() const;
        };

        // AST node representing continue expression within loops
        class ContinueExpr : public ExprWithoutBlock {
            bool has_label;
            Lifetime label;

          public:
            ::std::string as_string() const;
        };
        // TODO: merge "break" and "continue"? Or even merge in "return"?

        // AST node representing break expression within loops
        class BreakExpr : public ExprWithoutBlock {
            bool has_label;
            Lifetime label;

            bool has_break_expr;
            Expr* break_expr; // may be uninitialised

          public:
            ~BreakExpr() {
                if (has_break_expr) {
                    delete break_expr;
                }
            }

            ::std::string as_string() const;
        };

        // Base range expression AST node object
        class RangeExpr : public ExprWithoutBlock {};

        // Range from (inclusive) and to (exclusive) expression AST node object
        // aka RangeExpr; constructs a std::ops::Range object
        class RangeFromToExpr : public RangeExpr {
            Expr* from;
            Expr* to;

          public:
            ~RangeFromToExpr() {
                delete from;
                delete to;
            }

            ::std::string as_string() const;
        };

        // Range from (inclusive) expression AST node object
        // constructs a std::ops::RangeFrom object
        class RangeFromExpr : public RangeExpr {
            Expr* from;

          public:
            ~RangeFromExpr() {
                delete from;
            }

            ::std::string as_string() const;
        };

        // Range to (exclusive) expression AST node object
        // constructs a std::ops::RangeTo object
        class RangeToExpr : public RangeExpr {
            Expr* to;

          public:
            ~RangeToExpr() {
                delete to;
            }

            ::std::string as_string() const;
        };

        // Full range expression AST node object
        // constructs a std::ops::RangeFull object
        class RangeFullExpr : public RangeExpr {
            ::std::string as_string() const;
        };

        // Range from (inclusive) and to (inclusive) expression AST node object
        // aka RangeInclusiveExpr; constructs a std::ops::RangeInclusive object
        class RangeFromToInclExpr : public RangeExpr {
            Expr* from;
            Expr* to;

          public:
            ~RangeFromToInclExpr() {
                delete from;
                delete to;
            }

            ::std::string as_string() const;
        };

        // Range to (inclusive) expression AST node object
        // aka RangeToInclusiveExpr; constructs a std::ops::RangeToInclusive object
        class RangeToInclExpr : public RangeExpr {
            Expr* to;

          public:
            ~RangeToInclExpr() {
                delete to;
            }

            ::std::string as_string() const;
        };

        // Return expression AST node representation
        class ReturnExpr : public ExprWithoutBlock {
            bool has_return_expr;
            Expr* return_expr;

          public:
            ~ReturnExpr() {
                if (has_return_expr) {
                    delete return_expr;
                }
            }

            ::std::string as_string() const;
        };

        // Macro invocation expression AST node?
        class MacroInvocation : public ExprWithoutBlock {
          public: // TODO: remove if meant to be abstract
            ::std::string as_string() const;
        };

        // Statement sequence used inside blocks
        // TODO: inline this into BlockExpr?
        struct Statements {
            bool has_statements;
            ::std::vector<Statement> statements;

            bool has_expr;
            ExprWithoutBlock* expr;

          public:
            ~Statements() {
                if (has_expr) {
                    delete expr;
                }
            }
        };

        // A block AST node
        class BlockExpr : public ExprWithBlock {
            ::std::vector<Attribute> inner_attrs;

            bool has_statements;
            Statements statements;

          public:
            ::std::string as_string() const;
        };

        // An unsafe block AST node
        class UnsafeBlockExpr : public ExprWithBlock {
            // Or just have it extend BlockExpr
            BlockExpr* expr;

          public:
            ~UnsafeBlockExpr() {
                delete expr;
            }

            ::std::string as_string() const;
        };

        // Loop label expression AST node used with break and continue expressions
        // TODO: inline?
        class LoopLabel : public Node {
            Lifetime label; // or type LIFETIME_OR_LABEL

          public:
            ::std::string as_string() const;
        };

        // Base loop expression AST node - aka LoopExpr
        class BaseLoopExpr : public ExprWithBlock {
            bool has_loop_label;
            LoopLabel loop_label;

            BlockExpr* loop_block;

          public:
            ~BaseLoopExpr() {
                delete loop_block;
            }

            ::std::string as_string() const;
        };

        // 'Loop' expression (i.e. the infinite loop) AST node
        class LoopExpr : public BaseLoopExpr {};

        // While loop expression AST node (predicate loop)
        class WhileLoopExpr : public BaseLoopExpr {
            Expr* condition;

          public:
            ~WhileLoopExpr() {
                delete condition;
            }

            ::std::string as_string() const;
        };

        // While let loop expression AST node (predicate pattern loop)
        class WhileLetLoopExpr : public BaseLoopExpr {
            MatchArmPatterns patterns;
            Expr* condition;

          public:
            ~WhileLetLoopExpr() {
                delete condition;
            }

            ::std::string as_string() const;
        };

        // For loop expression AST node (iterator loop)
        class ForLoopExpr : public BaseLoopExpr {
            Pattern pattern;
            Expr* iterator_expr;

          public:
            ~ForLoopExpr() {
                delete iterator_expr;
            }

            ::std::string as_string() const;
        };

        // forward decl for IfExpr
        class IfLetExpr;

        // Base if expression with no "else" or "if let" AST node
        class IfExpr : public ExprWithBlock {
            Expr* condition;
            BlockExpr* if_block;
            /*union {
                BlockExpr else_block;
                IfExpr* if_expr;
                IfLetExpr if_let_expr;
            } consequent_block;*/

            /*~IfExpr() {
                // TODO: fix
                delete consequent_block.if_expr;
            }*/

          public:
            virtual ~IfExpr() {
                delete condition;
                delete if_block;
            }

            ::std::string as_string() const;

            // Note that multiple "else if"s are handled via nested ASTs rather than an array of
            // else ifs - i.e. not like a switch statement
        };

        // If expression with an ending "else" expression AST node (trailing)
        class IfExprConseqElse : public IfExpr {
            BlockExpr* else_block;

          public:
            ~IfExprConseqElse() {
                delete else_block;
            }

            ::std::string as_string() const;
        };

        // If expression with an ending "else if" expression AST node
        class IfExprConseqIf : public IfExpr {
            IfExpr* if_expr;

          public:
            ~IfExprConseqIf() {
                delete if_expr;
            }

            ::std::string as_string() const;
        };

        // If expression with an ending "else if let" expression AST node
        class IfExprIfConseqIfLet : public IfExpr {
            IfLetExpr* if_let_expr;

          public:
            ~IfExprIfConseqIfLet() {
                delete if_let_expr;
            }

            ::std::string as_string() const;
        };

        // Basic "if let" expression AST node with no else
        class IfLetExpr : public ExprWithBlock {
            MatchArmPatterns patterns;
            Expr* value;
            BlockExpr* if_block;
            /*union {
                BlockExpr else_block;
                IfExpr if_expr;
                IfLetExpr* if_let_expr;
            } consequent_block;*/

          public:
            virtual ~IfLetExpr() {
                // TODO: fix
                delete value;
                delete if_block;
            }

            ::std::string as_string() const;
        };

        // AST node representing "if let" expression with an "else" expression at the end
        class IfLetExprConseqElse : public IfLetExpr {
            BlockExpr* else_block;

          public:
            ~IfLetExprConseqElse() {
                delete else_block;
            }

            ::std::string as_string() const;
        };

        // AST node representing "if let" expression with an "else if" expression at the end
        class IfLetExprConseqIf : public IfLetExpr {
            IfExpr* if_expr;

          public:
            ~IfLetExprConseqIf() {
                delete if_expr;
            }

            ::std::string as_string() const;
        };

        // AST node representing "if let" expression with an "else if let" expression at the end
        class IfLetExprConseqIfLet : public IfLetExpr {
            IfLetExpr* if_let_expr;

          public:
            ~IfLetExprConseqIfLet() {
                delete if_let_expr;
            }

            ::std::string as_string() const;
        };

        struct MatchArmPatterns {
            // TODO: inline
            ::std::vector<Pattern> patterns;
        };

        struct MatchArmGuard {
            // TODO: inline
            Expr* guard_expr;

          public:
            ~MatchArmGuard() {
                delete guard_expr;
            }
        };

        // Match arm expression
        struct MatchArm {
            ::std::vector<Attribute> outer_attrs;
            MatchArmPatterns patterns;

            bool has_match_arm_guard;
            Expr* match_arm_guard; // TODO: inlined from MatchArmGuard

          public:
            ~MatchArm() {
                if (has_match_arm_guard) {
                    delete match_arm_guard;
                }
            }
        };

        // Base "match case" for a match expression - abstract
        class MatchCase {
            MatchArm arm;
        };

        // Block expression match case
        class MatchCaseBlockExpr : public MatchCase {
            BlockExpr* block_expr;

          public:
            ~MatchCaseBlockExpr() {
                delete block_expr;
            }
        };

        // Expression (except block expression) match case
        class MatchCaseExpr : public MatchCase {
            Expr* expr;

          public:
            ~MatchCaseExpr() {
                delete expr;
            }
        };

        struct MatchArms {
            ::std::vector<MatchCase> cases;
            // TODO: inline type?
        };

        // Match expression AST node
        class MatchExpr : public ExprWithBlock {
            Expr* branch_value;
            ::std::vector<Attribute> inner_attrs;

            bool has_match_arms;
            MatchArms match_arms;

          public:
            ~MatchExpr() {
                delete branch_value;
            }

            ::std::string as_string() const;
        };
    }
}

#endif