/* { dg-do compile } */
/* { dg-options "-std=gnu17 -pedantic" } */

/* Decimal float is a GCC extension before C23.  */

_Decimal32 x;		/* { dg-warning "GCC extension|ISO C" } */
_Decimal64 y;		/* { dg-warning "GCC extension|ISO C" } */
_Decimal128 z;		/* { dg-warning "GCC extension|ISO C" } */
