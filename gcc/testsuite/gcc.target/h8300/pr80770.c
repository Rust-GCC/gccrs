/* { dg-do compile } */
/* { dg-additional-options "-O2 -std=gnu99 -mint32" } */


struct S {
  _Bool b0: 1;
  _Bool b1: 1;
  _Bool b2: 1;
  _Bool b3: 1;
  _Bool b4: 1;
  _Bool b5: 1;
  _Bool b6: 1;
  _Bool b7: 1;
  _Bool b8: 1;
  _Bool b9: 1;
  _Bool b10: 1;
  _Bool b11: 1;
  _Bool b12: 1;
  _Bool b13: 1;
  _Bool b14: 1;
  _Bool b15: 1;
  _Bool b16: 1;
  _Bool b17: 1;
  _Bool b18: 1;
  _Bool b19: 1;
  _Bool b20: 1;
  _Bool b21: 1;
  _Bool b22: 1;
  _Bool b23: 1;
  _Bool b24: 1;
  _Bool b25: 1;
  _Bool b26: 1;
  _Bool b27: 1;
  _Bool b28: 1;
  _Bool b29: 1;
  _Bool b30: 1;
  _Bool b31: 1;
};

#define T(N) void fb##N (struct S *s) { s->b##N = !s->b##N; }

T(0)
T(1)
T(2)
T(3)
T(4)
T(5)
T(6)
T(7)
T(8)
T(9)
T(10)
T(11)
T(12)
T(13)
T(14)
T(15)
T(16)
T(17)
T(18)
T(19)
T(20)
T(21)
T(22)
T(23)
T(24)
T(25)
T(26)
T(27)
T(28)
T(29)
T(30)
T(31)

/* { dg-final { scan-assembler-times "xor\t|add.b\t|bnot\t" 32 } } */
