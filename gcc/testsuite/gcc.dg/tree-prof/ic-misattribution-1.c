/* { dg-options "-O2 -fdump-ipa-profile-note" } */
/* { dg-additional-sources "ic-misattribution-1a.c" } */

extern void other_caller (void);

void
callee (void)
{
  return;
}

void
caller(void (*func) (void))
{
  func ();
}

/* { dg-final-use-not-autofdo { scan-ipa-dump "hist->count 1 hist->all 1" "profile" } } */
