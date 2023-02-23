/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-tree-vectorize" } */

#include "riscv_vector.h"

void f (int32_t * restrict in, int32_t * restrict out, int32_t * restrict in2, int32_t * restrict out2,  int n, int cond)
{
  if (cond == 0)
    {
      for (int i = 0; i < n; i++)
        {
          out2[i] = out[i] + in[i];
        }
    }
  else if (cond == 1)
   {
      for (int i = 0; i < n; i++)
          {
            out2[i] = out[i] + in[i];
          }
      for (int i = 0; i < n; i++)
          {
            out[i] = out2[i] / in[i];
          }
      for (int i = 0; i < n; i++)
        {
          out2[i] = out[i] * in[i];
        }
   }
  else
   {
     for (int i = 0; i < n; i++) 
      {
        vint32mf2_t v1 = *(vint32mf2_t*)in2;
        *(vint32mf2_t*)(out + i + 10) = v1;
      }
   }
  for (int i = 0; i < n; i++) 
    {
      vint8mf8_t v1;
      *(vint8mf8_t*)(out + i + 10) = v1;
    }
}

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf8,\s*t[au],\s*m[au]\s+\.L[0-9]+:} 1 { target { no-opts "-O0" no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*mf2,\s*t[au],\s*m[au]} 1 { target { no-opts "-O0" no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli} 2 { target { no-opts "-O0"  no-opts "-O1"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
