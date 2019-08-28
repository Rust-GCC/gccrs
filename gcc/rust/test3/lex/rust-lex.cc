#include "rust-lex.h"

#include "diagnostic.h" // for error_at
#include "safe-ctype.h"

#include <sstream> // for ostringstream

namespace Rust {
    // Includes all allowable float digits EXCEPT _ and . as that needs lookahead for handling.
    bool is_float_digit(char number) {
        return ISDIGIT(number) || number == 'E' || number == 'e';
    }

    bool is_x_digit(char number) {
        // TODO: fix
        return number < 128 && ::isxdigit(number);
    }

    Lexer::Lexer(const char* filename, FILE* input) :
      input(input), current_line(1), current_column(1), line_map(0), input_source(input),
      input_queue(input_source), token_source(this), token_queue(token_source) {
        // inform line_table that file is being entered and is in line 1
        line_map
          = ::linemap_add(::line_table, ::LC_ENTER, /* sysp */ 0, filename, /* current_line */ 1);
    }

    Lexer::~Lexer() {
        ::linemap_add(::line_table, ::LC_LEAVE, /* sysp */ 0, /* filename */ NULL, /* to_line */ 0);
    }

    location_t Lexer::get_current_location() {
        return ::linemap_position_for_column(::line_table, current_column);
    }

    int Lexer::peek_input(int n) {
        return input_queue.peek(n);
    }

    int Lexer::peek_input() {
        return peek_input(0);
    }

    void Lexer::skip_input(int n) {
        input_queue.skip(n);
    }

    void Lexer::skip_input() {
        skip_input(0);
    }

    const_TokenPtr Lexer::peek_token(int n) {
        return token_queue.peek(n);
    }

    const_TokenPtr Lexer::peek_token() {
        return peek_token(0);
    }

    void Lexer::skip_token(int n) {
        token_queue.skip(n);
    }

    void Lexer::skip_token() {
        skip_token(0);
    }

    /* shitty anonymous namespace that can only be accessed inside the compilation unit - used for
     * classify_keyword
     * Binary search in sorted array of keywords created with x-macros. */
    namespace {
        const std::string keyword_index[] = {
#define RS_TOKEN(x, y)
#define RS_TOKEN_KEYWORD(name, keyword) keyword,
            RS_TOKEN_LIST
#undef RS_TOKEN_KEYWORD
#undef RS_TOKEN
        };

        TokenId keyword_keys[] = {
#define RS_TOKEN(x, y)
#define RS_TOKEN_KEYWORD(name, keyword) name,
            RS_TOKEN_LIST
#undef RS_TOKEN_KEYWORD
#undef RS_TOKEN
        };

        const int num_keywords = sizeof(keyword_index) / sizeof(*keyword_index);
    }

    TokenId Lexer::classify_keyword(const std::string& str) {
        const std::string* last = keyword_index + num_keywords;
        const std::string* idx = std::lower_bound(keyword_index, last, str);

        if (idx == last || str != *idx) {
            return IDENTIFIER;
        } else {
            return keyword_keys[idx - keyword_index];
        }
    }

