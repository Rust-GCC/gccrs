/* { dg-require-effective-target vect_double } */

#ifndef TYPE
#define TYPE double
#define FN __builtin_fmin
#endif

#include "vect-fmin-1.c"

/* { dg-final { scan-tree-dump "Detected reduction" "vect" } } */
/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" { target vect_max_reduc } } } */
