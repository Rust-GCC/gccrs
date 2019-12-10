/* { dg-do compile } */
/* { dg-options "-fgnu-tm -fdump-ipa-tmipa" } */
/* { dg-additional-options "-Wno-volatile" { target c++ } } */

/* Test that volatiles are allowed inside relaxed transactions.  */

volatile int test_var = 0;

int main()
{
  __transaction_relaxed {
    test_var++;
  }
}

/* { dg-final { scan-ipa-dump "GTMA_DOES_GO_IRREVOCABLE" "tmipa" } } */
