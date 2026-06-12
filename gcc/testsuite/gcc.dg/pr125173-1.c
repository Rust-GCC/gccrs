/* { dg-do compile } */
/* { dg-options "-O1 -fno-fuse-ops-with-volatile-access -fno-forward-propagate --param=max-cse-insns=0" } */

int a;
volatile short b;

struct
{
  char c;
  int d;
} s;

void
foo ()
{
  __asm__ ("":"+m" (b), "+m" (s.d), "+m" (a));
}
