/* { dg-do compile } */
/* { dg-options "-march=rv64gc_zbs -mabi=lp64d" { target rv64 } } */
/* { dg-options "-march=rv32gc_zbs -mabi=ilp32" { target rv32 } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-O1" } } */

int x264_macroblock_encode_p8x8(int dst, int a)
{
    dst &= ~(1 << a);
    return dst;
}

/* { dg-final { scan-assembler-times "bclr" 1 } } */
/* { dg-final { scan-assembler-times "sext.w" 1 { target rv64 } } } */
 
