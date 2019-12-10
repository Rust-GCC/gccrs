/* { dg-do run } */
/* { dg-options "-O3 -fdump-ipa-icf-optimized"  } */

struct A {                                                                      
  A() {ptr=&b;}                                                                 
  A(const A &a) {ptr = &b;}                                                     
  void test() { if (ptr != &b) __builtin_abort ();}                             
  int b;                                                                        
  int *ptr;                                                                     
};                                                                              

A test1(A a)
{
  a.test();
  return a;
}
A test2(A a)
{
  a.test();
  return a;
}
__attribute__ ((noinline))
static void
test_me (A (*t)(A))
{
  struct A a, b=t(a);
  b.test ();
}
int
main()
{
  test_me (test1);
  test_me (test2);
  return 0;
}
/* { dg-final { scan-ipa-dump-times "Unified; Wrapper has been created" 1 "icf"  } } */
