/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-options "-march=rv64gc_zvkned_zvknhb_zvkb_zvkt_zvkg" { target { rv64 } } } */
/* { dg-options "-march=rv32gc_zvkned_zvknhb_zvkb_zvkt_zvkg" { target { rv32 } } } */

#ifndef __riscv_zvkng
#error "Feature macro for `Zvkng' not defined"
#endif

#ifndef __riscv_zvkn
#error "Feature macro for `Zvkn' not defined"
#endif

#ifndef __riscv_zvkned
#error "Feature macro for `Zvkned' not defined"
#endif

#ifndef __riscv_zvknhb
#error "Feature macro for `Zvknhb' not defined"
#endif

#ifndef __riscv_zvkb
#error "Feature macro for `Zvkb' not defined"
#endif

#ifndef __riscv_zvkt
#error "Feature macro for `Zvkt' not defined"
#endif

#ifndef __riscv_zvkg
#error "Feature macro for `Zvkg' not defined"
#endif

int
foo (int a)
{
  return a;
}
