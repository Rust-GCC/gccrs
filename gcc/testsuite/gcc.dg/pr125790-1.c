/* { dg-do run { target int32 } } */
/* { dg-options "-O1" } */


__attribute__((noipa))
long long
rshift (long long x)
{
  return (~x) >> (unsigned char)x;
}

int
main(void)
{
  if (rshift(0xffffffff0000003fLL) != 0)
    __builtin_abort();
  return 0;
}
