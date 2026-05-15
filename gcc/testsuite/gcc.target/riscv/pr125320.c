/* { dg-do assemble } */
/* { dg-options "-O2 -mabi=lp64d -march=rv64gc_xtheadmemidx -fstack-protector-strong" } */

void __gen_tempname (char *, int, int, int);
void
tempnam ()
{
  char buf[4096];
  __gen_tempname (buf, 0, 0, 2);
}
