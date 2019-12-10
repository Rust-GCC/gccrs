/* { dg-do compile } */
/* { dg-prune-output "compilation terminated" } */

#include <arm_sve.h>

#pragma GCC target "+nosve"

void take_svuint8_eventually (float, float, float, float,
			      float, float, float, float, svuint8_t);

void
f (svuint8_t *ptr)
{
  take_svuint8_eventually (0, 0, 0, 0, 0, 0, 0, 0, *ptr); /* { dg-error {arguments of type '(svuint8_t|__SVUint8_t)' require the SVE ISA extension} } */
}
