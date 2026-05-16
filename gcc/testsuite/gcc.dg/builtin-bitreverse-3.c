/* { dg-do run } */
/* { dg-options "-O2" } */

#if __SIZEOF_INT__ == 4 && __CHAR_BIT__ == 8
[[gnu::noipa]] bool
foo (unsigned x)
{
  x &= 0x499a750a;
  x |= 0x5a7c04f;
  x = __builtin_bitreverse32 (x);
  return (x & ~0x351848) == 0x4fc0a705;
}

[[gnu::noipa]] bool
bar (unsigned x)
{
  x &= 0x499a750a;
  x |= 0x5a7c04f;
  x = __builtin_bitreverse32 (x);
  return (x & ~0xac1812) == 0xf203e5a0;
}
#endif

int
main ()
{
#if __SIZEOF_INT__ == 4 && __CHAR_BIT__ == 8
  if (foo (~0) || !bar (~0))
    __builtin_abort ();
#endif
}
