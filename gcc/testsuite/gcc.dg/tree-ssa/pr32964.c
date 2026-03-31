/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized" } */
/* We don't get the expected optimization any more when x87's excess precision
   is available.  */
/* { dg-additional-options "-msse -mfpmath=sse" { target x86 } } */

union A
{
 float a;
};

float t(float a)
{
  union A a1, a2, a3;
  int i;

  a1.a = a;
  for(i = 0; i<100; i++)
    {
      a2 = a1;
      a2.a += a;
      a1 = a2;
  }
  a3 = a1;
  return a3.a;
}

/* { dg-final { scan-tree-dump-times "union" 0 "optimized"} } */
