#include "analyzer-decls.h"

static int __attribute__((noipa))
as_int (unsigned char c)
{
  return c;
}

void test_1 (unsigned char c)
{
  __analyzer_eval (as_int (c) >= 0); /* { dg-warning "TRUE" } */
  __analyzer_eval (as_int (c) < 256); /* { dg-warning "TRUE" } */
}
