/* { dg-do compile } */
/* { dg-options "-O2 -fno-strict-volatile-bitfields" } */

typedef struct {
  char a:1;
  char b:7;
  int c;
} BitStruct;

volatile BitStruct bits;

int foo ()
{
  return bits.b;
}

/* { dg-final { scan-assembler "mov(q|l).*bits" } } */