    TokenPtr Lexer::build_token() {
        // loop to go through multiple characters to build a single token
        while (true) {
            location_t loc = get_current_location();
            int current_char = peek_input();
            skip_input();

            // return end of file token if end of file
            if (current_char == EOF) {
                return Token::make(END_OF_FILE, loc);
            }

            // detect shebang
            if (loc == 1 && current_line == 1 && current_char == '#') {
                current_char = peek_input();

                if (current_char == '!') {
                    skip_input();
                    current_char = peek_input();

                    switch (current_char) {
                        case '/':
                            // shebang

                            skip_input();

                            // ignore rest of line
                            while (current_char != '\n') {
                                current_char = peek_input();
                                skip_input();
                            }

                            // newline
                            current_line++;
                            current_column = 1;
                            // tell line_table that new line starts
                            linemap_line_start(::line_table, current_line, max_column_hint);
                            continue;
                            /*        case '[':
                                        //return Token::make(HASH, loc);
                                        // somehow add a hash then exclamation mark
                                    default:
                                        // didn't match anything so error
                                        error_at(loc, "unexpected character '%x'", current_char);
                                        current_column++;
                                        continue;
                                }
                            } else if (current_char == '[') {
                                return Token::make(HASH, loc);
                            } else {
                                // didn't match anything so error
                                error_at(loc, "unexpected character '%x'", current_char);
                                current_column++;
                                continue;
                            }*/
                            // crappy code that on second thought would probably be handled better in
                            // main switch
                    }

                    // if not end of file, start tokenising
                    switch (current_char) {
                        // ignore whitespace characters for tokens but continue updating location
                        case '\n': // newline
                            current_line++;
                            current_column = 1;
                            // tell line_table that new line starts
                            linemap_line_start(::line_table, current_line, max_column_hint);
                            continue;
                        case ' ': // space
                            current_column++;
                            continue;
                        case '\t': // tab
                            // width of a tab is not well-defined, assume 8 spaces
                            current_column += 8;
                            continue;

                        // punctuation - actual tokens
                        case '=':
                            if (peek_input() == '>') {
                                // match arm arrow
                                skip_input();
                                current_column += 2;

                                return Token::make(MATCH_ARROW, loc);
                            } else if (peek_input() == '=') {
                                // equality operator
                                skip_input();
                                current_column += 2;

                                return Token::make(EQUAL_EQUAL, loc);
                            } else {
                                // assignment operator
                                current_column++;
                                return Token::make(EQUAL, loc);
                            }
                        case '(':
                            current_column++;
                            return Token::make(LEFT_PAREN, loc);
                        case '-':
                            if (peek_input() == '>') {
                                // return type specifier
                                skip_input();
                                current_column += 2;

                                return Token::make(RETURN_TYPE, loc);
                            } else if (peek_input() == '=') {
                                // minus-assign
                                skip_input();
                                current_column += 2;

                                return Token::make(MINUS_EQ, loc);
                            } else {
                                // minus
                                current_column++;
                                return Token::make(MINUS, loc);
                            }
                        case '+':
                            if (peek_input() == '=') {
                                // add-assign
                                skip_input();
                                current_column += 2;

                                return Token::make(PLUS_EQ, loc);
                            } else {
                                // add
                                current_column++;
                                return Token::make(PLUS, loc);
                            }
                        case ')':
                            current_column++;
                            return Token::make(RIGHT_PAREN, loc);
                        case ';':
                            current_column++;
                            return Token::make(SEMICOLON, loc);
                        case '*':
                            if (peek_input() == '=') {
                                // multiplication-assign
                                skip_input();
                                current_column += 2;

                                return Token::make(ASTERISK_EQ, loc);
                            } else {
                                // multiplication
                                current_column++;
                                return Token::make(ASTERISK, loc);
                            }
                        case ',':
                            current_column++;
                            return Token::make(COMMA, loc);
                        case '/':
                            if (peek_input() == '=') {
                                // division-assign
                                skip_input();
                                current_column += 2;

                                return Token::make(DIV_EQ, loc);
                            } else if (peek_input() == '/') {
                                // single line comment
                                skip_input();
                                current_column += 2;

                                // basically ignore until line finishes
                                while (current_char != '\n') {
                                    skip_input();
                                    current_column++; // not used
                                    current_char = peek_input();
                                }
                                continue;
                                break;
                            } else if (peek_input() == '*') {
                                // block comment
                                skip_input();
                                current_column += 2;

                                // TODO: figure out how Rust does nested block comments
                                // ok they do it through a "level" system in their while loop where /*
                                // increments the level and */ decrements it - they keep running loop
                                // until level = 0. Would not work here without some modification due
                                // to not being able to declare variables - maybe have a function to
                                // handle this?

                                current_char = peek_input();

                                int level = 1;
                                while (level > 0) {
                                    skip_input();
                                    current_column++; // for error-handling
                                    current_char = peek_input();

                                    // if /* found
                                    if (current_char == '/') {
                                        if (peek_input(1) == '*') {
                                            // skip /* characters
                                            skip_input(1);

                                            // TODO: ensure correct amount of chars are skipped

                                            level += 1;
                                        }
                                    }

                                    // ignore until */ is found
                                    if (current_char == '*') {
                                        if (peek_input(1) == '/') {
                                            // skip */ characters
                                            skip_input(1);

                                            // TODO: ensure correct amount of chars are skipped
                                            // should only break inner loop here - seems to do so
                                            // break;

                                            level -= 1;
                                        }
                                    }
                                }

                                // refresh new token
                                continue;
                                break;
                            } else {
                                // division
                                current_column++;
                                return Token::make(DIV, loc);
                            }
                        case '%':
                            if (peek_input() == '=') {
                                // modulo-assign
                                current_column += 2;
                                return Token::make(PERCENT_EQ, loc);
                            } else {
                                // modulo
                                current_column++;
                                return Token::make(PERCENT, loc);
                            }
                        case '^':
                            if (peek_input() == '=') {
                                // xor-assign?
                                current_column += 2;
                                return Token::make(CARET_EQ, loc);
                            } else {
                                // xor?
                                current_column++;
                                return Token::make(CARET, loc);
                            }
                        case '<':
                            if (peek_input() == '<') {
                                if (peek_input(1) == '=') {
                                    // left-shift assign
                                    skip_input(1);
                                    current_column += 3;

                                    return Token::make(LEFT_SHIFT_EQ);
                                } else {
                                    // left-shift
                                    skip_input();
                                    current_column += 2;

                                    return Token::make(LEFT_SHIFT);
                                }
                            } else if (peek_input() == '=') {
                                // smaller than or equal to
                                skip_input();
                                current_column += 2;

                                return Token::make(LESS_OR_EQUAL, loc);
                            } else {
                                // smaller than
                                current_column++;
                                return Token::make(LEFT_ANGLE, loc);
                            }
                            break;
                        case '>':
                            if (peek_input() == '>') {
                                if (peek_input(1) == '=') {
                                    // right-shift-assign
                                    skip_input(1);
                                    current_column += 3;

                                    return Token::make(RIGHT_SHIFT_EQ, loc);
                                } else {
                                    // right-shift
                                    skip_input();
                                    current_column += 2;

                                    return Token::make(RIGHT_SHIFT, loc);
                                }
                            } else if (peek_input() == '=') {
                                // larger than or equal to
                                skip_input();
                                current_column += 2;

                                return Token::make(GREATER_OR_EQUAL, loc);
                            } else {
                                // larger than
                                current_column++;
                                return Token::make(RIGHT_ANGLE, loc);
                            }
                        case ':':
                            if (peek_input() == ':') {
                                // scope resolution ::
                                skip_input();
                                current_column += 2;

                                return Token::make(SCOPE_RESOLUTION, loc);
                            } else {
                                // single colon :
                                current_column++;
                                return Token::make(COLON, loc);
                            }
                        case '!':
                            // TODO: special handling for macros in lexer?
                            if (peek_input() == '=') {
                                // not equal boolean operator
                                skip_input();
                                current_column += 2;

                                return Token::make(NOT_EQUAL, loc);
                            } else {
                                // not equal unary operator
                                current_column++;

                                return Token::make(EXCLAM, loc);
                            }
                        case '?':
                            // TODO: special handling for end of expression vs ternary operator?
                            // does rust have a ternary operator?
                            current_column++;
                            return Token::make(QUESTION_MARK, loc);
                        case '#':
                            current_column++;
                            return Token::make(HASH, loc);
                        case '[':
                            current_column++;
                            return Token::make(LEFT_SQUARE, loc);
                        case ']':
                            current_column++;
                            return Token::make(RIGHT_SQUARE, loc);
                        case '{':
                            current_column++;
                            return Token::make(LEFT_CURLY, loc);
                        case '}':
                            current_column++;
                            return Token::make(RIGHT_CURLY, loc);
                        case '@':
                            // TODO: i don't know what this does, does it need special handling?
                            current_column++;
                            return Token::make(PATTERN_BIND, loc);
                        case '$':
                            // TODO: i don't know what this does, does it need special handling?
                            current_column++;
                            return Token::make(DOLLAR_SIGN, loc);
                        case '~':
                            // TODO: i don't know what this does, does it need special handling?
                            current_column++;
                            return Token::make(TILDE, loc);
                        case '\\':
                            // TODO: i don't know what this does, does it need special handling?
                            current_column++;
                            return Token::make(BACKSLASH, loc);
                        case '`':
                            // TODO: i don't know what this does, does it need special handling?
                            current_column++;
                            return Token::make(BACKTICK, loc);
                        case '|':
                            if (peek_input() == '=') {
                                // bitwise or-assign?
                                skip_input();
                                current_column += 2;

                                return Token::make(PIPE_EQ, loc);
                            } else if (peek_input() == '|') {
                                // logical or
                                skip_input();
                                current_column += 2;

                                return Token::make(OR, loc);
                            } else {
                                // bitwise or
                                current_column++;

                                return Token::make(PIPE, loc);
                            }
                        case '&':
                            if (peek_input() == '=') {
                                // bitwise and-assign?
                                skip_input();
                                current_column += 2;

                                return Token::make(AMP_EQ, loc);
                            } else if (peek_input() == '&') {
                                // logical and
                                skip_input();
                                current_column += 2;

                                return Token::make(LOGICAL_AND, loc);
                            } else {
                                // bitwise and/reference
                                current_column++;

                                return Token::make(AMP, loc);
                            }
                        case '.':
                            if (peek_input() == '.') {
                                if (peek_input(1) == '.') {
                                    // ellipsis
                                    skip_input(1);
                                    current_column += 3;

                                    return Token::make(ELLIPSIS, loc);
                                } else if (peek_input(1) == '=') {
                                    // ..=
                                    skip_input(1);
                                    current_column += 3;

                                    return Token::make(DOT_DOT_EQ, loc);
                                } else {
                                    // ..
                                    skip_input();
                                    current_column += 2;

                                    return Token::make(DOT_DOT, loc);
                                }
                            } else if (!ISDIGIT(peek_input())) {
                                // single dot .
                                // Only if followed by a non-number
                                current_column++;
                                return Token::make(DOT, loc);
                            }
                    }
                    // TODO: special handling of _ in the lexer? instead of being identifier

                    // byte and byte string test
                    if (current_char == 'b') {
                        if (peek_input() == '\'') {
                            // byte - allows any ascii or escapes
                            // would also have to take into account escapes: \x hex_digit hex_digit,
                            // \n, \r, \t, \\, \0

                            // char to save
                            char byte_char;

                            // make current char the quote character
                            current_char = peek_input();

                            // detect escapes
                            if (peek_input() == '\\') {
                                skip_input();

                                // make current_char next character (letter)
                                current_char = peek_input();

                                switch (current_char) {
                                    case 'n':
                                        byte_char = '\n';
                                        break;
                                    case 'r':
                                        byte_char = '\r';
                                        break;
                                    case 't':
                                        byte_char = '\t';
                                        break;
                                    case '\\':
                                        byte_char = '\\';
                                        break;
                                    case '0':
                                        byte_char = '\0';
                                        break;
                                    case 'x':
                                        // handle hex digit? or should this be done in parser?
                                        break;
                                    default:
                                        break;
                                }
                            } else {
                                // otherwise, get character from direct input character
                                byte_char = peek_input();
                            }

                            return Token::make_byte_char(loc, byte_char);
                        } else if (peek_input() == '"') {
                            // byte string

                            // skip quote character
                            skip_input();

                            std::string str;
                            str.reserve(16); // some sensible default

                            int length = 1;
                            current_char = peek_input();
                            // TODO: handle escapes properly
                            while (current_char != '\n' && current_char != '"') {
                                length++;

                                str += current_char;
                                skip_input();
                                current_char = peek_input();
                            }
                            // test handling escape:
                            while (true) {
                                if (current_char == '"') {
                                    break;
                                }

                                if (current_char == '\\') {
                                    skip_input();

                                    current_char = peek_input();

                                    switch (current_char) {}

                                    continue;
                                }

                                length++;

                                str += current_char;
                                skip_input();
                                current_char = peek_input();
                            }

                            current_column += length;

                            if (current_char == '\n') {
                                error_at(get_current_location(), "unended string literal");
                            } else if (current_char == '"') {
                                skip_input();
                            } else {
                                gcc_unreachable();
                            }

                            return Token::make_byte_string(loc, str);
                            // TODO: account for escapes and string continue
                        }
                    }

                    // find identifiers and keywords
                    if (ISALPHA(current_char)
                        || current_char == '_') { // is alphanumeric or _ (maybe just letters)
                        std::string str;
                        str.reserve(16); // default
                        str += current_char;

                        int length = 1;
                        current_char = peek_input();
                        // loop through entire name
                        while (
                          ISALPHA(current_char) || ISDIGIT(current_char) || current_char == '_') {
                            length++;

                            str += current_char;
                            skip_input();
                            current_char = peek_input();
                        }

                        current_column += length;

                        TokenId keyword = classify_keyword(str);
                        if (keyword == IDENTIFIER) {
                            return Token::make_identifier(loc, str);
                        } else {
                            return Token::make(keyword, loc);
                        }
                    }

                    // identify literals
                    // int or float literals - not processed properly
                    if (ISDIGIT(current_char)
                        || current_char == '.') { // assuming _ not allowed as first char
                        std::string str;
                        str.reserve(16); // some sensible default
                        str += current_char;

                        PrimitiveCoreType type_hint = CORETYPE_UNKNOWN;

                        bool is_real = (current_char == '.');

                        int length = 1;

                        // handle binary, octal, hex literals
                        if (current_char == '0' && !ISDIGIT(peek_input())) {
                            current_char = peek_input();

                            if (current_char == 'x') {
                                // hex (integer only)

                                skip_input();
                                current_char = peek_input();

                                length++;

                                // add 'x' to string after 0 so it is 0xFFAA or whatever
                                str += 'x';

                                // loop through to add entire hex number to string
                                while (is_x_digit(current_char) || current_char == '_') {
                                    if (current_char == '_') {
                                        // don't add _ to number
                                        skip_input();
                                        current_char = peek_input();

                                        length++;

                                        continue;
                                    }

                                    length++;

                                    // add raw hex numbers
                                    str += current_char;
                                    skip_input();
                                    current_char = peek_input();
                                }

                                current_column += length;

                                // convert hex value to decimal representation
                                long hex_num = ::std::strtol(str.c_str(), NULL, 16);

                                // create output string stream for hex value to be converted to string
                                // again
                                // TODO: if too slow, use sprintf
                                ::std::ostringstream ostr;
                                ostr << hex_num;

                                // reassign string representation to converted value
                                str = ostr.str();
                            } else if (current_char == 'o') {
                                // octal (integer only)

                                skip_input();
                                current_char = peek_input();

                                length++;

                                // don't add any characters as C octals are just 0124 or whatever

                                // loop through to add entire octal number to string
                                while (is_octal_digit(current_char) || current_char == '_') {
                                    if (current_char == '_') {
                                        // don't add _ to number
                                        skip_input();
                                        current_char = peek_input();

                                        length++;

                                        continue;
                                    }

                                    length++;

                                    // add raw octal numbers
                                    str += current_char;
                                    skip_input();
                                    current_char = peek_input();
                                }

                                current_column += length;

                                // convert octal value to decimal representation
                                long octal_num = ::std::strtol(str.c_str(), NULL, 8);

                                // create output string stream for octal value to be converted to
                                // string again
                                // TODO: if too slow, use sprintf
                                ::std::ostringstream ostr;
                                ostr << octal_num;

                                // reassign string representation to converted value
                                str = ostr.str();
                            } else if (current_char == 'b') {
                                // binary (integer only)

                                skip_input();
                                current_char = peek_input();

                                length++;

                                // don't add any characters as C binary numbers are not really
                                // supported

                                // loop through to add entire binary number to string
                                while (is_bin_digit(current_char) || current_char == '_') {
                                    if (current_char == '_') {
                                        // don't add _ to number
                                        skip_input();
                                        current_char = peek_input();

                                        length++;

                                        continue;
                                    }

                                    length++;

                                    // add raw binary numbers
                                    str += current_char;
                                    skip_input();
                                    current_char = peek_input();
                                }

                                current_column += length;

                                // convert binary value to decimal representation
                                long bin_num = ::std::strtol(str.c_str(), NULL, 2);

                                // create output string stream for binary value to be converted to
                                // string again
                                // TODO: if too slow, use sprintf
                                ::std::ostringstream ostr;
                                ostr << bin_num;

                                // reassign string representation to converted value
                                str = ostr.str();
                            }
                        } else {
                            // handle decimals (integer or float)

                            current_char = peek_input();

                            /* only allow a single '.' char in the number - if there are multiple, it
                             * may be a .. or ... token. Also only allow one . at all so have is_real
                             * set after. */
                            /*while (
                              is_float_digit(current_char)
                              || (!is_real && current_char == '.' && is_float_digit(peek_input(1)))) {
                                if (current_char == '_') {
                                    // don't add _ to number
                                    skip_input();
                                    current_char = peek_input();

                                    continue;
                                }

                                if (current_char == 'e' || current_char == 'E') {
                                    if
                                }

                                length++;

                                is_real = is_real || (current_char == '.');

                                str += current_char;
                                skip_input();
                                current_char = peek_input();
                            }*/

                            // parse initial decimal literal - assuming integer
                            // TODO: test if works
                            parse_in_decimal(current_char, str, length);
#if 0
                            while (ISDIGIT(current_char) || current_char == '_') {
                                if (current_char == '_') {
                                    // don't add _ to number
                                    skip_input();
                                    current_char = peek_input();

                                    length++;

                                    continue;
                                }

                                length++;

                                str += current_char;
                                skip_input();
                                current_char = peek_input();
                            }
#endif

                            // detect float literal
                            if (current_char == '.' && is_float_digit(peek_input(1))) {
                                // float with a '.', parse another decimal into it

                                is_real = true;

                                // add . to str
                                str += current_char;
                                skip_input();
                                current_char = peek_input();

                                length++;

                                // parse another decimal number for float
                                // TODO: test if works
                                parse_in_decimal(current_char, str, length);
#if 0
                                while (ISDIGIT(current_char) || current_char == '_') {
                                    if (current_char == '_') {
                                        // don't add _ to number
                                        skip_input();
                                        current_char = peek_input();

                                        continue;
                                    }

                                    length++;

                                    str += current_char;
                                    skip_input();
                                    current_char = peek_input();
                                }
#endif

                                // parse in exponent part if it exists
                                // test to see if this works:
                                parse_in_exponent_part(current_char, str, length);
#if 0
                                if (current_char == 'E' || current_char == 'e') {
                                    // add exponent to string as strtod works with it
                                    str += current_char;
                                    skip_input();
                                    current_char = peek_input();

                                    // special - and + handling
                                    if (current_char == '-') {
                                        str += '-';

                                        skip_input();
                                        current_char = peek_input();
                                    } else if (current_char == '+') {
                                        // don't add + but still skip input
                                        skip_input();
                                        current_char = peek_input();
                                    }

                                    // parse another decimal number for exponent
                                    while (ISDIGIT(current_char) || current_char == '_') {
                                        if (current_char == '_') {
                                            // don't add _ to number
                                            skip_input();
                                            current_char = peek_input();

                                            continue;
                                        }

                                        length++;

                                        str += current_char;
                                        skip_input();
                                        current_char = peek_input();
                                    }
                                }
#endif

                                // parse in type suffix if it exists
                                // TODO: see if works:
                                parse_in_type_suffix(current_char, type_hint, length);
#if 0
                                if (current_char == 'f') {
                                    skip_input();
                                    current_char = peek_input();

                                    // f32
                                    if (current_char == '3' && peek_input(1) == '2') {
                                        skip_input(1);
                                        current_char = peek_input();

                                        type_hint = CORETYPE_F32;
                                        // f64
                                    } else if (current_char == '6' && peek_input(1) == '4') {
                                        skip_input(1);
                                        current_char = peek_input();

                                        type_hint = CORETYPE_F64;
                                    }
                                }
#endif
                            } else if (current_char == 'E' || current_char == 'e') {
                                is_real = true;

                                // parse exponent part
                                parse_in_exponent_part(current_char, str, length);

                                // parse in type suffix if it exists
                                parse_in_type_suffix(current_char, type_hint, length);
                            } else {
                                // is an integer

                                // parse decimal integer
                                parse_in_decimal(current_char, str, length);

                                // parse in type suffix if it exists
                                parse_in_type_suffix(current_char, type_hint, length);
                            }

                            current_column += length;
                        }

                        // actually make the tokens
                        if (is_real) {
                            return Token::make_float(loc, str, type_hint);
                        } else {
                            return Token::make_int(loc, str, type_hint);
                        }
                    }

                    // string literals - not processed properly
                    if (current_char == '"') {
                        std::string str;
                        str.reserve(16); // some sensible default

                        int length = 1;
                        current_char = peek_input();
                        while (current_char != '\n' && current_char != '"') {
                            length++;

                            str += current_char;
                            skip_input();
                            current_char = peek_input();
                        }

                        current_column += length;

                        if (current_char == '\n') {
                            error_at(get_current_location(), "unended string literal");
                        } else if (current_char == '"') {
                            skip_input();
                        } else {
                            gcc_unreachable();
                        }

                        return Token::make_string(loc, str);
                        // TODO: account for escapes and string continue
                        // also, in rust a string is a series of unicode characters (4 bytes)
                    }

                    // char literal attempt
                    if (current_char == '\'') {
                        // rust chars are 4 bytes and have some weird unicode representation thing
                        uint32_t shit;

                        // also need to account for escapes - quote escape \' and \"
                        // ascii escape: \x octal_digit hex_digit and \n \r \t \\ \0
                        // unicode escape: (i can't even figure out how to represent this)
                    }

                    // TODO: other literals: raw string literal, byte literal, byte string literal,
                    // raw byte string literal, boolean literal raw (don't process escapes):
                    // r#"hello"# - same amount of hashes on either side (or none) byte (ascii char):
                    // b'H' byte string (ascii char array): b"hello" raw byte string (ascii char array
                    // with no escapes): br#"hello"# - also same hash number MAYBE boolean literals -
                    // maybe reserved word impl is enough for now but we'll see

                    // didn't match anything so error
                    error_at(loc, "unexpected character '%x'", current_char);
                    current_column++;
                }
            }
        }
    }

