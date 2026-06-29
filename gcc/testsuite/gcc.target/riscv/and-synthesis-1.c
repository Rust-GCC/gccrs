/* { dg-do compile { target rv64 } } */
/* { dg-options "-march=rv64gcb -mabi=lp64d" } */

#define T(L,S) unsigned long t_##L##_##S(unsigned long x) { return x & ((((1UL << L) - 1) << S)); }

T(1, 31)
T(2, 30)
T(3, 29)
T(4, 28)
T(5, 27)
T(6, 26)
T(7, 25)
T(8, 24)
T(9, 23)
T(10, 22)
T(11, 21)
T(12, 20)
T(13, 19)
T(14, 18)
T(15, 17)
T(16, 16)
T(17, 15)
T(18, 14)
T(19, 13)
T(20, 12)
T(21, 11)
T(22, 10)
T(23, 9)
T(24, 8)
T(25, 7)
T(26, 6)
T(27, 5)
T(28, 4)
T(29, 3)
T(30, 2)
T(31, 1)

/* { dg-final { scan-assembler-times "\\tsrliw" 30 } } */
/* { dg-final { scan-assembler-times "\\tslli" 30 } } */
/* { dg-final { scan-assembler-times "\\tbseti" 1 } } */
/* { dg-final { scan-assembler-times "\\tand" 1 } } */

