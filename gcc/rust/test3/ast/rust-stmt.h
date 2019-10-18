#ifndef RUST_AST_STATEMENT_H
#define RUST_AST_STATEMENT_H

#include "rust-ast.h"
#include "rust-expr.h"
#include "rust-path.h"

namespace Rust {
    namespace AST {
        // Just a semi-colon, which apparently is a statement.
        class EmptyStatement : public Statement {
          public:
            ::std::string as_string() const {
                return ::std::string(1, ';');
            }

            EmptyStatement() {}
        };

        /* This is syntactically identical to declaring an item inside a module BUT it has block
         * scope. Type of "declaration statement" as it introduces new name into scope */
        class ItemStatement : public Statement {
            // TODO: put in same params as regular item
            // maybe even merge data structure with module item?

          public:
            ::std::string as_string() const;
        };

        /* Variable assignment let statement - type of "declaration statement" as it introduces new
         * name into scope */
        class LetStatement : public Statement {
            // bool has_outer_attrs;
            ::std::vector<Attribute> outer_attrs;

            // Pattern variables_pattern;
            ::gnu::unique_ptr<Pattern> variables_pattern;

            // bool has_type;
            // Type type;
            ::gnu::unique_ptr<Type> type;

            // bool has_init_expr;
            // Expr* init_expr;
            ::gnu::unique_ptr<Expr> init_expr;

          public:
            // Returns whether let statement has outer attributes.
            inline bool has_outer_attrs() const {
                return !outer_attrs.empty();
            }

            // Returns whether let statement has a given return type.
            inline bool has_type() const {
                return type != NULL;
            }

            // Returns whether let statement has an initialisation expression.
            inline bool has_init_expr() const {
                return init_expr != NULL;
            }

            /*~LetStatement() {
                if (has_init_expr) {
                    delete init_expr;
                }
            }*/

            ::std::string as_string() const;

            LetStatement(Pattern* variables_pattern, Expr* init_expr, Type* type,
              ::std::vector<Attribute> outer_attrs) :
              variables_pattern(variables_pattern),
              init_expr(init_expr), type(type), outer_attrs(outer_attrs) {}

            // Copy constructor with clone
            LetStatement(LetStatement const& other) :
              variables_pattern(other.variables_pattern->clone_pattern()),
              init_expr(other.init_expr->clone_expr()), type(other.type->clone_type()),
              outer_attrs(other.outer_attrs) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            LetStatement& operator=(LetStatement const& other) {
                variables_pattern = other.variables_pattern->clone_pattern();
                init_expr = other.init_expr->clone_expr();
                type = other.type->clone_type();
                outer_attrs = other.outer_attrs;

                return *this;
            }

            // no move constructors as not supported in c++03
            /*LetStatement(LetStatement&& other) = default;
            LetStatement& operator=(LetStatement&& other) = default;*/
        };

        // Abstract base class for expression statements (statements containing an expression)
        class ExpressionStatement : public Statement {};

        // Statement containing an expression without a block
        class ExpressionStatementWithoutBlock : public ExpressionStatement {
            // ExprWithoutBlock* expr;
            ::gnu::unique_ptr<ExprWithoutBlock> expr;

          public:
            /*~ExpressionStatementWithoutBlock() {
                delete expr;
            }*/

            ::std::string as_string() const;

            ExpressionStatementWithoutBlock(ExprWithoutBlock* expr) : expr(expr) {}

            // Copy constructor with clone
            ExpressionStatementWithoutBlock(ExpressionStatementWithoutBlock const& other) :
              expr(other.expr->clone_expr_without_block()) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            ExpressionStatementWithoutBlock& operator=(ExpressionStatementWithoutBlock const& other) {
                expr = other.expr->clone_expr_without_block();

                return *this;
            }

            // no move constructors as not supported in c++03
            /*ExpressionStatementWithoutBlock(ExpressionStatementWithoutBlock&& other) = default;
            ExpressionStatementWithoutBlock& operator=(ExpressionStatementWithoutBlock&& other) =
            default;*/
        };

        // Statement containing an expression with a block
        class ExpressionStatementWithBlock : public ExpressionStatement {
            // ExprWithBlock* expr;
            ::gnu::unique_ptr<ExprWithBlock> expr;

          public:
            /*~ExpressionStatementWithBlock() {
                delete expr;
            }*/

            ::std::string as_string() const;

            ExpressionStatementWithBlock(ExprWithBlock* expr) : expr(expr) {}

            // Copy constructor with clone
            ExpressionStatementWithBlock(ExpressionStatementWithBlock const& other) :
              expr(other.expr->clone_expr_with_block()) {}

            // Destructor - define here if required

            // Overloaded assignment operator to clone
            ExpressionStatementWithBlock& operator=(ExpressionStatementWithBlock const& other) {
                expr = other.expr->clone_expr_with_block();

                return *this;
            }

            // no move constructors as not supported in c++03
            /*ExpressionStatementWithBlock(ExpressionStatementWithBlock&& other) = default;
            ExpressionStatementWithBlock& operator=(ExpressionStatementWithBlock&& other) = default;*/
        };

        // Replaced definition of MacroInvocationSemi with forward decl - defined in rust-macro.h
        class MacroInvocationSemi;
        /*class MacroInvocationSemi : public Statement {
          public:
            ::std::string as_string() const;
        };*/
    }
}

#endif