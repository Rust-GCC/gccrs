#ifndef RUST_TOKEN_H
#define RUST_TOKEN_H

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "input.h"
// order: config, system, coretypes, input

#include <string>
#include <tr1/memory> // as shared_ptr is not available in std memory in c++03

namespace Rust {
// RS_TOKEN(name, description)
// RS_TOKEN_KEYWORD(name, identifier)
//
// Keep RS_TOKEN_KEYWORD sorted

// note that abstract, async, become, box, do, final, macro, override, priv, try, typeof, unsized,
// virtual, and yield are unused
// TODO finish converting to rust keywords
#define RS_TOKEN_LIST                                                                         \
    RS_TOKEN(FIRST_TOKEN, "<first-token-marker>")                                             \
    RS_TOKEN(END_OF_FILE, "end of file")                                                      \
    RS_TOKEN(EXCLAM, "!")                                                                     \
    RS_TOKEN(NOT_EQUAL, "!=")                                                                 \
    RS_TOKEN(PERCENT, "%")                                                                    \
    RS_TOKEN(PERCENT_EQ, "%=")                                                                \
    RS_TOKEN(AMP, "&")                                                                        \
    RS_TOKEN(AMP_EQ, "&=")                                                                    \
    RS_TOKEN(LOGICAL_AND, "&&")                                                               \
    RS_TOKEN(ASTERISK, "*")                                                                   \
    RS_TOKEN(ASTERISK_EQ, "*=")                                                               \
    RS_TOKEN(PLUS, "+")                                                                       \
    RS_TOKEN(PLUS_EQ, "+=")                                                                   \
    RS_TOKEN(COMMA, ",")                                                                      \
    RS_TOKEN(MINUS, "-")                                                                      \
    RS_TOKEN(MINUS_EQ, "-=")                                                                  \
    RS_TOKEN(RETURN_TYPE, "->")                                                               \
    RS_TOKEN(DOT, ".")                                                                        \
    RS_TOKEN(DOT_DOT, "..")                                                                   \
    RS_TOKEN(DOT_DOT_EQ, "..=")                                                               \
    RS_TOKEN(ELLIPSIS, "...")                                                                 \
    RS_TOKEN(DIV, "/")                                                                        \
    RS_TOKEN(DIV_EQ, "/=")                                                                    \
    RS_TOKEN(COLON, ":")                                                                      \
    RS_TOKEN(SEMICOLON, ";")                                                                  \
    RS_TOKEN(LEFT_SHIFT, "<<")                                                                \
    RS_TOKEN(LEFT_SHIFT_EQ, "<<=")                                                            \
    RS_TOKEN(LEFT_ANGLE, "<")                                                                 \
    RS_TOKEN(LESS_OR_EQUAL, "<=")                                                             \
    RS_TOKEN(EQUAL, "=")                                                                      \
    RS_TOKEN(EQUAL_EQUAL, "==")                                                               \
    RS_TOKEN(MATCH_ARROW, "=>")                                                               \
    RS_TOKEN(RIGHT_ANGLE, ">")                                                                \
    RS_TOKEN(GREATER_OR_EQUAL, ">=")                                                          \
    RS_TOKEN(RIGHT_SHIFT, ">>")                                                               \
    RS_TOKEN(RIGHT_SHIFT_EQ, ">>=")                                                           \
    RS_TOKEN(PATTERN_BIND, "@")                                                               \
    RS_TOKEN(TILDE, "~")                                                                      \
    RS_TOKEN(BACKSLASH, "\\")                                                                 \
    RS_TOKEN(BACKTICK, "`")                                                                   \
    RS_TOKEN(CARET, "^")                                                                      \
    RS_TOKEN(CARET_EQ, "^=")                                                                  \
    RS_TOKEN(PIPE, "|")                                                                       \
    RS_TOKEN(PIPE_EQ, "|=")                                                                   \
    RS_TOKEN(OR, "||")                                                                        \
    RS_TOKEN(QUESTION_MARK, "?")                                                              \
    RS_TOKEN(HASH, "#")                                                                       \
    /* from here on, dodgy and may not be correct. not operators and may be symbols */        \
    /* RS_TOKEN(SPACE, " ") probably too dodgy */                                             \
    /* RS_TOKEN(NEWLINE, "\n")*/                                                              \
    RS_TOKEN(SCOPE_RESOLUTION, "::") /* dodgy */                                              \
    RS_TOKEN(SINGLE_QUOTE, "'")      /* should i differentiate from lifetime? */              \
    RS_TOKEN(DOUBLE_QUOTE, "\"")                                                              \
    RS_TOKEN(UNDERSCORE, "_") /* TODO: treat as reserved word like mrustc instead? */         \
    RS_TOKEN(IDENTIFIER, "identifier")                                                        \
    RS_TOKEN(INT_LITERAL,                                                                     \
      "integer literal") /* do different int and float types need different literal types? */ \
    RS_TOKEN(FLOAT_LITERAL, "float literal")                                                  \
    RS_TOKEN(STRING_LITERAL, "string literal") /* maybe need char and bytestring literals? */ \
    /* Have lifetime token? mrustc has one. */                                                \
    /* Have "interpolated" tokens (whatever that means)? identifer, path, type, pattern, */   \
    /* expression, statement, block, meta, item in mrustc. */                                 \
    RS_TOKEN(LEFT_PAREN, "(")                                                                 \
    RS_TOKEN(RIGHT_PAREN, ")")                                                                \
    RS_TOKEN(LEFT_CURLY, "{")                                                                 \
    RS_TOKEN(RIGHT_CURLY, "}")                                                                \
    RS_TOKEN(LEFT_SQUARE, "[")                                                                \
    RS_TOKEN(RIGHT_SQUARE, "]")                                                               \
    /* Macros */                                                                              \
    RS_TOKEN(DOLLAR_SIGN, "$")                                                                \
    /* Comments */                                                                            \
    RS_TOKEN(LINE_COMMENT, "//")                                                              \
    RS_TOKEN(INNER_LINE_DOC, "//!")                                                           \
    RS_TOKEN(OUTER_LINE_DOC, "///")                                                           \
    RS_TOKEN(BLOCK_COMMENT_START, "/*")                                                       \
    RS_TOKEN(BLOCK_COMMENT_END, "*/")                                                         \
    RS_TOKEN(INNER_BLOCK_DOC_START, "/*!")                                                    \
    RS_TOKEN(OUTER_BLOCK_DOC_START, "/**")                                                    \
                                                                                              \
    RS_TOKEN_KEYWORD(ABSTRACT, "abstract") /* unused */                                       \
    RS_TOKEN_KEYWORD(AS, "as")                                                                \
    RS_TOKEN_KEYWORD(ASYNC, "async")   /* unused */                                           \
    RS_TOKEN_KEYWORD(BECOME, "become") /* unused */                                           \
    RS_TOKEN_KEYWORD(BOX, "box")       /* unused */                                           \
    RS_TOKEN_KEYWORD(BREAK, "break")                                                          \
    RS_TOKEN_KEYWORD(CONST, "const")                                                          \
    RS_TOKEN_KEYWORD(CONTINUE, "continue")                                                    \
    RS_TOKEN_KEYWORD(CRATE, "crate")                                                          \
    RS_TOKEN_KEYWORD(DO, "do") /* unused */                                                   \
    RS_TOKEN_KEYWORD(DYN, "dyn")                                                              \
    RS_TOKEN_KEYWORD(ELSE, "else")                                                            \
    RS_TOKEN_KEYWORD(ENUM, "enum")                                                            \
    RS_TOKEN_KEYWORD(EXTERN, "extern")                                                        \
    RS_TOKEN_KEYWORD(FALSE_LITERAL, "false")                                                  \
    RS_TOKEN_KEYWORD(FINAL_TOK, "final") /* unused */                                         \
    RS_TOKEN_KEYWORD(FN, "fn")                                                                \
    RS_TOKEN_KEYWORD(FOR, "for")                                                              \
    RS_TOKEN_KEYWORD(IF, "if")                                                                \
    RS_TOKEN_KEYWORD(IMPL, "impl")                                                            \
    RS_TOKEN_KEYWORD(IN, "in")                                                                \
    RS_TOKEN_KEYWORD(LET, "let")                                                              \
    RS_TOKEN_KEYWORD(LOOP, "loop")                                                            \
    RS_TOKEN_KEYWORD(MACRO, "macro") /* unused */                                             \
    RS_TOKEN_KEYWORD(MATCH, "match")                                                          \
    RS_TOKEN_KEYWORD(MOD, "mod")                                                              \
    RS_TOKEN_KEYWORD(MOVE, "move")                                                            \
    RS_TOKEN_KEYWORD(MUT, "mut")                                                              \
    RS_TOKEN_KEYWORD(OVERRIDE_TOK, "override") /* unused */                                   \
    RS_TOKEN_KEYWORD(PRIV, "priv")             /* unused */                                   \
    RS_TOKEN_KEYWORD(PUB, "pub")                                                              \
    RS_TOKEN_KEYWORD(REF, "ref")                                                              \
    RS_TOKEN_KEYWORD(RETURN, "return")                                                        \
    RS_TOKEN_KEYWORD(SELF_ALIAS, "Self") /* mrustc does not treat this as a reserved word*/   \
    RS_TOKEN_KEYWORD(SELF, "self")                                                            \
    RS_TOKEN_KEYWORD(STATIC, "static")                                                        \
    RS_TOKEN_KEYWORD(STRUCT, "struct")                                                        \
    RS_TOKEN_KEYWORD(SUPER, "super")                                                          \
    RS_TOKEN_KEYWORD(TRAIT, "trait")                                                          \
    RS_TOKEN_KEYWORD(TRUE_LITERAL, "true")                                                    \
    RS_TOKEN_KEYWORD(TRY, "try") /* unused */                                                 \
    RS_TOKEN_KEYWORD(TYPE, "type")                                                            \
    RS_TOKEN_KEYWORD(TYPEOF, "typeof") /* unused */                                           \
    RS_TOKEN_KEYWORD(UNSAFE, "unsafe")                                                        \
    RS_TOKEN_KEYWORD(UNSIZED, "unsized") /* unused */                                         \
    RS_TOKEN_KEYWORD(USE, "use")                                                              \
    RS_TOKEN_KEYWORD(VIRTUAL, "virtual") /* unused */                                         \
    RS_TOKEN_KEYWORD(WHERE, "where")                                                          \
    RS_TOKEN_KEYWORD(WHILE, "while")                                                          \
    RS_TOKEN_KEYWORD(YIELD, "yield") /* unused */                                             \
                                                                                              \
    RS_TOKEN(LAST_TOKEN, "<last-token-marker>")

