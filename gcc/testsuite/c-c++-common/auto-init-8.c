/* Verify pattern initialization for array, union, and structure type automatic variables.  */
/* { dg-do compile } */
/* { dg-options "-ftrivial-auto-var-init=pattern -fdump-tree-gimple" } */

struct S
{
  int f1;
  float f2;
  char f3[20];
};

union U
{
  char u1[5];
  int u2;
  float u3; 
};

double result;

double foo()
{
  int temp1[3];
  double temp2[3];
  struct S temp3;
  union U temp4;
  
  result = temp1[2] + temp2[1] + temp3.f2 + temp4.u3;
  return result;
}

/* { dg-final { scan-tree-dump "temp1 = .DEFERRED_INIT \\(12, 1, \&\"temp1\"" "gimple" } } */
/* { dg-final { scan-tree-dump "temp2 = .DEFERRED_INIT \\(24, 1, \&\"temp2\"" "gimple" } } */
/* { dg-final { scan-tree-dump "temp3 = .DEFERRED_INIT \\(28, 1, \&\"temp3\"" "gimple" } } */
/* { dg-final { scan-tree-dump "temp4 = .DEFERRED_INIT \\((8|5), 1, \&\"temp4\"" "gimple" } } */
