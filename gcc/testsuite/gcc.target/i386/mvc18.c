/* { dg-do compile } */
/* { dg-require-ifunc "" } */

/* { dg-final { scan-assembler "foo\.avx10_2\[,@\]" } } */
/* { dg-final { scan-assembler "foo\.apxf\[,@\]" } } */
/* { dg-final { scan-assembler "foo\.arch_diamondrapids\[,@\]" } } */
/* { dg-final { scan-assembler "foo\.arch_novalake\[,@\]" } } */

__attribute__((target_clones("avx10.2","apxf","arch=diamondrapids",
			     "arch=novalake","default")))
int foo ();

int main()
{
  return foo();
}
