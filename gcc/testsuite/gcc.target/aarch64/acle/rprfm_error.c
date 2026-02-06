/* { dg-do compile } */
/* { dg-options "-march=armv8-a -O2" } */

#include <arm_acle.h>

/* Access kind specifiers.  */
#define PLD 0
#define PST 1
/* Retention policies.  */
#define KEEP 0
#define STRM 1

int test (void *a, uint64_t m) {
  __pld_range (2, KEEP, m, a);                        /* { dg-error "argument 1 must be a constant immediate in range \\\[0,1\\\]" } */
  __pld_range (-1, KEEP, m, a);                       /* { dg-error "argument 1 must be a constant immediate in range \\\[0,1\\\]" } */
  __pld_range (PLD, 2, m, a);                         /* { dg-error "argument 2 must be a constant immediate in range \\\[0,1\\\]" } */
  __pld_range (PLD, -1, m, a);                        /* { dg-error "argument 2 must be a constant immediate in range \\\[0,1\\\]" } */

  __pldx_range (2, KEEP, 0, 1, 0, 0, a);              /* { dg-error "argument 1 must be a constant immediate in range \\\[0,1\\\]" } */
  __pldx_range (PLD, 2, 0, 1, 0, 0, a);               /* { dg-error "argument 2 must be a constant immediate in range \\\[0,1\\\]" } */
  __pldx_range (-1, KEEP, 0, 1, 0, 0, a);             /* { dg-error "argument 1 must be a constant immediate in range \\\[0,1\\\]" } */
  __pldx_range (PLD, -1, 0, 1, 0, 0, a);              /* { dg-error "argument 2 must be a constant immediate in range \\\[0,1\\\]" } */

  __pldx_range (PLD, KEEP, -(1<<21) - 1, 1, 0, 0, a); /* { dg-error "argument 3 must be a constant immediate in range \\\[-2097152,2097151\\\]" } */
  __pldx_range (PLD, KEEP, (1<<21), 1, 0, 0, a);      /* { dg-error "argument 3 must be a constant immediate in range \\\[-2097152,2097151\\\]" } */
  __pldx_range (PLD, KEEP, 0, 0, 0, 0, a);            /* { dg-error "argument 4 must be a constant immediate in range \\\[1,65536\\\]" } */
  __pldx_range (PLD, KEEP, 0, (1<<16) + 1, 0, 0, a);  /* { dg-error "argument 4 must be a constant immediate in range \\\[1,65536\\\]" } */
  __pldx_range (PLD, KEEP, 0, 1, -(1<<21)-1, 0, a);   /* { dg-error "argument 5 must be a constant immediate in range \\\[-2097152,2097151\\\]" } */
  __pldx_range (PLD, KEEP, 0, 1, (1<<21), 0, a);      /* { dg-error "argument 5 must be a constant immediate in range \\\[-2097152,2097151\\\]" } */
  __pldx_range (PLD, KEEP, 0, 1, 0, -1, a);     /* { dg-error "argument 6 must be a constant immediate in range \\\[0,9223372036854775806\\\]" } */
}
