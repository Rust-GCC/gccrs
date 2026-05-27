/* PR target/122827.

   With -msve-vector-bits=128 and SVE-only autovec, the inlined
   madd_v3 calls are vectorised into SVE partial-mode FMLAs that share
   a single vec_duplicate of the scalar FAC.
   Before the fix, the register allocator placed the VNx2SF
   broadcast in V14 across the call to clobber() because IRA thought
   V14 was preserved (only the low 64 bits, D14, are callee-saved under
   AAPCS64).  The SVE FMLA reads the full Z register, so the upper
   lanes of the broadcast are corrupted by the call and the result of
   the FMLA is wrong.  */

/* { dg-do run { target aarch64_sve128_hw } } */
/* { dg-options "-Ofast -msve-vector-bits=128 --param=aarch64-autovec-preference=sve-only" } */
/* { dg-require-effective-target aarch64_little_endian } */

struct R
{
  float combined[3];
  float spec[3];
  float diff[3];
  float diffshad[3];
  float shad[3];
};

struct I
{
  float fac;
  int passflag;
};

/* AAPCS64 only requires callees to preserve the low 64 bits of V8-V15;
   the upper lanes are caller-clobbered.  The buggy register allocator
   in this test places the broadcast in V14 specifically, so clobbering
   V14s upper bits here is sufficient to expose the miscompile.  The
   asm clobber list lets GCC save/restore D14 around the write, so we
   honour the ABI contract for the low half while leaving the upper
   half trashed, what any normal callee is permitted to do.  */
__attribute__((noipa)) static void
clobber (struct I *si, struct R *r)
{
  asm volatile ("movi v14.16b, #0xff" : : : "v14");
}

static inline __attribute__((always_inline)) void
madd_v3 (float r[3], const float a[3], float f)
{
  r[0] += a[0] * f;
  r[1] += a[1] * f;
  r[2] += a[2] * f;
}

__attribute__((noipa)) void
test (struct I *si, struct R *r)
{
  struct R t = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 },
		 { 10, 11, 12 }, { 13, 14, 15 } };

  clobber (si, r);
  if (si->fac != 0.0f)
    {
      float fac = si->fac;
      clobber (si, &t);
      madd_v3 (r->combined, t.combined, fac);
      if (si->passflag & 1)
	madd_v3 (r->spec, t.spec, fac);
      if (si->passflag & 2)
	{
	  madd_v3 (r->diff, t.diff, fac);
	  madd_v3 (r->diffshad, t.diffshad, fac);
	}
      if (si->passflag & 4)
	madd_v3 (r->shad, t.shad, fac);
    }
}

int
main (void)
{
  struct I si = { 2.0f, 7 };
  struct R r = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
		 { 0, 0, 0 }, { 0, 0, 0 } };
  test (&si, &r);
  if (r.combined[0] != 2.f || r.combined[1] != 4.f || r.combined[2] != 6.f)
    __builtin_abort ();
  if (r.spec[0] != 8.f || r.spec[1] != 10.f || r.spec[2] != 12.f)
    __builtin_abort ();
  if (r.diff[0] != 14.f || r.diff[1] != 16.f || r.diff[2] != 18.f)
    __builtin_abort ();
  if (r.diffshad[0] != 20.f || r.diffshad[1] != 22.f || r.diffshad[2] != 24.f)
    __builtin_abort ();
  if (r.shad[0] != 26.f || r.shad[1] != 28.f || r.shad[2] != 30.f)
    __builtin_abort ();
  return 0;
}

