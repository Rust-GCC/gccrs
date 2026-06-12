/* { dg-do compile } */
/* { dg-options "-O3 -mcpu=c1-ultra -mautovec-preference=sve-only" } */

_BitInt(256) b;

void
foo()
{
  for (;;)
    b ^= 0x100000000020000000000000000000000000000000000wb;
}
