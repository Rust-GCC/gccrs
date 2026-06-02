/* { dg-do compile } */
/* { dg-require-effective-target c99_runtime } */
/* { dg-options "-O -g -fdump-tree-backprop-details" }  */

#include <math.h>
#include <errno.h>

int errno_cache;

/* Test an update that includes vops.  */
#define TEST_FUNCTION(TYPE, SUFFIX)				\
  TYPE								\
  test##SUFFIX (TYPE x, int sel)				\
  {								\
    TYPE input = __builtin_fabs##SUFFIX (x);			\
    TYPE res;							\
    if (sel)							\
      res = sin##SUFFIX (input);				\
    else							\
      res = sinh##SUFFIX (input);				\
    errno_cache = errno;					\
    return __builtin_fabs##SUFFIX (res);			\
  }

TEST_FUNCTION (float, f)
TEST_FUNCTION (double, )
TEST_FUNCTION (long double, l)

/* { dg-final { scan-tree-dump-times {Deleting[^\n]* = ABS_EXPR <x} 3 "backprop" } } */
