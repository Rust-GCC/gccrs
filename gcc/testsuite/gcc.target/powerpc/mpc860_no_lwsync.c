/* { dg-do compile { target { powerpc*-*-* && ilp32 } } } */
/* { dg-options "-mcpu=860 -O2" } */

/* Check that we do not emit lwsync when targeting MPC8xx (MPCCORE),
   since the MPC8xx core does not support and faults on this instruction.  */

void
store_release (int *ptr, int val)
{
  __atomic_store_n (ptr, val, __ATOMIC_RELEASE);
}

/* { dg-final { scan-assembler-not "\mlwsync\M" } } */
