#ifndef RUST_PARSE_H
#define RUST_PARSE_H

#include "rust-lex.h"

namespace Rust {
    // Parser implementation for gccrs.
    class Parser {
      private:
        void skip_after_semicolon();
        void skip_after_end();

        bool skip_token(TokenId t);
        const_TokenPtr expect_token(TokenId t);
        void unexpected_token(const_TokenPtr t);

        // expression parsing
        // crap

        // etc.

        void parse_statement_seq(bool (Parser::*done)());

        bool done_end();
        bool done_end_or_else();
        bool done_end_of_file();

        // etc.

      public:
        // Construct parser with specified lexer reference.
        Parser(Lexer& parLexer) : lexer(parLexer) {}

        // Main entry point for parser.
        void parse_program();

      private:
        // The lexer associated with the parser.
        Lexer& lexer;

        typedef bool (Parser::*BinaryHandler)(const_TokenPtr);
        BinaryHandler get_binary_handler(TokenId id);

// x-macro list for binary operators
#define BINARY_HANDLER_LIST                         \
    BINARY_HANDLER(plus, PLUS)                      \
    BINARY_HANDLER(minus, MINUS)                    \
    BINARY_HANDLER(mult, ASTERISK)                  \
    BINARY_HANDLER(div, SLASH)                      \
    BINARY_HANDLER(mod, PERCENT)                    \
                                                    \
    BINARY_HANDLER(equal, EQUAL)                    \
    BINARY_HANDLER(different, DIFFERENT)            \
    BINARY_HANDLER(smaller_than, SMALLER)           \
    BINARY_HANDLER(smaller_equal, SMALLER_OR_EQUAL) \
    BINARY_HANDLER(greater_than, GREATER)           \
    BINARY_HANDLER(greater_equal, GREATER_OR_EQUAL) \
                                                    \
    BINARY_HANDLER(logical_and, AND)                \
    BINARY_HANDLER(logical_or, OR)

#define BINARY_HANDLER(name, _) bool binary_##name(const_TokenPtr tok);
        BINARY_HANDLER_LIST
#undef BINARY_HANDLER
    };
}

#endif // RUST_PARSE_H