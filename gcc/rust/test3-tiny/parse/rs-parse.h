#ifndef RS_PARSE_H
#define RS_PARSE_H

#include "rs-lex.h"

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
        //crap

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
    };
}

#endif