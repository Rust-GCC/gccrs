/* { dg-do compile { target { i?86-*-* x86_64-*-* } } } */

#define getfs() ((int __seg_fs *)0)

int main()
{
  return *getfs(); /* { dg-bogus "-Wanalyzer-null-dereference" } */
}
