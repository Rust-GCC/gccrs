/* { dg-final { scan-assembler "__sync_synchronize|dmb|mcr" { target arm*-*-linux-* arm*-*-uclinux* } } } */

void *foo (void)
{
  __sync_synchronize();
}
