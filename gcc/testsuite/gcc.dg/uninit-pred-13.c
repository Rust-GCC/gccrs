/* { dg-do compile } */
/* { dg-options "-Wuninitialized -O2" } */
/* { dg-skip-if "tail-merge ifcombine fires on ccmp targets" { ! { aarch64*-*-* || apxf } } } */

/* Tail-merge runs ifcombine to combine the conditions guarding V's definition.
   Check that uninit still proves the guarded uses safe and still warns when V
   can be used uninitialized.  */

int g;
void bar (void);
void blah (int);

/* V is defined only under (n > 10) && l.  The second use is guarded by
   l && n > 12, which implies the definition guard, so no warning must be
   emitted even after the conditions are combined.  */
int
guarded (int n, int l, int m, int r)
{
  int v;

  if (n > 10)
    if (l)
      v = r;

  if (m) g++; else bar ();

  if ((n > 10) && l)
    blah (v); /* { dg-bogus "uninitialized" } */

  if (l)
    if (n > 12)
      blah (v); /* { dg-bogus "uninitialized" } */

  return 0;
}

/* Same shape and the same tail-merge + ifcombine combination, but the second
   use's guard l && n > 8 does not imply the definition guard (n > 10) && l:
   V is uninitialized for n in { 9, 10 }, so it must still warn -- the
   relaxation must not over-suppress.  */
int
unguarded (int n, int l, int m, int r)
{
  int v;

  if (n > 10)
    if (l)
      v = r;

  if (m) g++; else bar ();

  if ((n > 10) && l)
    blah (v); /* { dg-bogus "uninitialized" } */

  if (l)
    if (n > 8)
      blah (v); /* { dg-warning "uninitialized" } */

  return 0;
}