    /* Test handling for byte escape characters? For use in byte char and byte string
     * literals. Invoke with peek_input() returning the '\' character. */
    char Lexer::handle_byte_escape_char() {
        skip_input();

        // make current_char next character (letter)
        char current_char = peek_input();

        switch (current_char) {
            case 'n':
                return '\n';
            case 'r':
                return '\r';
            case 't':
                return '\t';
            case '\\':
                return '\\';
            case '0':
                return '\0';
            case 'x':
                // handle hex digit - presumably just represents ascii char
                // sprintf? don't really know how to do this
                {
                    skip_input();
                    auto hi = peek_input();
                    hi = hi.to_digit(16); // converts to digit of base 16 (hex)

                    skip_input();
                    auto lo = peek_input();
                    lo = lo.to_digit(16); // converts to digit of base 16 (hex)

                    auto value = hi * 16 + lo;

                    if (!mode.is_bytes() && !is_ascii(value)) {
                        // make error here
                    }
                    unsigned char valueNu = value;

                    // ok dodgy
                    return (char)valueNu;
                }
                break;
            default:
                // didn't match anything so error
                error_at(get_current_location(), "invalid escape '\\%x'", current_char);
                return '\0';
        }
    }

    // TODO: fix
    uint32_t Lexer::parse_escape(char enclosing) {
        auto ch = peek_input();
        skip_input();

        switch (ch) {
            case 'x': {
                // this does not work at all: TODO fix when figure out how to use UTF-8 in C++
                ch = peek_input();
                skip_input();
                if (!ISXDIGIT(ch)) {
                    // error_at: found invalid character
                    return 0;
                }
                char tmp[3] = { static_cast<char>(ch), 0, 0 };

                ch = peek_input();
                skip_input();

                if (!ISXDIGIT(ch)) {
                    // error_at: found invalid character
                    return 0;
                }
                tmp[1] = static_cast<char>(ch);
                return ::std::strtol(tmp, NULL, 16);
            } break;
            case 'u': {
                // Unicode (up to 6 hex digits)
            } break;
        }
    }

