/* Test that _Decimal* keywords are accepted in C2X mode.  */
/* { dg-do compile } */
/* { dg-options "-std=c2x -pedantic-errors" } */

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;
