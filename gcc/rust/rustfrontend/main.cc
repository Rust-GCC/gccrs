#include "rust-system.h"
#include "rust-c.h"

#include "rdot.h"
#include "rs-parser.h"

extern FILE* yyin;

void rust_parse_input_files (const char** in,
                             unsigned int n,
                             bool only_check_syntax)
{
    size_t i;
    for (i = 0; i < n; ++i)
    {
        printf("found input: %s", in[i]);

        yyin = fopen(in[i], "rb");
        if (yyin == NULL) {
            fatal_error(UNKNOWN_LOCATION, "FAILED TO OPEN %s", in[i]);
            return;
        }

        yyparse();
    }

    rdot_pretty_print("test.out", CompilationUnitAst::get().getNodes());
}

