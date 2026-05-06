/* { dg-skip-if "requires x86 intrinsic header" { ! { x86_64-*-* i?86-*-* } } } */
/* { dg-require-effective-target avx2_runtime } */
/* { dg-require-effective-target fma } */
/* { dg-additional-options "-mavx2 -mfma" } */

/* PR tree-optimization/NNNNN
   Wrong code with AVX2 complex double multiply at -O2.
   Reduced from a sparse matrix kernel.  */

#include <immintrin.h>

#include "tree-vect.h"

static inline __m256d
cmul (__m256d a, __m256d b)
{
  __m256d t = _mm256_mul_pd (_mm256_movedup_pd (_mm256_permute_pd (a, 0x5)), b);
  return _mm256_fmaddsub_pd (_mm256_movedup_pd (a), b,
     _mm256_permute_pd (t, 0x5));
}

__attribute__((noipa)) void
kern (long n, long nnz, const double *alpha, const double *val,
      const long *col, const double *B, long ldb, double *C)
{
  for (long p = 0; p < nnz; p += 4)
    {
      long blkr, p0, p1, p2, p3;
      long left = nnz - p;
      if (left == 1)      { blkr=1; p0=p; p1=p;   p2=p;   p3=p;   }
      else if (left == 2) { blkr=2; p0=p; p1=p+1; p2=p;   p3=p;   }
      else if (left == 3) { blkr=3; p0=p; p1=p+1; p2=p+2; p3=p;   }
      else                { blkr=4; p0=p; p1=p+1; p2=p+2; p3=p+3; }

      double ar0 = alpha[0]*val[p0*2] - alpha[1]*val[p0*2+1];
      double ai0 = alpha[0]*val[p0*2+1] + alpha[1]*val[p0*2];
      double ar1 = alpha[0]*val[p1*2] - alpha[1]*val[p1*2+1];
      double ai1 = alpha[0]*val[p1*2+1] + alpha[1]*val[p1*2];
      double ar2 = alpha[0]*val[p2*2] - alpha[1]*val[p2*2+1];
      double ai2 = alpha[0]*val[p2*2+1] + alpha[1]*val[p2*2];
      double ar3 = alpha[0]*val[p3*2] - alpha[1]*val[p3*2+1];
      double ai3 = alpha[0]*val[p3*2+1] + alpha[1]*val[p3*2];

      long b0 = col[p0]*ldb*2, b1 = col[p1]*ldb*2;
      long b2 = col[p2]*ldb*2, b3 = col[p3]*ldb*2;

      for (long j = 0; j < n * 2; j += 4)
{
  __m256d cv  = _mm256_loadu_pd (&C[j]);
  __m256d bv0 = _mm256_loadu_pd (&B[j + b0]);
  __m256d bv1 = _mm256_loadu_pd (&B[j + b1]);
  __m256d bv2 = _mm256_loadu_pd (&B[j + b2]);
  __m256d bv3 = _mm256_loadu_pd (&B[j + b3]);
  __m256d av0 = _mm256_set_pd (ai0, ar0, ai0, ar0);
  __m256d av1 = _mm256_set_pd (ai1, ar1, ai1, ar1);
  __m256d av2 = _mm256_set_pd (ai2, ar2, ai2, ar2);
  __m256d av3 = _mm256_set_pd (ai3, ar3, ai3, ar3);

  switch (blkr)
    {
    case 4: cv = _mm256_add_pd (cmul (av0, bv0), cv);
    cv = _mm256_add_pd (cmul (av1, bv1), cv);
    cv = _mm256_add_pd (cmul (av2, bv2), cv);
    cv = _mm256_add_pd (cmul (av3, bv3), cv);
    break;
    case 3: cv = _mm256_add_pd (cmul (av0, bv0), cv);
    cv = _mm256_add_pd (cmul (av1, bv1), cv);
    cv = _mm256_add_pd (cmul (av2, bv2), cv);
    break;
    case 2: cv = _mm256_add_pd (cmul (av0, bv0), cv);
    cv = _mm256_add_pd (cmul (av1, bv1), cv);
    break;
    case 1: cv = _mm256_add_pd (cmul (av0, bv0), cv);
    break;
    }
  _mm256_storeu_pd (&C[j], cv);
}
    }
}

int
main (void)
{
  check_vect ();

  double alpha[2] = { 1.0, -2.0 };
  double val[] = { 5, 12, 5, 40, -1, 5 };
  long col[] = { 0, 1, 2 };
  double B[] = {
    10, 240, 24, 523,
    -23, 233, 55, 28,
    1, 2, 2, 52
  };
  double ref[4] = { 0 }, res[4] = { 0 };
  int i, j, k;

  for (k = 0; k < 3; k++)
    {
      double ar = alpha[0]*val[k*2] - alpha[1]*val[k*2+1];
      double ai = alpha[0]*val[k*2+1] + alpha[1]*val[k*2];
      for (j = 0; j < 2; j++)
	{
	  ref[j*2]   += ar*B[(col[k]*2+j)*2] - ai*B[(col[k]*2+j)*2+1];
	  ref[j*2+1] += ar*B[(col[k]*2+j)*2+1] + ai*B[(col[k]*2+j)*2];
	}
    }

  kern (2, 3, alpha, val, col, B, 2, res);

  for (i = 0; i < 4; i++)
    {
      double d = res[i] - ref[i];
      if (d * d > 1e-10)
	__builtin_abort ();
    }

  return 0;
}
