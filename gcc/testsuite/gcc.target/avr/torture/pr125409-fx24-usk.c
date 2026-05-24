/* { dg-do link { target { ! avr_tiny } } } */
/* { dg-additional-options "-std=gnu99 -Wno-pedantic" } */

#define TT __uint24
#define UU signed
#define FX _Accum

#include "pr125409.h"
