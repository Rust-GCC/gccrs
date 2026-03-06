/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc_zba_zbb -mabi=lp64d -fdump-rtl-ext_dce" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } } */

/* Verify ext-dce promotes chained SI-mode operations to DI-mode when the
   sign-extended bits are dead, and skips standalone (isolated) promotions.
   Chain functions (test_add, test_sub, test_sh1add, test_sh3add) have their
   result feeding into another candidate via the += r pattern.
   Standalone functions (test_and, test_ior, test_xor, test_neg, test_not,
   test_sll) are not promoted.  */

/* PLUS: addw -> add */
unsigned short test_add (int a, int b, unsigned short r)
{
  r += a + b;
  return r;
}

/* MINUS: subw -> sub */
unsigned short test_sub (int a, int b, unsigned short r)
{
  r += a - b;
  return r;
}

/* AND: andw -> and (though RV64 AND is mode-independent, this tests
   that the promotion path handles it) */
unsigned short test_and (int a, int b)
{
  return (unsigned short)(a & b);
}

/* IOR: orw -> or */
unsigned short test_ior (int a, int b)
{
  return (unsigned short)(a | b);
}

/* XOR: xorw -> xor */
unsigned short test_xor (int a, int b)
{
  return (unsigned short)(a ^ b);
}

/* NEG: negw -> neg */
unsigned short test_neg (int a)
{
  return (unsigned short)(-a);
}

/* NOT: xori/not promotion */
unsigned short test_not (int a)
{
  return (unsigned short)(~a);
}

/* ASHIFT: slliw -> slli */
unsigned short test_sll (int a)
{
  return (unsigned short)(a << 3);
}

/* sh1add: promotion of ASHIFT by 1 + PLUS enables sh1add matching */
unsigned short test_sh1add (int a, int b, unsigned short r)
{
  r += a * 2 + b;
  return r;
}

/* sh3add: promotion of ASHIFT by 3 + PLUS enables sh3add matching */
unsigned short test_sh3add (int a, int b, unsigned short r)
{
  r += a * 8 - b;
  return r;
}

/* Negative test: right-shift should NOT be promoted because the result
   depends on the upper bits of the input.  srliw must be preserved.  */
unsigned short test_srl_negative (unsigned a)
{
  return (unsigned short)(a >> 3);
}

/* Chain promotions still fire.  */
/* { dg-final { scan-rtl-dump "Successfully promoted to:" "ext_dce" } } */
/* { dg-final { scan-assembler "sh1add\t" } } */
/* { dg-final { scan-assembler "sh3add\t" } } */
/* { dg-final { scan-assembler-not "\\taddw\t" } } */
/* { dg-final { scan-assembler-not "\\tsubw\t" } } */
/* Standalone promotions are skipped.  */
/* { dg-final { scan-rtl-dump "Skipping standalone promotion" "ext_dce" } } */
/* Negative test: right-shift stays as srliw.  */
/* { dg-final { scan-assembler "\\tsrliw\t" } } */
