/* { dg-do run } */
/* { dg-options "-O2" } */

[[gnu::noipa]] static unsigned char
br8 (unsigned char x)
{
  return __builtin_bitreverse8 (x);
}

[[gnu::noipa]] static unsigned short
br16 (unsigned short x)
{
  return __builtin_bitreverse16 (x);
}

[[gnu::noipa]] static unsigned
br32 (unsigned x)
{
  return __builtin_bitreverse32 (x);
}

[[gnu::noipa]] static unsigned long long
br64 (unsigned long long x)
{
  return __builtin_bitreverse64 (x);
}

int
main ()
{
#if __CHAR_BIT__ == 8
  if (br8 (0x00u) != 0x00u)
    __builtin_abort ();
  if (br8 (0x01u) != 0x80u)
    __builtin_abort ();
  if (br8 (0x34u) != 0x2cu)
    __builtin_abort ();
  if (br8 (0xffu) != 0xffu)
    __builtin_abort ();
#if __SIZEOF_SHORT__ == 2
  if (br16 (0x0000u) != 0x0000u)
    __builtin_abort ();
  if (br16 (0x0001u) != 0x8000u)
    __builtin_abort ();
  if (br16 (0x1234u) != 0x2c48u)
    __builtin_abort ();
  if (br16 (0xffffu) != 0xffffu)
    __builtin_abort ();
#endif
#if __SIZEOF_INT__ == 4
  if (br32 (0x00000000u) != 0x00000000u)
    __builtin_abort ();
  if (br32 (0x00000001u) != 0x80000000u)
    __builtin_abort ();
  if (br32 (0x01234567u) != 0xe6a2c480u)
    __builtin_abort ();
  if (br32 (0xffffffffu) != 0xffffffffu)
    __builtin_abort ();
#endif
#if __SIZEOF_LONG_LONG__ == 8
  if (br64 (0x0000000000000000ull) != 0x0000000000000000ull)
    __builtin_abort ();
  if (br64 (0x0000000000000001ull) != 0x8000000000000000ull)
    __builtin_abort ();
  if (br64 (0x0123456789abcdefull) != 0xf7b3d591e6a2c480ull)
    __builtin_abort ();
  if (br64 (0xffffffffffffffffull) != 0xffffffffffffffffull)
    __builtin_abort ();
#endif
#endif
}
