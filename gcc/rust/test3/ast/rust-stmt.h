#ifndef RUST_AST_STATEMENT_H
#define RUST_AST_STATEMENT_H

#include "rust-ast.h"
#include "rust-expr.h"

namespace Rust {
    namespace AST {
        /* Base statement abstract class. Note that statements in Rust are only called as such if they
         * are inside a block inside a function or outer expression, i.e. standalone, module-scope
         * declarations are not statements. */
        class Statement : public Node {};

        // Just a semi-colon, which apparently is a statement.
        class EmptyStatement : public Statement {
          public:
            ::std::string as_string() const {
                return ::std::string(1, ';');
            }
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

            Pattern variables_pattern;

            bool has_type;
            Type type;

            bool has_init_expr;
            Expr* init_expr;

          public:
            bool has_outer_attrs() const {
                return !outer_attrs.empty();
            }

            ~LetStatement() {
                if (has_init_expr) {
                    delete init_expr;
                }
            }

            ::std::string as_string() const;
        };

        // Abstract base class for expression statements (statements containing an expression)
        class ExpressionStatement : public Statement {};

        // Statement containing an expression without a block
        class ExpressionStatementWithoutBlock : public ExpressionStatement {
            ExprWithoutBlock* expr;

          public:
            ~ExpressionStatementWithoutBlock() {
                delete expr;
            }

            ::std::string as_string() const;
        };

        // Statement containing an expression with a block
        class ExpressionStatementWithBlock : public ExpressionStatement {
            ExprWithBlock* expr;

          public:
            ~ExpressionStatementWithBlock() {
                delete expr;
            }

            ::std::string as_string() const;
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