/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-options "-O2 -mdejagnu-cpu=power5 -mabi=altivec -mno-altivec -mno-vsx" } */
/* { dg-require-effective-target powerpc_altivec } */
/* { dg-final { scan-assembler	   "\[ \t\]and "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]or "      } } */
/* { dg-final { scan-assembler	   "\[ \t\]xor "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]nor "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]andc "    } } */
/* { dg-final { scan-assembler	   "\[ \t\]eqv "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]orc "     } } */
/* { dg-final { scan-assembler	   "\[ \t\]nand "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vand "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vandc "   } } */
/* { dg-final { scan-assembler-not "\[ \t\]vor "     } } */
/* { dg-final { scan-assembler-not "\[ \t\]vxor "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]vnor "    } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxland "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlor "   } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlxor "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlnor "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlandc " } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxleqv "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlorc "  } } */
/* { dg-final { scan-assembler-not "\[ \t\]xxlnand " } } */

#ifndef TYPE
typedef int v4si __attribute__ ((vector_size (16)));
#define TYPE v4si
#endif

#include "bool2.h"
