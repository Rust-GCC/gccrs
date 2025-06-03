/* Test for bogus diagnostics for dremf definition.  Although this
   definition is formally incorrect in ISO C, a GNU extension permits
   a prototype followed by unpromoted types in a function definition,
   so it should be permitted when the function is built in.  Bug
   16666.  Test with -pedantic-errors, where the problem should still
   be diagnosed.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu17 -pedantic-errors" } */

float dremf (float, float); /* { dg-error "prototype declaration" } */

float
dremf (x, y)
     float x; /* { dg-error "promoted argument 'x' doesn't match prototype" } */
     float y; /* { dg-error "promoted argument 'y' doesn't match prototype" } */
{
  return x + y;
}
