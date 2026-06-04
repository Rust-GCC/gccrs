/* { dg-do compile { target rv64} } */
/* { dg-additional-options "-mtune=tt-ascalon-d8 -march=rv64gc" } 
/* { dg-skip-if "" { *-*-* } { "-Os" "-Oz" } } */

long foo1 (long x) { return x / 10000L; }
long foo2 (long x) { return x / 30; }

/* { dg-final { scan-assembler-times "mulh" 2 } } */
/* { dg-final { scan-assembler-not "\tdiv\t" } } */
