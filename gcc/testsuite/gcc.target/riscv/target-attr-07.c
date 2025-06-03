/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-flto" } { "" } } */
/* { dg-options "-march=rv64gc_zba -O2 -mabi=lp64 -mtune=rocket" } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** foo:
**   ...
**   # tune = sifive-5-series
**   ...
*/
long foo (long a, long b) __attribute__((target("tune=sifive-5-series")));
long foo (long a, long b)
{
  return a + (b * 2);
}

/*
** bar:
**   ...
**   sh1add\s*a0,a1,a0
**   ...
*/
long bar (long a, long b)
{
  return a + (b * 2);
}
