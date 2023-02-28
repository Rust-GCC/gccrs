/* { dg-do run } */
/* { dg-require-effective-target mbranch_protection_ok } */
/* { dg-require-effective-target arm_pacbti_hw } */
/* { dg-skip-if "do not override march" { *-*-* } { "-march=*" } { "-march=armv8.1-m.main" } } */
/* { dg-additional-options "-march=armv8.1-m.main -mbranch-protection=bti+pac-ret" } */

#if !defined (__ARM_FEATURE_BTI_DEFAULT)
#error "Feature test macro __ARM_FEATURE_BTI_DEFAULT should be defined."
#endif

#if !defined (__ARM_FEATURE_PAC_DEFAULT)
#error "Feature test macro __ARM_FEATURE_PAC_DEFAULT should be defined."
#endif

int
main()
{
  if (__ARM_FEATURE_BTI_DEFAULT != 1)
    __builtin_abort ();

  if (__ARM_FEATURE_PAC_DEFAULT != 1)
    __builtin_abort ();

  return 0;
}
