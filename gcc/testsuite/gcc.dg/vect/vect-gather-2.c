/* { dg-do compile } */

#define N 16

void
f1 (int *restrict y, int *restrict x1, int *restrict x2,
    int *restrict indices)
{
  for (int i = 0; i < N; ++i)
    {
      /* Different base.  */
      y[i * 2] = x1[indices[i * 2]] + 1;
      y[i * 2 + 1] = x2[indices[i * 2 + 1]] + 2;
    }
}

void
f2 (int *restrict y, int *restrict x, int *restrict indices)
{
  for (int i = 0; i < N; ++i)
    {
      /* Different scale.  */
      y[i * 2] = *(int *)((char *)x + (__UINTPTR_TYPE__)indices[i * 2] * 4) + 1;
      y[i * 2 + 1] = *(int *)((char *)x + (__UINTPTR_TYPE__)indices[i * 2 + 1] * 2) + 2;
    }
}

void
f3 (int *restrict y, int *restrict x, int *restrict indices)
{
  for (int i = 0; i < N; ++i)
    {
      /* Different type.  */
      y[i * 2] = x[indices[i * 2]] + 1;
      y[i * 2 + 1] = x[((unsigned int *) indices)[i * 2 + 1]] + 2;
    }
}

/* { dg-final { scan-tree-dump "different gather base" vect { target { ! vect_gather_load_ifn } } } } */
/* { dg-final { scan-tree-dump "different gather scale" vect { target { ! vect_gather_load_ifn } } } } */
