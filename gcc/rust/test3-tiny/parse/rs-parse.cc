#include "rs-parse.h"

/* parsing notes: 
 *  kinds of "syntactic units" used:
 *  - statement: expresses an action to be carried out (executed), e.g.:
 *      function calls
 *      goto 
 *      return 
 *      (maybe) variable definition
 *      blocks (apparently) - as in {}
 *      control structures - if, while, do-while, switch
 *     cannot return a result and are executed only for side effects
 *     distinction may not exist in rust entirely due to functional influence of lots of things being
 *      expressions
 * 
 *  - expression: stuff that evaluates to a value, e.g.:
 *      2 + 3
 *      y * 6 
 *  
 * - variable definition (maybe - if not a statement), e.g.:
 *      y = x + 2 */

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
    void Parser::unexpected_token(const_TokenPtr t) {
        error_at(t->get_locus(), "unexpected %s\n", t->get_token_description());
    }

    // Crappy "error recovery" performed after error by skipping tokens until a semi-colon is found
    void Parser::skip_after_semicolon() {
        const_TokenPtr t = lexer.peek_token();

        while (t->get_id() != END_OF_FILE && t->get_id() != SEMICOLON) {
            lexer.skip_token();
            t = lexer.peek_token();
        }

        if (t->get_id() == SEMICOLON)
            lexer.skip_token();
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

    /* Checks if current token has inputted id - skips it and returns true if so, diagnoses an error
     * and returns false otherwise. */
    bool Parser::skip_token(TokenId token_id) {
        return expect_token(token_id) != const_TokenPtr();
    }

    /* Checks the current token - if id is same as expected, skips and returns it, otherwise diagnoses
     * error and returns null. */
    const_TokenPtr Parser::expect_token(TokenId token_id) {
        const_TokenPtr t = lexer.peek_token();
        if (t->get_id() == token_id) {
            lexer.skip_token();
            return t;
        } else {
            error_at(t->get_locus(), "expecting %s but %s found!\n", get_token_description(token_id),
              t->get_token_description());

            return const_TokenPtr();
        }
    }

    // Parses type in variable declaration.
    bool Parser::parse_type() {
        const_TokenPtr t = lexer.peek_token();

        switch (t->get_id()) {
            case INT:
                lexer.skip_token();
                return true;
            case FLOAT:
                lexer.skip_token();
                return true;
            default:
                unexpected_token(t);
                return false;
        }
    }

    // Parses an if statement. Probably important to study as it seems complex.
    void Parser::parse_if_statement() {
        // skip if statement token
        if (!skip_token(IF)) {
            skip_after_end();
            return;
        }

        // parse expression in statement body
        parse_expression();

        // skip the "THEN" after expression
        skip_token(THEN);

        // parse statement sequence? inside if body. Finish at "end if" or "else"
        parse_statement_seq(&Parser::done_end_or_else);

        const_TokenPtr tok = lexer.peek_token();
        // if there is an else, parse statement seq inside its body too
        if (tok->get_id() == ELSE) {
            // Consume 'else'
            skip_token(ELSE);

            parse_statement_seq(&Parser::done_end);
            // Consume 'end'
            skip_token(END);
        } else if (tok->get_id() == END) {
            // Consume 'end'
            skip_token(END);
        } else {
            unexpected_token(tok);
            skip_after_end();
        }
    }

    // Skips all tokens until EOF.
    void Parser::skip_after_end() {
        const_TokenPtr t = lexer.peek_token();

        while (t->get_id() != END_OF_FILE && t->get_id() != END) {
            lexer.skip_token();
            t = lexer.peek_token();
        }

        if (t->get_id() == END) {
            lexer.skip_token();
        }
    }

    // Pratt parser impl of parse_expression.
    bool Parser::parse_expression(int right_binding_power) {
        const_TokenPtr current_token = lexer.peek_token();
        lexer.skip_token();

        if (!null_denotation(current_token))
            return false;

        while (right_binding_power < left_binding_power(lexer.peek_token())) {
            current_token = lexer.peek_token();
            lexer.skip_token();

            if (!left_denotation(current_token))
                return false;
        }

        return true;
    }

    // Entry point to parse an expression.
    bool Parser::parse_expression() {
        return parse_expression(LBP_LOWEST);
    }
}