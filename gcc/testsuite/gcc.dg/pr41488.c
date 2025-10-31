/* { dg-do compile } */
/* { dg-options "-O2 -fno-tree-scev-cprop -fdump-tree-ivcanon-scev" } */

struct struct_t
{
  int* data;
};

void foo (struct struct_t* sp, int start, int end)
{
  int i;

  for (i = 0; i+start < end; i++)
    sp->data[i+start] = 0;
}

/* { dg-final { scan-tree-dump-times "Simplify PEELED_CHREC into POLYNOMIAL_CHREC" 1 "ivcanon" } } */
