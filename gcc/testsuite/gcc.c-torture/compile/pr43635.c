/* { dg-require-effective-target untyped_assembly } */
/* { dg-require-effective-target indirect_calls } */

extern void d (void);

void (*foo (void)) (float)
{
  void (*(*x) (void)) (float) = d;
  return (*x) ();
}
