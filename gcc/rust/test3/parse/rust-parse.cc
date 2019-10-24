#include "rust-parse.h"

#include "cgraph.h"
#include "config.h"
#include "convert.h"
#include "coretypes.h"
#include "diagnostic.h"
#include "fold-const.h"
#include "gimple-expr.h"
#include "gimplify.h"
#include "input.h"
#include "print-tree.h"
#include "stor-layout.h"
#include "stringpool.h"
#include "system.h"
#include "target.h"
#include "tree-iterator.h"
#include "tree.h"
/* order: config, system, coretypes, target, tree, tree-iterator, input, diagnostic, stringpool,
 * cgraph, gimplify, gimple-expr, convert, print-tree, stor-layout, fold-const  */
// probably don't need all these

#include <algorithm> // for std::find

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
    // Left binding powers of operations.
    enum binding_powers {
        // Highest priority
        LBP_HIGHEST = 100,

        LBP_PATH = 95,

        LBP_METHOD_CALL = 90,

        LBP_FIELD_EXPR = 85,

        // LBP_DOT = 80, /* method call and field expr have different precedence now */

        LBP_FUNCTION_CALL = 80,
        LBP_ARRAY_REF = LBP_FUNCTION_CALL,

        LBP_QUESTION_MARK = 75, // unary postfix - counts as left

        LBP_UNARY_PLUS = 70,                 // Used only when the null denotation is +
        LBP_UNARY_MINUS = LBP_UNARY_PLUS,    // Used only when the null denotation is -
        LBP_UNARY_ASTERISK = LBP_UNARY_PLUS, // deref operator - unary prefix
        LBP_UNARY_EXCLAM = LBP_UNARY_PLUS,
        LBP_UNARY_AMP = LBP_UNARY_PLUS,
        LBP_UNARY_AMP_MUT = LBP_UNARY_PLUS,

        LBP_AS = 65,

        LBP_MUL = 60,
        LBP_DIV = LBP_MUL,
        LBP_MOD = LBP_MUL,

        LBP_PLUS = 55,
        LBP_MINUS = LBP_PLUS,

        LBP_L_SHIFT = 50,
        LBP_R_SHIFT = LBP_L_SHIFT,

        LBP_AMP = 45,

        LBP_CARET = 40,

        LBP_PIPE = 35,

        LBP_EQUAL = 30,
        LBP_NOT_EQUAL = LBP_EQUAL,
        LBP_SMALLER_THAN = LBP_EQUAL,
        LBP_SMALLER_EQUAL = LBP_EQUAL,
        LBP_GREATER_THAN = LBP_EQUAL,
        LBP_GREATER_EQUAL = LBP_EQUAL,

        LBP_LOGICAL_AND = 25,

        LBP_LOGICAL_OR = 20,

        LBP_DOT_DOT = 15,
        LBP_DOT_DOT_EQ = LBP_DOT_DOT,

        // TODO: note all these assig operators are RIGHT associative!
        LBP_ASSIG = 10,
        LBP_PLUS_ASSIG = LBP_ASSIG,
        LBP_MINUS_ASSIG = LBP_ASSIG,
        LBP_MULT_ASSIG = LBP_ASSIG,
        LBP_DIV_ASSIG = LBP_ASSIG,
        LBP_MOD_ASSIG = LBP_ASSIG,
        LBP_AMP_ASSIG = LBP_ASSIG,
        LBP_PIPE_ASSIG = LBP_ASSIG,
        LBP_CARET_ASSIG = LBP_ASSIG,
        LBP_L_SHIFT_ASSIG = LBP_ASSIG,
        LBP_R_SHIFT_ASSIG = LBP_ASSIG,

        // return, break, and closures as lowest priority?
        // LBP_RETURN = 5,
        // LBP_BREAK = LBP_RETURN,
        // LBP_CLOSURE = LBP_RETURN,

        // lowest priority
        LBP_LOWEST = 0
    };

    // Checks if Tree has a string type (tree code pointer_type and tree variant char node).
    bool is_string_type(Tree type) {
        // assert node represents a type
        gcc_assert(TYPE_P(type.get_tree()));

        // ensure main variant of pointee is char_type_node (i.e. type is char*)
        return type.get_tree_code() == POINTER_TYPE
               && TYPE_MAIN_VARIANT(TREE_TYPE(type.get_tree())) == char_type_node;
    }

    // Checks if Tree has array type.
    bool is_array_type(Tree type) {
        gcc_assert(TYPE_P(type.get_tree()));
        return type.get_tree_code() == ARRAY_TYPE;
    }

    // Checks if Tree has record type.
    bool is_record_type(Tree type) {
        gcc_assert(TYPE_P(type.get_tree()));
        return type.get_tree_code() == RECORD_TYPE;
    }

    // Gets left binding power for specified token.
    int Parser::left_binding_power(const_TokenPtr token) {
        switch (token->get_id()) {
            /* TODO: issue here - distinguish between method calls and field access somehow?
                Also would have to distinguish between paths and function calls (:: operator),
                maybe more stuff. */

            // TODO: handle operator overloading - have a function replace the operator?

            /*case DOT:
                return LBP_DOT;*/

            /* TODO: BIG ISSUE - scope resolution can be for "path" or "function call", which have
             * different precedences (and also relative precedences - method and field are between)*/
            case SCOPE_RESOLUTION:
                return LBP_PATH;

            /* TODO: BIG ISSUE - dot can be for "method call" or "field access", which have
             * different precedences (though at least they have none between) */
            case DOT:
                return LBP_METHOD_CALL;

            case LEFT_SQUARE:
                return LBP_ARRAY_REF;

            // postfix question mark (i.e. error propagation expression)
            case QUESTION_MARK:
                return LBP_QUESTION_MARK;

            case AS:
                return LBP_AS;

            case ASTERISK:
                return LBP_MUL;
            case DIV:
                return LBP_DIV;
            case PERCENT:
                return LBP_MOD;

            case PLUS:
                return LBP_PLUS;
            case MINUS:
                return LBP_MINUS;

            case LEFT_SHIFT:
                return LBP_L_SHIFT;
            case RIGHT_SHIFT:
                return LBP_R_SHIFT;

            // binary & operator
            case AMP:
                return LBP_AMP;

            // binary ^ operator
            case CARET:
                return LBP_CARET;

            // binary | operator
            case PIPE:
                return LBP_PIPE;

            case EQUAL_EQUAL:
                return LBP_EQUAL;
            case NOT_EQUAL:
                return LBP_NOT_EQUAL;
            case RIGHT_ANGLE:
                return LBP_GREATER_THAN;
            case GREATER_OR_EQUAL:
                return LBP_GREATER_EQUAL;
            case LEFT_ANGLE:
                return LBP_SMALLER_THAN;
            case LESS_OR_EQUAL:
                return LBP_SMALLER_EQUAL;

            case LOGICAL_AND:
                return LBP_LOGICAL_AND;

            case OR:
                return LBP_LOGICAL_OR;

            case DOT_DOT:
                return LBP_DOT_DOT;

            case DOT_DOT_EQ:
                return LBP_DOT_DOT_EQ;

            case EQUAL:
                return LBP_ASSIG;
            case PLUS_EQ:
                return LBP_PLUS_ASSIG;
            case MINUS_EQ:
                return LBP_MINUS_ASSIG;
            case ASTERISK_EQ:
                return LBP_MULT_ASSIG;
            case DIV_EQ:
                return LBP_DIV_ASSIG;
            case PERCENT_EQ:
                return LBP_MOD_ASSIG;
            case AMP_EQ:
                return LBP_AMP_ASSIG;
            case CARET_EQ:
                return LBP_CARET_ASSIG;
            case LEFT_SHIFT_EQ:
                return LBP_L_SHIFT_ASSIG;
            case RIGHT_SHIFT_EQ:
                return LBP_R_SHIFT_ASSIG;

            // anything that can't appear in an infix position is given lowest priority
            default:
                return LBP_LOWEST;
        }
    }

    TreeStmtList& Parser::get_current_stmt_list() {
        return stack_stmt_list.back();
    }

    // Parse statements until done (EOF) and append to current stmt list.
    void Parser::parse_statement_seq(bool (Parser::*done)()) {
        // Parse statements until done and append to the current stmt list
        while (!(this->*done)()) {
            // get stmt tree for parsed statement
            Tree stmt = parse_statement();
            // append each stmt tree to current stmt list
            get_current_stmt_list().append(stmt);
        }
    }

    // Parse "items" until done (EOF) and append to current something list. Seems to be method taken
    // rather than statements in rust.
    /*void Parser::parse_item_seq(bool (Parser::*done)()) {
        // Parse statements until done and append to the current stmt list
        // TODO: fix
        while (!(this->*done)()) {
            // get stmt tree for parsed statement
            Tree item = parse_item();
            // append each stmt tree to current stmt list
            get_current_stmt_list().append(stmt);
        }
    }*/

    // Returns true when current token is EOF.
    bool Parser::done_end_of_file() {
        const_TokenPtr t = lexer.peek_token();
        return (t->get_id() == END_OF_FILE);
    }

    // Entry point - parse entire program (in file) from here.
    void Parser::parse_program() {
        // should be only able to parse decls at this point (scope)?

        // TODO: convert to a crate-based approach? parse_crate()?
        parse_crate();

        // TODO: structural changes - strongly-typed AST instead of Trees?

        // TODO: how much of this is fake main function vs actually required for any function?
        // Built type of main "int (int, char**)"
        tree main_fndecl_type_param[]
          = { integer_type_node, build_pointer_type(build_pointer_type(char_type_node)) };
        tree main_fndecl_type
          = build_function_type_array(integer_type_node, 2, main_fndecl_type_param);
        // Create function declaration "int main(int, char**)"
        main_fndecl = build_fn_decl("main", main_fndecl_type);

        // Enter top-level scope.
        enter_scope();
        // program -> statement*
        parse_statement_seq(&Parser::done_end_of_file);
        // Append "return 0;"
        tree resdecl = build_decl(UNKNOWN_LOCATION, RESULT_DECL, NULL_TREE, integer_type_node);
        // create decl_context of resdecl for main function (local variable of main function)
        DECL_CONTEXT(resdecl) = main_fndecl;
        DECL_RESULT(main_fndecl) = resdecl;
        tree set_result = build2(INIT_EXPR, void_type_node, DECL_RESULT(main_fndecl),
          build_int_cst_type(integer_type_node, 0));
        tree return_stmt = build1(RETURN_EXPR, void_type_node, set_result);

        get_current_stmt_list().append(return_stmt);

        // Leave top-level scope, get its binding expression and its main block
        TreeSymbolMapping main_tree_scope = leave_scope();
        Tree main_block = main_tree_scope.block;

        // Finish/finalise main function
        BLOCK_SUPERCONTEXT(main_block.get_tree()) = main_fndecl;
        DECL_INITIAL(main_fndecl) = main_block.get_tree();
        DECL_SAVED_TREE(main_fndecl) = main_tree_scope.bind_expr.get_tree();

        // Main function is not external
        DECL_EXTERNAL(main_fndecl) = 0;
        // Preserve main function
        DECL_PRESERVE_P(main_fndecl) = 1;

        // Convert from GENERIC to GIMPLE
        gimplify_function_tree(main_fndecl);

        // Insert it into the graph (queue for compilation?)
        cgraph_node::finalize_function(main_fndecl, true);

        main_fndecl = NULL_TREE;
    }

    // Parses a crate (compilation unit) - entry point
    AST::Crate Parser::parse_crate() {
        /* TODO: determine if has utf8bom and shebang. Currently, they are eliminated by the lexing
         * phase.
         * Neither are useful for the compiler anyway, so maybe a better idea would be to eliminate
         * the has_utf8bom and has_shebang variables from the crate data structure. */
        bool has_utf8bom = false;
        bool has_shebang = false;

        // parse inner attributes
        ::std::vector<AST::Attribute> inner_attrs = parse_inner_attributes();

        // parse items
        ::std::vector< ::gnu::unique_ptr<AST::Item> > items = parse_items();

        AST::Crate crate(items, inner_attrs, has_utf8bom, has_shebang);

        return crate;
    }

    // Parse a contiguous block of inner attributes.
    ::std::vector<AST::Attribute> Parser::parse_inner_attributes() {
        ::std::vector<AST::Attribute> inner_attributes;

        while (lexer.peek_token()->get_id() == HASH) {
            AST::Attribute inner_attr = parse_inner_attribute();

            // Ensure only valid inner attributes are added to the inner_attributes list
            if (!inner_attr.is_empty()) {
                inner_attributes.push_back(inner_attr);
            } else {
                /* If no more valid inner attributes, break out of loop (only contiguous inner
                 * attributes parsed). */
                break;
            }
        }

        return inner_attributes;
    }

    // Parse a single inner attribute.
    AST::Attribute Parser::parse_inner_attribute() {
        if (lexer.peek_token()->get_id() != HASH)
            return AST::Attribute::create_empty();

        lexer.skip_token();

        if (lexer.peek_token()->get_id() != EXCLAM)
            return AST::Attribute::create_empty();

        lexer.skip_token();

        if (lexer.peek_token()->get_id() != LEFT_SQUARE)
            return AST::Attribute::create_empty();

        lexer.skip_token();

        AST::Attribute actual_attribute = parse_attribute_body();

        if (lexer.peek_token()->get_id() != RIGHT_SQUARE)
            return AST::Attribute::create_empty();

        lexer.skip_token();

        return actual_attribute;
    }

    // Parses the body of an attribute (inner or outer).
    AST::Attribute Parser::parse_attribute_body() {
        AST::SimplePath attr_path = parse_simple_path();
        // ensure path is valid to parse attribute input
        if (attr_path.is_empty()) {
            error_at(lexer.peek_token()->get_locus(), "empty simple path in attribute");

            // Skip past potential further info in attribute (i.e. attr_input)
            skip_after_end_attribute();
            return AST::Attribute::create_empty();
        }

        AST::AttrInput* attr_input = parse_attr_input();
        // AttrInput is allowed to be null, so no checks here

        return AST::Attribute(attr_path, attr_input);
    }

    // Parses a SimplePath AST node
    AST::SimplePath Parser::parse_simple_path() {
        bool has_opening_scope_resolution = false;

        // Checks for opening scope resolution (i.e. global scope fully-qualified path)
        if (lexer.peek_token()->get_id() == SCOPE_RESOLUTION) {
            has_opening_scope_resolution = true;
            lexer.skip_token();
        }

        // Parse single required simple path segment
        AST::SimplePathSegment segment = parse_simple_path_segment();

        ::std::vector<AST::SimplePathSegment> segments;

        // Return empty vector if first, actually required segment is an error
        if (segment.is_error()) {
            return segments;
        }

        segments.push_back(segment);

        // Parse all other simple path segments
        while (lexer.peek_token()->get_id() == SCOPE_RESOLUTION) {
            // Skip scope resolution operator
            lexer.skip_token();

            AST::SimplePathSegment new_segment = parse_simple_path_segment();

            // Return path as currently constructed if segment in error state.
            if (segment.is_error()) {
                return segments;
            }
            segments.push_back(new_segment);
        }

        return segments;
    }

    // Parses a single SimplePathSegment (does not handle the scope resolution operators)
    AST::SimplePathSegment Parser::parse_simple_path_segment() {
        const_TokenPtr t = lexer.peek_token();
        switch (t->get_id()) {
            case IDENTIFIER:
                lexer.skip_token();

                return AST::SimplePathSegment(t->get_str());
            case SUPER:
                lexer.skip_token();

                return AST::SimplePathSegment(::std::string("super"));
            case SELF:
                lexer.skip_token();

                return AST::SimplePathSegment(::std::string("self"));
            case CRATE:
                lexer.skip_token();

                return AST::SimplePathSegment(::std::string("crate"));
            case DOLLAR_SIGN:
                if (lexer.peek_token(1)->get_id() == CRATE) {
                    lexer.skip_token(1);

                    return AST::SimplePathSegment(::std::string("$crate"));
                }
            default:
                // do nothing but inactivates warning from gcc when compiling
                // could put the error_at thing here but fallthrough (from failing $crate condition)
                // isn't completely obvious if it is.
        }

        error_at(
          t->get_locus(), "invalid token '%s' in simple path segment", t->get_token_description());
        return AST::SimplePathSegment::create_error();
    }

    // Parses an AttrInput AST node (polymorphic, as AttrInput is abstract)
    AST::AttrInput* Parser::parse_attr_input() {
        const_TokenPtr t = lexer.peek_token();
        switch (t->get_id()) {
            case LEFT_PAREN:
            case LEFT_SQUARE:
            case LEFT_CURLY:
                // must be a delimited token tree, so parse that
                DelimTokenTree* input_tree = new DelimTokenTree(parse_delim_token_tree());

                // TODO: potential checks on DelimTokenTree before returning

                return input_tree;
            case EQUAL: {
                // = LiteralExpr
                skip_token();

                t = lexer.peek_token();

                // Ensure token is a "literal expression" (literally only a literal token of any type)
                if (!t->is_literal()) {
                    error_at(t->get_locus(),
                      "unknown token '%s' in attribute body - literal expected",
                      t->get_token_description());
                    skip_after_end_attribute();
                    return NULL;
                }

                AST::LiteralExpr::LitType lit_type = STRING;
                // Crappy mapping of token type to literal type
                switch (t->get_id()) {
                    case INT_LITERAL:
                        lit_type = INT;
                        break;
                    case FLOAT_LITERAL:
                        lit_type = FLOAT;
                        break;
                    case CHAR_LITERAL:
                        lit_type = CHAR;
                        break;
                    case BYTE_CHAR_LITERAL:
                        lit_type = BYTE_CHAR;
                        break;
                    case BYTE_STRING_LITERAL:
                        lit_type = BYTE_STRING;
                        break;
                    case STRING_LITERAL:
                    default:
                        lit_type = STRING;
                        break; // TODO: raw string? don't eliminate it from lexer?
                }

                // create actual LiteralExpr
                AST::LiteralExpr lit_expr(t->get_str(), lit_type);

                AST::AttrInputLiteral* attr_input_lit = new AST::AttrInputLiteral(lit_expr);

                // do checks or whatever? none required, really

                return attr_input_lit;
            } break;
            case RIGHT_SQUARE:
                // means AttrInput is missing, which is allowed
                return NULL;
            default:
                error_at(t->get_locus(),
                  "unknown token '%s' in attribute body - attribute input or none expected",
                  t->get_token_description());
                skip_after_end_attribute();
                return NULL;
        }
    }

    /* Returns true if the token id matches the delimiter type. Note that this only operates for
     * END delimiter tokens. */
    inline bool token_id_matches_delims(TokenId token_id, AST::DelimTokenTree::DelimType delim_type) {
        return ((token_id == RIGHT_PAREN && delim_type == PARENS)
                || (token_id == RIGHT_SQUARE && delim_type == SQUARE)
                || (token_id == RIGHT_CURLY && delim_type == CURLY));
    }

    // Parses a delimited token tree
    AST::DelimTokenTree Parser::parse_delim_token_tree() {
        const_TokenPtr t = lexer.peek_token();
        lexer.skip_token();

        // save delim type to ensure it is reused later
        AST::DelimTokenTree::DelimType delim_type = PARENS;

        // Map tokens to DelimType
        switch (t->get_id()) {
            case LEFT_PAREN:
                delim_type = PARENS;
                break;
            case LEFT_SQUARE:
                delim_type = SQUARE;
                break;
            case LEFT_CURLY:
                delim_type = CURLY;
                break;
            default:
                error_at(t->get_locus(),
                  "unexpected token '%s' - expecting delimiters (for a delimited token tree)",
                  t->get_token_description());
                return DelimTokenTree::create_empty();
        }

        t = lexer.peek_token();

        // parse actual token tree vector - 0 or more
        ::std::vector< ::gnu::unique_ptr<TokenTree> > token_trees_in_tree;

        // repeat loop until finding the matches delimiter
        while (!token_id_matches_delims(t->get_id(), delim_type)) {
            TokenTree* tok_tree = parse_token_tree();

            // may need attention in C++11 move
            token_trees_in_tree.push_back(::gnu::unique_ptr<TokenTree>(tok_tree));

            // lexer.skip_token();
            // t = lexer.peek_token();
        }

        // TODO: put in std::move for vector in constructor or here?
        AST::DelimTokenTree token_tree(delim_type, token_trees_in_tree);

        // parse end delimiters
        t = lexer.peek_token();

        if (token_id_matches_delims(t->get_id(), delim_type)) {
            // tokens match opening delimiter, so skip.
            lexer.skip_token();

            return token_tree;
        } else {
            // tokens don't match opening delimiters, so produce error
            error_at(t->get_locus(),
              "unexpected token '%s' - expecting closing delimiter '%s' (for a delimited token tree)",
              t->get_token_description(),
              (delim_type == PARENS ? ")" : (delim_type == SQUARE ? "]" : "}")));

            /* return empty token tree despite possibly parsing valid token tree - TODO is this a
             * good idea? */
            return DelimTokenTree::create_empty();
        }
    }

    /* Parses a TokenTree syntactical production. This is either a delimited token tree or a
     * non-delimiter token. */
    AST::TokenTree* Parser::parse_token_tree() {
        const_TokenPtr t = lexer.peek_token();

        switch (t->get_id()) {
            case LEFT_PAREN:
            case LEFT_SQUARE:
            case LEFT_CURLY:
                // Parse delimited token tree
                return new parse_delim_token_tree();
            case RIGHT_PAREN:
            case RIGHT_SQUARE:
            case RIGHT_CURLY:
                // error - should not be called when this a token
                error_at(t->get_locus(),
                  "unexpected closing delimiter '%s' - token tree requires either paired delimiters "
                  "or non-delimiter tokens",
                  t->get_token_description());
                lexer.skip_token();
                return NULL;
            default:
                // parse token itself as TokenTree
                lexer.skip_token();
                // TODO: fix that token constructor, possibly with c++11 features
                return new AST::Token(t);
        }
    }

    // Parses a sequence of items within a module or the implicit top-level module in a crate.
    ::std::vector< ::gnu::unique_ptr<AST::Item> > Parser::parse_items() {
        ::std::vector< ::gnu::unique_ptr<AST::Item> > items;

        // TODO: replace with do-while loop?
        // infinite loop to save on comparisons (may be a tight loop) - breaks when next item is null
        while (true) {
            AST::Item* item = parse_item();

            if (item != NULL) {
                items.push_back(::gnu::unique_ptr<AST::Item>(item));
            } else {
                break;
            }
        }

        return items;
    }

    // Parses a single item
    AST::Item* Parser::parse_item() {
        // parse outer attributes for item
        ::std::vector<Attribute> outer_attrs = parse_outer_attributes();

        // TODO: decide how to deal with VisItem vs MacroItem dichotomy
        // best current solution: catch all keywords that would imply a VisItem in a switch and have
        // MacroItem as a last resort

        const_TokenPtr t = lexer.peek_token();

        switch (t->get_id()) {
            case PUB:
            case MOD:
            case EXTERN_TOK:
            case USE:
            case FN_TOK:
            case TYPE:
            case STRUCT_TOK:
            case ENUM_TOK:
            case UNION: // TODO: implement union keyword but not really because of context-dependence
            case CONST:
            case STATIC_TOK:
            case TRAIT:
            case IMPL:
            case UNSAFE: // maybe - unsafe traits are a thing
                // if any of these (should be all possible VisItem prefixes), parse a VisItem
                return parse_vis_item();
                break;
            default:
                // otherwise parse a MacroItem
                return parse_macro_item();
                break;
        }
    }

    // Parses a contiguous block of outer attributes.
    ::std::vector<AST::Attribute> Parser::parse_outer_attributes() {
        ::std::vector<AST::Attribute> outer_attributes;

        while (lexer.peek_token()->get_id() == HASH) {
            AST::Attribute outer_attr = parse_outer_attribute();

            // Ensure only valid outer attributes are added to the outer_attributes list
            if (!outer_attr.is_empty()) {
                outer_attributes.push_back(outer_attr);
            } else {
                /* If no more valid outer attributes, break out of loop (only contiguous outer
                 * attributes parsed). */
                break;
            }
        }

        return outer_attributes;

        // TODO: this shares basically all code with parse_inner_attributes except function call
        // find way of making it more modular?
    }

    // Parse a single outer attribute.
    AST::Attribute Parser::parse_outer_attribute() {
        /* OuterAttribute -> '#' '[' Attr ']' */

        if (lexer.peek_token()->get_id() != HASH)
            return AST::Attribute::create_empty();

        lexer.skip_token();

        TokenId id = lexer.peek_token()->get_id();
        if (id != LEFT_SQUARE) {
            if (id == EXCLAM) {
                // this is inner attribute syntax, so throw error
                error_at(lexer.peek_token()->get_locus(),
                  "token '!' found, indicating inner attribute definition. Inner attributes are not "
                  "possible at this location.");
            } // TODO: are there any cases where this wouldn't be an error?
            return AST::Attribute::create_empty();
        }

        lexer.skip_token();

        AST::Attribute actual_attribute = parse_attribute_body();

        if (lexer.peek_token()->get_id() != RIGHT_SQUARE)
            return AST::Attribute::create_empty();

        lexer.skip_token();

        return actual_attribute;
    }

    // Parses a VisItem (item that can have non-default visibility).
    AST::VisItem* Parser::parse_vis_item() {
        // parse visibility, which may or may not exist
        AST::Visibility* vis = parse_visibility();

        // select VisItem to create depending on keyword
        const_TokenPtr t = lexer.peek_token();

        switch (t->get_id()) {
            case MOD:
                return parse_module(vis);
            case EXTERN_TOK: 
                // lookahead to resolve syntactical production
                t = lexer.peek_token(1);

                switch (t->get_id()) {
                    case CRATE:
                        return parse_extern_crate(vis);
                    case FN_TOK: // extern function
                        return parse_function(vis);
                    case LEFT_CURLY: // extern block
                        return parse_extern_block(vis);
                    case STRING_LITERAL: // for specifying extern ABI
                        // could be extern block or extern function, so more lookahead
                        t = lexer.peek_token(2);

                        switch (t->get_id()) {
                            case FN_TOK:
                                return parse_function(vis);
                            case LEFT_CURLY:
                                return parse_extern_block(vis);
                            default:
                                error_at(t->get_locus(),
                                  "unexpected token '%s' in some sort of extern production",
                                  t->get_token_description());
                                lexer.skip_token(2); // TODO: is this right thing to do?
                                return NULL;
                        }
                    default:
                        error_at(t->get_locus(),
                          "unexpected token '%s' in some sort of extern production",
                          t->get_token_description());
                        lexer.skip_token(1); // TODO: is this right thing to do?
                        return NULL;
                }
            case USE:
                return parse_use_decl(vis);
            case FN_TOK:
                return parse_function(vis);
            case TYPE:
                return parse_type_alias(vis);
            case STRUCT_TOK:
                return parse_struct(vis);
            case ENUM_TOK:
                return parse_enum(vis);
            case UNION: // TODO: implement union keyword but not really because of context-dependence
                return parse_union(vis);
            case CONST: 
                // lookahead to resolve syntactical production
                t = lexer.peek_token(1);

                switch (t->get_id()) {
                    case IDENTIFIER:
                    case UNDERSCORE:
                        return parse_const_item(vis);
                    case UNSAFE: 
                    case EXTERN_TOK:
                    case FN_TOK:
                        return parse_function(vis);
                    default:
                        error_at(t->get_locus(),
                          "unexpected token '%s' in some sort of const production",
                          t->get_token_description());
                        lexer.skip_token(1); // TODO: is this right thing to do?
                        return NULL;
                }
            case STATIC_TOK:
                return parse_static_item(vis);
            case TRAIT:
                return parse_trait(vis);
            case IMPL:
                return parse_impl(vis);
            case UNSAFE: // unsafe traits, unsafe functions, unsafe impls (trait impls),
                // lookahead to resolve syntactical production
                t = lexer.peek_token(1);

                switch (t->get_id()) {
                    case TRAIT:
                        return parse_trait(vis);
                    case EXTERN_TOK:
                    case FN_TOK:
                        return parse_function(vis);
                    case IMPL:
                        return parse_impl(vis);
                    default:
                        error_at(t->get_locus(),
                          "unexpected token '%s' in some sort of unsafe production",
                          t->get_token_description());
                        lexer.skip_token(1); // TODO: is this right thing to do?
                        return NULL;
                }
            default:
                // otherwise vis item clearly doesn't exist, which is not an error
                return NULL;
        }
    }

    // Parses a MacroItem (item that has something to do with macros).
    AST::MacroItem* Parser::parse_macro_item() {}

    // Parses a visibility syntactical production (i.e. creating a non-default visibility)
    AST::Visibility* Parser::parse_visibility() {
        // AST::Visibility vis;
        // return vis;
    }

    // TODO: rename to "parse_module_body"?
    /*AST::Module Parser::parse_module() {
        // const_TokenPtr t = lexer.peek_token();
        AST::Module module;

        while (true) {
            // check end of module
            switch (lexer.peek_token()->get_id()) {
                case RIGHT_CURLY:
                case END_OF_FILE:
                    return module;
                default:
                    break;
            }

            // parse item attributes here
            AST::AttributeList attrs;

            parse_module_item(module, attrs);
        }

        return module;
    }*/

    /*void Parser::parse_module_item(
      AST::Module module_for_items, AST::AttributeList item_outer_attrs) {
        AST::Visibility visibility = parse_visibility();

        switch (lexer.peek_token()->get_id()) {
            case MOD: {
                // TODO: function call to eventually move all this stuff into:
                // parse_module();
                // Note that this is not the current "parse_module" function.

                // skip "mod" token
                lexer.skip_token();

                // next token should be module name
                const_TokenPtr identifier = expect_token(IDENTIFIER);

                switch (lexer.peek_token()->get_id()) {
                    case SEMICOLON:
                        // parse module without body - from referenced file
                    case LEFT_CURLY:
                        // parse module with body
                    default:
                        error_at(identifier->get_locus(),
                          "invalid module definition syntax; unexpected token '%s'",
                          lexer.peek_token()->get_str().c_str());
                        skip_after_end();
                }
            } break;
            case USE:
                // TODO: parse "use" declaration
                break;
            case EXTERN_TOK:
                // TODO: parse "extern" statement - extern crate and extern block
                break;
            case FN_TOK:
                // TODO: parse function declaration
                break;
            case STRUCT_TOK:
                // TODO: parse function declaration
                break;
            case IMPL:
                // TODO: parse struct impl
                break;
            // TODO: type alias?
            case ENUM_TOK:
                // TODO: parse enum
                break;
            case TRAIT:
                // TODO: parse trait decl
                break;
                // TODO: parse "union"? This is the context-aware one, I think
                // TODO: parse "constant item"
                // TODO: parse "static item"
                // etc: add more - all module-level allowed constructs should be represented here
        }
    }*/

    // Parses a statement. Selects how to parse based on token id.
    Tree Parser::parse_statement() {
        /*
  statement ->  ;
          |  item
          |  let_statement
          |  expression_statement
          |  macro_invocation_semi
          */
        // peek current token
        const_TokenPtr t = lexer.peek_token();

        // call method to parse statement if recognised
        switch (t->get_id()) {
            // is item declaration only for nested functions?
            case FN_TOK:
                // TODO: fix - rust reference gives nested function as an example?
                // return parse_item_declaration();
                break;
            /*case VAR:
                return parse_variable_declaration();
                break;*/
            case LET:
                // return parse_let_statement();
                break;
            // parse expression statement somehow? any expression with ending semicolon
            /*case TYPE:
                return parse_type_declaration();
                break;*/
            /*case IF:
                return parse_if_statement();
                break;
            case WHILE:
                return parse_while_statement();
                break;
            case FOR:
                return parse_for_statement();
                break;*/
            /*case READ:
                return parse_read_statement();
                break;
            case WRITE:
                return parse_write_statement();
                break;*/
            /*case IDENTIFIER:
                return parse_assignment_statement();
                break;*/
            default:
                // if not recognised, error with unexpected token and attempt resume
                unexpected_token(t);
                skip_after_semicolon();
                return Tree::error();
                break;
        }
    }

    // "Unexpected token" panic mode - flags gcc error at unexpected token
    void Parser::unexpected_token(const_TokenPtr t) {
        ::error_at(t->get_locus(), "unexpected %s\n", t->get_token_description());
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

#if 0
    // Parses a "let" statement (variable declaration).
    Tree Parser::parse_let_statement() {
        /*
  let_statement: ->  outer_attribute* "let" pattern (":" type)? ("=" expression)? ";" */

        // TODO: parse "outer attribute"?
        // auto t = peek_token()
        // while (t.get_id() == LEFT_SQUARE) {
        //    OuterAttribute attributes = parse_outer_attribute();
        //}
        // etc.

        // ensure "let" token actually exists
        if (!skip_token(LET)) {
            skip_after_semicolon();
            return Tree::error();
        }

        
    }
#endif

    // Parses a variable declaration statement.
    Tree Parser::parse_variable_declaration() {
        // skip initial var keyword - TODO: fix
        if (!skip_token(/*VAR*/ COLON)) {
            skip_after_semicolon();
            return Tree::error();
        }

        // keep identifier token as used later
        const_TokenPtr identifier = expect_token(IDENTIFIER);
        if (identifier == NULL) {
            skip_after_semicolon();
            return Tree::error();
        }

        // skip colon
        if (!skip_token(COLON)) {
            skip_after_semicolon();
            return Tree::error();
        }

        // parse the actual type of the variable
        Tree type_tree = parse_type();

        if (type_tree.is_error()) {
            skip_after_semicolon();
            return Tree::error();
        }

        // skip the semicolon
        skip_token(SEMICOLON);

        // check if current mapping of the scope already contains a mapping for identifier
        if (scope.get_current_mapping().get(identifier->get_str())) {
            // create error if this is true
            error_at(identifier->get_locus(), "name %s already declared in this scope",
              identifier->get_str().c_str());
            return Tree::error();
        }

        // create a new symbol using the given identifier
        SymbolPtr sym(new Symbol(VARIABLE, identifier->get_str()));
        // put new symbol into scope mapping
        scope.get_current_mapping().insert(sym);

        // create GENERIC tree for variable declaration
        Tree decl = build_decl(identifier->get_locus(), VAR_DECL,
          get_identifier(sym->get_name().c_str()), type_tree.get_tree());
        // set decl_context of decl to main function (make it local variable of main function)
        DECL_CONTEXT(decl.get_tree()) = main_fndecl;

        // keep VAR_DECL tree in top list of stack_var_decl_chain stack for block purposes
        gcc_assert(!stack_var_decl_chain.empty());
        stack_var_decl_chain.back().append(decl);

        // associate new symbol with VAR_DECL tree
        sym->set_tree_decl(decl);

        Tree stmt = build_tree(DECL_EXPR, identifier->get_locus(), void_type_node, decl);

        return stmt;
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
    Tree Parser::parse_type() {
        const_TokenPtr t = lexer.peek_token();

        Tree type;

        switch (t->get_id()) {
            // TODO: fix
            case /*INT*/ COLON:
                lexer.skip_token();
                type = integer_type_node;
                break;
            case /*FLOAT*/ ELLIPSIS:
                lexer.skip_token();
                type = float_type_node;
                break;
            case /*BOOL*/ BREAK:
                lexer.skip_token();
                type = boolean_type_node;
                break;
            case IDENTIFIER: {
                SymbolPtr s = query_type(t->get_str(), t->get_locus());
                lexer.skip_token();

                if (s == NULL)
                    type = Tree::error();
                else {
                    type = TREE_TYPE(s->get_tree_decl().get_tree());
                }
            } break;
            case /*RECORD*/ DOT:
                type = parse_record();
                break;
            default:
                unexpected_token(t);
                return Tree::error();
                break;
        }

        // start parsing index ranges: list of expression pairs (lower and upper indexes of array)
        typedef std::vector<std::pair<Tree, Tree> > Dimensions;
        Dimensions dimensions;

        t = lexer.peek_token();
        while (t->get_id() == LEFT_PAREN || t->get_id() == LEFT_SQUARE) {
            lexer.skip_token();

            // array bounds
            Tree lower_bound, upper_bound;

            if (t->get_id() == LEFT_SQUARE) {
                // for array of form [e]
                Tree size = parse_integer_expression();
                skip_token(RIGHT_SQUARE);

                lower_bound = Tree(build_int_cst_type(integer_type_node, 0), size.get_locus());

                // set upper to e - 1
                upper_bound = build_tree(MINUS_EXPR, size.get_locus(), integer_type_node, size,
                  build_int_cst(integer_type_node, 1));
            } else if (t->get_id() == LEFT_PAREN) {
                // for array of form [e0:e1]
                // parse e0
                lower_bound = parse_integer_expression();
                skip_token(COLON);

                // parse e1
                upper_bound = parse_integer_expression();
                skip_token(RIGHT_PAREN);
            } else {
                gcc_unreachable();
            }

            dimensions.push_back(std::make_pair(lower_bound, upper_bound));
            t = lexer.peek_token();
        }

        // start building array type
        // transverse list in reverse order
        for (Dimensions::reverse_iterator it = dimensions.rbegin(); it != dimensions.rend(); it++) {
            // fold lower and upper expressions (simplify expressions if possible)
            it->first = Tree(fold(it->first.get_tree()), it->first.get_locus());
            it->second = Tree(fold(it->second.get_tree()), it->second.get_locus());

            if (!type.is_error()) {
                // build GCC range type using lower and upper
                Tree range_type
                  = build_range_type(integer_type_node, it->first.get_tree(), it->second.get_tree());
                // build array type
                type = build_array_type(type.get_tree(), range_type.get_tree());
            }
        }

        return type;
    }

    // Parses an if statement. Probably important to study as it seems complex.
    Tree Parser::parse_if_statement() {
        // skip if statement token
        if (!skip_token(IF)) {
            skip_after_end();
            return Tree::error();
        }

        // parse expression in statement body
        Tree expr = parse_boolean_expression();

        // skip the "THEN" after expression
        skip_token(LEFT_CURLY);

        // enter new block scope
        enter_scope();
        // parse statement sequence? inside if body. Finish at "end if" or "else"
        parse_statement_seq(&Parser::done_end_or_else);

        TreeSymbolMapping then_tree_scope = leave_scope();
        Tree then_stmt = then_tree_scope.bind_expr;

        Tree else_stmt;
        const_TokenPtr tok = lexer.peek_token();
        // if there is an else, parse statement seq inside its body too
        if (tok->get_id() == ELSE) {
            // Consume 'else'
            skip_token(ELSE);

            // enter block scope
            enter_scope();
            parse_statement_seq(&Parser::done_end);
            TreeSymbolMapping else_tree_scope = leave_scope();
            else_stmt = else_tree_scope.bind_expr;

            // Consume 'end'
            skip_token(RIGHT_CURLY);
        } else if (tok->get_id() == RIGHT_CURLY) {
            // Consume 'end'
            skip_token(RIGHT_CURLY);
        } else {
            unexpected_token(tok);
            return Tree::error();
        }

        // build GENERIC if statement node.
        return build_if_statement(expr, then_stmt, else_stmt);
    }

    // Builds an if statement tree.
    Tree Parser::build_if_statement(Tree bool_expr, Tree then_part, Tree else_part) {
        if (bool_expr.is_error())
            return bool_expr;

        // create then label declaration tree
        Tree then_label_decl = build_label_decl("then", then_part.get_locus());

        // create else label declaration if it exists
        Tree else_label_decl;
        if (!else_part.is_null())
            else_label_decl = build_label_decl("else", else_part.get_locus());

        // create endif label declaration
        Tree endif_label_decl = build_label_decl("end_if", then_part.get_locus());

        // create goto expressions for entering "if" branch, "else" branch, and code after if block
        Tree goto_then
          = build_tree(GOTO_EXPR, bool_expr.get_locus(), void_type_node, then_label_decl);
        Tree goto_endif
          = build_tree(GOTO_EXPR, bool_expr.get_locus(), void_type_node, endif_label_decl);

        Tree goto_else_or_endif;
        if (!else_part.is_null())
            goto_else_or_endif
              = build_tree(GOTO_EXPR, bool_expr.get_locus(), void_type_node, else_label_decl);
        else
            goto_else_or_endif = goto_endif;

        // create statement list for if statement which will have required statements appended
        TreeStmtList stmt_list;

        // create conditional branch expression and append to stmt_list
        Tree cond_expr = build_tree(
          COND_EXPR, bool_expr.get_locus(), void_type_node, bool_expr, goto_then, goto_else_or_endif);
        stmt_list.append(cond_expr);

        // define location related to "then" part and append to stmt_list
        Tree then_label_expr
          = build_tree(LABEL_EXPR, then_part.get_locus(), void_type_node, then_label_decl);
        stmt_list.append(then_label_expr);

        // append parameter "then_part" to statement list
        stmt_list.append(then_part);

        // if else part exists, append a goto endif
        if (!else_part.is_null()) {
            // Make sure after then part has been executed we go to the end if
            stmt_list.append(goto_endif);

            // define location of else label, append it, and append else_part parameter tree
            Tree else_label_expr
              = build_tree(LABEL_EXPR, else_part.get_locus(), void_type_node, else_label_decl);
            stmt_list.append(else_label_expr);
            stmt_list.append(else_part);
            // do not need to jump to endif as handled implicitly here
        }

        // define label for endif, append to statement list
        Tree endif_label_expr
          = build_tree(LABEL_EXPR, UNKNOWN_LOCATION, void_type_node, endif_label_decl);
        stmt_list.append(endif_label_expr);

        // return the statement list in tree form
        return stmt_list.get_tree();
    }

    // Builds a GENERIC tree LABEL_DECL (represents a label, as in a "goto" label).
    Tree Parser::build_label_decl(const char* name, location_t loc) {
        tree t = build_decl(loc, LABEL_DECL, get_identifier(name), void_type_node);

        gcc_assert(main_fndecl != NULL_TREE);
        DECL_CONTEXT(t) = main_fndecl;

        return t;
    }

    // Skips all tokens until EOF or }.
    void Parser::skip_after_end() {
        const_TokenPtr t = lexer.peek_token();

        while (t->get_id() != END_OF_FILE && t->get_id() != RIGHT_CURLY) {
            lexer.skip_token();
            t = lexer.peek_token();
        }

        if (t->get_id() == RIGHT_CURLY) {
            lexer.skip_token();
        }
    }

    /* A slightly more aware error-handler that skips all tokens until it reaches the end of the
     * block scope (i.e. when left curly brackets = right curly brackets).  */
    void Parser::skip_after_end_block() {
        const_TokenPtr t = lexer.peek_token();
        int curly_count = 0;

        // initial loop before any curly braces
        while (t->get_id() != END_OF_FILE && t->get_id() != LEFT_CURLY) {
            lexer.skip_token();
            t = lexer.peek_token();
        }

        // start curly_count thing if curly braces found
        if (t->get_id() == LEFT_CURLY) {
            lexer.skip_token();
            curly_count++;
        }

        // repeat until curly_count = 0
        while (t->get_id() != END_OF_FILE && /*t->get_id() != RIGHT_CURLY*/ curly_count > 0) {
            if (t->get_id() == LEFT_CURLY)
                curly_count++;
            if (t->get_id() == RIGHT_CURLY)
                curly_count--;

            lexer.skip_token();
            t = lexer.peek_token();
        }

        /*if (t->get_id() == RIGHT_CURLY) {
            lexer.skip_token();
        }*/
    }

    // Skips all tokens until ] (the end of an attribute) - does not skip the ] (as designed for
    // attribute body use)
    void Parser::skip_after_end_attribute() {
        const_TokenPtr t = lexer.peek_token();

        while (t->get_id() != RIGHT_SQUARE) {
            lexer.skip_token();
            t = lexer.peek_token();
        }

        // Don't skip the RIGHT_SQUARE token
        /*if (t->get_id() == RIGHT_SQUARE) {
            lexer.skip_token();
        }*/
    }

    // Pratt parser impl of parse_expression.
    Tree Parser::parse_expression(int right_binding_power) {
        const_TokenPtr current_token = lexer.peek_token();
        lexer.skip_token();

        Tree expr = null_denotation(current_token);

        if (expr.is_error()) {
            return Tree::error();
        }

        // stop parsing if find lower priority token - parse higher priority first
        while (right_binding_power < left_binding_power(lexer.peek_token())) {
            current_token = lexer.peek_token();
            lexer.skip_token();

            expr = left_denotation(current_token, expr);
            if (expr.is_error())
                return Tree::error();
        }

        return expr;
    }

    // Parse an expression with lowest left binding power.
    Tree Parser::parse_expression() {
        return parse_expression(LBP_LOWEST);
    }

    // Parses a boolean expression (basically parses expression and ensures boolean result).
    Tree Parser::parse_boolean_expression() {
        Tree expr = parse_expression();
        if (expr.is_error())
            return expr;

        if (expr.get_type() != boolean_type_node) {
            error_at(expr.get_locus(), "expected expression of boolean type but its type is %s",
              print_type(expr.get_type()));
            return Tree::error();
        }

        return expr;
    }

    // Parses an integer expression (basically parses expression and ensures integer result).
    Tree Parser::parse_integer_expression() {
        Tree expr = parse_expression();
        if (expr.is_error())
            return expr;

        if (expr.get_type() != integer_type_node) {
            error_at(expr.get_locus(), "expected expression of integer type but its type is %s",
              print_type(expr.get_type()));
            return Tree::error();
        }

        return expr;
    }

    // Determines action to take when finding token at beginning of expression.
    Tree Parser::null_denotation(const_TokenPtr tok) {
        // note: tok is previous character in input stream, not current one, as parse_expression
        // skips it before passing it in

        /* as a Pratt parser (which works by decomposing expressions into a null denotation and then a
         * left denotation), null denotations handle primaries and unary operands (but only prefix
         * unary operands?)*/

        switch (tok->get_id()) {
            case IDENTIFIER: {
                // when encountering identifier, lookup in scope
                SymbolPtr s = scope.lookup(tok->get_str());
                if (s == NULL) {
                    error_at(tok->get_locus(), "variable '%s' not declared in the current scope",
                      tok->get_str().c_str());

                    return Tree::error();
                }
                // expression is just its VAR_DECL that was stored in the Symbol at declaration
                return Tree(s->get_tree_decl(), tok->get_locus());
            }
            case INT_LITERAL:
                // we should check the range, but ignore for now
                // literal itself encodes value, so token's text has to be interpreted as int. use
                // atoi for this
                return Tree(build_int_cst_type(integer_type_node, atoi(tok->get_str().c_str())),
                  tok->get_locus());
            case FLOAT_LITERAL: {
                REAL_VALUE_TYPE float_value;
                // invoke real_from_string3 to get float value representation from string
                real_from_string3(&float_value, tok->get_str().c_str(), TYPE_MODE(float_type_node));
                // this is because machine-dependent

                // create actual tree with that built constant value
                return Tree(build_real(float_type_node, float_value), tok->get_locus());
            }
            case STRING_LITERAL: {
                // get c string from token
                std::string str = tok->get_str();
                const char* c_str = str.c_str();

                // use GCC's build_string_literal (with null terminator) to create tree
                return Tree(build_string_literal(strlen(c_str) + 1, c_str), tok->get_locus());
            }
            case TRUE_LITERAL: {
                // construct tree with code INTEGER_CST and value 1 but with boolean_type_node
                return Tree(build_int_cst_type(boolean_type_node, 1), tok->get_locus());
                break;
            }
            case FALSE_LITERAL: {
                return Tree(build_int_cst_type(boolean_type_node, 0), tok->get_locus());
                break;
            }
            case LEFT_PAREN: { // have to parse whole expression if inside brackets
                /* recursively invoke parse_expression with lowest priority possible as it it were
                 * a top-level expression. */
                Tree expr = parse_expression();
                tok = lexer.peek_token();

                // end of expression must be a close-bracket
                if (tok->get_id() != RIGHT_PAREN)
                    error_at(
                      tok->get_locus(), "expecting ')' but %s found\n", tok->get_token_description());
                else
                    lexer.skip_token();

                return Tree(expr, tok->get_locus());
            }
            case PLUS: { // unary plus operator
                // invoke parse_expression recursively with appropriate priority, etc. for below
                Tree expr = parse_expression(LBP_UNARY_PLUS);

                if (expr.is_error())
                    return Tree::error();
                // can only apply to integer and float expressions
                if (expr.get_type() != integer_type_node || expr.get_type() != float_type_node) {
                    error_at(tok->get_locus(),
                      "operand of unary plus must be int or float but it is %s",
                      print_type(expr.get_type()));
                    return Tree::error();
                }

                return Tree(expr, tok->get_locus());
            }
            case MINUS: { // unary minus - TODO: does not work on unsigned integers
                Tree expr = parse_expression(LBP_UNARY_MINUS);

                if (expr.is_error())
                    return Tree::error();
                // can only apply to integer and float expressions
                if (expr.get_type() != integer_type_node || expr.get_type() != float_type_node) {
                    error_at(tok->get_locus(),
                      "operand of unary minus must be int or float but it is %s",
                      print_type(expr.get_type()));
                    return Tree::error();
                }

                // create NEGATE_EXPR tree, which computes negation of operand
                expr = build_tree(NEGATE_EXPR, tok->get_locus(), expr.get_type(), expr);
                return expr;
            }
            case EXCLAM: { // logical not - TODO: this could also be bitwise not
                Tree expr = parse_expression(LBP_UNARY_EXCLAM /*LOGICAL_NOT*/);

                if (expr.is_error())
                    return Tree::error();
                // can only apply to boolean expressions
                if (expr.get_type() != boolean_type_node) {
                    error_at(tok->get_locus(),
                      "operand of logical not must be a boolean but it is %s",
                      print_type(expr.get_type()));
                    return Tree::error();
                }

                // create TRUTH_NOT_EXPR tree, which computes logical negation of operand
                expr = build_tree(TRUTH_NOT_EXPR, tok->get_locus(), boolean_type_node, expr);
                return expr;
            }
            case ASTERISK: {
                // TODO: fix: this is pointer dereference only, I think
                Tree expr = parse_expression(LBP_UNARY_ASTERISK);
                return expr;
            }
            case AMP: {
                // TODO: fix: this is reference only, I think
                Tree expr = NULL_TREE;

                if (lexer.peek_token()->get_id() == MUT)
                    expr = parse_expression(LBP_UNARY_AMP_MUT);
                else
                    expr = parse_expression(LBP_UNARY_AMP);

                return expr;
            }
            case SCOPE_RESOLUTION: {
                // TODO: fix: this is for global paths, i.e. ::std::string::whatever
            }
            default:
                unexpected_token(tok);
                return Tree::error();
        }
    }

    /* Called for each token that can appear in infix (between) position. Can be operators or other
     * punctuation.
     * Returns a function pointer to member function that implements the left denotation for the token
     * given. */
    Tree Parser::left_denotation(const_TokenPtr tok, Tree left) {
        BinaryHandler binary_handler = get_binary_handler(tok->get_id());
        if (binary_handler == NULL) {
            unexpected_token(tok);
            return Tree::error();
        }

        return (this->*binary_handler)(tok, left);
    }

    // Gets method for handling binary operation parsing for specific token type.
    Parser::BinaryHandler Parser::get_binary_handler(TokenId id) {
        switch (id) {
#define BINARY_HANDLER(name, token_id) \
    case token_id:                     \
        return &Parser::binary_##name;
            BINARY_HANDLER_LIST
#undef BINARY_HANDLER
            default:
                return NULL;
        }
    }

    /* Returns the type of the binary operation. May also modify input trees if types do not match,
     * e.g. change a float and int to two floats in addition.   */
    Tree Parser::coerce_binary_arithmetic(const_TokenPtr tok, Tree* left, Tree* right) {
        Tree left_type = left->get_type();
        Tree right_type = right->get_type();

        // return error if either types are invalid
        if (left_type.is_error() || right_type.is_error())
            return Tree::error();

        // good, easy, no type conversion required
        if (left_type == right_type) {
            if (left_type == integer_type_node || left_type == float_type_node) {
                return left_type;
            }
            // dodgy type coercion happening if types don't match but are both numerical
        } else if ((left_type == integer_type_node && right_type == float_type_node)
                   || (left_type == float_type_node && right_type == integer_type_node)) {
            // rebuild one tree as float
            if (left_type == integer_type_node) {
                *left = build_tree(FLOAT_EXPR, left->get_locus(), float_type_node, left->get_tree());
            } else {
                *right
                  = build_tree(FLOAT_EXPR, right->get_locus(), float_type_node, right->get_tree());
            }

            return float_type_node;
        }

        // unhandled - e.g. int + boolean
        error_at(tok->get_locus(), "invalid operands of type %s and %s for operator %s",
          print_type(left_type), print_type(right_type), tok->get_token_description());
        return Tree::error();
    }

    // Verifies that both left and right trees are boolean-type nodes.
    bool Parser::check_logical_operands(const_TokenPtr tok, Tree left, Tree right) {
        // ensure both operands are boolean types
        if (left.get_type() != boolean_type_node || right.get_type() != boolean_type_node) {
            error_at(tok->get_locus(),
              "operands of operator %s must be boolean but they are %s and %s\n",
              tok->get_token_description(), print_type(left.get_type()),
              print_type(right.get_type()));

            return false;
        }

        return true;
    }

    // Implementation of addition expression parsing.
    Tree Parser::binary_plus(const_TokenPtr tok, Tree left) {
        // parse RHS (as tok has already been consumed in parse_expression)
        Tree right = parse_expression(LBP_PLUS);
        if (right.is_error())
            return Tree::error();

        /* compute resulting type of binary operator with coerce_binary_arithmetic method, which may
         * also modify input trees.    */
        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        // construct tree with code PLUS_EXPR to represent binary addition
        return build_tree(PLUS_EXPR, tok->get_locus(), tree_type, left, right);
    }

    // Implementation of subtraction expression parsing.
    Tree Parser::binary_minus(const_TokenPtr tok, Tree left) {
        // parse RHS (as tok has already been consumed in parse_expression)
        Tree right = parse_expression(LBP_PLUS);
        if (right.is_error())
            return Tree::error();

        /* compute resulting type of binary operator with coerce_binary_arithmetic method, which may
         * also modify input trees.    */
        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        // construct tree with code MINUS_EXPR to represent binary subtraction
        return build_tree(MINUS_EXPR, tok->get_locus(), tree_type, left, right);
    }

    // Implementation of multiplication expression parsing.
    Tree Parser::binary_mult(const_TokenPtr tok, Tree left) {
        // parse RHS (as tok has already been consumed in parse_expression)
        Tree right = parse_expression(LBP_PLUS);
        if (right.is_error())
            return Tree::error();

        /* compute resulting type of binary operator with coerce_binary_arithmetic method, which may
         * also modify input trees.    */
        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        // construct tree with code MULT_EXPR to represent binary multiplication
        return build_tree(MULT_EXPR, tok->get_locus(), tree_type, left, right);
    }

    // Implementation of divison expression parsing.
    Tree Parser::binary_div(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_DIV);
        if (right.is_error())
            return Tree::error();

        // if integer division, create C-like truncated division expression tree
        if (left.get_type() == integer_type_node && right.get_type() == integer_type_node) {
            return build_tree(TRUNC_DIV_EXPR, tok->get_locus(), integer_type_node, left, right);
        } else {
            // floating-point division
            Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
            if (tree_type.is_error())
                return Tree::error();

            gcc_assert(tree_type == float_type_node);

            return build_tree(RDIV_EXPR, tok->get_locus(), tree_type, left, right);
        }
    }

    // Implementation of modulo expression parsing.
    Tree Parser::binary_mod(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_MOD);
        if (right.is_error())
            return Tree::error();

        // if integer modulo, create truncated modulo expression
        if (left.get_type() == integer_type_node && right.get_type() == integer_type_node) {
            return build_tree(TRUNC_MOD_EXPR, tok->get_locus(), integer_type_node, left, right);
        } else {
            // no floating-point modulo
            return Tree::error();
        }
    }

    // Implementation of binary equal comparison relational operator parsing.
    Tree Parser::binary_equal(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_EQUAL);
        if (right.is_error())
            return Tree::error();

        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        return build_tree(EQ_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary different comparison relational operator parsing.
    Tree Parser::binary_not_equal /*different*/ (const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_NOT_EQUAL);
        if (right.is_error())
            return Tree::error();

        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        return build_tree(NE_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary smaller than comparison relational operator parsing.
    Tree Parser::binary_smaller_than(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_SMALLER_THAN);
        if (right.is_error())
            return Tree::error();

        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        return build_tree(LT_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary greater than comparison relational operator parsing.
    Tree Parser::binary_greater_than(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_GREATER_THAN);
        if (right.is_error())
            return Tree::error();

        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        return build_tree(GT_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary smaller than or equal to comparison relational operator parsing.
    Tree Parser::binary_smaller_equal(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_SMALLER_EQUAL);
        if (right.is_error())
            return Tree::error();

        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        return build_tree(LE_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary greater than or equal to comparison relational operator parsing.
    Tree Parser::binary_greater_equal(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_GREATER_EQUAL);
        if (right.is_error())
            return Tree::error();

        Tree tree_type = coerce_binary_arithmetic(tok, &left, &right);
        if (tree_type.is_error())
            return Tree::error();

        return build_tree(GE_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary "and" logical operator parsing.
    Tree Parser::binary_logical_and(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_LOGICAL_AND);
        if (right.is_error())
            return Tree::error();

        if (!check_logical_operands(tok, left, right))
            return Tree::error();

        return build_tree(TRUTH_ANDIF_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary "or" logical operator parsing.
    Tree Parser::binary_logical_or(const_TokenPtr tok, Tree left) {
        // parse RHS
        Tree right = parse_expression(LBP_LOGICAL_OR);
        if (right.is_error())
            return Tree::error();

        if (!check_logical_operands(tok, left, right))
            return Tree::error();

        return build_tree(TRUTH_ORIF_EXPR, tok->get_locus(), boolean_type_node, left, right);
    }

    // Implementation of binary array reference ([) operator parsing;
    Tree Parser::binary_array_ref(const_TokenPtr tok, Tree left) {
        // parse integer expression inside square brackets (array index)
        Tree right = parse_integer_expression();
        if (right.is_error())
            return Tree::error();

        // array close token
        if (!skip_token(RIGHT_SQUARE))
            return Tree::error();

        // verify left operand has array type
        if (!is_array_type(left.get_type())) {
            error_at(left.get_locus(), "does not have array type");
            return Tree::error();
        }

        // compute type of array element
        Tree element_type = TREE_TYPE(left.get_type().get_tree());

        // build GENERIC tree ARRAY_REF that represents array access
        return build_tree(ARRAY_REF, tok->get_locus(), element_type, left, right, Tree(), Tree());
    }

    // Parses a binary field access on a record.
    Tree Parser::binary_field_ref(const_TokenPtr tok, Tree left) {
        const_TokenPtr identifier = expect_token(IDENTIFIER);
        if (identifier == NULL)
            return Tree::error();

        // ensure left expression has record type
        if (!is_record_type(left.get_type())) {
            error_at(left.get_locus(), "does not have record type");
            return Tree::error();
        }

        // traverse each FIELD_DECL chaining through TREE_CHAIN
        // list of fields in record type is available through TYPE_FIELDS
        Tree field_decl = TYPE_FIELDS(left.get_type().get_tree());
        while (!field_decl.is_null()) {
            // FIELD_DECL has a DECL_NAME containing an IDENTIFIER_POINTER to get field name
            Tree decl_name = DECL_NAME(field_decl.get_tree());
            const char* field_name = IDENTIFIER_POINTER(decl_name.get_tree());

            if (field_name == identifier->get_str())
                break;

            field_decl = TREE_CHAIN(field_decl.get_tree());
        }

        // if can't find a field with given name, this is an error
        if (field_decl.is_null()) {
            error_at(left.get_locus(), "record type does not have a field named '%s'",
              identifier->get_str().c_str());
            return Tree::error();
        }

        // build COMPONENT_REF tree using left tree (record type) and appropriate FIELD_DECL
        return build_tree(COMPONENT_REF, tok->get_locus(), TREE_TYPE(field_decl.get_tree()), left,
          field_decl, Tree());
    }

    // Method stub
    Tree Parser::binary_bitwise_or(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_left_shift(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_bitwise_and(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_bitwise_xor(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_right_shift(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_as_cast(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_div_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_mod_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_mult_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_plus_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_minus_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_assignment_expr(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_bitwise_or_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_bitwise_xor_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_bitwise_and_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_left_shift_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Method stub
    Tree Parser::binary_right_shift_assig(const_TokenPtr tok, Tree left) {
        return NULL_TREE;
    }

    // Parse variable assignment statement. This is not the same as variable declaration.
    Tree Parser::parse_assignment_statement() {
        Tree variable = parse_lhs_assignment_expression();

        if (variable.is_error())
            return Tree::error();

        // TODO: fix
        const_TokenPtr assig_tok = expect_token(/*ASSIG*/ COLON);
        if (assig_tok == NULL) {
            skip_after_semicolon();
            return Tree::error();
        }

        // skip assignment token and parse expression
        const_TokenPtr first_of_expr = lexer.peek_token();

        Tree expr = parse_expression();
        if (expr.is_error())
            return Tree::error();

        // skip semicolon token
        skip_token(SEMICOLON);

        // enforce rule that rhs of assignment has to have same type as declared lhs type
        if (variable.get_type() != expr.get_type()) {
            // diagnostic
            error_at(first_of_expr->get_locus(),
              "cannot assign value of type %s to a variable of type %s", print_type(expr.get_type()),
              print_type(variable.get_type()));

            return Tree::error();
        }

        Tree assig_expr
          = build_tree(MODIFY_EXPR, assig_tok->get_locus(), void_type_node, variable, expr);
        return assig_expr;
    }

    // Print human-readable name for type.
    const char* Parser::print_type(Tree type) {
        gcc_assert(TYPE_P(type.get_tree()));

        if (type == void_type_node)
            return "void";
        else if (type == integer_type_node)
            return "int";
        else if (type == float_type_node)
            return "float";
        else if (is_string_type(type))
            return "string";
        else if (type == boolean_type_node)
            return "boolean";
        else
            return "<<unknown-type>>";
    }

    // Returns address to function declaration of printf.
    Tree Parser::get_printf_addr() {
        // only run if printf_fn is null to avoid making repeated function declarations
        if (printf_fn.is_null()) {
            // build const char* type (printf fixed parameter)
            tree fndecl_type_param[]
              = { build_pointer_type(build_qualified_type(char_type_node, TYPE_QUAL_CONST)) };

            // build function type as vararg function
            tree fndecl_type
              = build_varargs_function_type_array(integer_type_node, 1, fndecl_type_param);

            // build declaration
            tree printf_fn_decl = build_fn_decl("printf", fndecl_type);
            // mark as external
            DECL_EXTERNAL(printf_fn_decl) = 1;

            // build an ADDR_EXPR, which returns a pointer to type of function type (function address)
            printf_fn = build1(ADDR_EXPR, build_pointer_type(fndecl_type), printf_fn_decl);
        }

        return printf_fn;
    }

    // Returns address to function declaration of puts.
    Tree Parser::get_puts_addr() {
        if (puts_fn.is_null()) {
            // build const char* type (puts fixed parameter)
            tree fndecl_type_param[]
              = { build_pointer_type(build_qualified_type(char_type_node, TYPE_QUAL_CONST)) };

            // build function type
            tree fndecl_type = build_function_type_array(integer_type_node, 1, fndecl_type_param);

            // build declaration
            tree puts_fn_decl = build_fn_decl("puts", fndecl_type);
            // mark as external
            DECL_EXTERNAL(puts_fn_decl) = 1;

            // build an ADDR_EXPR, which returns a pointer to type of function type (function address)
            puts_fn = build1(ADDR_EXPR, build_pointer_type(fndecl_type), puts_fn_decl);
        }

        return puts_fn;
    }

    // Returns address to function declaration of scanf.
    Tree Parser::get_scanf_addr() {
        // only run if scanf_fn is null to avoid making repeated function declarations
        if (scanf_fn.is_null()) {
            // build const char* type (scanf fixed parameter)
            tree fndecl_type_param[]
              = { build_pointer_type(build_qualified_type(char_type_node, TYPE_QUAL_CONST)) };

            // build function type as vararg function
            tree fndecl_type
              = build_varargs_function_type_array(integer_type_node, 1, fndecl_type_param);

            // build declaration
            tree scanf_fn_decl = build_fn_decl("scanf", fndecl_type);
            // mark as external
            DECL_EXTERNAL(scanf_fn_decl) = 1;

            // build an ADDR_EXPR, which returns a pointer to type of function type (function address)
            scanf_fn = build1(ADDR_EXPR, build_pointer_type(fndecl_type), scanf_fn_decl);
        }

        return scanf_fn;
    }

    // Parses a "write statement".
    /*Tree Parser::parse_write_statement() {
        // write_statement -> "write" expression ";"

        if (!skip_token(WRITE)) {
            skip_after_semicolon();
            return Tree::error();
        }

        const_TokenPtr first_of_expr = lexer.peek_token();
        Tree expr = parse_expression();

        skip_token(SEMICOLON);

        if (expr.is_error())
            return Tree::error();

        // enable printing of value of expression
        if (expr.get_type() == integer_type_node) {
            // printf("%d\n", expr)
            // build format string for integer (also add null terminator) and integer expression
            const char* format_integer = "%d\n";
            tree args[]
              = { build_string_literal(strlen(format_integer) + 1, format_integer), expr.get_tree() };

            // used as trees do not allow calling a FUNCTIONAL_DECL directly
            Tree printf_fn = get_printf_addr();

            // build call to print function (printf_fn), in which two arguments in args are passed
            // first argument is format string
            tree stmt = build_call_array_loc(
              first_of_expr->get_locus(), integer_type_node, printf_fn.get_tree(), 2, args);

            return stmt;
        } else if (expr.get_type() == float_type_node) {
            // printf("%f\n" (double)expr)
            // have to convert float to double
            const char* format_float = "%f\n";
            tree args[] = { build_string_literal(strlen(format_float) + 1, format_float),
                convert(double_type_node, expr.get_tree()) };

            Tree printf_fn = get_printf_addr();

            // build call, etc.
            tree stmt = build_call_array_loc(
              first_of_expr->get_locus(), integer_type_node, printf_fn.get_tree(), 2, args);

            return stmt;
        } else if (is_string_type(expr.get_type())) {
            // Alternatively we could use printf('%s\n', expr) instead of puts(expr)
            tree args[] = { expr.get_tree() };

            Tree puts_fn = get_puts_addr();

            tree stmt = build_call_array_loc(
              first_of_expr->get_locus(), integer_type_node, puts_fn.get_tree(), 1, args);

            return stmt;
        } else {
            // no more valid types
            error_at(first_of_expr->get_locus(), "value of type %s is not a valid write operand",
              print_type(expr.get_type()));
            return Tree::error();
        }

        gcc_unreachable();
    }*/

    // Enters new scope (like block scope or whatever).
    void Parser::enter_scope() {
        // push new symbol mapping
        scope.push_scope();

        TreeStmtList stmt_list;
        // Used as stack for statements.
        stack_stmt_list.push_back(stmt_list);

        // Used as stack of var decls.
        stack_var_decl_chain.push_back(TreeChain());
        // Used as stack for blocks.
        stack_block_chain.push_back(BlockChain());
    }

    // Leaves current scope (as defined by blocks - like block scope).
    Parser::TreeSymbolMapping Parser::leave_scope() {
        // Get current list of statements and pop them from stack of statement lists
        TreeStmtList current_stmt_list = get_current_stmt_list();
        stack_stmt_list.pop_back();

        // Get current list of var decls and pop them from stack of var decl lists
        TreeChain var_decl_chain = stack_var_decl_chain.back();
        stack_var_decl_chain.pop_back();

        // Get current list of blocks and pop them from stack of block lists
        BlockChain subblocks = stack_block_chain.back();
        stack_block_chain.pop_back();

        // Create a new block from var decls and subblocks
        tree new_block = build_block(var_decl_chain.first.get_tree(), subblocks.first.get_tree(),
          /* supercontext */ NULL_TREE, /* chain */ NULL_TREE);

        // Add the new block to the current chain of blocks (if any)
        if (!stack_block_chain.empty()) {
            stack_block_chain.back().append(new_block);
        }

        // Set the subblocks to have the new block as their parent
        for (tree it = subblocks.first.get_tree(); it != NULL_TREE; it = BLOCK_CHAIN(it))
            BLOCK_SUPERCONTEXT(it) = new_block;
        // Do it this way because of double-linkage

        // Create BIND_EXPR from decl chain, stmt list, and new block
        tree bind_expr = build3(BIND_EXPR, void_type_node, var_decl_chain.first.get_tree(),
          current_stmt_list.get_tree(), new_block);

        // create, basically, a tuple of bind_expr and new_block
        TreeSymbolMapping tree_scope;
        tree_scope.bind_expr = bind_expr;
        tree_scope.block = new_block;

        // pop symbol mapping
        scope.pop_scope();

        return tree_scope;
    }

#if 0
    // Parses the "read" statement.
    Tree Parser::parse_read_statement() {
        if (!skip_token(READ)) {
            skip_after_semicolon();
            return Tree::error();
        }

        const_TokenPtr first_of_expr = lexer.peek_token();
        Tree expr = parse_expression_naming_variable();

        skip_token(SEMICOLON);

        if (expr.is_error())
            return Tree::error();

        // force variable name instead of manually looking up identifier token
        /* if (expr.get_tree_code() != VAR_DECL) {
            error_at(first_of_expr->get_locus(), "invalid expression in read statement");
            return Tree::error();
        }*/
        // not used anymore due to parse_expression_naming_variable

        // Variable must be addressable (variable needs address computed)
        TREE_ADDRESSABLE(expr.get_tree()) = 1;

        // determine appropriate format string
        const char* format = NULL;
        if (expr.get_type() == integer_type_node) {
            format = "%d";
        } else if (expr.get_type() == float_type_node) {
            format = "%f";
        } else {
            error_at(first_of_expr->get_locus(), "variable of type %s is not a valid read operand",
              print_type(expr.get_type()));
            return Tree::error();
        }

        // build args for scanf
        tree args[] = { build_string_literal(strlen(format) + 1, format),
            build_tree(ADDR_EXPR, first_of_expr->get_locus(),
              build_pointer_type(expr.get_type().get_tree()), expr)
              .get_tree() };

        // get scanf address
        Tree scanf_fn = get_scanf_addr();

        // create tree to call scanf
        tree stmt = build_call_array_loc(
          first_of_expr->get_locus(), integer_type_node, scanf_fn.get_tree(), 2, args);

        return stmt;
    }
#endif

    // Parses a while statement.
    Tree Parser::parse_while_statement() {
        if (!skip_token(WHILE)) {
            skip_after_end();
            return Tree::error();
        }

        // parse while's conditional expression
        Tree expr = parse_boolean_expression();
        if (!skip_token(DO)) {
            skip_after_end();
            return Tree::error();
        }

        // enter loop body scope
        enter_scope();
        parse_statement_seq(&Parser::done_end);
        TreeSymbolMapping while_body_tree_scope = leave_scope();

        Tree while_body_stmt = while_body_tree_scope.bind_expr;

        skip_token(RIGHT_CURLY);

        // build while statement tree
        return build_while_statement(expr, while_body_stmt);
    }

    // Builds a while statement tree.
    Tree Parser::build_while_statement(Tree bool_expr, Tree while_body) {
        if (bool_expr.is_error())
            return Tree::error();

        TreeStmtList stmt_list;

        // build label decl for while condition check
        Tree while_check_label_decl = build_label_decl("while_check", bool_expr.get_locus());

        // build label expr for while condition check and add to statement list
        Tree while_check_label_expr
          = build_tree(LABEL_EXPR, bool_expr.get_locus(), void_type_node, while_check_label_decl);
        stmt_list.append(while_check_label_expr);

        // build label decl for loop body and end of loop
        Tree while_body_label_decl = build_label_decl("while_body", while_body.get_locus());
        Tree end_of_while_label_decl = build_label_decl("end_of_while", UNKNOWN_LOCATION);

        // add cond_expr tree that evaluates condition expression and branches to correct label
        Tree cond_expr = build_tree(COND_EXPR, bool_expr.get_locus(), void_type_node, bool_expr,
          build_tree(GOTO_EXPR, bool_expr.get_locus(), void_type_node, while_body_label_decl),
          build_tree(GOTO_EXPR, bool_expr.get_locus(), void_type_node, end_of_while_label_decl));
        stmt_list.append(cond_expr);

        // define location of label for body of loop and append to while body
        Tree while_body_label_expr
          = build_tree(LABEL_EXPR, while_body.get_locus(), void_type_node, while_body_label_decl);
        stmt_list.append(while_body_label_expr);

        stmt_list.append(while_body);

        // branch back to condition check (as it is a loop)
        Tree goto_check
          = build_tree(GOTO_EXPR, UNKNOWN_LOCATION, void_type_node, while_check_label_decl);
        stmt_list.append(goto_check);

        // define location of label for end of the while loop
        Tree end_of_while_label_expr
          = build_tree(LABEL_EXPR, UNKNOWN_LOCATION, void_type_node, end_of_while_label_decl);
        stmt_list.append(end_of_while_label_expr);

        return stmt_list.get_tree();
    }

    // Parse a for statement.
    Tree Parser::parse_for_statement() {
        // for -> for <identifier> := <expression> to <expression> do <statements> end
        if (!skip_token(FOR)) {
            skip_after_end();
            return Tree::error();
        }

        const_TokenPtr identifier = expect_token(IDENTIFIER);
        if (identifier == NULL) {
            skip_after_end();
            return Tree::error();
        }

        // TODO
        if (!skip_token(/*ASSIG*/ COLON)) {
            skip_after_end();
            return Tree::error();
        }

        // parse lower bound expression
        Tree lower_bound = parse_integer_expression();

        // TODO
        if (!skip_token(/*TO*/ DOT)) {
            skip_after_end();
            return Tree::error();
        }

        // parse upper bound expression
        Tree upper_bound = parse_integer_expression();

        if (!skip_token(DO)) {
            skip_after_end();
            return Tree::error();
        }

        // enter loop body scope and parse internal statements
        enter_scope();
        parse_statement_seq(&Parser::done_end);

        TreeSymbolMapping for_body_tree_scope = leave_scope();
        Tree for_body_stmt = for_body_tree_scope.bind_expr;

        skip_token(RIGHT_CURLY);

        // Induction variable ("loop counter" variable) handling
        SymbolPtr ind_var = query_integer_variable(identifier->get_str(), identifier->get_locus());

        // build for statement
        return build_for_statement(ind_var, lower_bound, upper_bound, for_body_stmt);
    }

    // Builds a for statement tree (piggybacks on while statement tree building).
    Tree Parser::build_for_statement(
      SymbolPtr ind_var, Tree lower_bound, Tree upper_bound, Tree for_body_stmt_list) {
        if (ind_var == NULL)
            return Tree::error();
        Tree ind_var_decl = ind_var->get_tree_decl();

        // lower
        if (lower_bound.is_error())
            return Tree::error();

        // upper
        if (upper_bound.is_error())
            return Tree::error();

        // ind_var = lower
        TreeStmtList stmt_list;

        // initialise induction variable with value of lower bound and append to stmt_list
        Tree init_ind_var
          = build_tree(MODIFY_EXPR, UNKNOWN_LOCATION, void_type_node, ind_var_decl, lower_bound);
        stmt_list.append(init_ind_var);

        // define condition ind_var <= upper for use in while loop
        Tree while_condition = build_tree(
          LE_EXPR, upper_bound.get_locus(), boolean_type_node, ind_var_decl, upper_bound);

        // simulate incrementing ind_var
        Tree incr_ind_var = build_tree(MODIFY_EXPR, UNKNOWN_LOCATION, void_type_node, ind_var_decl,
          build_tree(PLUS_EXPR, UNKNOWN_LOCATION, integer_type_node, ind_var_decl,
            build_int_cst_type(integer_type_node, 1)));

        // Wrap as stmt list
        TreeStmtList for_stmt_list = for_body_stmt_list;
        for_stmt_list.append(incr_ind_var);

        // construct associated while statement and append to stmt_list
        Tree while_stmt = build_while_statement(while_condition, for_stmt_list.get_tree());
        stmt_list.append(while_stmt);

        // return entire stmt_list
        return stmt_list.get_tree();
    }

    // Gets type (as in typedef) of name in current scope.
    SymbolPtr Parser::query_type(const std::string& name, location_t loc) {
        SymbolPtr sym = scope.lookup(name);
        if (sym == NULL) {
            error_at(loc, "type '%s' not declared in the current scope", name.c_str());
        } else if (sym->get_kind() != TYPENAME) {
            error_at(loc, "name '%s' is not a type", name.c_str());
            sym = SymbolPtr();
        }

        return sym;
    }

    // Get variable of name in current scope.
    SymbolPtr Parser::query_variable(const std::string& name, location_t loc) {
        SymbolPtr sym = scope.lookup(name);
        if (sym == NULL) {
            error_at(loc, "variable '%s' not declared in the current scope", name.c_str());
        } else if (sym->get_kind() != VARIABLE) {
            error_at(loc, "name '%s' is not a variable", name.c_str());
            sym = SymbolPtr();
        }
        return sym;
    }

    // Gets variable of name in current scope and ensures it has integer type.
    SymbolPtr Parser::query_integer_variable(const std::string& name, location_t loc) {
        SymbolPtr sym = query_variable(name, loc);
        if (sym != NULL) {
            Tree var_decl = sym->get_tree_decl();
            gcc_assert(!var_decl.is_null());

            if (var_decl.get_type() != integer_type_node) {
                error_at(loc, "variable '%s' does not have integer type", name.c_str());
                sym = SymbolPtr();
            }
        }

        return sym;
    }

    // Returns true if the next token is END, ELSE, or EOF;
    bool Parser::done_end_or_else() {
        const_TokenPtr t = lexer.peek_token();
        return (t->get_id() == RIGHT_CURLY || t->get_id() == ELSE || t->get_id() == END_OF_FILE);
    }

    // Returns true if the next token is END or EOF.
    bool Parser::done_end() {
        const_TokenPtr t = lexer.peek_token();
        return (t->get_id() == RIGHT_CURLY || t->get_id() == END_OF_FILE);
    }

    // Parses expression and ensures it is a variable declaration or array reference.
    Tree Parser::parse_expression_naming_variable() {
        Tree expr = parse_expression();
        if (expr.is_error())
            return expr;

        if (expr.get_tree_code() != VAR_DECL && expr.get_tree_code() != ARRAY_REF
            && expr.get_tree_code() != COMPONENT_REF) {
            error_at(expr.get_locus(), "does not designate a variable, array element or field");
            return Tree::error();
        }

        return expr;
    }

    // Parses expression and ensures it is an assignment expression?
    Tree Parser::parse_lhs_assignment_expression() {
        return parse_expression_naming_variable();
    }

    // Parses type (as in typedef) declaration statement.
    /*Tree Parser::parse_type_declaration() {
        // type_declaration -> "type" identifier ":" type ";"
        if (!skip_token(TYPE)) {
            skip_after_semicolon();
            return Tree::error();
        }

        // get identifier
        const_TokenPtr identifier = expect_token(IDENTIFIER);
        if (identifier == NULL) {
            skip_after_semicolon();
            return Tree::error();
        }

        // skip colon
        if (!skip_token(COLON)) {
            skip_after_semicolon();
            return Tree::error();
        }

        // get type of expression
        Tree type_tree = parse_type();

        if (type_tree.is_error()) {
            skip_after_semicolon();
            return Tree::error();
        }

        skip_token(SEMICOLON);

        // ensure not already delcared in scope
        if (scope.get_current_mapping().get(identifier->get_str())) {
            error_at(identifier->get_locus(), "name '%s' already declared in this scope",
              identifier->get_str().c_str());
        }

        // create new symbol for typedef and put in mapping for current scope
        SymbolPtr sym(new Symbol(TYPENAME, identifier->get_str()));
        scope.get_current_mapping().insert(sym);

        // build typedef tree
        Tree decl = build_decl(identifier->get_locus(), TYPE_DECL,
          get_identifier(sym->get_name().c_str()), type_tree.get_tree());
        DECL_CONTEXT(decl.get_tree()) = main_fndecl;

        // add type declaration to variable declaration stack
        gcc_assert(!stack_var_decl_chain.empty());
        stack_var_decl_chain.back().append(decl);

        // set symbol's declaration tree to declaration tree
        sym->set_tree_decl(decl);

        // build declaration statement for tree
        Tree stmt = build_tree(DECL_EXPR, identifier->get_locus(), void_type_node, decl);

        return stmt;
    }*/

    // Parses a record type field declaration.
    /*Tree Parser::parse_field_declaration(std::vector<std::string>& field_names) {
        // identifier ':' type ';'
        const_TokenPtr identifier = expect_token(IDENTIFIER);
        if (identifier == NULL) {
            skip_after_semicolon();
            return Tree::error();
        }

        skip_token(COLON);

        Tree type = parse_type();

        skip_token(SEMICOLON);

        if (type.is_error())
            return Tree::error();

        // pass vector of fields to avoid repeated field names - error if they exist
        if (std::find(field_names.begin(), field_names.end(), identifier->get_str())
            != field_names.end()) {
            error_at(identifier->get_locus(), "repeated field name");
            return Tree::error();
        }

        field_names.push_back(identifier->get_str());

        // create GENERIC FIELD_DECL tree with name of tree and type
        Tree field_decl = build_decl(identifier->get_locus(), FIELD_DECL,
          get_identifier(identifier->get_str().c_str()), type.get_tree());
        // required for read statement to work on fields
        TREE_ADDRESSABLE(field_decl.get_tree()) = 1;

        return field_decl;
    }*/

    // TODO: remove: here to solve link errors
    Tree Parser::parse_record() {
        return Tree::error();
    }
    // Parses a record.
    /*Tree Parser::parse_record() {
        // "record" field-decl* "end"
        const_TokenPtr record_tok = expect_token(RECORD);
        if (record_tok == NULL) {
            skip_after_semicolon();
            return Tree::error();
        }

        // create empty record type tree
        Tree record_type = make_node(RECORD_TYPE);
        Tree field_list, field_last;
        std::vector<std::string> field_names;

        // parse field declarations inside record until the end token is found
        const_TokenPtr next = lexer.peek_token();
        while (next->get_id() != END) {
            Tree field_decl = parse_field_declaration(field_names);

            if (!field_decl.is_error()) {
                // set field declaration's decl_context to this record type
                DECL_CONTEXT(field_decl.get_tree()) = record_type.get_tree();
                if (field_list.is_null())
                    field_list = field_decl;
                if (!field_last.is_null())
                    // chain fields in record type by using tree_chain
                    TREE_CHAIN(field_last.get_tree()) = field_decl.get_tree();
                field_last = field_decl;
            }

            next = lexer.peek_token();
        }

        skip_token(END);

        // first field sets TYPE_FIELDS attribute of the RECORD_TYPE tree
        TYPE_FIELDS(record_type.get_tree()) = field_list.get_tree();
        // request GCC to layout type in memory
        layout_type(record_type.get_tree());

        return record_type;
    }*/

    // Dumps lexer output to stderr.
    void Parser::debug_dump_lex_output() {
        Rust::const_TokenPtr tok = lexer.peek_token();

        while (true) {
            bool has_text
              = tok->get_id() == Rust::IDENTIFIER || tok->get_id() == Rust::INT_LITERAL
                || tok->get_id() == Rust::FLOAT_LITERAL || tok->get_id() == Rust::STRING_LITERAL
                || tok->get_id() == Rust::CHAR_LITERAL || tok->get_id() == Rust::BYTE_STRING_LITERAL
                || tok->get_id() == Rust::BYTE_CHAR_LITERAL;

            location_t loc = tok->get_locus();

            fprintf(stderr, "<id=%s%s, %s, line=%d, col=%d>\n", tok->token_id_to_str(),
              has_text ? (std::string(", text=") + tok->get_str() + std::string(", typehint=")
                           + std::string(tok->get_type_hint_str()))
                           .c_str()
                       : "",
              LOCATION_FILE(loc), LOCATION_LINE(loc), LOCATION_COLUMN(loc));

            if (tok->get_id() == Rust::END_OF_FILE)
                break;

            lexer.skip_token();
            tok = lexer.peek_token();
        }
    }
}