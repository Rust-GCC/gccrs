#ifndef RUST_AST_EXPR_H
#define RUST_AST_EXPR_H

#include "rust-ast.h"
#include "rust-path.h"

namespace Rust {
    namespace AST {
        /* TODO: if GCC moves to C++17 or allows boost, replace some boolean "has_whatever" pairs with
         * optional types (std::optional or boost::optional)? */

        // forward decls: defined in rust-path.h, rust-type.h, rust-pattern.h, and rust-stmt.h
        /*class PathInExpression;
        class QualifiedPathInExpression;
        class PathExprSegment;*/ // decls no longer required as "rust-path.h" is included
        /*class Type;
        class TypeNoBounds;
        class Lifetime;
        class Pattern;
        class Statement;*/ // decls no longer required as definitions moved to rust-ast.h

        // Decl as definition moved to rust-ast.h
        class ExprWithoutBlock;

        // AST node for an expression with an accompanying block - abstract
        class ExprWithBlock : public Expr {
          protected:
            ExprWithBlock(::std::vector<Attribute> outer_attrs) : Expr(outer_attrs) {}

            ExprWithBlock() : Expr() {}

            // pure virtual clone implementation
            virtual ExprWithBlock* clone_expr_with_block_impl() const = 0;

          public:
            // Unique pointer custom clone function
            ::std::unique_ptr<ExprWithBlock> clone_expr_with_block() const {
                return ::std::unique_ptr<ExprWithBlock>(clone_expr_with_block_impl());
            }
        };

        // Literals? Or literal base?
        class LiteralExpr : public ExprWithoutBlock {
          public:
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
            };

          private:
            // TODO: maybe make subclasses of each type of literal with their typed values (or
            // generics)
            ::std::string value_as_string;
            LitType type;

          public:
            ::std::string as_string() const {
                return value_as_string;
            }

            inline LitType get_lit_type() const {
                return type;
            }

            LiteralExpr(
              ::std::string value_as_string, LitType type, ::std::vector<Attribute> outer_attrs) :
              value_as_string(value_as_string),
              type(type), ExprWithoutBlock(outer_attrs) {}

            LiteralExpr(::std::string value_as_string, LitType type) :
              value_as_string(value_as_string), type(type) {}

            // Unique pointer custom clone function
            ::std::unique_ptr<LiteralExpr> clone_literal_expr() const {
                return ::std::unique_ptr<LiteralExpr>(clone_literal_expr_impl());
            }

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual LiteralExpr* clone_expr_impl() const OVERRIDE {
                return new LiteralExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual LiteralExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new LiteralExpr(*this);
            }

            // not virtual as currently no subclasses of LiteralExpr, but could be in future
            /*virtual*/ LiteralExpr* clone_literal_expr_impl() const {
                return new LiteralExpr(*this);
            }
        };

        // Literal expression attribute body (non-macro attribute)
        class AttrInputLiteral : public AttrInput {
            // Literal expression WITHOUT SUFFIX
            // LiteralExpr* literal_expr;
            //::std::unique_ptr<LiteralExpr> literal_expr;
            LiteralExpr literal_expr; // as not using polymorphic behaviour, doesn't require pointer
            // TODO: will require pointer if LiteralExpr is changed to have subclassing

          public:
            AttrInputLiteral(LiteralExpr lit_expr) : literal_expr(lit_expr) {}
            /*~AttrInputLiteral() {
                delete literal_expr;
            }*/
          protected:
            // Use covariance to implement clone function as returning an AttrInputLiteral object
            virtual AttrInputLiteral* clone_attr_input_impl() const OVERRIDE {
                return new AttrInputLiteral(*this);
            }
        };

        // A literal meta item
        class MetaItemLit : public MetaItem {
            // LiteralExpr* expr;
            //::std::unique_ptr<LiteralExpr> expr;
            LiteralExpr expr; // as LiteralExpr not subclassed (currently, at least), ptr not needed

          public:
            /*~MetaItemLit() {
                delete expr;
            }*/

            MetaItemLit(LiteralExpr expr, SimplePath path) : expr(expr), MetaItem(path) {}

          protected:
            // Use covariance to implement clone function as returning derived object
            virtual MetaItemLit* clone_meta_item_impl() const OVERRIDE {
                return new MetaItemLit(*this);
            }
        };

        // An inner meta item
        struct MetaItemInner {
            // Allows EITHER MetaItem or LiteralExpression (without suffix)
            // bool lit_active;
            /*MetaItem* item;
            LiteralExpr* expr;*/
            ::std::unique_ptr<MetaItem> item;
            //::std::unique_ptr<LiteralExpr> expr;
            ::std::unique_ptr<LiteralExpr> expr;

            // as no more conditional delete on expr member variable, must initialise it as NULL
          public:
            /*~MetaItemInner() {
                if (lit_active) {
                    delete expr;
                }
                delete item;
            }*/

            // Returns whether the MetaItemInner is in an error state.
            inline bool is_error_state() const {
                return (item == NULL && expr == NULL) || (item != NULL && expr != NULL);
            }

            // Returns whether the item is active
            inline bool is_item_active() const {
                return item != NULL && expr == NULL;
            }

            // Returns whether the literal expr is active
            inline bool is_expr_active() const {
                return !is_item_active();
            }

            // Constructor with MetaItem
            MetaItemInner(MetaItem* item) : item(item) /*, expr(NULL)*/ {}

            // Constructor with LitExpr
            MetaItemInner(LiteralExpr* expr) : /*item(NULL), */ expr(expr) {}

            // Copy constructor with clone
            MetaItemInner(MetaItemInner const& other) :
              item(other.item->clone_meta_item()), expr(other.expr->clone_literal_expr()) {}

            // Destructor - define here if required

            // Overload assignment operator to use clone
            MetaItemInner& operator=(MetaItemInner const& other) {
                item = other.item->clone_meta_item();
                expr = other.expr->clone_literal_expr();

                return *this;
            }

            // move constructors
            MetaItemInner(MetaItemInner&& other) = default;
            MetaItemInner& operator=(MetaItemInner&& other) = default;
        };

        // A sequence meta item
        class MetaItemSeq : public MetaItem {
            // bool has_sequence;
            ::std::vector<MetaItemInner> sequence;

          public:
            // Returns whether the sequence meta item actually has a sequence
            inline bool has_sequence() const {
                return !sequence.empty();
            }

            MetaItemSeq(SimplePath path, ::std::vector<MetaItemInner> sequence) :
              sequence(sequence), MetaItem(path) {}

          protected:
            // Use covariance to implement clone function as returning derived object
            virtual MetaItemSeq* clone_meta_item_impl() const OVERRIDE {
                return new MetaItemSeq(*this);
            }
        };

        // Something to do with subsets of MetaItem syntax or something
        struct MetaWord {
            Identifier word;

          public:
            MetaWord(Identifier word) : word(word) {}
        };

        // A name-value string
        struct MetaNameValueStr {
            Identifier name;
            ::std::string value;

          public:
            MetaNameValueStr(Identifier name, ::std::string value) : name(name), value(value) {}
        };

        // A list of paths
        struct MetaListPaths {
            Identifier type_thing;
            ::std::vector<SimplePath> paths;

          public:
            MetaListPaths(Identifier type_thing, ::std::vector<SimplePath> paths) :
              type_thing(type_thing), paths(paths) {}
        };

        // A list of identifiers
        struct MetaListIdents {
            Identifier directive_thing;
            ::std::vector<Identifier> idents_to_use;

          public:
            MetaListIdents(Identifier directive_thing, ::std::vector<Identifier> idents_to_use) :
              directive_thing(directive_thing), idents_to_use(idents_to_use) {}
        };

        // A list of MetaNameValueStr
        struct MetaListNameValueStr {
            Identifier macro_name_thing;
            ::std::vector<MetaNameValueStr> list;

          public:
            MetaListNameValueStr(Identifier macro_name_thing, ::std::vector<MetaNameValueStr> list) :
              macro_name_thing(macro_name_thing), list(list) {}
        };

        // Base path expression AST node - abstract
        class PathExpr : public ExprWithoutBlock {
          protected:
            PathExpr(::std::vector<Attribute> outer_attribs) : ExprWithoutBlock(outer_attribs) {}
        };

        // AST node for a non-qualified path expression
        class PathExprNonQual : public PathExpr {
            PathInExpression path;

          public:
            ::std::string as_string() const {
                return path.as_string();
            }

