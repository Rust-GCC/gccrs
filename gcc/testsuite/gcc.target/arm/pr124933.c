/* { dg-require-effective-target arm_arch_v8_1m_main_fp_hard_ok } */
/* { dg-add-options arm_arch_v8_1m_main_fp_hard } */
/* { dg-additional-options "-O2 -mfp16-format=ieee" } */
/* { dg-do assemble } */

__fp16 f;
__fp16 g;

void __attribute__((noinline,noclone)) h (__fp16 x)
{
  g = x;
}

void __attribute__((noinline,noclone)) i ()
{
  h (f);
}
