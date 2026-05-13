/* PR middle-end/123286 */
/* { dg-do compile } */
/* { dg-additional-options "-O2 -fdump-tree-optimized" } */

#include <arm_neon.h>
#include <stdint.h>

uint64_t
f1 (uint64_t a)
{
  uint64x1_t va;
  uint64x1_t vz;

  va = vdup_n_u64 (a);
  vz = vdup_n_u64 (0);

  return vqadd_u64 (va, vz)[0];
}

uint64_t
f2 (uint64_t a)
{
  uint64x1_t va;
  uint64x1_t vz;

  va = vdup_n_u64 (0);
  vz = vdup_n_u64 (a);

  return vqadd_u64 (va, vz)[0];
}

/* Both SAT_ADD calls should fold away.  */
/* { dg-final { scan-tree-dump-not "\\.SAT_ADD" "optimized" } } */
