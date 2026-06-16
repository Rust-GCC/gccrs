/* BB SLP must not abandon a vectorizable group when the classical (affine)
   data-dependence test cannot analyze a runtime array subscript but the alias
   oracle can still prove the two references do not alias.

   The per-lane reciprocals are discovered as an SLP group, then the group is
   torn down by vect_slp_analyze_data_ref_dependence reporting "can't determine
   dependence" between the restrict output store and a runtime-indexed input
   load, even though the distinct restrict objects provably do not alias.  */

/* { dg-do compile } */
/* { dg-require-effective-target vect_double } */
/* { dg-additional-options "-O3 -ffast-math -fno-trapping-math" } */

struct VA { double data[8]; };
struct Tensor { struct VA comp[4]; };

/* Opaque: the returned index is a runtime value the affine subscript test
   cannot analyze.  */
unsigned __attribute__((noipa)) pick (unsigned k) { return k & 3; }

void f (const struct Tensor *in, double *__restrict out,
        unsigned nq, unsigned base)
{
  for (unsigned q = 0; q < nq; q++)
    {
      const struct VA *rho = &in[q].comp[0];     /* divisor: contiguous .data[i] */
      double inv[8];
      for (unsigned i = 0; i < 8; i++)
        inv[i] = 1.0 / rho->data[i];             /* reciprocal group, reused below */
      for (unsigned d = 0; d < 3; d++)
        {
          const struct VA *mom = &in[q].comp[pick (base + d)];  /* runtime-indexed numerator */
          for (unsigned i = 0; i < 8; i++)
            out[(q * 3 + d) * 8 + i] = mom->data[i] * inv[i];
        }
    }
}

/* The reciprocal group must survive the dependence check and vectorize.  */
/* { dg-final { scan-tree-dump "basic block part vectorized" "slp1" } } */
