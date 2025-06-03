/* { dg-do compile { target *-*-linux* *-*-solaris* } } */
/* { dg-options "-Wno-pedantic" } */

struct S {
  int a;
  int b[];
} S;

struct S s = { 1, { 2, 3 } };

/* { dg-final { scan-assembler ".size\[\t \]*s, 12" } } */
