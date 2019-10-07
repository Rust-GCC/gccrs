#ifndef RUST_TYPES_STATEMENT_H
#define RUST_TYPES_STATEMENT_H

#include "rust-types.h"
#include "rust-types-expr.h"

namespace Rust {
    namespace AST {
        /* Base statement abstract class. Note that statements in Rust are only called as such if they
         * are inside a block inside a function or outer expression, i.e. standalone, module-scope
         * declarations are not statements. */
        struct Statement : public Node {};

        // Just a semi-colon, which apparently is a statement.
        struct EmptyStatement : public Statement {};

        /* This is syntactically identical to declaring an item inside a module BUT it has block scope.
         * Type of "declaration statement" as it introduces new name into scope */
        struct ItemStatement : public Statement {
            // TODO: put in same params as regular item
            // maybe even merge data structure with module item?
        };

        // Variable assignment let statement - type of "declaration statement" as it introduces new name into scope
        struct LetStatement : public Statement {
            //bool has_outer_attrs;
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
        };

        // Abstract base class for expression statements
        struct ExpressionStatement : public Statement {};

        struct ExpressionStatementWithoutBlock : public ExpressionStatement {
            ExprWithoutBlock* expr;

            ~ExpressionStatementWithoutBlock() {
                delete expr;
            }
        };

        struct ExpressionStatementWithBlock : public ExpressionStatement {
            ExprWithBlock* expr;

            ~ExpressionStatementWithBlock() {
                delete expr;
            }
        };

        // Is this the same MacroInvocationSemi as the item one? Probably.
        struct MacroInvocationSemi : public Statement {};
    }
}

#endif