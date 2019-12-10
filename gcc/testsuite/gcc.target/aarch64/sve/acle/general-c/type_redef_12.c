/* { dg-do compile } */

/* This isn't explicitly allowed or disallowed, but mustn't ICE.  */
struct svint8x2_t { int x; };

#pragma GCC aarch64 "arm_sve.h"

void
f (svint8x2_t *a, struct svint8x2_t *b)
{
  *a = *b; /* { dg-error {incompatible types} } */
}
