#include "rust-system.h"
#include "rust-c.h"

#include "rdot.h"
#include "rs-parser.h"
#include "node.h"

extern FILE* yyin;

void rust_parse_input_files (const char** in,
                             unsigned int n,
                             bool only_check_syntax)
{
    int verbose = 1;
    parser_init(verbose);

    int ret = 0;
    
    size_t i;
    for (i = 0; i < n; ++i)
    {
        printf("found input: %s\n", in[i]);

        yyin = fopen(in[i], "rb");
        if (yyin == NULL) {
            fatal_error(UNKNOWN_LOCATION, "FAILED TO OPEN %s", in[i]);
            return;
        }

        ret = yyparse();
    }

    printf("--- PARSE COMPLETE: ret:%d, n_nodes:%d ---\n", ret, n_nodes);
    if (nodes) {	
        print_node(nodes, 0);	
    }

    // FIXME double-free exists here
    // struct node *tmp;
    // while (nodes) {	
    //     tmp = nodes;
    //     nodes = tmp->next;	
    //     if (tmp->own_string) {	
    //         free((void*)tmp->name);
    //     }
    //     free(tmp);	
    // }

    if (only_check_syntax)
        return;

    // convert to RDOT

    // do type inferance

    // convert to GENERIC tree's
}

