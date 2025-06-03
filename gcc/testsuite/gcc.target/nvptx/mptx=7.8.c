/* { dg-do assemble } */
/* { dg-options {-march=sm_30 -mptx=7.8} } */
/* { dg-additional-options -save-temps } */
/* { dg-final { scan-assembler-times {(?n)^	\.version	7\.8$} 1 } } */
/* { dg-final { scan-assembler-times {(?n)^	\.target	sm_30$} 1 } } */

#if __PTX_ISA_VERSION_MAJOR__ != 7
#error wrong value for __PTX_ISA_VERSION_MAJOR__
#endif

#if __PTX_ISA_VERSION_MINOR__ != 8
#error wrong value for __PTX_ISA_VERSION_MINOR__
#endif

#if __PTX_SM__ != 300
#error wrong value for __PTX_SM__
#endif

int dummy;
