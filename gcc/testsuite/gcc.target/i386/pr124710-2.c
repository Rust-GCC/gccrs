/* PR target/124710 */
/* { dg-do assemble } */
/* { dg-options "-O2 -masm=intel -mavx10.2" } */
/* { dg-require-effective-target masm_intel } */
/* { dg-require-effective-target avx10_2 } */

typedef float __v4sf __attribute__ ((__vector_size__ (16)));
typedef double __v2df __attribute__ ((__vector_size__ (16)));
typedef long long __v2di __attribute__ ((__vector_size__ (16)));

/* vcvttps2qqs 128-bit: source V4SF is half-width of dest V2DI,
   Intel syntax needs "qword ptr" for memory operand.
   At -O2 the compiler folds *p into the instruction.  */

__v2di
test_vcvttps2qqs128 (__v4sf *p)
{
  return (__v2di) __builtin_ia32_cvttps2qqs128_mask (*p, (__v2di) { 0, 0 },
						     (unsigned char) -1);
}

__v2di
test_vcvttps2uqqs128 (__v4sf *p)
{
  return (__v2di) __builtin_ia32_cvttps2uqqs128_mask (*p, (__v2di) { 0, 0 },
						      (unsigned char) -1);
}

/* vcvttsd2sis: source V2DF extracts scalar double (64-bit),
   Intel syntax needs "qword ptr" for memory operand.  */

int
test_vcvttsd2sis32 (__v2df *p)
{
  return (int) __builtin_ia32_cvttsd2sis32_round (*p, 4);
}

unsigned int
test_vcvttsd2usis32 (__v2df *p)
{
  return (unsigned int) __builtin_ia32_cvttsd2usis32_round (*p, 4);
}

/* vcvttss2sis: source V4SF extracts scalar float (32-bit),
   Intel syntax needs "dword ptr" for memory operand.  */

int
test_vcvttss2sis32 (__v4sf *p)
{
  return (int) __builtin_ia32_cvttss2sis32_round (*p, 4);
}

unsigned int
test_vcvttss2usis32 (__v4sf *p)
{
  return (unsigned int) __builtin_ia32_cvttss2usis32_round (*p, 4);
}
