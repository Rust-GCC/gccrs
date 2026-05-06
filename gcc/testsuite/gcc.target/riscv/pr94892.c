/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-O1" } } */


  static __inline__ int sign(int x)
  {
    return (x >> 31) | ((unsigned)-x >> 31);
  }

  void f1(void);
  void f2(void);
  void f(int x)
  {
    if (sign(x) > -1)
      f1();
    else
      f2();
  }

/* { dg-final { scan-assembler-not "srai" } } */
/* { dg-final { scan-assembler-times "blt\t" 1 } } */

