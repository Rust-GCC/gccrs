/* { dg-do compile } */
/* { dg-options "-O -Wuninitialized -ftrivial-auto-var-init=zero" } */

typedef _Complex float C;
C foo(int cond, float r)
{
  C f;
  __imag__ f = 0;
  if (cond)
    __real__ f = r; /* No constant to avoid a CCP induced false negative. */
  return f;	/* { dg-warning "may be used" "unconditional" } */
}
