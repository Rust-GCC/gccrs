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
    // crate forward decl
    namespace AST {
        struct Crate;
    }

    // Defines compiler options (e.g. dump, etc.).
    struct CompileOptions {
        // TODO: use bitfield for smaller memory requirements?

        // FIXME: this is set up for "instead of" dumping - in future, dumps should not inhibit compilation
        enum DumpOptions {
            NO_DUMP,
            LEXER_DUMP,
            PARSER_AST_DUMP,
            REGISTER_PLUGINS_DUMP,
            INJECTION_DUMP,
            EXPANSION_DUMP,
            NAME_RESOLUTION_DUMP,
            // TODO: add more?
        } dump_option;
    };

    /* Defines a compiler session. This is for a single compiler invocation, so potentially includes 
     * parsing multiple crates. */
    struct Session {
        CompileOptions options;
        // This should really be in a per-crate storage area but it is wiped with every file so eh.
        ::std::string injected_crate_name;

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

        // pipeline stages - TODO maybe move?
        /* Register plugins pipeline stage. TODO maybe move to another object? Currently dummy stage. In
         * future will handle attribute injection (top-level inner attribute creation from command line
         * arguments), setting options maybe, registering lints maybe, loading plugins maybe. */
        void register_plugins(AST::Crate& crate);
        /* Injection pipeline stage. TODO maybe move to another object? Maybe have some lint checks (in
         * future, obviously), register builtin macros, crate injection. */
        void injection(AST::Crate& crate);
        /* Expansion pipeline stage. TODO maybe move to another object? Expands all macros, maybe
         * build test harness in future, AST validation, maybe create macro crate (if not rustdoc).*/
        void expansion(AST::Crate& crate);
        /* Name resolution pipeline stage. TODO maybe move to another object. Performs name resolution,
         * maybe complete gated feature checking, maybe create buffered lints in future. */
        void name_resolution(AST::Crate& crate);
    };
}

#endif