    // Shitty pass-by-reference way of parsing in type suffix.
    bool Lexer::parse_in_type_suffix(char& current_char, PrimitiveCoreType& type_hint, int& length) {
        ::std::string suffix;
        suffix.reserve(5);

        // get suffix
        while (ISALPHA(current_char) || ISDIGIT(current_char) || current_char == '_') {
            if (current_char == '_') {
                // don't add _ to suffix
                skip_input();
                current_char = peek_input();

                length++;

                continue;
            }

            length++;

            suffix += current_char;
            skip_input();
            current_char = peek_input();
        }

        if (suffix == "f32") {
            type_hint = CORETYPE_F32;
        } else if (suffix == "f64") {
            type_hint = CORETYPE_F64;
        } else if (suffix == "i8") {
            type_hint = CORETYPE_I8;
        } else if (suffix == "i16") {
            type_hint = CORETYPE_I16;
        } else if (suffix == "i32") {
            type_hint = CORETYPE_I32;
        } else if (suffix == "i64") {
            type_hint = CORETYPE_I64;
        } else if (suffix == "i128") {
            type_hint = CORETYPE_I128;
        } else if (suffix == "isize") {
            type_hint = CORETYPE_ISIZE;
        } else if (suffix == "u8") {
            type_hint = CORETYPE_U8;
        } else if (suffix == "u16") {
            type_hint = CORETYPE_U16;
        } else if (suffix == "u32") {
            type_hint = CORETYPE_U32;
        } else if (suffix == "u64") {
            type_hint = CORETYPE_U64;
        } else if (suffix == "u128") {
            type_hint = CORETYPE_U128;
        } else if (suffix == "usize") {
            type_hint = CORETYPE_USIZE;
        } else {
            error_at(get_current_location(), "unknown number suffix '%s'", suffix.c_str());
            return false;
        }

        return true;
    }

