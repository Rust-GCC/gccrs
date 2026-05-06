/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcb_zicond -mabi=lp64d" { target rv64 } } */
/* { dg-additional-options "-march=rv32gcb_zicond -mabi=ilp32" { target rv32 } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" } } */

unsigned f1 (unsigned x, unsigned m)
{
    x >>= ((m & 0x008080) ? 8 : 0);
    return x;
}

/* { dg-final { scan-assembler-not "addi\t" } } */
/* { dg-final { scan-assembler-not "and\t" } } */
/* { dg-final { scan-assembler-not "snez\t" } } */
/* { dg-final { scan-assembler-not "slli\t" } } */
/* { dg-final { scan-assembler-not "slliw\t" } } */
/* { dg-final { scan-assembler-times "srai\t" 1 } } */
/* { dg-final { scan-assembler-times "andi\t" 1 } } */
/* { dg-final { scan-assembler-times "czero" 1 } } */
