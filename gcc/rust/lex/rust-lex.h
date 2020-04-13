#ifndef RUST_LEX_H
#define RUST_LEX_H

#include "rust-linemap.h"
#include "rust-buffered-queue.h"
#include "rust-token.h"

namespace Rust {
    class Lexer {
      private:
        // Request new Location for current column in line_table
        Location get_current_location();

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

        // ok maybe all these may mean the lexer structure needs to be rethought
        /* separated into functions because main method was too long, but they rely on and change
         * state in the lexer, so variables must be passed by reference. */
        inline void parse_in_decimal(/*char& current_char, */std::string& str, int& length);
        inline void parse_in_exponent_part(/*char& current_char, */std::string& str, int& length);
        inline bool parse_in_type_suffix(
          /*char& current_char, */PrimitiveCoreType& type_hint, int& length);
        inline bool parse_ascii_escape(/*char& current_char, */int& length, char& output_char);
        inline bool parse_quote_escape(/*char& current_char, */int& length, char& output_char);
        inline bool parse_unicode_escape(/*char& current_char, */int& length, Codepoint& output_char);
        inline bool parse_byte_escape(/*char& current_char, */int& length, char& output_char);
        inline bool parse_escape(int& length, char& output_char, char opening_char);
        inline bool parse_utf8_escape(int& length, Codepoint& output_char, char opening_char);
        inline int test_get_input_codepoint_length();
        inline int test_get_input_codepoint_n_length(int n_start_offset);
        inline Codepoint test_peek_codepoint_input();
        inline Codepoint test_peek_codepoint_input(int n); // maybe can use get_input_codepoint_length to get starting index
        inline void test_skip_codepoint_input();

      public:
        // Construct lexer with input file and filename provided
        Lexer(const char* filename, FILE* input, Linemap* linemap);
        ~Lexer();

        // Returns token n tokens ahead of current position.
        const_TokenPtr peek_token(int n);
        // Peeks the current token.
        const_TokenPtr peek_token();

        // Advances current token to n + 1 tokens ahead of current position.
        void skip_token(int n);
        // Skips the current token.
        void skip_token();

        // Replaces the current token with a specified token.
        void replace_current_token(TokenPtr replacement);

        Linemap* get_line_map() { return line_map; }

      private:
        // File for use as input.
        FILE* input;

        // Current line number.
        int current_line;
        // Current column number.
        int current_column;
        // Line map.
        Linemap* line_map;

        // Max column number that can be quickly allocated - higher may require allocating new linemap
        static const int max_column_hint = 80;

        // Input source wrapper thing.
        struct InputSource {
            // Input source file.
            FILE* input;

            // Create new input source from file.
            InputSource(FILE* input) : input(input) {}

            // Overload operator () to return next char from input stream.
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
            // The lexer object that will use this TokenSource.
            Lexer* lexer;

            // Create a new TokenSource with given lexer.
            TokenSource(Lexer* parLexer) : lexer(parLexer) {}

            // Overload operator () to build token in lexer.
            TokenPtr operator()() {
                return lexer->build_token();
            }
        };

        // The token source for the lexer.
        TokenSource token_source;
        // Token stream queue.
        buffered_queue<std::shared_ptr<Token>, TokenSource> token_queue;

        // START CRAPPY CHANGES
        int current_char;

        // END CRAPPY CHANGES
    };
}

#endif
