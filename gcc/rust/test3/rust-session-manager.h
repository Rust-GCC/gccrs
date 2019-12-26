#ifndef RUST_SESSION_MANAGER_H
#define RUST_SESSION_MANAGER_H
// Session manager - controls compiler session.

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "options.h"
// order: config, system, coretypes, options

#include <string>

namespace Rust {
    // parser forward decl
    class Parser;

    // Defines compiler options (e.g. dump, etc.).
    struct CompileOptions {
        // TODO: use bitfield for smaller memory requirements?

        // FIXME: this is set up for "instead of" dumping - in future, dumps should not inhibit compilation
        enum DumpOptions {
            NO_DUMP,
            LEXER_DUMP,
            PARSER_AST_DUMP,
            LOAD_CRATES,
            // TODO: add more?
        } dump_option;
    };

    // Defines a compiler session.
    struct Session {
        CompileOptions options;

      public:
        /* Initialise compiler session. Corresponds to langhook grs_langhook_init(). Note that this is 
         * called after option handling. */
        void init();
        bool handle_option(enum opt_code code, const char* arg, HOST_WIDE_INT value, int kind,
          location_t loc, const struct cl_option_handlers* handlers);
        void parse_files(int num_files, const char** files);
        void init_options();

      private:
        // TODO: should this be private or public?
        void parse_file(const char* filename);
        bool enable_dump(::std::string arg);

        void debug_dump_load_crates(Parser& parser);
    };
}

#endif