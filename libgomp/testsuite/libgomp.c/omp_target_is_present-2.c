// { dg-do run }

#define REQ_SELF_MAPS 1

#pragma omp requires self_maps

#include "omp_target_is_present.c"