            PathExprNonQual(PathInExpression path, ::std::vector<Attribute> outer_attribs) :
              path(path), PathExpr(outer_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual PathExprNonQual* clone_expr_impl() const OVERRIDE {
                return new PathExprNonQual(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual PathExprNonQual* clone_expr_without_block_impl() const OVERRIDE {
                return new PathExprNonQual(*this);
            }
        };

        // AST node for a qualified path expression
        class PathExprQual : public PathExpr {
            QualifiedPathInExpression path;

          public:
            ::std::string as_string() const {
                return path.as_string();
            }

            PathExprQual(QualifiedPathInExpression path, ::std::vector<Attribute> outer_attribs) :
              path(path), PathExpr(outer_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual PathExprQual* clone_expr_impl() const OVERRIDE {
                return new PathExprQual(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual PathExprQual* clone_expr_without_block_impl() const OVERRIDE {
                return new PathExprQual(*this);
            }
        };

        // Represents an expression using unary or binary operators as AST node. Can be overloaded.
        class OperatorExpr : public ExprWithoutBlock {
          protected:
            // Variable must be protected to allow derived classes to modify it
            // Expr* main_or_left_expr;
            ::std::unique_ptr<Expr> main_or_left_expr;

            // Constructor (only for initialisation of expr purposes)
            OperatorExpr(Expr* main_or_left_expr_, ::std::vector<Attribute> outer_attribs) :
              main_or_left_expr(main_or_left_expr_), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor (only for initialisation of expr purposes)
            OperatorExpr(OperatorExpr const& other) :
              main_or_left_expr(other.main_or_left_expr->clone_expr()), ExprWithoutBlock(other) {}

            // Overload assignment operator to deep copy expr
            OperatorExpr& operator=(OperatorExpr const& other) {
                ExprWithoutBlock::operator=(other);
                main_or_left_expr = other.main_or_left_expr->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            OperatorExpr(OperatorExpr&& other) = default;
            OperatorExpr& operator=(OperatorExpr&& other) = default;

          public:
            // Destructor (only for initialisation of expr purposes)
            ~OperatorExpr() = default;

            /*virtual ~OperatorExpr() {
                delete main_or_left_expr;
            }*/
        };

        // Unary prefix & or &mut (or && and &&mut) borrow operator. Cannot be overloaded.
        class BorrowExpr : public OperatorExpr {
            bool is_mut;
            bool double_borrow;

          public:
            ::std::string as_string() const;

            // Constructor calls OperatorExpr's protected constructor
            BorrowExpr(Expr* borrow_lvalue, bool is_mut_borrow, bool is_double_borrow,
              ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(borrow_lvalue, outer_attribs),
              is_mut(is_mut_borrow), double_borrow(is_double_borrow) {}

            // Copy constructor - define here if required

            // Destructor - define here if required

            // Overload assignment operator here if required

            // Move semantics here if required
          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual BorrowExpr* clone_expr_impl() const OVERRIDE {
                return new BorrowExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual BorrowExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new BorrowExpr(*this);
            }
        };

        // Unary prefix * deference operator
        class DereferenceExpr : public OperatorExpr {
          public:
            ::std::string as_string() const;

            // Constructor calls OperatorExpr's protected constructor
            DereferenceExpr(Expr* deref_lvalue, ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(deref_lvalue, outer_attribs) {}

            // Copy constructor - define here if required

            // Destructor - define here if required

            // Overload assignment operator here if required

            // Move semantics here if required

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual DereferenceExpr* clone_expr_impl() const OVERRIDE {
                return new DereferenceExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual DereferenceExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new DereferenceExpr(*this);
            }
        };

        // Unary postfix ? error propogation operator. Cannot be overloaded.
        class ErrorPropogationExpr : public OperatorExpr {
          public:
            ::std::string as_string() const;

            // Constructor calls OperatorExpr's protected constructor
            ErrorPropogationExpr(
              Expr* potential_error_value, ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(potential_error_value, outer_attribs) {}

            // Copy constructor - define here if required

            // Destructor - define here if required

            // Overload assignment operator here if required

            // Move semantics here if required

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ErrorPropogationExpr* clone_expr_impl() const OVERRIDE {
                return new ErrorPropogationExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ErrorPropogationExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new ErrorPropogationExpr(*this);
            }
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

            // Constructor calls OperatorExpr's protected constructor
            NegationExpr(Expr* negated_value, NegationType negation_kind,
              ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(negated_value, outer_attribs),
              negation_type(negation_kind) {}

            // Copy constructor - define here if required

            // Destructor - define here if required

            // Overload assignment operator here if required

            // Move semantics here if required

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual NegationExpr* clone_expr_impl() const OVERRIDE {
                return new NegationExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual NegationExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new NegationExpr(*this);
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

            // Expr* right_expr;
            ::std::unique_ptr<Expr> right_expr;

          public:
            /*~ArithmeticOrLogicalExpr() {
                delete right_expr;
            }*/

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }

            // Constructor calls OperatorExpr's protected constructor
            ArithmeticOrLogicalExpr(Expr* left_value, Expr* right_value, ExprType expr_kind,
              ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(left_value, outer_attribs),
              right_expr(right_value), expr_type(expr_kind) {}

            // Copy constructor - probably required due to unique pointer
            ArithmeticOrLogicalExpr(ArithmeticOrLogicalExpr const& other) :
              OperatorExpr(other), right_expr(other.right_expr->clone_expr()),
              expr_type(other.expr_type) {}

            // Destructor - define here if required

            // Overload assignment operator
            ArithmeticOrLogicalExpr& operator=(ArithmeticOrLogicalExpr const& other) {
                OperatorExpr::operator=(other);
                main_or_left_expr = other.main_or_left_expr->clone_expr();
                right_expr = other.right_expr->clone_expr();
                expr_type = other.expr_type;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            ArithmeticOrLogicalExpr(ArithmeticOrLogicalExpr&& other) = default;
            ArithmeticOrLogicalExpr& operator=(ArithmeticOrLogicalExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ArithmeticOrLogicalExpr* clone_expr_impl() const OVERRIDE {
                return new ArithmeticOrLogicalExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ArithmeticOrLogicalExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new ArithmeticOrLogicalExpr(*this);
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

            // Expr* right_expr;
            ::std::unique_ptr<Expr> right_expr;

          public:
            /*~ComparisonExpr() {
                delete right_expr;
            }*/

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }

            // Constructor requires pointers for polymorphism
            ComparisonExpr(Expr* left_value, Expr* right_value, ExprType comparison_kind,
              ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(left_value, outer_attribs),
              right_expr(right_value), expr_type(comparison_kind) {}

            // Copy constructor also calls OperatorExpr's protected constructor
            ComparisonExpr(ComparisonExpr const& other) :
              OperatorExpr(other), right_expr(other.right_expr->clone_expr()),
              expr_type(other.expr_type) {}

            // Destructor - define here if required

            // Overload assignment operator to deep copy
            ComparisonExpr& operator=(ComparisonExpr const& other) {
                OperatorExpr::operator=(other);
                main_or_left_expr = other.main_or_left_expr->clone_expr();
                right_expr = other.right_expr->clone_expr();
                expr_type = other.expr_type;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            ComparisonExpr(ComparisonExpr&& other) = default;
            ComparisonExpr& operator=(ComparisonExpr&& other) = default;

            // TODO: implement via a function call to std::cmp::PartialEq::eq(&op1, &op2) maybe?
          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ComparisonExpr* clone_expr_impl() const OVERRIDE {
                return new ComparisonExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ComparisonExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new ComparisonExpr(*this);
            }
        };

        // Infix binary lazy boolean logical operators && and ||.
        class LazyBooleanExpr : public OperatorExpr {
            enum ExprType { LOGICAL_OR, LOGICAL_AND } expr_type;

            // Expr* right_expr;
            ::std::unique_ptr<Expr> right_expr;

          public:
            /*~LazyBooleanExpr() {
                delete right_expr;
            }*/

            // Constructor calls OperatorExpr's protected constructor
            LazyBooleanExpr(Expr* left_bool_expr, Expr* right_bool_expr, ExprType expr_kind,
              ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(left_bool_expr, outer_attribs),
              right_expr(right_bool_expr), expr_type(expr_kind) {}

            // Copy constructor also calls OperatorExpr's protected constructor
            LazyBooleanExpr(LazyBooleanExpr const& other) :
              OperatorExpr(other), right_expr(other.right_expr->clone_expr()),
              expr_type(other.expr_type) {}

            // Destructor - define here if required

            // Overload assignment operator to deep copy
            LazyBooleanExpr& operator=(LazyBooleanExpr const& other) {
                OperatorExpr::operator=(other);
                main_or_left_expr = other.main_or_left_expr->clone_expr();
                right_expr = other.right_expr->clone_expr();
                expr_type = other.expr_type;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            LazyBooleanExpr(LazyBooleanExpr&& other) = default;
            LazyBooleanExpr& operator=(LazyBooleanExpr&& other) = default;

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual LazyBooleanExpr* clone_expr_impl() const OVERRIDE {
                return new LazyBooleanExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual LazyBooleanExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new LazyBooleanExpr(*this);
            }
        };

        // Binary infix "as" cast expression.
        class TypeCastExpr : public OperatorExpr {
            // TypeNoBounds type_to_convert_to;
            ::std::unique_ptr<TypeNoBounds> type_to_convert_to;

            // Note: only certain allowed, outlined in reference
          public:
            ::std::string as_string() const;

            // Constructor requires calling protected constructor of OperatorExpr
            TypeCastExpr(Expr* expr_to_cast, TypeNoBounds* type_to_cast_to,
              ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(expr_to_cast, outer_attribs),
              type_to_convert_to(type_to_cast_to) {}

            // Copy constructor also requires calling protected constructor
            TypeCastExpr(TypeCastExpr const& other) :
              OperatorExpr(other),
              type_to_convert_to(other.type_to_convert_to->clone_type_no_bounds()) {}

            // Destructor - define here if required

            // Overload assignment operator to deep copy
            TypeCastExpr& operator=(TypeCastExpr const& other) {
                OperatorExpr::operator=(other);
                main_or_left_expr = other.main_or_left_expr->clone_expr();
                type_to_convert_to = other.type_to_convert_to->clone_type_no_bounds();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors as not supported in c++03
            TypeCastExpr(TypeCastExpr&& other) = default;
            TypeCastExpr& operator=(TypeCastExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual TypeCastExpr* clone_expr_impl() const OVERRIDE {
                return new TypeCastExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual TypeCastExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new TypeCastExpr(*this);
            }
        };

        // Binary assignment expression.
        class AssignmentExpr : public OperatorExpr {
            // Expr* right_expr;
            ::std::unique_ptr<Expr> right_expr;

          public:
            /*~AssignmentExpr() {
                delete right_expr;
            }*/

            ::std::string as_string() const;

            // Call OperatorExpr constructor to initialise left_expr
            AssignmentExpr(Expr* value_to_assign_to, Expr* value_to_assign,
              ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(value_to_assign_to, outer_attribs),
              right_expr(value_to_assign) {}

            // Call OperatorExpr constructor in copy constructor, as well as clone
            AssignmentExpr(AssignmentExpr const& other) :
              OperatorExpr(other), right_expr(other.right_expr->clone_expr()) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique_ptr right_expr
            AssignmentExpr& operator=(AssignmentExpr const& other) {
                OperatorExpr::operator=(other);
                main_or_left_expr = other.main_or_left_expr->clone_expr();
                right_expr = other.right_expr->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            AssignmentExpr(AssignmentExpr&& other) = default;
            AssignmentExpr& operator=(AssignmentExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual AssignmentExpr* clone_expr_impl() const OVERRIDE {
                return new AssignmentExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual AssignmentExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new AssignmentExpr(*this);
            }
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

            // Expr* right_expr;
            ::std::unique_ptr<Expr> right_expr;

          public:
            /*~CompoundAssignmentExpr() {
                delete right_expr;
            }*/

            ::std::string as_string() const;

            inline ExprType get_expr_type() const {
                return expr_type;
            }

            // Use pointers in constructor to enable polymorphism
            CompoundAssignmentExpr(Expr* value_to_assign_to, Expr* value_to_assign,
              ExprType expr_kind, ::std::vector<Attribute> outer_attribs) :
              OperatorExpr(value_to_assign_to, outer_attribs),
              right_expr(value_to_assign), expr_type(expr_kind) {}

            // Have clone in copy constructor
            CompoundAssignmentExpr(CompoundAssignmentExpr const& other) :
              OperatorExpr(other), right_expr(other.right_expr->clone_expr()),
              expr_type(other.expr_type) {}

            // Destructor - define here if required

            // Overload assignment operator to clone
            CompoundAssignmentExpr& operator=(CompoundAssignmentExpr const& other) {
                OperatorExpr::operator=(other);
                main_or_left_expr = other.main_or_left_expr->clone_expr();
                right_expr = other.right_expr->clone_expr();
                expr_type = other.expr_type;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            CompoundAssignmentExpr(CompoundAssignmentExpr&& other) = default;
            CompoundAssignmentExpr& operator=(CompoundAssignmentExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual CompoundAssignmentExpr* clone_expr_impl() const OVERRIDE {
                return new CompoundAssignmentExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual CompoundAssignmentExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new CompoundAssignmentExpr(*this);
            }
        };

        // Expression in parentheses (i.e. like literally just any 3 + (2 * 6))
        class GroupedExpr : public ExprWithoutBlock {
            ::std::vector<Attribute> inner_attrs;
            // Expr* expr_in_parens;
            ::std::unique_ptr<Expr> expr_in_parens;

          public:
            /*~GroupedExpr() {
                delete expr_in_parens;
            }*/

            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }

            // Use pointer in constructor for polymorphism reasons
            GroupedExpr(Expr* parenthesised_expr, ::std::vector<Attribute> inner_attribs,
              ::std::vector<Attribute> outer_attribs) :
              expr_in_parens(parenthesised_expr),
              inner_attrs(inner_attribs), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor includes clone for expr_in_parens
            GroupedExpr(GroupedExpr const& other) :
              expr_in_parens(other.expr_in_parens->clone_expr()), inner_attrs(other.inner_attrs),
              ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone expr_in_parens
            GroupedExpr& operator=(GroupedExpr const& other) {
                ExprWithoutBlock::operator=(other);
                inner_attrs = other.inner_attrs;
                expr_in_parens = other.expr_in_parens->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            GroupedExpr(GroupedExpr&& other) = default;
            GroupedExpr& operator=(GroupedExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual GroupedExpr* clone_expr_impl() const OVERRIDE {
                return new GroupedExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual GroupedExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new GroupedExpr(*this);
            }
        };

        // Base array initialisation internal element representation thing
        // aka ArrayElements
        class ArrayElems {
          public:
            virtual ~ArrayElems() {}

            // Unique pointer custom clone ArrayElems function
            ::std::unique_ptr<ArrayElems> clone_array_elems() const {
                return ::std::unique_ptr<ArrayElems>(clone_array_elems_impl());
            }

          protected:
            // pure virtual clone implementation
            virtual ArrayElems* clone_array_elems_impl() const = 0;
        };

        // Value array elements
        class ArrayElemsValues : public ArrayElems {
            //::std::vector<Expr> values;
            ::std::vector< ::std::unique_ptr<Expr> > values;

          public:
            /*inline ::std::vector< ::std::unique_ptr<Expr> > get_values() const {
                return values;
            }*/

            ArrayElemsValues(::std::vector< ::std::unique_ptr<Expr> > elems) : values(elems) {}

          protected:
            virtual ArrayElemsValues* clone_array_elems_impl() const OVERRIDE {
                return new ArrayElemsValues(*this);
            }
        };

        // Copied array element and number of copies
        class ArrayElemsCopied : public ArrayElems {
            // Expr* elem_to_copy;
            ::std::unique_ptr<Expr> elem_to_copy;
            // Expr* num_copies;
            ::std::unique_ptr<Expr> num_copies;

          public:
            /*~ArrayElemsCopied() {
                delete num_copies;
                delete elem_to_copy;
            }*/

            // Constructor requires pointers for polymorphism
            ArrayElemsCopied(Expr* copied_elem, Expr* copy_amount) :
              elem_to_copy(copied_elem), num_copies(copy_amount) {}

            // Copy constructor required due to unique_ptr - uses custom clone
            ArrayElemsCopied(ArrayElemsCopied const& other) :
              elem_to_copy(other.elem_to_copy->clone_expr()),
              num_copies(other.num_copies->clone_expr()) {}

            // Destructor - define here if required

            // Overloaded assignment operator for deep copying
            ArrayElemsCopied& operator=(ArrayElemsCopied const& other) {
                elem_to_copy = other.elem_to_copy->clone_expr();
                num_copies = other.num_copies->clone_expr();

                return *this;
            }

            // no move constructors as not supported in c++03
            /*ArrayElemsCopied(ArrayElemsCopied&& other) = default;
            ArrayElemsCopied& operator=(ArrayElemsCopied&& other) = default;*/

          protected:
            virtual ArrayElemsCopied* clone_array_elems_impl() const OVERRIDE {
                return new ArrayElemsCopied(*this);
            }
        };

        // Array definition-ish expression
        class ArrayExpr : public ExprWithoutBlock {
            ::std::vector<Attribute> inner_attrs;
            // ArrayElems internal_elements;
            ::std::unique_ptr<ArrayElems> internal_elements;

          public:
            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }

            // Constructor requires ArrayElems pointer
            ArrayExpr(ArrayElems* array_elems, ::std::vector<Attribute> inner_attribs,
              ::std::vector<Attribute> outer_attribs) :
              internal_elements(array_elems),
              inner_attrs(inner_attribs), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor requires cloning ArrayElems for polymorphism to hold
            ArrayExpr(ArrayExpr const& other) :
              internal_elements(other.internal_elements->clone_array_elems()),
              inner_attrs(other.inner_attrs), ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone internal_elements
            ArrayExpr& operator=(ArrayExpr const& other) {
                ExprWithoutBlock::operator=(other);
                inner_attrs = other.inner_attrs;
                internal_elements = other.internal_elements->clone_array_elems();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            ArrayExpr(ArrayExpr&& other) = default;
            ArrayExpr& operator=(ArrayExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ArrayExpr* clone_expr_impl() const OVERRIDE {
                return new ArrayExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ArrayExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new ArrayExpr(*this);
            }
        };

        // Aka IndexExpr (also applies to slices)
        // Apparently a[b] is equivalent to *std::ops::Index::index(&a, b) or
        // *std::ops::Index::index_mut(&mut a, b)
        // Also apparently deref operations on a will be repeatedly applied to find an implementation
        class ArrayIndexExpr : public ExprWithoutBlock {
            /*Expr* array_expr;
            Expr* index_expr;*/
            ::std::unique_ptr<Expr> array_expr;
            ::std::unique_ptr<Expr> index_expr;

          public:
            /*~ArrayIndexExpr() {
                delete index_expr;
                delete array_expr;
            }*/

            ::std::string as_string() const;

            ArrayIndexExpr(
              Expr* array_expr_, Expr* array_index_expr, ::std::vector<Attribute> outer_attribs) :
              array_expr(array_expr_),
              index_expr(array_index_expr), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor requires special cloning due to unique_ptr
            ArrayIndexExpr(ArrayIndexExpr const& other) :
              array_expr(other.array_expr->clone_expr()), index_expr(other.index_expr->clone_expr()),
              ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique_ptrs
            ArrayIndexExpr& operator=(ArrayIndexExpr const& other) {
                ExprWithoutBlock::operator=(other);
                array_expr = other.array_expr->clone_expr();
                index_expr = other.index_expr->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            ArrayIndexExpr(ArrayIndexExpr&& other) = default;
            ArrayIndexExpr& operator=(ArrayIndexExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ArrayIndexExpr* clone_expr_impl() const OVERRIDE {
                return new ArrayIndexExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ArrayIndexExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new ArrayIndexExpr(*this);
            }
        };

        // AST representation of a tuple
        class TupleExpr : public ExprWithoutBlock {
            ::std::vector<Attribute> inner_attrs;

            //::std::vector<Expr> tuple_elems;
            ::std::vector< ::std::unique_ptr<Expr> > tuple_elems;
            // replaces (inlined version of) TupleElements

          public:
            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }

            TupleExpr(::std::vector< ::std::unique_ptr<Expr> > tuple_elements,
              ::std::vector<Attribute> inner_attribs, ::std::vector<Attribute> outer_attribs) :
              tuple_elems(tuple_elements),
              inner_attrs(inner_attribs), ExprWithoutBlock(outer_attribs) {}

            // Note: syntactically, can disambiguate single-element tuple from parens with comma, i.e.
            // (0,) rather than (0)

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual TupleExpr* clone_expr_impl() const OVERRIDE {
                return new TupleExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual TupleExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new TupleExpr(*this);
            }
        };

        // aka TupleIndexingExpr
        // AST representation of a tuple indexing expression
        class TupleIndexExpr : public ExprWithoutBlock {
            // Expr* tuple_expr;
            ::std::unique_ptr<Expr> tuple_expr;
            // TupleIndex is a decimal int literal with no underscores or suffix
            TupleIndex tuple_index;

            // i.e. pair.0

          public:
            /*~TupleIndexExpr() {
                delete tuple_expr;
            }*/

            ::std::string as_string() const;

            inline TupleIndex get_tuple_index() const {
                return tuple_index;
            }

            TupleIndexExpr(
              Expr* tuple_expr_, TupleIndex index, ::std::vector<Attribute> outer_attribs) :
              tuple_expr(tuple_expr_),
              tuple_index(index), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor requires a clone for tuple_expr
            TupleIndexExpr(TupleIndexExpr const& other) :
              tuple_expr(other.tuple_expr->clone_expr()), tuple_index(other.tuple_index),
              ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overload assignment operator in order to clone
            TupleIndexExpr& operator=(TupleIndexExpr const& other) {
                ExprWithoutBlock::operator=(other);
                tuple_expr = other.tuple_expr->clone_expr();
                tuple_index = other.tuple_index;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            TupleIndexExpr(TupleIndexExpr&& other) = default;
            TupleIndexExpr& operator=(TupleIndexExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual TupleIndexExpr* clone_expr_impl() const OVERRIDE {
                return new TupleIndexExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual TupleIndexExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new TupleIndexExpr(*this);
            }
        };

        // Base struct/tuple/union value creator AST node (abstract)
        class StructExpr : public ExprWithoutBlock {
            PathInExpression struct_name;

          protected:
            // Protected constructor to allow initialising struct_name
            StructExpr(PathInExpression struct_path, ::std::vector<Attribute> outer_attribs) :
              struct_name(struct_path), ExprWithoutBlock(outer_attribs) {}

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

            // Constructor has to call protected constructor of base class
            StructExprStruct(PathInExpression struct_path, ::std::vector<Attribute> inner_attribs,
              ::std::vector<Attribute> outer_attribs) :
              StructExpr(struct_path, outer_attribs),
              inner_attrs(inner_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprStruct* clone_expr_impl() const OVERRIDE {
                return new StructExprStruct(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprStruct* clone_expr_without_block_impl() const OVERRIDE {
                return new StructExprStruct(*this);
            }
        };

        // AST node representing expression used to fill a struct's fields from another struct
        struct StructBase {
          private:
            // Expr* base_struct;
            ::std::unique_ptr<Expr> base_struct;

          public:
            StructBase(Expr* base_struct_ptr) : base_struct(base_struct_ptr) {}

            // Copy constructor requires clone
            StructBase(StructBase const& other) : base_struct(other.base_struct->clone_expr()) {}

            // Destructor - define here if required

            // Overload assignment operator to clone base_struct
            StructBase& operator=(StructBase const& other) {
                base_struct = other.base_struct->clone_expr();

                return *this;
            }

            // move constructors
            StructBase(StructBase&& other) = default;
            StructBase& operator=(StructBase&& other) = default;

            /*~StructBase() {
                delete base_struct;
            }*/

            // Returns a null expr-ed StructBase - error state
            static StructBase error() {
                return StructBase(NULL);
            }

            // Returns whether StructBase is in error state
            inline bool is_invalid() const {
                return base_struct == NULL;
            }
        };

        // Base AST node for a single struct expression field (in struct instance creation) - abstract
        class StructExprField {
          public:
            virtual ~StructExprField() {}
        };

        // Identifier-only variant of StructExprField AST node
        class StructExprFieldIdentifier : public StructExprField {
            Identifier field_name;

          public:
            StructExprFieldIdentifier(Identifier field_identifier) : field_name(field_identifier) {}
        };

        // Base AST node for a single struct expression field with an assigned value - abstract
        class StructExprFieldWithVal : public StructExprField {
            // Expr* value;
            ::std::unique_ptr<Expr> value;

          protected:
            StructExprFieldWithVal(Expr* field_value) : value(field_value) {}

            // Copy constructor requires clone
            StructExprFieldWithVal(StructExprFieldWithVal const& other) :
              value(other.value->clone_expr()) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique_ptr
            StructExprFieldWithVal& operator=(StructExprFieldWithVal const& other) {
                value = other.value->clone_expr();

                return *this;
            }

            // move constructors
            StructExprFieldWithVal(StructExprFieldWithVal&& other) = default;
            StructExprFieldWithVal& operator=(StructExprFieldWithVal&& other) = default;

          public:
            /*~StructExprFieldWithVal() {
                delete value;
            }*/
        };

        // Identifier and value variant of StructExprField AST node
        class StructExprFieldIdentifierValue : public StructExprFieldWithVal {
            Identifier field_name;

          public:
            StructExprFieldIdentifierValue(Identifier field_identifier, Expr* field_value) :
              StructExprFieldWithVal(field_value), field_name(field_identifier) {}

            // copy constructor, destructor, and overloaded assignment operator should carry through
        };

        // Tuple index and value variant of StructExprField AST node
        class StructExprFieldIndexValue : public StructExprFieldWithVal {
            TupleIndex index;

          public:
            StructExprFieldIndexValue(TupleIndex tuple_index, Expr* field_value) :
              StructExprFieldWithVal(field_value), index(tuple_index) {}

            // copy constructor, destructor, and overloaded assignment operator should carry through
        };

        // AST node of a struct creator with fields
        class StructExprStructFields : public StructExprStruct {
            //::std::vector<StructExprField> fields;
            ::std::vector< ::std::unique_ptr<StructExprField> > fields;

            // bool has_struct_base;
            StructBase struct_base;

          public:
            ::std::string as_string() const;

            inline bool has_struct_base() const {
                return !struct_base.is_invalid();
            }

            /*inline ::std::vector< ::std::unique_ptr<StructExprField> > get_fields() const {
                return fields;
            }*/

            /*inline StructBase get_struct_base() const {
                return has_struct_base ? struct_base : StructBase::error();
            }*/

            // Constructor for StructExprStructFields when no struct base is used
            StructExprStructFields(PathInExpression struct_path,
              ::std::vector< ::std::unique_ptr<StructExprField> > expr_fields,
              ::std::vector<Attribute> inner_attribs, ::std::vector<Attribute> outer_attribs) :
              fields(expr_fields),
              struct_base(StructBase::error()),
              StructExprStruct(struct_path, inner_attribs, outer_attribs) {}

            // Constructor for StructExprStructFields when a struct base is ued
            StructExprStructFields(PathInExpression struct_path, StructBase base_struct,
              ::std::vector< ::std::unique_ptr<StructExprField> > expr_fields,
              ::std::vector<Attribute> inner_attribs, ::std::vector<Attribute> outer_attribs) :
              fields(expr_fields),
              struct_base(base_struct), StructExprStruct(struct_path, inner_attribs, outer_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprStructFields* clone_expr_impl() const OVERRIDE {
                return new StructExprStructFields(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprStructFields* clone_expr_without_block_impl() const OVERRIDE {
                return new StructExprStructFields(*this);
            }
        };

        // AST node of the functional update struct creator
        class StructExprStructBase : public StructExprStruct {
            StructBase struct_base;

          public:
            ::std::string as_string() const;

            /*inline StructBase get_struct_base() const {
                return struct_base;
            }*/

            StructExprStructBase(PathInExpression struct_path, StructBase base_struct,
              ::std::vector<Attribute> inner_attribs, ::std::vector<Attribute> outer_attribs) :
              struct_base(base_struct),
              StructExprStruct(struct_path, inner_attribs, outer_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprStructBase* clone_expr_impl() const OVERRIDE {
                return new StructExprStructBase(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprStructBase* clone_expr_without_block_impl() const OVERRIDE {
                return new StructExprStructBase(*this);
            }
        };

        // AST node of a tuple struct creator
        class StructExprTuple : public StructExpr {
            ::std::vector<Attribute> inner_attrs;
            //::std::vector<Expr> exprs;
            ::std::vector< ::std::unique_ptr<Expr> > exprs;

          public:
            ::std::string as_string() const;

            inline ::std::vector<Attribute> get_inner_attrs() const {
                return inner_attrs;
            }

            /*inline ::std::vector< ::std::unique_ptr<Expr> > get_exprs() const {
                return exprs;
            }*/

            StructExprTuple(PathInExpression struct_path,
              ::std::vector< ::std::unique_ptr<Expr> > tuple_exprs,
              ::std::vector<Attribute> inner_attribs, ::std::vector<Attribute> outer_attribs) :
              exprs(tuple_exprs),
              inner_attrs(inner_attribs), StructExpr(struct_path, outer_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprTuple* clone_expr_impl() const OVERRIDE {
                return new StructExprTuple(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprTuple* clone_expr_without_block_impl() const OVERRIDE {
                return new StructExprTuple(*this);
            }
        };

        // AST node of a "unit" struct creator (no fields and no braces)
        class StructExprUnit : public StructExpr {
          public:
            ::std::string as_string() const {
                return get_struct_name().as_string();
                // return struct_name.as_string();
            }

            StructExprUnit(PathInExpression struct_path, ::std::vector<Attribute> outer_attribs) :
              StructExpr(struct_path, outer_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprUnit* clone_expr_impl() const OVERRIDE {
                return new StructExprUnit(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual StructExprUnit* clone_expr_without_block_impl() const OVERRIDE {
                return new StructExprUnit(*this);
            }
        };

        // aka EnumerationVariantExpr
        // Base AST node representing creation of an enum variant instance - abstract
        class EnumVariantExpr : public ExprWithoutBlock {
            PathInExpression enum_variant_path;

          protected:
            // Protected constructor for initialising enum_variant_path
            EnumVariantExpr(
              PathInExpression path_to_enum_variant, ::std::vector<Attribute> outer_attribs) :
              enum_variant_path(path_to_enum_variant),
              ExprWithoutBlock(outer_attribs) {}

          public:
            // TODO: maybe remove and have string version gotten here directly
            inline PathInExpression get_enum_variant_path() const {
                return enum_variant_path;
            }
        };

        // Base AST node for a single enum expression field (in enum instance creation) - abstract
        class EnumExprField {
          public:
            virtual ~EnumExprField() {}
        };

        // Identifier-only variant of EnumExprField AST node
        class EnumExprFieldIdentifier : public EnumExprField {
            Identifier field_name;

          public:
            EnumExprFieldIdentifier(Identifier field_identifier) : field_name(field_identifier) {}
        };

        // Base AST node for a single enum expression field with an assigned value - abstract
        class EnumExprFieldWithVal : public EnumExprField {
          protected:
            /*~EnumExprFieldWithVal() {
                delete value;
            }*/

            // Expr* value;
            ::std::unique_ptr<Expr> value;

            EnumExprFieldWithVal(Expr* field_value) : value(field_value) {}

            // Copy constructor must clone unique_ptr value
            EnumExprFieldWithVal(EnumExprFieldWithVal const& other) :
              value(other.value->clone_expr()) {}

            // Destructor - define here if required

            // Overload assignment operator to clone
            EnumExprFieldWithVal& operator=(EnumExprFieldWithVal const& other) {
                value = other.value->clone_expr();

                return *this;
            }

            // move constructors
            EnumExprFieldWithVal(EnumExprFieldWithVal&& other) = default;
            EnumExprFieldWithVal& operator=(EnumExprFieldWithVal&& other) = default;
        };

        // Identifier and value variant of EnumExprField AST node
        class EnumExprFieldIdentifierValue : public EnumExprFieldWithVal {
            Identifier field_name;

          public:
            EnumExprFieldIdentifierValue(Identifier field_name, Expr* field_value) :
              field_name(field_name), EnumExprFieldWithVal(field_value) {}

            // copy constructor, destructor, and assignment operator should not need defining
        };

        // Tuple index and value variant of EnumExprField AST node
        class EnumExprFieldIndexValue : public EnumExprFieldWithVal {
            TupleIndex index;
            // TODO: implement "with val" as a template with EnumExprField as type param?

          public:
            EnumExprFieldIndexValue(TupleIndex field_index, Expr* field_value) :
              index(field_index), EnumExprFieldWithVal(field_value) {}

            // copy constructor, destructor, and assignment operator should not need defining
        };

        // Struct-like syntax enum variant instance creation AST node
        class EnumExprStruct : public EnumVariantExpr {
            //::std::vector<EnumExprField> fields;
            ::std::vector< ::std::unique_ptr<EnumExprField> > fields;

          public:
            ::std::string as_string() const;

            /*inline ::std::vector< ::std::unique_ptr<EnumExprField> > get_fields() const {
                return fields;
            }*/

            EnumExprStruct(PathInExpression enum_variant_path,
              ::std::vector< ::std::unique_ptr<EnumExprField> > variant_fields,
              ::std::vector<Attribute> outer_attribs) :
              fields(variant_fields),
              EnumVariantExpr(enum_variant_path, outer_attribs) {}

            // copy constructor, destructor, and assignment operator should not need defining

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual EnumExprStruct* clone_expr_impl() const OVERRIDE {
                return new EnumExprStruct(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual EnumExprStruct* clone_expr_without_block_impl() const OVERRIDE {
                return new EnumExprStruct(*this);
            }
        };

        // Tuple-like syntax enum variant instance creation AST node
        class EnumExprTuple : public EnumVariantExpr {
            //::std::vector<Expr> values;
            ::std::vector< ::std::unique_ptr<Expr> > values;

          public:
            ::std::string as_string() const;

            /*inline ::std::vector< ::std::unique_ptr<Expr> > get_values() const {
                return values;
            }*/

            EnumExprTuple(PathInExpression enum_variant_path,
              ::std::vector< ::std::unique_ptr<Expr> > variant_values,
              ::std::vector<Attribute> outer_attribs) :
              values(variant_values),
              EnumVariantExpr(enum_variant_path, outer_attribs) {}

            // copy constructor, destructor, and assignment operator should not need defining

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual EnumExprTuple* clone_expr_impl() const OVERRIDE {
                return new EnumExprTuple(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual EnumExprTuple* clone_expr_without_block_impl() const OVERRIDE {
                return new EnumExprTuple(*this);
            }
        };

        // No-field enum variant instance creation AST node
        class EnumExprFieldless : public EnumVariantExpr {
          public:
            ::std::string as_string() const {
                // return enum_variant_path.as_string();
                return get_enum_variant_path().as_string();
            }

            EnumExprFieldless(
              PathInExpression enum_variant_path, ::std::vector<Attribute> outer_attribs) :
              EnumVariantExpr(enum_variant_path, outer_attribs) {}

            // copy constructor, destructor, and assignment operator should not need defining

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual EnumExprFieldless* clone_expr_impl() const OVERRIDE {
                return new EnumExprFieldless(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual EnumExprFieldless* clone_expr_without_block_impl() const OVERRIDE {
                return new EnumExprFieldless(*this);
            }
        };

        // inlined
        /*struct CallParams {
            //::std::vector<Expr> params;
            ::std::vector< ::std::unique_ptr<Expr> > params;
        };*/

        // Function call expression AST node
        class CallExpr : public ExprWithoutBlock {
            // Expr* function;
            ::std::unique_ptr<Expr> function;
            //::std::vector<Expr> params; // inlined form of CallParams
            ::std::vector< ::std::unique_ptr<Expr> > params;

          public:
            /*~CallExpr() {
                delete function;
            }*/

            ::std::string as_string() const;

            /*inline ::std::vector< ::std::unique_ptr<Expr> > get_params() const {
                return params;
            }*/

            CallExpr(Expr* function_expr, ::std::vector< ::std::unique_ptr<Expr> > function_params,
              ::std::vector<Attribute> outer_attribs) :
              function(function_expr),
              params(function_params), ExprWithoutBlock(outer_attribs) {}

            // copy constructor requires clone
            CallExpr(CallExpr const& other) :
              function(other.function->clone_expr()), params(other.params), ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone
            CallExpr& operator=(CallExpr const& other) {
                ExprWithoutBlock::operator=(other);
                function = other.function->clone_expr();
                params = other.params;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            CallExpr(CallExpr&& other) = default;
            CallExpr& operator=(CallExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual CallExpr* clone_expr_impl() const OVERRIDE {
                return new CallExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual CallExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new CallExpr(*this);
            }
        };

        // Method call expression AST node
        class MethodCallExpr : public ExprWithoutBlock {
            // Expr* receiver;
            ::std::unique_ptr<Expr> receiver;
            PathExprSegment method_name;
            //::std::vector<Expr> params; // inlined form of CallParams
            ::std::vector< ::std::unique_ptr<Expr> > params;

          public:
            /*~MethodCallExpr() {
                delete receiver;
            }*/

            ::std::string as_string() const;

            /*inline ::std::vector< ::std::unique_ptr<Expr> > get_params() const {
                return params;
            }*/

            MethodCallExpr(Expr* call_receiver, PathExprSegment method_path,
              ::std::vector< ::std::unique_ptr<Expr> > method_params,
              ::std::vector<Attribute> outer_attribs) :
              receiver(call_receiver),
              method_name(method_path), params(method_params), ExprWithoutBlock(outer_attribs) {}

            // copy constructor required due to cloning
            MethodCallExpr(MethodCallExpr const& other) :
              receiver(other.receiver->clone_expr()), method_name(other.method_name),
              params(other.params), ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone receiver object
            MethodCallExpr& operator=(MethodCallExpr const& other) {
                ExprWithoutBlock::operator=(other);
                receiver = other.receiver->clone_expr();
                method_name = other.method_name;
                params = other.params;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            MethodCallExpr(MethodCallExpr&& other) = default;
            MethodCallExpr& operator=(MethodCallExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual MethodCallExpr* clone_expr_impl() const OVERRIDE {
                return new MethodCallExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual MethodCallExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new MethodCallExpr(*this);
            }
        };

        // aka FieldExpression
        // Struct or union field access expression AST node
        class FieldAccessExpr : public ExprWithoutBlock {
            // Expr* receiver;
            ::std::unique_ptr<Expr> receiver;
            Identifier field;

          public:
            /*~FieldAccessExpr() {
                delete receiver;
            }*/

            ::std::string as_string() const;

            FieldAccessExpr(Expr* field_access_receiver, Identifier field_name,
              ::std::vector<Attribute> outer_attribs) :
              receiver(field_access_receiver),
              field(field_name), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor required due to unique_ptr cloning
            FieldAccessExpr(FieldAccessExpr const& other) :
              receiver(other.receiver->clone_expr()), field(other.field), ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique_ptr
            FieldAccessExpr& operator=(FieldAccessExpr const& other) {
                ExprWithoutBlock::operator=(other);
                receiver = other.receiver->clone_expr();
                field = other.field;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            FieldAccessExpr(FieldAccessExpr&& other) = default;
            FieldAccessExpr& operator=(FieldAccessExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual FieldAccessExpr* clone_expr_impl() const OVERRIDE {
                return new FieldAccessExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual FieldAccessExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new FieldAccessExpr(*this);
            }
        };

        // Closure parameter data structure
        struct ClosureParam {
          private:
            // Pattern pattern;
            ::std::unique_ptr<Pattern> pattern;

            // bool has_type_given;
            // Type type;
            ::std::unique_ptr<Type> type;

          public:
            // Returns whether the type of the parameter has been given.
            inline bool has_type_given() const {
                return type != NULL;
            }

            // Constructor for closure parameter with no given type
            ClosureParam(Pattern* param_pattern) : pattern(param_pattern) /*, type(NULL)*/ {}

            // Constructor for closure parameter with a given type
            ClosureParam(Pattern* param_pattern, Type* param_type) :
              pattern(param_pattern), type(param_type) {}

            // Copy constructor required due to cloning as a result of unique_ptrs
            ClosureParam(ClosureParam const& other) :
              pattern(other.pattern->clone_pattern()), type(other.type->clone_type()) {}

            ~ClosureParam() = default;

            // Assignment operator must be overloaded to clone as well
            ClosureParam& operator=(ClosureParam const& other) {
                pattern = other.pattern->clone_pattern();
                type = other.type->clone_type();

                return *this;
            }

            // move constructors
            ClosureParam(ClosureParam&& other) = default;
            ClosureParam& operator=(ClosureParam&& other) = default;
        };

        // Base closure definition expression AST node - abstract
        class ClosureExpr : public ExprWithoutBlock {
            bool has_move;
            ::std::vector<ClosureParam> params; // may be empty
            // also note a double pipe "||" can be used for empty params - does not need a space

          protected:
            ClosureExpr(::std::vector<ClosureParam> closure_params, bool has_move,
              ::std::vector<Attribute> outer_attribs) :
              params(closure_params),
              has_move(has_move), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor, destructor, and assignment operator override should not be needed
        };

        // Represents a non-type-specified closure expression AST node
        class ClosureExprInner : public ClosureExpr {
            // Expr* closure_inner;
            ::std::unique_ptr<Expr> closure_inner;

          public:
            /*~ClosureExprInner() {
                delete closure_inner;
            }*/

            ::std::string as_string() const;

            // Constructor for a ClosureExprInner without a move
            ClosureExprInner(Expr* closure_inner_expr, ::std::vector<ClosureParam> closure_params,
              ::std::vector<Attribute> outer_attribs) :
              closure_inner(closure_inner_expr),
              ClosureExpr(closure_params, false, outer_attribs) {}

            // Constructor for a ClosureExprInner with maybe a move
            ClosureExprInner(Expr* closure_inner_expr, ::std::vector<ClosureParam> closure_params,
              bool is_move, ::std::vector<Attribute> outer_attribs) :
              closure_inner(closure_inner_expr),
              ClosureExpr(closure_params, is_move, outer_attribs) {}

            // Copy constructor must be defined to allow copying via cloning of unique_ptr
            ClosureExprInner(ClosureExprInner const& other) :
              closure_inner(other.closure_inner->clone_expr()), ClosureExpr(other) {}
            // TODO: ensure that this actually constructs properly

            // Destructor - define here if required

            // Overload assignment operator to clone closure_inner
            ClosureExprInner& operator=(ClosureExprInner const& other) {
                ClosureExpr::operator=(other);
                closure_inner = other.closure_inner->clone_expr();
                // params = other.params;
                // has_move = other.has_move;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            ClosureExprInner(ClosureExprInner&& other) = default;
            ClosureExprInner& operator=(ClosureExprInner&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ClosureExprInner* clone_expr_impl() const OVERRIDE {
                return new ClosureExprInner(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ClosureExprInner* clone_expr_without_block_impl() const OVERRIDE {
                return new ClosureExprInner(*this);
            }
        };

        // Forward decl BlockExpr for ClosureExprInnerTyped
        // class BlockExpr;

        // A block AST node
        class BlockExpr : public ExprWithBlock {
            ::std::vector<Attribute> inner_attrs;

            /*bool has_statements;
            Statements statements;*/

            // bool has_statements;
            ::std::vector< ::std::unique_ptr<Statement> > statements;
            // bool has_expr;
            ::std::unique_ptr<ExprWithoutBlock> expr; // inlined from Statements

          public:
            ::std::string as_string() const;

            // Returns whether the block contains statements.
            inline bool has_statements() const {
                return !statements.empty();
            }

            // Returns whether the block contains an expression
            inline bool has_expr() const {
                return expr != NULL;
            }

            BlockExpr(::std::vector< ::std::unique_ptr<Statement> > block_statements,
              ExprWithoutBlock* block_expr, ::std::vector<Attribute> inner_attribs,
              ::std::vector<Attribute> outer_attribs) :
              statements(block_statements),
              expr(block_expr), inner_attrs(inner_attribs), ExprWithBlock(outer_attribs) {}

            // Copy constructor with clone
            BlockExpr(BlockExpr const& other) :
              statements(other.statements), expr(other.expr->clone_expr_without_block()),
              inner_attrs(other.inner_attrs), ExprWithBlock(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone pointer
            BlockExpr& operator=(BlockExpr const& other) {
                ExprWithBlock::operator=(other);
                statements = other.statements;
                expr = other.expr->clone_expr_without_block();
                inner_attrs = other.inner_attrs;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            BlockExpr(BlockExpr&& other) = default;
            BlockExpr& operator=(BlockExpr&& other) = default;

            // Unique pointer custom clone function
            ::std::unique_ptr<BlockExpr> clone_block_expr() const {
                return ::std::unique_ptr<BlockExpr>(clone_block_expr_impl());
            }

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual BlockExpr* clone_expr_impl() const OVERRIDE {
                return new BlockExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual BlockExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new BlockExpr(*this);
            }

            /* This is the base method as not an abstract class - not virtual but could be in future
             * if required. */
            /*virtual*/ BlockExpr* clone_block_expr_impl() const {
                return new BlockExpr(*this);
            }
        };

        // Represents a type-specified closure expression AST node
        class ClosureExprInnerTyped : public ClosureExpr {
            // Type return_type;
            ::std::unique_ptr<Type> return_type;
            // BlockExpr* expr;
            ::std::unique_ptr<BlockExpr> expr; // only used because may be polymorphic in future

          public:
            /*~ClosureExprInnerTyped() {
                delete expr;
            }*/

            ::std::string as_string() const;

            // Constructor with no move
            ClosureExprInnerTyped(Type* closure_return_type, BlockExpr* closure_expr,
              ::std::vector<ClosureParam> closure_params, ::std::vector<Attribute> outer_attribs) :
              return_type(closure_return_type),
              expr(closure_expr), ClosureExpr(closure_params, false, outer_attribs) {}

            // Constructor potentially with a move
            ClosureExprInnerTyped(Type* closure_return_type, BlockExpr* closure_expr,
              ::std::vector<ClosureParam> closure_params, bool is_move,
              ::std::vector<Attribute> outer_attribs) :
              return_type(closure_return_type),
              expr(closure_expr), ClosureExpr(closure_params, is_move, outer_attribs) {}

            // Copy constructor requires cloning
            ClosureExprInnerTyped(ClosureExprInnerTyped const& other) :
              return_type(other.return_type->clone_type()), expr(other.expr->clone_block_expr()),
              ClosureExpr(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique_ptrs
            ClosureExprInnerTyped& operator=(ClosureExprInnerTyped const& other) {
                ClosureExpr::operator=(other);
                return_type = other.return_type->clone_type();
                expr = other.expr->clone_block_expr();
                // params = other.params;
                // has_move = other.has_move;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            ClosureExprInnerTyped(ClosureExprInnerTyped&& other) = default;
            ClosureExprInnerTyped& operator=(ClosureExprInnerTyped&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ClosureExprInnerTyped* clone_expr_impl() const OVERRIDE {
                return new ClosureExprInnerTyped(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ClosureExprInnerTyped* clone_expr_without_block_impl() const OVERRIDE {
                return new ClosureExprInnerTyped(*this);
            }
        };

        // AST node representing continue expression within loops
        class ContinueExpr : public ExprWithoutBlock {
            // bool has_label;
            Lifetime label;

          public:
            ::std::string as_string() const;

            // Returns true if the continue expr has a label.
            inline bool has_label() const {
                return !label.is_error();
            }

            // Constructor for a ContinueExpr without a label.
            ContinueExpr(::std::vector<Attribute> outer_attribs) :
              label(Lifetime::error()), ExprWithoutBlock(outer_attribs) {}

            // Constructor for a ContinueExpr with a label.
            ContinueExpr(Lifetime label, ::std::vector<Attribute> outer_attribs) :
              label(label), ExprWithoutBlock(outer_attribs) {}

            // copy constructor, destructor, and assignment operator should not need defining

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ContinueExpr* clone_expr_impl() const OVERRIDE {
                return new ContinueExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ContinueExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new ContinueExpr(*this);
            }
        };
        // TODO: merge "break" and "continue"? Or even merge in "return"?

        // AST node representing break expression within loops
        class BreakExpr : public ExprWithoutBlock {
            // bool has_label;
            Lifetime label;

            // bool has_break_expr;
            // Expr* break_expr; // may be uninitialised
            ::std::unique_ptr<Expr> break_expr;

          public:
            /*~BreakExpr() {
                if (has_break_expr) {
                    delete break_expr;
                }
            }*/

            ::std::string as_string() const;

            // Returns whether the break expression has a label or not.
            inline bool has_label() const {
                return !label.is_error();
            }

            // Returns whether the break expression has an expression used in the break or not.
            inline bool has_break_expr() const {
                return break_expr != NULL;
            }

            // Constructor for a break expression without a label or expression in break
            BreakExpr(::std::vector<Attribute> outer_attribs) :
              label(Lifetime::error()), /*break_expr(NULL),*/ ExprWithoutBlock(outer_attribs) {}

            // Constructor for a break expression with potentially a label and expression in break
            BreakExpr(
              Lifetime break_label, Expr* expr_in_break, ::std::vector<Attribute> outer_attribs) :
              label(break_label),
              break_expr(expr_in_break), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor defined to use clone for unique pointer
            BreakExpr(BreakExpr const& other) :
              label(other.label), break_expr(other.break_expr->clone_expr()),
              ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique pointer
            BreakExpr& operator=(BreakExpr const& other) {
                ExprWithoutBlock::operator=(other);
                label = other.label;
                break_expr = other.break_expr->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            BreakExpr(BreakExpr&& other) = default;
            BreakExpr& operator=(BreakExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual BreakExpr* clone_expr_impl() const OVERRIDE {
                return new BreakExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual BreakExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new BreakExpr(*this);
            }
        };

        // Base range expression AST node object - abstract
        class RangeExpr : public ExprWithoutBlock {
          protected:
            RangeExpr(::std::vector<Attribute> outer_attribs) : ExprWithoutBlock(outer_attribs) {}
        };

        // Range from (inclusive) and to (exclusive) expression AST node object
        // aka RangeExpr; constructs a std::ops::Range object
        class RangeFromToExpr : public RangeExpr {
            /*Expr* from;
            Expr* to;*/
            ::std::unique_ptr<Expr> from;
            ::std::unique_ptr<Expr> to;

          public:
            /*~RangeFromToExpr() {
                delete from;
                delete to;
            }*/

            ::std::string as_string() const;

            RangeFromToExpr(
              Expr* range_from, Expr* range_to, ::std::vector<Attribute> outer_attribs) :
              from(range_from),
              to(range_to), RangeExpr(outer_attribs) {}

            // Copy constructor with cloning
            RangeFromToExpr(RangeFromToExpr const& other) :
              from(other.from->clone_expr()), to(other.to->clone_expr()), RangeExpr(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique pointers
            RangeFromToExpr& operator=(RangeFromToExpr const& other) {
                RangeExpr::operator=(other);
                from = other.from->clone_expr();
                to = other.to->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors as not supported in c++03
            RangeFromToExpr(RangeFromToExpr&& other) = default;
            RangeFromToExpr& operator=(RangeFromToExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFromToExpr* clone_expr_impl() const OVERRIDE {
                return new RangeFromToExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFromToExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new RangeFromToExpr(*this);
            }
        };

        // Range from (inclusive) expression AST node object
        // constructs a std::ops::RangeFrom object
        class RangeFromExpr : public RangeExpr {
            // Expr* from;
            ::std::unique_ptr<Expr> from;

          public:
            /*~RangeFromExpr() {
                delete from;
            }*/

            ::std::string as_string() const;

            RangeFromExpr(Expr* range_from, ::std::vector<Attribute> outer_attribs) :
              from(range_from), RangeExpr(outer_attribs) {}

            // Copy constructor with clone
            RangeFromExpr(RangeFromExpr const& other) :
              from(other.from->clone_expr()), RangeExpr(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique_ptr
            RangeFromExpr& operator=(RangeFromExpr const& other) {
                RangeExpr::operator=(other);
                from = other.from->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            RangeFromExpr(RangeFromExpr&& other) = default;
            RangeFromExpr& operator=(RangeFromExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFromExpr* clone_expr_impl() const OVERRIDE {
                return new RangeFromExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFromExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new RangeFromExpr(*this);
            }
        };

        // Range to (exclusive) expression AST node object
        // constructs a std::ops::RangeTo object
        class RangeToExpr : public RangeExpr {
            // Expr* to;
            ::std::unique_ptr<Expr> to;

          public:
            /*~RangeToExpr() {
                delete to;
            }*/

            ::std::string as_string() const;

            RangeToExpr(Expr* range_to, ::std::vector<Attribute> outer_attribs) :
              to(range_to), RangeExpr(outer_attribs) {}

            // Copy constructor with clone
            RangeToExpr(RangeToExpr const& other) : to(other.to->clone_expr()), RangeExpr(other) {}

            // Destructor - define here if required

            // Overload assignment operator to clone unique_ptr
            RangeToExpr& operator=(RangeToExpr const& other) {
                RangeExpr::operator=(other);
                to = other.to->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            RangeToExpr(RangeToExpr&& other) = default;
            RangeToExpr& operator=(RangeToExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeToExpr* clone_expr_impl() const OVERRIDE {
                return new RangeToExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeToExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new RangeToExpr(*this);
            }
        };

        // Full range expression AST node object
        // constructs a std::ops::RangeFull object
        class RangeFullExpr : public RangeExpr {
          public:
            ::std::string as_string() const;

            RangeFullExpr(::std::vector<Attribute> outer_attribs) : RangeExpr(outer_attribs) {}

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFullExpr* clone_expr_impl() const OVERRIDE {
                return new RangeFullExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFullExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new RangeFullExpr(*this);
            }
        };

        // Range from (inclusive) and to (inclusive) expression AST node object
        // aka RangeInclusiveExpr; constructs a std::ops::RangeInclusive object
        class RangeFromToInclExpr : public RangeExpr {
            /*Expr* from;
            Expr* to;*/
            ::std::unique_ptr<Expr> from;
            ::std::unique_ptr<Expr> to;

          public:
            /*~RangeFromToInclExpr() {
                delete from;
                delete to;
            }*/

            ::std::string as_string() const;

            RangeFromToInclExpr(
              Expr* range_from, Expr* range_to, ::std::vector<Attribute> outer_attribs) :
              from(range_from),
              to(range_to), RangeExpr(outer_attribs) {}

            // Copy constructor with clone
            RangeFromToInclExpr(RangeFromToInclExpr const& other) :
              from(other.from->clone_expr()), to(other.to->clone_expr()), RangeExpr(other) {}

            // Destructor - define here if required

            // Overload assignment operator to use clone
            RangeFromToInclExpr& operator=(RangeFromToInclExpr const& other) {
                RangeExpr::operator=(other);
                from = other.from->clone_expr();
                to = other.to->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            RangeFromToInclExpr(RangeFromToInclExpr&& other) = default;
            RangeFromToInclExpr& operator=(RangeFromToInclExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFromToInclExpr* clone_expr_impl() const OVERRIDE {
                return new RangeFromToInclExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeFromToInclExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new RangeFromToInclExpr(*this);
            }
        };

        // Range to (inclusive) expression AST node object
        // aka RangeToInclusiveExpr; constructs a std::ops::RangeToInclusive object
        class RangeToInclExpr : public RangeExpr {
            // Expr* to;
            ::std::unique_ptr<Expr> to;

          public:
            /*~RangeToInclExpr() {
                delete to;
            }*/

            ::std::string as_string() const;

            RangeToInclExpr(Expr* range_to, ::std::vector<Attribute> outer_attribs) :
              to(range_to), RangeExpr(outer_attribs) {}

            // Copy constructor with clone
            RangeToInclExpr(RangeToInclExpr const& other) :
              to(other.to->clone_expr()), RangeExpr(other) {}

            // Define destructor here if required

            // Overload assignment operator to clone pointer
            RangeToInclExpr& operator=(RangeToInclExpr const& other) {
                RangeExpr::operator=(other);
                to = other.to->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            RangeToInclExpr(RangeToInclExpr&& other) = default;
            RangeToInclExpr& operator=(RangeToInclExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeToInclExpr* clone_expr_impl() const OVERRIDE {
                return new RangeToInclExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual RangeToInclExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new RangeToInclExpr(*this);
            }
        };

        // Return expression AST node representation
        class ReturnExpr : public ExprWithoutBlock {
            // bool has_return_expr;
            // Expr* return_expr;
            ::std::unique_ptr<Expr> return_expr;

          public:
            /*~ReturnExpr() {
                if (has_return_expr) {
                    delete return_expr;
                }
            }*/

            ::std::string as_string() const;

            // Returns whether the object has an expression returned (i.e. not void return type).
            inline bool has_return_expr() const {
                return return_expr != NULL;
            }

            // Constructor for ReturnExpr without a returned expression.
            ReturnExpr(::std::vector<Attribute> outer_attribs) :
              /*return_expr(NULL),*/ ExprWithoutBlock(outer_attribs) {}

            // Constructor for ReturnExpr with a returned expression.
            ReturnExpr(Expr* returned_expr, ::std::vector<Attribute> outer_attribs) :
              return_expr(returned_expr), ExprWithoutBlock(outer_attribs) {}

            // Copy constructor with clone
            ReturnExpr(ReturnExpr const& other) :
              return_expr(other.return_expr->clone_expr()), ExprWithoutBlock(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone return_expr pointer
            ReturnExpr& operator=(ReturnExpr const& other) {
                ExprWithoutBlock::operator=(other);
                return_expr = other.return_expr->clone_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            ReturnExpr(ReturnExpr&& other) = default;
            ReturnExpr& operator=(ReturnExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ReturnExpr* clone_expr_impl() const OVERRIDE {
                return new ReturnExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ReturnExpr* clone_expr_without_block_impl() const OVERRIDE {
                return new ReturnExpr(*this);
            }
        };

        // Forward decl - defined in rust-macro.h
        class MacroInvocation;
        /*class MacroInvocation : public ExprWithoutBlock {
          public:
            ::std::string as_string() const;
        };*/

        // An unsafe block AST node
        class UnsafeBlockExpr : public ExprWithBlock {
            // Or just have it extend BlockExpr
            // BlockExpr* expr;
            ::std::unique_ptr<BlockExpr> expr;

          public:
            /*~UnsafeBlockExpr() {
                delete expr;
            }*/

            ::std::string as_string() const;

            UnsafeBlockExpr(BlockExpr* block_expr, ::std::vector<Attribute> outer_attribs) :
              expr(block_expr), ExprWithBlock(outer_attribs) {}

            // Copy constructor with clone
            UnsafeBlockExpr(UnsafeBlockExpr const& other) :
              expr(other.expr->clone_block_expr()), ExprWithBlock(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            UnsafeBlockExpr& operator=(UnsafeBlockExpr const& other) {
                ExprWithBlock::operator=(other);
                expr = other.expr->clone_block_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            UnsafeBlockExpr(UnsafeBlockExpr&& other) = default;
            UnsafeBlockExpr& operator=(UnsafeBlockExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual UnsafeBlockExpr* clone_expr_impl() const OVERRIDE {
                return new UnsafeBlockExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual UnsafeBlockExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new UnsafeBlockExpr(*this);
            }
        };

        // Loop label expression AST node used with break and continue expressions
        // TODO: inline?
        class LoopLabel : public Node {
            Lifetime label; // or type LIFETIME_OR_LABEL

          public:
            ::std::string as_string() const;

            LoopLabel(Lifetime loop_label) : label(loop_label) {}

            // Returns whether the LoopLabel is in an error state.
            inline bool is_error() const {
                return label.is_error();
            }

            // Creates an error state LoopLabel.
            static LoopLabel error() {
                return LoopLabel(Lifetime::error());
            }
        };

        // Base loop expression AST node - aka LoopExpr
        class BaseLoopExpr : public ExprWithBlock {
            // bool has_loop_label;
            LoopLabel loop_label;

            // BlockExpr* loop_block;
            ::std::unique_ptr<BlockExpr> loop_block;

          protected:
            // Constructor for BaseLoopExpr without a loop label
            BaseLoopExpr(BlockExpr* loop_block, ::std::vector<Attribute> outer_attribs) :
              loop_block(loop_block), loop_label(LoopLabel::error()), ExprWithBlock(outer_attribs) {}

            // Constructor for BaseLoopExpr with a loop label
            BaseLoopExpr(
              BlockExpr* loop_block, LoopLabel loop_label, ::std::vector<Attribute> outer_attribs) :
              loop_block(loop_block),
              loop_label(loop_label), ExprWithBlock(outer_attribs) {}

            // Copy constructor for BaseLoopExpr with clone
            BaseLoopExpr(BaseLoopExpr const& other) :
              loop_block(other.loop_block->clone_block_expr()), loop_label(other.loop_label),
              ExprWithBlock(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            BaseLoopExpr& operator=(BaseLoopExpr const& other) {
                ExprWithBlock::operator=(other);
                loop_block = other.loop_block->clone_block_expr();
                loop_label = other.loop_label;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            BaseLoopExpr(BaseLoopExpr&& other) = default;
            BaseLoopExpr& operator=(BaseLoopExpr&& other) = default;

          public:
            /*~BaseLoopExpr() {
                delete loop_block;
            }*/

            inline bool has_loop_label() const {
                return !loop_label.is_error();
            }
        };

        // 'Loop' expression (i.e. the infinite loop) AST node
        class LoopExpr : public BaseLoopExpr {
          public:
            ::std::string as_string() const;

            // Constructor for LoopExpr without a loop label
            LoopExpr(BlockExpr* loop_block, ::std::vector<Attribute> outer_attribs) :
              BaseLoopExpr(loop_block, outer_attribs) {}

            // Constructor for LoopExpr with a loop label
            LoopExpr(
              BlockExpr* loop_block, LoopLabel loop_label, ::std::vector<Attribute> outer_attribs) :
              BaseLoopExpr(loop_block, loop_label, outer_attribs) {}

            // copy constructor, destructor, and assignment operator should not need modification

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual LoopExpr* clone_expr_impl() const OVERRIDE {
                return new LoopExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual LoopExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new LoopExpr(*this);
            }
        };

        // While loop expression AST node (predicate loop)
        class WhileLoopExpr : public BaseLoopExpr {
            // Expr* condition;
            ::std::unique_ptr<Expr> condition;

          public:
            /*~WhileLoopExpr() {
                delete condition;
            }*/

            ::std::string as_string() const;

            // Constructor for while loop without loop label
            WhileLoopExpr(
              Expr* loop_condition, BlockExpr* loop_block, ::std::vector<Attribute> outer_attribs) :
              condition(loop_condition),
              BaseLoopExpr(loop_block, outer_attribs) {}

            // Constructor for while loop with loop label
            WhileLoopExpr(Expr* loop_condition, BlockExpr* loop_block, LoopLabel loop_label,
              ::std::vector<Attribute> outer_attribs) :
              condition(loop_condition),
              BaseLoopExpr(loop_block, loop_label, outer_attribs) {}

            // Copy constructor with clone
            WhileLoopExpr(WhileLoopExpr const& other) :
              condition(other.condition->clone_expr()), BaseLoopExpr(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            WhileLoopExpr& operator=(WhileLoopExpr const& other) {
                BaseLoopExpr::operator=(other);
                condition = other.condition->clone_expr();
                // loop_block = other.loop_block->clone_block_expr();
                // loop_label = other.loop_label;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            WhileLoopExpr(WhileLoopExpr&& other) = default;
            WhileLoopExpr& operator=(WhileLoopExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual WhileLoopExpr* clone_expr_impl() const OVERRIDE {
                return new WhileLoopExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual WhileLoopExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new WhileLoopExpr(*this);
            }
        };

        // Forward decl MatchArmPatterns
        // struct MatchArmPatterns;

        // While let loop expression AST node (predicate pattern loop)
        class WhileLetLoopExpr : public BaseLoopExpr {
            // MatchArmPatterns patterns;
            ::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns; // inlined
            // Expr* condition;
            ::std::unique_ptr<Expr> condition;

          public:
            /*~WhileLetLoopExpr() {
                delete condition;
            }*/

            ::std::string as_string() const;

            // Constructor with no loop label
            WhileLetLoopExpr(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns,
              Expr* condition, BlockExpr* loop_block, ::std::vector<Attribute> outer_attribs) :
              match_arm_patterns(match_arm_patterns),
              condition(condition), BaseLoopExpr(loop_block, outer_attribs) {}

            // Constructor with a loop label
            WhileLetLoopExpr(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns,
              Expr* condition, BlockExpr* loop_block, LoopLabel loop_label,
              ::std::vector<Attribute> outer_attribs) :
              match_arm_patterns(match_arm_patterns),
              condition(condition), BaseLoopExpr(loop_block, loop_label, outer_attribs) {}

            // Copy constructor with clone
            WhileLetLoopExpr(WhileLetLoopExpr const& other) :
              match_arm_patterns(other.match_arm_patterns), condition(other.condition->clone_expr()),
              BaseLoopExpr(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone pointers
            WhileLetLoopExpr& operator=(WhileLetLoopExpr const& other) {
                BaseLoopExpr::operator=(other);
                match_arm_patterns = other.match_arm_patterns;
                condition = other.condition->clone_expr();
                // loop_block = other.loop_block->clone_block_expr();
                // loop_label = other.loop_label;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            WhileLetLoopExpr(WhileLetLoopExpr&& other) = default;
            WhileLetLoopExpr& operator=(WhileLetLoopExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual WhileLetLoopExpr* clone_expr_impl() const OVERRIDE {
                return new WhileLetLoopExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual WhileLetLoopExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new WhileLetLoopExpr(*this);
            }
        };

        // For loop expression AST node (iterator loop)
        class ForLoopExpr : public BaseLoopExpr {
            // Pattern pattern;
            ::std::unique_ptr<Pattern> pattern;
            // Expr* iterator_expr;
            ::std::unique_ptr<Expr> iterator_expr;

          public:
            /*~ForLoopExpr() {
                delete iterator_expr;
            }*/

            ::std::string as_string() const;

            // Constructor without loop label
            ForLoopExpr(Pattern* loop_pattern, Expr* iterator_expr, BlockExpr* loop_body,
              ::std::vector<Attribute> outer_attribs) :
              pattern(loop_pattern),
              iterator_expr(iterator_expr), BaseLoopExpr(loop_body, outer_attribs) {}

            // Constructor with loop label
            ForLoopExpr(Pattern* loop_pattern, Expr* iterator_expr, BlockExpr* loop_body,
              LoopLabel loop_label, ::std::vector<Attribute> outer_attribs) :
              pattern(loop_pattern),
              iterator_expr(iterator_expr), BaseLoopExpr(loop_body, loop_label, outer_attribs) {}

            // Copy constructor with clone
            ForLoopExpr(ForLoopExpr const& other) :
              pattern(other.pattern->clone_pattern()),
              iterator_expr(other.iterator_expr->clone_expr()), BaseLoopExpr(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            ForLoopExpr& operator=(ForLoopExpr const& other) {
                BaseLoopExpr::operator=(other);
                pattern = other.pattern->clone_pattern();
                iterator_expr = other.iterator_expr->clone_expr();
                /*loop_block = other.loop_block->clone_block_expr();
                loop_label = other.loop_label;
                outer_attrs = other.outer_attrs;*/

                return *this;
            }

            // move constructors
            ForLoopExpr(ForLoopExpr&& other) = default;
            ForLoopExpr& operator=(ForLoopExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual ForLoopExpr* clone_expr_impl() const OVERRIDE {
                return new ForLoopExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual ForLoopExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new ForLoopExpr(*this);
            }
        };

        // forward decl for IfExpr
        class IfLetExpr;

        // Base if expression with no "else" or "if let" AST node
        class IfExpr : public ExprWithBlock {
            /*Expr* condition;
            BlockExpr* if_block;*/
            ::std::unique_ptr<Expr> condition;
            ::std::unique_ptr<BlockExpr> if_block;
            /*union {
                BlockExpr else_block;
                IfExpr* if_expr;
                IfLetExpr if_let_expr;
            } consequent_block;*/

          public:
            /*virtual ~IfExpr() {
                delete condition;
                delete if_block;
            }*/

            ::std::string as_string() const;

            IfExpr(Expr* condition, BlockExpr* if_block, ::std::vector<Attribute> outer_attribs) :
              condition(condition), if_block(if_block), ExprWithBlock(outer_attribs) {}

            // Copy constructor with clone
            IfExpr(IfExpr const& other) :
              condition(other.condition->clone_expr()), if_block(other.if_block->clone_block_expr()),
              ExprWithBlock(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone expressions
            IfExpr& operator=(IfExpr const& other) {
                ExprWithBlock::operator=(other);
                condition = other.condition->clone_expr();
                if_block = other.if_block->clone_block_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfExpr(IfExpr&& other) = default;
            IfExpr& operator=(IfExpr&& other) = default;

            // Unique pointer custom clone function
            ::std::unique_ptr<IfExpr> clone_if_expr() const {
                return ::std::unique_ptr<IfExpr>(clone_if_expr_impl());
            }

            // Note that multiple "else if"s are handled via nested ASTs rather than an array of
            // else ifs - i.e. not like a switch statement

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExpr* clone_expr_impl() const OVERRIDE {
                return new IfExpr(*this);
            }

            // Base clone function but still concrete as concrete base class
            virtual IfExpr* clone_if_expr_impl() const {
                return new IfExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new IfExpr(*this);
            }
        };

        // If expression with an ending "else" expression AST node (trailing)
        class IfExprConseqElse : public IfExpr {
            // BlockExpr* else_block;
            ::std::unique_ptr<BlockExpr> else_block;

          public:
            /*~IfExprConseqElse() {
                delete else_block;
            }*/

            ::std::string as_string() const;

            IfExprConseqElse(Expr* condition, BlockExpr* if_block, BlockExpr* else_block,
              ::std::vector<Attribute> outer_attribs) :
              else_block(else_block),
              IfExpr(condition, if_block, outer_attribs) {}

            // Copy constructor with clone
            IfExprConseqElse(IfExprConseqElse const& other) :
              else_block(other.else_block->clone_block_expr()), IfExpr(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator with cloning
            IfExprConseqElse& operator=(IfExprConseqElse const& other) {
                IfExpr::operator=(other);
                // condition = other.condition->clone_expr();
                // if_block = other.if_block->clone_block_expr();
                else_block = other.else_block->clone_block_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfExprConseqElse(IfExprConseqElse&& other) = default;
            IfExprConseqElse& operator=(IfExprConseqElse&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqElse* clone_expr_impl() const OVERRIDE {
                return new IfExprConseqElse(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqElse* clone_expr_with_block_impl() const OVERRIDE {
                return new IfExprConseqElse(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqElse* clone_if_expr_impl() const OVERRIDE {
                return new IfExprConseqElse(*this);
            }
        };

        // If expression with an ending "else if" expression AST node
        class IfExprConseqIf : public IfExpr {
            // IfExpr* if_expr;
            ::std::unique_ptr<IfExpr> if_expr;

          public:
            /*~IfExprConseqIf() {
                delete if_expr;
            }*/

            ::std::string as_string() const;

            IfExprConseqIf(Expr* condition, BlockExpr* if_block, IfExpr* conseq_if_expr,
              ::std::vector<Attribute> outer_attribs) :
              if_expr(conseq_if_expr),
              IfExpr(condition, if_block, outer_attribs) {}

            // Copy constructor with clone
            IfExprConseqIf(IfExprConseqIf const& other) :
              if_expr(other.if_expr->clone_if_expr()), IfExpr(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to use clone
            IfExprConseqIf& operator=(IfExprConseqIf const& other) {
                IfExpr::operator=(other);
                // condition = other.condition->clone_expr();
                // if_block = other.if_block->clone_block_expr();
                if_expr = other.if_expr->clone_if_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfExprConseqIf(IfExprConseqIf&& other) = default;
            IfExprConseqIf& operator=(IfExprConseqIf&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqIf* clone_expr_impl() const OVERRIDE {
                return new IfExprConseqIf(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqIf* clone_expr_with_block_impl() const OVERRIDE {
                return new IfExprConseqIf(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqIf* clone_if_expr_impl() const OVERRIDE {
                return new IfExprConseqIf(*this);
            }
        };

        // Basic "if let" expression AST node with no else
        class IfLetExpr : public ExprWithBlock {
            // MatchArmPatterns patterns;
            ::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns; // inlined
            /*Expr* value;
            BlockExpr* if_block;*/
            ::std::unique_ptr<Expr> value;
            ::std::unique_ptr<BlockExpr> if_block;
            /*union {
                BlockExpr else_block;
                IfExpr if_expr;
                IfLetExpr* if_let_expr;
            } consequent_block;*/

          public:
            ::std::string as_string() const;

            IfLetExpr(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns, Expr* value,
              BlockExpr* if_block, ::std::vector<Attribute> outer_attribs) :
              match_arm_patterns(match_arm_patterns),
              value(value), if_block(if_block), ExprWithBlock(outer_attribs) {}

            // copy constructor with clone
            IfLetExpr(IfLetExpr const& other) :
              match_arm_patterns(other.match_arm_patterns), value(other.value->clone_expr()),
              if_block(other.if_block->clone_block_expr()), ExprWithBlock(other) {}

            // destructor - define here if required

            // overload assignment operator to clone
            IfLetExpr& operator=(IfLetExpr const& other) {
                ExprWithBlock::operator=(other);
                match_arm_patterns = other.match_arm_patterns;
                value = other.value->clone_expr();
                if_block = other.if_block->clone_block_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfLetExpr(IfLetExpr&& other) = default;
            IfLetExpr& operator=(IfLetExpr&& other) = default;

            // Unique pointer custom clone function
            ::std::unique_ptr<IfLetExpr> clone_if_let_expr() const {
                return ::std::unique_ptr<IfLetExpr>(clone_if_let_expr_impl());
            }

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExpr* clone_expr_impl() const OVERRIDE {
                return new IfLetExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new IfLetExpr(*this);
            }

            // Base clone function but still concrete as concrete base class
            virtual IfLetExpr* clone_if_let_expr_impl() const {
                return new IfLetExpr(*this);
            }
        };

        // If expression with an ending "else if let" expression AST node
        class IfExprConseqIfLet : public IfExpr {
            // IfLetExpr* if_let_expr;
            ::std::unique_ptr<IfLetExpr> if_let_expr;

          public:
            /*~IfExprIfConseqIfLet() {
                delete if_let_expr;
            }*/

            ::std::string as_string() const;

            IfExprConseqIfLet(Expr* condition, BlockExpr* if_block, IfLetExpr* conseq_if_let_expr,
              ::std::vector<Attribute> outer_attribs) :
              if_let_expr(conseq_if_let_expr),
              IfExpr(condition, if_block, outer_attribs) {}

            // Copy constructor with clone
            IfExprConseqIfLet(IfExprConseqIfLet const& other) :
              if_let_expr(other.if_let_expr->clone_if_let_expr()), IfExpr(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to use clone
            IfExprConseqIfLet& operator=(IfExprConseqIfLet const& other) {
                IfExpr::operator=(other);
                // condition = other.condition->clone_expr();
                // if_block = other.if_block->clone_block_expr();
                if_let_expr = other.if_let_expr->clone_if_let_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfExprConseqIfLet(IfExprConseqIfLet&& other) = default;
            IfExprConseqIfLet& operator=(IfExprConseqIfLet&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqIfLet* clone_expr_impl() const OVERRIDE {
                return new IfExprConseqIfLet(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqIfLet* clone_expr_with_block_impl() const OVERRIDE {
                return new IfExprConseqIfLet(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfExprConseqIfLet* clone_if_expr_impl() const OVERRIDE {
                return new IfExprConseqIfLet(*this);
            }
        };

        // AST node representing "if let" expression with an "else" expression at the end
        class IfLetExprConseqElse : public IfLetExpr {
            // BlockExpr* else_block;
            ::std::unique_ptr<BlockExpr> else_block;

          public:
            /*~IfLetExprConseqElse() {
                delete else_block;
            }*/

            ::std::string as_string() const;

            IfLetExprConseqElse(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns,
              Expr* value, BlockExpr* if_block, BlockExpr* else_block,
              ::std::vector<Attribute> outer_attribs) :
              IfLetExpr(match_arm_patterns, value, if_block, outer_attribs),
              else_block(else_block) {}

            // copy constructor with clone
            IfLetExprConseqElse(IfLetExprConseqElse const& other) :
              else_block(other.else_block->clone_block_expr()), IfLetExpr(other) {}

            // destructor - define here if required

            // overload assignment operator to clone
            IfLetExprConseqElse& operator=(IfLetExprConseqElse const& other) {
                IfLetExpr::operator=(other);
                // match_arm_patterns = other.match_arm_patterns;
                // value = other.value->clone_expr();
                // if_block = other.if_block->clone_block_expr();
                else_block = other.else_block->clone_block_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfLetExprConseqElse(IfLetExprConseqElse&& other) = default;
            IfLetExprConseqElse& operator=(IfLetExprConseqElse&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqElse* clone_expr_impl() const OVERRIDE {
                return new IfLetExprConseqElse(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqElse* clone_expr_with_block_impl() const OVERRIDE {
                return new IfLetExprConseqElse(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqElse* clone_if_let_expr_impl() const OVERRIDE {
                return new IfLetExprConseqElse(*this);
            }
        };

        // AST node representing "if let" expression with an "else if" expression at the end
        class IfLetExprConseqIf : public IfLetExpr {
            // IfExpr* if_expr;
            ::std::unique_ptr<IfExpr> if_expr;

          public:
            /*~IfLetExprConseqIf() {
                delete if_expr;
            }*/

            ::std::string as_string() const;

            IfLetExprConseqIf(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns,
              Expr* value, BlockExpr* if_block, IfExpr* if_expr,
              ::std::vector<Attribute> outer_attribs) :
              IfLetExpr(match_arm_patterns, value, if_block, outer_attribs),
              if_expr(if_expr) {}

            // copy constructor with clone
            IfLetExprConseqIf(IfLetExprConseqIf const& other) :
              if_expr(other.if_expr->clone_if_expr()), IfLetExpr(other) {}

            // destructor - define here if required

            // overload assignment operator to clone
            IfLetExprConseqIf& operator=(IfLetExprConseqIf const& other) {
                IfLetExpr::operator=(other);
                // match_arm_patterns = other.match_arm_patterns;
                // value = other.value->clone_expr();
                // if_block = other.if_block->clone_block_expr();
                if_expr = other.if_expr->clone_if_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfLetExprConseqIf(IfLetExprConseqIf&& other) = default;
            IfLetExprConseqIf& operator=(IfLetExprConseqIf&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqIf* clone_expr_impl() const OVERRIDE {
                return new IfLetExprConseqIf(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqIf* clone_expr_with_block_impl() const OVERRIDE {
                return new IfLetExprConseqIf(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqIf* clone_if_let_expr_impl() const OVERRIDE {
                return new IfLetExprConseqIf(*this);
            }
        };

        // AST node representing "if let" expression with an "else if let" expression at the end
        class IfLetExprConseqIfLet : public IfLetExpr {
            // IfLetExpr* if_let_expr;
            ::std::unique_ptr<IfLetExpr> if_let_expr;

          public:
            /*~IfLetExprConseqIfLet() {
                delete if_let_expr;
            }*/

            ::std::string as_string() const;

            IfLetExprConseqIfLet(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns,
              Expr* value, BlockExpr* if_block, IfLetExpr* if_let_expr,
              ::std::vector<Attribute> outer_attribs) :
              IfLetExpr(match_arm_patterns, value, if_block, outer_attribs),
              if_let_expr(if_let_expr) {}

            // copy constructor with clone
            IfLetExprConseqIfLet(IfLetExprConseqIfLet const& other) :
              if_let_expr(other.if_let_expr->clone_if_let_expr()), IfLetExpr(other) {}

            // destructor - define here if required

            // overload assignment operator to clone
            IfLetExprConseqIfLet& operator=(IfLetExprConseqIfLet const& other) {
                IfLetExpr::operator=(other);
                // match_arm_patterns = other.match_arm_patterns;
                // value = other.value->clone_expr();
                // if_block = other.if_block->clone_block_expr();
                if_let_expr = other.if_let_expr->clone_if_let_expr();
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            IfLetExprConseqIfLet(IfLetExprConseqIfLet&& other) = default;
            IfLetExprConseqIfLet& operator=(IfLetExprConseqIfLet&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqIfLet* clone_expr_impl() const OVERRIDE {
                return new IfLetExprConseqIfLet(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqIfLet* clone_expr_with_block_impl() const OVERRIDE {
                return new IfLetExprConseqIfLet(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual IfLetExprConseqIfLet* clone_if_let_expr_impl() const OVERRIDE {
                return new IfLetExprConseqIfLet(*this);
            }
        };

        // Match arm expression
        struct MatchArm {
          private:
            ::std::vector<Attribute> outer_attrs;
            // MatchArmPatterns patterns;
            ::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns; // inlined

            // bool has_match_arm_guard;
            // Expr* match_arm_guard; // inlined from MatchArmGuard
            ::std::unique_ptr<Expr> guard_expr;

          public:
            /*~MatchArm() {
                if (has_match_arm_guard) {
                    delete match_arm_guard;
                }
            }*/

            // Returns whether the MatchArm has a match arm guard expression
            inline bool has_match_arm_guard() const {
                return guard_expr != NULL;
            }

            // Constructor for match arm without a guard expression
            MatchArm(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns,
              ::std::vector<Attribute> outer_attrs) :
              match_arm_patterns(match_arm_patterns),
              outer_attrs(outer_attrs) /*, guard_expr(NULL)*/ {}

            // Constructor for match arm with a guard expression
            MatchArm(::std::vector< ::std::unique_ptr<Pattern> > match_arm_patterns, Expr* guard_expr,
              ::std::vector<Attribute> outer_attrs) :
              match_arm_patterns(match_arm_patterns),
              guard_expr(guard_expr), outer_attrs(outer_attrs) {}

            // Copy constructor with clone
            MatchArm(MatchArm const& other) :
              match_arm_patterns(other.match_arm_patterns), outer_attrs(other.outer_attrs),
              guard_expr(other.guard_expr->clone_expr()) {}

            ~MatchArm() = default;

            // Overload assignment operator to clone
            MatchArm& operator=(MatchArm const& other) {
                match_arm_patterns = other.match_arm_patterns;
                outer_attrs = other.outer_attrs;
                guard_expr = other.guard_expr->clone_expr();

                return *this;
            }

            // move constructors
            MatchArm(MatchArm&& other) = default;
            MatchArm& operator=(MatchArm&& other) = default;
        };

        // Base "match case" for a match expression - abstract
        class MatchCase {
            MatchArm arm;

          protected:
            MatchCase(MatchArm arm) : arm(arm) {}

            // Should not require copy constructor or assignment operator overloading
          public:
            virtual ~MatchCase() {}
        };

        // Block expression match case
        class MatchCaseBlockExpr : public MatchCase {
            // BlockExpr* block_expr;
            ::std::unique_ptr<BlockExpr> block_expr;

          public:
            /*~MatchCaseBlockExpr() {
                delete block_expr;
            }*/

            MatchCaseBlockExpr(MatchArm arm, BlockExpr* block_expr) :
              block_expr(block_expr), MatchCase(arm) {}

            // Copy constructor requires clone
            MatchCaseBlockExpr(MatchCaseBlockExpr const& other) :
              block_expr(other.block_expr->clone_block_expr()), MatchCase(other) {}

            // Destructor - define here if required

            // Overload assignment operator to have clone
            MatchCaseBlockExpr& operator=(MatchCaseBlockExpr const& other) {
                MatchCase::operator=(other);
                block_expr = other.block_expr->clone_block_expr();
                // arm = other.arm;

                return *this;
            }

            // move constructors
            MatchCaseBlockExpr(MatchCaseBlockExpr&& other) = default;
            MatchCaseBlockExpr& operator=(MatchCaseBlockExpr&& other) = default;
        };

        // Expression (except block expression) match case
        class MatchCaseExpr : public MatchCase {
            // Expr* expr;
            ::std::unique_ptr<Expr> expr;

          public:
            /*~MatchCaseExpr() {
                delete expr;
            }*/

            MatchCaseExpr(MatchArm arm, Expr* expr) : expr(expr), MatchCase(arm) {}

            // Copy constructor requires clone
            MatchCaseExpr(MatchCaseExpr const& other) :
              expr(other.expr->clone_expr()), MatchCase(other) {}

            // Destructor - define here if required

            // Overload assignment operator to have clone
            MatchCaseExpr& operator=(MatchCaseExpr const& other) {
                MatchCase::operator=(other);
                expr = other.expr->clone_expr();
                // arm = other.arm;

                return *this;
            }

            // move constructors
            MatchCaseExpr(MatchCaseExpr&& other) = default;
            MatchCaseExpr& operator=(MatchCaseExpr&& other) = default;
        };

        // Match expression AST node
        class MatchExpr : public ExprWithBlock {
            // Expr* branch_value;
            ::std::unique_ptr<Expr> branch_value;
            ::std::vector<Attribute> inner_attrs;

            // bool has_match_arms;
            // MatchArms match_arms;
            ::std::vector< ::std::unique_ptr<MatchCase> > match_arms; // inlined from MatchArms

          public:
            /*~MatchExpr() {
                delete branch_value;
            }*/

            ::std::string as_string() const;

            // Returns whether the match expression has any match arms.
            inline bool has_match_arms() const {
                return !match_arms.empty();
            }

            MatchExpr(Expr* branch_value, ::std::vector< ::std::unique_ptr<MatchCase> > match_arms,
              ::std::vector<Attribute> inner_attrs, ::std::vector<Attribute> outer_attrs) :
              branch_value(branch_value),
              match_arms(match_arms), inner_attrs(inner_attrs), ExprWithBlock(outer_attrs) {}

            // Copy constructor requires clone due to unique_ptr
            MatchExpr(MatchExpr const& other) :
              branch_value(other.branch_value->clone_expr()), match_arms(other.match_arms),
              inner_attrs(other.inner_attrs), ExprWithBlock(other) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone due to unique_ptr
            MatchExpr& operator=(MatchExpr const& other) {
                ExprWithBlock::operator=(other);
                branch_value = other.branch_value->clone_expr();
                match_arms = other.match_arms;
                inner_attrs = other.inner_attrs;
                // outer_attrs = other.outer_attrs;

                return *this;
            }

            // move constructors
            MatchExpr(MatchExpr&& other) = default;
            MatchExpr& operator=(MatchExpr&& other) = default;

          protected:
            // Use covariance to implement clone function as returning this object rather than base
            virtual MatchExpr* clone_expr_impl() const OVERRIDE {
                return new MatchExpr(*this);
            }

            // Use covariance to implement clone function as returning this object rather than base
            virtual MatchExpr* clone_expr_with_block_impl() const OVERRIDE {
                return new MatchExpr(*this);
            }
        };
    }
}

#endif