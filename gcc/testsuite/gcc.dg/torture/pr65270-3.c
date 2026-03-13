/* { dg-do run } */

typedef int v4si_unaligned __attribute__((vector_size(16),aligned(4)));
typedef int v4si __attribute__((vector_size(16)));

v4si __attribute__((noipa))
foo (bool aligned, bool write, int *p)
{
  if (write)
    *(v4si_unaligned *)p = (v4si){ 0, 0, 0, 0 };
  v4si tem;
  if (aligned)
    tem = *(v4si *)p;
  else
    tem = *(v4si_unaligned *)p;
  return tem;
}

int a[8] __attribute__((aligned(4*sizeof(int))));
int main()
{
  foo (false, false, &a[1]);
  return 0;
}
