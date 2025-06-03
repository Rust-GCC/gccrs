/* { dg-do compile { target { powerpc64*-*-* } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx -O2" } */
/* { dg-require-effective-target powerpc_vsx } */

/* Verify that we optimize vector1 = (vector2 == vector3) by not loading up
   0/-1.  */

vector int
test (vector int a, vector int b)
{
  return a == b;
}

/* { dg-final { scan-assembler     {\mvcmpequw\M} } } */
/* { dg-final { scan-assembler-not {\mxxspltib\M} } } */
/* { dg-final { scan-assembler-not {\mvspltisw\M} } } */
/* { dg-final { scan-assembler-not {\mxxlxor\M}   } } */
/* { dg-final { scan-assembler-not {\mxxlxorc\M}  } } */
/* { dg-final { scan-assembler-not {\mxxsel\M}    } } */
