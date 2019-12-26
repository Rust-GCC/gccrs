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
        } else if (arg == "load-crates") {
            options.dump_option = CompileOptions::LOAD_CRATES;
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
        // TODO: should semantic analysis be dealed with here? or per file? for now, per-file.
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
            case CompileOptions::LOAD_CRATES:
                debug_dump_load_crates(parser);
                break;
            // semantic analysis when completed
            default:
                fatal_error(UNKNOWN_LOCATION, "unrecognised dump option: '%u'", options.dump_option);
                break;
        }

        fclose(file);
    }

    // Checks whether 'cfg' attribute prevents compilation.
    bool check_cfg(const AST::Attribute& attr) {
        // if "has sub items", and if 'cfg' attr, recursively call this on sub items?

        // TODO: actually implement. assume true for now

        return true;
    }

    // Checks whether any 'cfg' attribute on the item prevents compilation of that item.
    bool check_item_cfg(::std::vector<AST::Attribute> attrs) {
        for (const auto& attr : attrs) {
            if (attr.get_path() == "cfg" && !check_cfg(attr)) {
                return false;
            }
        }

        return true;
    }

    // TODO: actually implement method
    void load_extern_crate(::std::string crate_name) {}

    // Parses up to the "load (external) crates" part of the frontend. 
    // TODO: lots of this code is probably actually useful outside of dumping, so maybe split off function
    void Session::debug_dump_load_crates(Parser& parser) {
        // parse crate as AST
        AST::Crate crate = parser.parse_crate();

        ::std::vector< ::std::string> crate_names;
        for (const auto& item : crate.items) {
            // if item is extern crate, add name? to list of stuff ONLY IF config is checked
            // if item is module, iterate this loop inside it as well (recursive?) ONLY IF config is checked

            // TODO: actually do the checks somewhere - probably in the items

            item->add_crate_name(crate_names);
        }

        /* loop through list of crate names/paths/whatever, attempting to load each one. save loaded crates
         * to a Session variable? Or save to current AST::Crate? */
        for (const auto& name : crate_names) {
            load_extern_crate(name/*, basename = ""?*/);
        }
        //  for each loaded crate, load dependencies of it as well
    }

    // NOTEs:
    /* mrustc compile pipeline:
     *  - target load (pass target spec to parser?)
     *  - parse (convert source to AST)
     *  - load crates (load any explicitly mentioned extern crates [not all of them]) 
     *  - expand (AST transformations from attributes and macros, loads remaining extern crates [std/core 
     *    and any triggered by macro expansion]) 
     *  - implicit crates (test harness, allocator crate, panic crate)
     *  - resolve use (annotate every 'use' item with source [supposedly handles nasty recursion])
     *  - resolve index (generate index of visible items for every module [avoids recursion in next pass])
     *  - resolve absolute (resolve all paths into either variable names [types/values] or absolute paths)
     *  - HIR lower (convert modified AST to simpler HIR [both expressions and module tree]) 
     *  - resolve type aliases (replace any usages of type aliases with actual type [except associated 
     *    types])
     *  - resolve bind (iterate HIR tree and set binding annotations on all concrete types [avoids path lookups later])
     *  - resolve HIR markings (generate "markings" [e.g. for Copy/Send/Sync/...] for all types 
     *  - sort impls (small pass - sort impls into groups) 
     *  - resolve UFCS outer (determine source trait for all top-level <T>::Type [qualified] paths)
     *  - resolve UFCS paths (do the same, but include for exprs this time. also normalises results of 
     *    previous pass [expanding known associated types]) 
     *  - constant evaluate (evaluate all constants)
     *  - typecheck outer (checks impls are sane)
     *  - typecheck expressions (resolve and check types for all exprs)
     *  - expand HIR annotate (annotate how exprs are used - used for closure extractions and reborrows)
     *  - expand HIR closures (extract closures into structs implementing Fn* traits)
     *  - expand HIR vtables (generate vtables for types with dyn dispatch)
     *  - expand HIR calls (converts method and callable calls into explicit function calls)
     *  - expand HIR reborrows (apply reborrow rules [taking '&mut *v' instead of 'v'])
     *  - expand HIR erasedtype (replace all erased types 'impl Trait' with the true type)
     *  - typecheck expressions (validate - double check that previous passes haven't broke type system 
     *    rules)
     *  - lower MIR (convert HIR exprs into a control-flow graph [MIR])
     *  - MIR validate (check that the generated MIR is consistent)
     *  - MIR cleanup (perform various transformations on MIR - replace reads of const items with the item
     *    itself; convert casts to unsized types into 'MakeDst' operations)
     *  - MIR optimise (perform various simple optimisations on the MIR - constant propagation, dead code
     *    elimination, borrow elimination, some inlining)
     *  - MIR validate PO (re-validate the MIR)
     *  - MIR validate full (optionally: perform expensive state-tracking validation on MIR)
     *  - trans enumerate (enumerate all items needed for code generation, primarily types used for generics)
     *  - trans auto impls (create magic trait impls as enumerated in previous pass)
     *  - trans monomorph (generate monomorphised copies of all functions [with generics replaced with real types])
     *  - MIR optimise inline (run optimisation again, this time with full type info [primarily for inlining])
     *  - HIR serialise (write out HIR dump [module tree and generic/inline MIR])
     *  - trans codegen (generate final output file: emit C source file and call C compiler) */

    /* rustc compile pipeline (basic, in way less detail):
     *  - parse input (parse .rs to AST)
     *  - name resolution, macro expansion, and configuration (process AST recursively, resolving paths,
     *    expanding macros, processing #[cfg] nodes [i.e. maybe stripping stuff from AST])
     *  - lower to HIR 
     *  - type check and other analyses (e.g. privacy checking)
     *  - lower to MIR and post-processing (and do stuff like borrow checking)
     *  - translation to LLVM IR and LLVM optimisations (produce the .o files)
     *  - linking (link together .o files) */
}