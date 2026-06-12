/* { dg-do compile } */
/* { dg-options "-O2 -march=z14 -fPIC" } */
/* { dg-final { scan-assembler-not {lay\t%r15,-[0-9]+\(%r15\)} } } */

/* Ensure no spills and ultimately that no stack frame is allocated.  */

long strstr_ifunc_hwcap;
void __redirect_strstr();
__typeof(__redirect_strstr) __strstr_vx __attribute__((visibility("hidden")));
__typeof(__redirect_strstr) __strstr_arch13 __attribute__((visibility("hidden")));
__typeof(__redirect_strstr) *strstr_ifunc()
{
  return strstr_ifunc_hwcap ? __strstr_arch13 : __strstr_vx;
}
