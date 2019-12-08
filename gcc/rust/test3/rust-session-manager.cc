#include "rust-session-manager.h"

#include "diagnostic.h"

#include "rust-lex.h"
#include "rust-parse.h"

namespace Rust {
    void Session::init() {
        // nothing yet
    }

    // Initialise default options. Actually called before handle_option, unlike init itself.
    void Session::init_options() {
        options.dump_option = CompileOptions::NO_DUMP;
    }

    // Handle option selection.
    bool Session::handle_option(enum opt_code code, const char* arg, HOST_WIDE_INT value ATTRIBUTE_UNUSED, int kind ATTRIBUTE_UNUSED,
      location_t loc ATTRIBUTE_UNUSED, const struct cl_option_handlers* handlers ATTRIBUTE_UNUSED) {
        // used to store whether results of various stuff are successful
        bool ret = true;

        // Handles options as listed in lang.opt.
        switch (code) {
            case OPT_I:
                // TODO: add search path
                break;
            case OPT_L:
                // TODO: add library link path or something
                break;
            case OPT_frust_dump_:
                // enable dump and return whether this was successful
                if (arg != NULL) {
                    ret = enable_dump(::std::string(arg));
                } else {
                    ret = false;
                }
                break;
            // no option handling for -o
            default:
                // return 1 to indicate option is valid
                break;
        }

        return ret;
    }

    /* Enables a certain dump depending on the name passed in. Returns true if name is valid, false 
     * otherwise. */
    bool Session::enable_dump(::std::string arg) {
        // FIXME: change dumping algorithm when new non-inhibiting dump system is created
        if (arg == "all") {
            error_at(UNKNOWN_LOCATION, "dumping all is not supported as of now. choose 'lex' or 'parse'");
            return false;
        } else if (arg == "lex") {
            options.dump_option = CompileOptions::LEXER_DUMP;
        } else if (arg == "parse") {
            options.dump_option = CompileOptions::PARSER_AST_DUMP;
        } else if (arg == "") {
            error_at(UNKNOWN_LOCATION, "dump option was not given a name. choose 'lex' or 'parse'");
            return false;
        } else {
            error_at(UNKNOWN_LOCATION, "dump option '%s' was unrecognised. choose 'lex' or 'parse'", arg.c_str());
            return false;
        }
        return true;
    }

    /* Actual main entry point for front-end. Called from langhook to parse files. */
    void Session::parse_files(int num_files, const char** files) {
        for (int i = 0; i < num_files; i++) {
            parse_file(files[i]);
        }
    }

    // Parses a single file with filename filename.
    void Session::parse_file(const char* filename) {
        FILE* file = fopen(filename, "r");

        if (file == NULL) {
            fatal_error(UNKNOWN_LOCATION, "cannot open filename %s: %m", filename);
        }

        // parse file here
        // create lexer and parser - these are file-specific and so aren't instance variables
        Rust::Lexer lex(filename, file);
        Rust::Parser parser(lex);

        // determine parsing method from options
        /* FIXME: currently, the dump means that full compilation will not occur as of present. In future,
         * dumps should not inhibit full compilation. */
        switch (options.dump_option) {
            case CompileOptions::NO_DUMP:
                fatal_error(UNKNOWN_LOCATION, "no-dump parsing has not been enabled yet");
                break;
            case CompileOptions::LEXER_DUMP:
                parser.debug_dump_lex_output();
                break;
            case CompileOptions::PARSER_AST_DUMP:
                parser.debug_dump_ast_output();
                break;
            // semantic analysis when completed
            default:
                fatal_error(UNKNOWN_LOCATION, "unrecognised dump option: '%u'", options.dump_option);
                break;
        }

        fclose(file);
    }
}