/* { dg-do run { target { lp64 && s390_vx } } } */
/* { dg-options "-march=native -O2 -ftree-vectorize -fno-vect-cost-model -ffast-math -fno-trapping-math" } */

/* { dg-additional-options "--param=vect-partial-vector-usage=1" } */

#include "s390-vec-length-run-7.h"

