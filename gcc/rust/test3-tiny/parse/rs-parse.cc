#include "rs-parse.h"

namespace Rust {
    void Parser::parse_statement_seq(bool (Parser::*done)()) {
        // Parse statements until done and append to the current stmt list
        while (!(this->*done)()) {
            parse_statement();
        }
    }

    bool Parser::done_end_of_file() {
        const_TokenPtr t = lexer.peek_token();
        return (t->get_id() == END_OF_FILE);
    }

    void Parser::parse_program() {
        parse_statement_seq(&Parser::done_end_of_file);
    }

    // Parses a statement. Selects how to parse based on token id.
    void Parser::parse_statement() {
        // peek current token
        const_TokenPtr t = lexer.peek_token();

        // call method to parse statement if recognised
        switch (t->get_id()) {
            case VAR:
                parse_variable_declaration();
                break;
            case IF:
                parse_if_statement();
                break;
            case WHILE:
                parse_while_statement();
                break;
            case FOR:
                parse_for_statement();
                break;
            case READ:
                parse_read_statement();
                break;
            case WRITE:
                parse_write_statement();
                break;
            case IDENTIFIER:
                parse_assignment_statement();
                break;
            default:
                // if not recognised, error with unexpected token and attempt resume
                unexpected_token(t);
                skip_after_semicolon();
                break;
        }
    }

    // "Unexpected token" panic mode - flags gcc error at unexpected token
    // "Unexpected token" panic mode - flags gcc error at unexpected token
    void Parser::unexpected_token(const_TokenPtr t) {
        error_at(t->get_locus(), "unexpected %s\n", t->get_token_description());

        // Crappy "error recovery" performed after error by skipping tokens until a semi-colon is
        // found    }

        // Crappy "error recovery" performed after calling error by
        void Parser::skip_after_semicolon() {
            const_TokenPtr t = lexer.peek_token();

            while (t->get_id() != END_OF_FILE && t->get_id() != SEMICOLON) {
                lexer.skip_token();
                t = lexer.peek_token();
            }

            if (t->get_id() == SEMICOLON)

            // Parses a variable declaration statement.            lexer.skip_token();
        }

        // Parses a variable declaration statement.
        void Parser::parse_variable_declaration() {
            if (!skip_token(VAR)) {
                skip_after_semicolon();
                return;
            }

            const_TokenPtr identifier = expect_token(IDENTIFIER);
            if (identifier == NULL) {
                skip_after_semicolon();
                return;
            }

            if (!skip_token(COLON)) {
                skip_after_semicolon();
                return;
            }

            if (!parse_type()) {
                return;
            }

            skip_token(SEMICOLON);
        }

        /* Checks if current token has inputted id - skips it and returns true if so, diagnoses an
         * error and returns false otherwise. */
        bool Parser::skip_token(TokenId token_id) {
            return expect_token(token_id) != const_TokenPtr();
        }

        /* Checks the current token - if id is same as expected, skips and returns it, otherwise
         * diagnoses error and returns null. */
        const_TokenPtr Parser::expect_token(TokenId token_id) {
            const_TokenPtr t = lexer.peek_token();
            if (t->get_id() == token_id) {
                lexer.skip_token();
                return t;
            } else {
                error_at(t->get_locus(), "expecting %s but %s found!\n",
                  get_token_description(token_id), t->get_token_description());

                return const_TokenPtr();
            }
        }
    