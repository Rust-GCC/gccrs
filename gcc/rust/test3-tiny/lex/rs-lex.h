#ifndef RS_LEX_H
#define RS_LEX_H

#include "rs-buffered-queue.h"
#include "rs-token.h"

namespace Rust {
    class Lexer {
      private:
        // Request new location_t for current column in line_table
        location_t get_current_location();

        // Skips the current input char.
        void skip_input();
        // Advances current input char to n + 1 chars ahead of current position.
        void skip_input(int n);

        // Returns char n chars ahead of current position.
        int peek_input();
        // Peeks the current char.
        int peek_input(int n);

        // Classifies keyword (i.e. gets id for keyword).
        TokenId classify_keyword(const std::string& str);

        // Builds a token from the input queue.
        TokenPtr build_token();

      public:
        // Construct lexer with input file and filename porovided
        Lexer(const char* filename, FILE* input);
        ~Lexer();

        // Returns token n tokens ahead of current position.
        const_TokenPtr peek_token(int n);
        // Peeks the current token.
        const_TokenPtr peek_token();

        // Advances current token to n + 1 tokens ahead of current position.
        void skip_token(int n);
        // Skips the current token.
        void skip_token();

      private:
        // File for use as input.
        FILE* input;

        // Current line number.
        int current_line;
        // Current column number.
        int current_column;
        // Line map.
        const struct line_map* line_map;

        static const int max_column_hint = 80;

        // Input source wrapper thing.
        struct InputSource {
            FILE* input;

            InputSource(FILE* input) : input(input) {}

            // overload operator () to return next char from input stream
            int operator()() {
                return fgetc(input);
            }
        };

        // The input source for the lexer.
        InputSource input_source;
        // Input file queue.
        buffered_queue<int, InputSource> input_queue;

        // Token source wrapper thing.
        struct TokenSource {
            Lexer* lexer;
            TokenSource(Lexer* parLexer) : lexer(parLexer) {}

            // overload operator () to build token in lexer
            TokenPtr operator()() {
                return lexer->build_token();
            }
        };

        // The token source for the lexer.
        TokenSource token_source;
        // Token stream queue.
        buffered_queue<std::tr1::shared_ptr<Token>, TokenSource> token_queue;
    };
}

#endif