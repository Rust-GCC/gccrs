/* { dg-require-effective-target untyped_assembly } */
/* { dg-skip-if "no __builtin_apply in eBPF" { bpf-*-* } } */

g (a, b) {}

f (xx)
     void* xx;
{
  __builtin_apply ((void*)g, xx, 200);
}
