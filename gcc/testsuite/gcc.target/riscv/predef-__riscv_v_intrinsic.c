/* { dg-do compile } */
/* { dg-options "-march=rv64imafdcv -mabi=lp64d" } */

int main () {

#if __riscv_v_intrinsic != 1000000
#error "__riscv_v_intrinsic"
#endif

  return 0;
}