    // Contains all token types. Crappy implementation via x-macros.
    enum TokenId {
#define RS_TOKEN(name, _) name,
#define RS_TOKEN_KEYWORD(x, y) RS_TOKEN(x, y)
        RS_TOKEN_LIST
#undef RS_TOKEN_KEYWORD
#undef RS_TOKEN
    };

    // dodgy "TokenPtr" declaration with Token forward declaration
    class Token;
    // A smart pointer (shared_ptr) to Token.
    typedef std::tr1::shared_ptr<Token> TokenPtr;
    // A smart pointer (shared_ptr) to a constant Token.
    typedef std::tr1::shared_ptr<const Token> const_TokenPtr;

    // Hackily defined way to get token description for enum value using x-macros
    const char* get_token_description(TokenId id);
    // Hackily defined way to get token description as a string for enum value using x-macros
    const char* token_id_to_str(TokenId id);

    // Represents a single token. Create using factory static methods.
    class Token {
      private:
        // Token kind.
        TokenId token_id;
        // Token location.
        location_t locus;
        // Associated text (if any) of token.
        std::string* str;

        // Token constructor from token id and location. Has a null string.
        Token(TokenId token_id, location_t location) :
          token_id(token_id), locus(location), str(NULL) {}

        // Token constructor from token id, location, and a string.
        Token(TokenId token_id, location_t location, const std::string& paramStr) :
          token_id(token_id), locus(location), str(new std::string(paramStr)) {}

