/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gcbv_zicond -mabi=lp64d" { target { rv64 } } } */
/* { dg-options "-O2 -march=rv32gcbv_zicond -mabi=ilp32" { target { rv32 } } } */

int foo(int x, int y) { return (y < x) ? 1 : -1; }


/* { dg-final { scan-assembler-times {(slli|ori)\t} 1 } } */
/* { dg-final { scan-assembler-times {addi\t} 1 } } */
/* { dg-final { scan-assembler-not {czero.eqz\t} } } */