    void Lexer::parse_in_exponent_part(char& current_char, std::string& str, int& length) {
        if (current_char == 'E' || current_char == 'e') {
            // add exponent to string as strtod works with it
            str += current_char;
            skip_input();
            current_char = peek_input();

            length++;

            // special - and + handling
            if (current_char == '-') {
                str += '-';

                skip_input();
                current_char = peek_input();

                length++;
            } else if (current_char == '+') {
                // don't add + but still skip input
                skip_input();
                current_char = peek_input();

                length++;
            }

            // parse another decimal number for exponent
            parse_in_decimal(current_char, str, length);

#if 0
            // parse another decimal number for exponent
            while (ISDIGIT(current_char) || current_char == '_') {
                if (current_char == '_') {
                    // don't add _ to number
                    skip_input();
                    current_char = peek_input();

                    length++;

                    continue;
                }

                length++;

                str += current_char;
                skip_input();
                current_char = peek_input();
            }
#endif
        }
    }

    void Lexer::parse_in_decimal(char& current_char, std::string& str, int& length) {
        while (ISDIGIT(current_char) || current_char == '_') {
            if (current_char == '_') {
                // don't add _ to number
                skip_input();
                current_char = peek_input();

                length++;

                continue;
            }

            length++;

            str += current_char;
            skip_input();
            current_char = peek_input();
        }
    }
}