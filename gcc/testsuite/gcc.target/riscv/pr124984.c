/* { dg-do compile { target bitint } } */
/* { dg-options "-mcpu=thead-c906 -ftracer -fno-tree-dominator-opts -O2 -fharden-conditional-branches" } */

int g;

void
foo(int s)
{
  g = 0x258d9b711584058072a68c6c9d0ff404fd077b87d59ee75dd2e349825d3a288c61a054e3aa1804c26adb2b9ce2287f17888af3aca18a430bcb4f334672553aecd010e5618a1293691bfd6f3e1d0d8ea3dbdb9176a53d1f657032263707cae4e5e70badc7769309b006495fbd3e76ca25f4d02df66107dc351e928wb >> (s & 1023);
}