        // No default initialiser.
        Token();
        // Do not copy/assign tokens.
        Token(const Token&);
        Token& operator=(const Token&);

      public:
        ~Token() {
            delete str;
        }

        // TODO: need quick creation for char literal and byte string if tokens?

        // Makes and returns a new TokenPtr (with null string).
        static TokenPtr make(TokenId token_id, location_t locus) {
            return TokenPtr(new Token(token_id, locus));
        }

        // Makes and returns a new TokenPtr of type IDENTIFIER.
        static TokenPtr make_identifier(location_t locus, const std::string& str) {
            return TokenPtr(new Token(IDENTIFIER, locus, str));
        }

        // Makes and returns a new TokenPtr of type INT_LITERAL.
        static TokenPtr make_int(location_t locus, const std::string& str) {
            return TokenPtr(new Token(INT_LITERAL, locus, str));
        }

        // Makes and returns a new TokenPtr of type FLOAT_LITERAL.
        static TokenPtr make_float(location_t locus, const std::string& str) {
            return TokenPtr(new Token(FLOAT_LITERAL, locus, str));
        }

        // Makes and returns a new TokenPtr of type STRING_LITERAL.
        static TokenPtr make_string(location_t locus, const std::string& str) {
            return TokenPtr(new Token(STRING_LITERAL, locus, str));
        }

        // Makes and returns a new TokenPtr of type BYTE_CHAR (fix).
        static TokenPtr make_byte_char(location_t locus, char byte_char) {
            return TokenPtr(new Token(BYTE_CHAR_LITERAL, locus, byte_char));
        }

        // Makes and returns a new TokenPtr of type STRING_LITERAL.
        static TokenPtr make_byte_string(location_t locus, const std::string& str) {
            return TokenPtr(new Token(BYTE_STRING_LITERAL, locus, str));
        }

        // Gets id of the token.
        TokenId get_id() const {
            return token_id;
        }

        // Gets location of the token.
        location_t get_locus() const {
            return locus;
        }

        // Gets string description of the token.
        const std::string& get_str() const {
            gcc_assert(str != NULL);
            return *str;
        }

        // diagnostics
        const char* get_token_description() const {
            return Rust::get_token_description(token_id);
        }

        // debugging
        const char* token_id_to_str() const {
            return Rust::token_id_to_str(token_id);
        }
    };
}

#endif