/* { dg-do compile } */
/* { dg-additional-options "-DT=int -march=rv64gc_zba -mabi=lp64d -mbranch-cost=4" { target rv64 } } */
/* { dg-additional-options "-DT=short -march=rv32gc_zba -mabi=ilp32 -mbranch-cost=4" { target rv32 } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" } } */

typedef long unsigned int size_t;
extern void *xcalloc (size_t, size_t) ;
typedef struct sparseset_def
{
  unsigned T *dense;
  unsigned T *sparse;
  unsigned T members;
  unsigned T size;
  unsigned T iter;
  unsigned char iter_inc;
  unsigned char iterating;
  unsigned T elms[2];
} *sparseset;
sparseset
sparseset_alloc (unsigned T n_elms)
{
  unsigned T n_bytes = sizeof (struct sparseset_def)
    + ((n_elms - 1) * 2 * sizeof (unsigned T));
  sparseset set = (sparseset) xcalloc (1, n_bytes);
  return set;
}




/* { dg-final { scan-assembler "slli\t" } } */
/* { dg-final { scan-assembler "srli\t" } } */
/* { dg-final { scan-assembler-not "zext.w\t" } } */
/* { dg-final { scan-assembler-not "andi\t" } } */

