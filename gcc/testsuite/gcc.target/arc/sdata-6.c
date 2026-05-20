/* { dg-do compile } */
/* { dg-options "-O2 -msdata" } */

#include <stdint.h>

__attribute__((section(".sdata"))) int a[300];

#define TEST(optype, threshold)                                                \
  void consume##optype (int, int, int, int, optype);                           \
  optype test_r0_fit_##optype (void) {                                         \
    return ((optype *)a)[threshold];                                           \
  }                                                                            \
  optype test_r0_no_fit_##optype (void)                                        \
  {                                                                            \
    return ((optype *)a)[threshold + 1];                                       \
  }                                                                            \
  void test_r4_fit_##optype (void)                                             \
  {                                                                            \
    return consume##optype (0, 0, 0, 0, ((optype *)a)[threshold]);             \
  }                                                                            \
  void test_r4_no_fit_##optype (void)                                          \
  {                                                                            \
    return consume##optype (0, 0, 0, 0, ((optype *)a)[threshold + 1]);         \
  }

TEST (char,    255)
TEST (short,   255)
TEST (int,     255)
TEST (int64_t, 127)

/* { dg-final { scan-assembler "ldb_s\\s+r0,\\\[gp,@a@sda\\+255\\\]" } } */
/* { dg-final { scan-assembler "ldb\\s+r0,\\\[@a\\+256\\\]" } } */
/* { dg-final { scan-assembler "ldb\\s+r4,\\\[gp,@a@sda\\+255\\\]" } } */
/* { dg-final { scan-assembler "ldb\\s+r4,\\\[@a\\+256\\\]" } } */

/* { dg-final { scan-assembler "ld\[hw\].x.as\\s+r0,\\\[gp,@a@sda\\+510\\\]" } } */
/* { dg-final { scan-assembler "ld\[hw\].x\\s+r0,\\\[@a\\+512\\\]" } } */
/* { dg-final { scan-assembler "ld\[hw\].x.as\\s+r4,\\\[gp,@a@sda\\+510\\\]" } } */
/* { dg-final { scan-assembler "ld\[hw\].x\\s+r4,\\\[@a\\+512\\\]" } } */

/* { dg-final { scan-assembler "ld_s\\s+r0,\\\[gp,@a@sda\\+1020\\\]" } } */
/* { dg-final { scan-assembler "ld\\s+r0,\\\[@a\\+1024\\\]" } } */
/* { dg-final { scan-assembler "ld.as\\s+r4,\\\[gp,@a@sda\\+1020\\\]" } } */
/* { dg-final { scan-assembler "ld\\s+r4,\\\[@a\\+1024\\\]" } } */

/* { dg-final { scan-assembler "ldd.as\\s+r0,\\\[gp,@a@sda\\+1016\\\]" { target ll64} } } */
/* { dg-final { scan-assembler "ldd\\s+r0,\\\[@a\\+1024\\\]" { target ll64} } } */
/* { dg-final { scan-assembler "ldd.as\\s+r4,\\\[gp,@a@sda\\+1016\\\]" { target ll64} } } */
/* { dg-final { scan-assembler "ldd\\s+r4,\\\[@a\\+1024\\\]" { target ll64} } } */
