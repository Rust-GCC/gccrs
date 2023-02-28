/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -fno-tree-vectorize" } */

#include "riscv_vector.h"

void f (int8_t * restrict in, int8_t * restrict out, int n)
{
  switch (n)
  {
  case 0:{
    vint8mf8_t v = *(vint8mf8_t*)(in + 500);
    *(vint8mf8_t*)(out + 500) = v;
    break;
  }
  case 1:{
    vint8mf8_t v = *(vint8mf8_t*)(in + 500);
    *(vint8mf8_t*)(out + 500) = v;
    break;
  }
  default:{
    vint8mf8_t v = *(vint8mf8_t*)(in + 700);
    *(vint8mf8_t*)(out + 700) = v;
    break;
  }
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vint8mf8_t v = *(vint8mf8_t*)(in + 900 + i);
    *(vint8mf8_t*)(out + 900 + i) = v;
  }
}

void f2 (int8_t * restrict in, int8_t * restrict out, int n)
{
  switch (n)
  {
  case 0:{
    vint8mf4_t v = *(vint8mf4_t*)(in + 500);
    *(vint8mf4_t*)(out + 500) = v;
    break;
  }
  case 1:{
    vint8mf4_t v = *(vint8mf4_t*)(in + 500);
    *(vint8mf4_t*)(out + 500) = v;
    break;
  }
  default:{
    vint8mf4_t v = *(vint8mf4_t*)(in + 700);
    *(vint8mf4_t*)(out + 700) = v;
    break;
  }
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vint8mf4_t v = *(vint8mf4_t*)(in + 900 + i);
    *(vint8mf4_t*)(out + 900 + i) = v;
  }
}

void f3 (int8_t * restrict in, int8_t * restrict out, int n)
{
  switch (n)
  {
  case 0:{
    vint8mf2_t v = *(vint8mf2_t*)(in + 500);
    *(vint8mf2_t*)(out + 500) = v;
    break;
  }
  case 1:{
    vint8mf2_t v = *(vint8mf2_t*)(in + 500);
    *(vint8mf2_t*)(out + 500) = v;
    break;
  }
  default:{
    vint8mf2_t v = *(vint8mf2_t*)(in + 700);
    *(vint8mf2_t*)(out + 700) = v;
    break;
  }
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vint8mf2_t v = *(vint8mf2_t*)(in + 900 + i);
    *(vint8mf2_t*)(out + 900 + i) = v;
  }
}

void f4 (int8_t * restrict in, int8_t * restrict out, int n)
{
  switch (n)
  {
  case 0:{
    vint16mf4_t v = *(vint16mf4_t*)(in + 500);
    *(vint16mf4_t*)(out + 500) = v;
    break;
  }
  case 1:{
    vint16mf4_t v = *(vint16mf4_t*)(in + 500);
    *(vint16mf4_t*)(out + 500) = v;
    break;
  }
  default:{
    vint16mf4_t v = *(vint16mf4_t*)(in + 700);
    *(vint16mf4_t*)(out + 700) = v;
    break;
  }
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vint16mf4_t v = *(vint16mf4_t*)(in + 900 + i);
    *(vint16mf4_t*)(out + 900 + i) = v;
  }
}

void f5 (int8_t * restrict in, int8_t * restrict out, int n)
{
  switch (n)
  {
  case 0:{
    vint16mf2_t v = *(vint16mf2_t*)(in + 500);
    *(vint16mf2_t*)(out + 500) = v;
    break;
  }
  case 1:{
    vint16mf2_t v = *(vint16mf2_t*)(in + 500);
    *(vint16mf2_t*)(out + 500) = v;
    break;
  }
  default:{
    vint16mf2_t v = *(vint16mf2_t*)(in + 700);
    *(vint16mf2_t*)(out + 700) = v;
    break;
  }
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vint16mf2_t v = *(vint16mf2_t*)(in + 900 + i);
    *(vint16mf2_t*)(out + 900 + i) = v;
  }
}

void f6 (int8_t * restrict in, int8_t * restrict out, int n)
{
  switch (n)
  {
  case 0:{
    vuint32mf2_t v = *(vuint32mf2_t*)(in + 500);
    *(vuint32mf2_t*)(out + 500) = v;
    break;
  }
  case 1:{
    vuint32mf2_t v = *(vuint32mf2_t*)(in + 500);
    *(vuint32mf2_t*)(out + 500) = v;
    break;
  }
  default:{
    vuint32mf2_t v = *(vuint32mf2_t*)(in + 700);
    *(vuint32mf2_t*)(out + 700) = v;
    break;
  }
  }

  for (int i = 0 ; i < n * n; i++)
    out[i] = out[i] + out[i];
  
  for (int i = 0 ; i < n * n * n; i++)
    out[i] = out[i] * out[i];

  for (int i = 0 ; i < n * n * n * n; i++)
    out[i] = out[i] * out[i];
  
  for (int i = 0 ; i < n * n * n * n; i++) {
    vuint32mf2_t v = *(vuint32mf2_t*)(in + 900 + i);
    *(vuint32mf2_t*)(out + 900 + i) = v;
  }
}

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf8,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf4,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {add\ta[0-7],a[0-7],a[0-7]\s+\.L[0-9][0-9]\:\s+vle8\.v\s+(?:v[0-9]|v[1-2][0-9]|v3[0-1]),0\s*\([a-x0-9]+\)} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*mf4,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*mf2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {add\ta[0-7],a[0-7],a[0-7]\s+\.L[0-9][0-9]\:\s+vle16\.v\s+(?:v[0-9]|v[1-2][0-9]|v3[0-1]),0\s*\([a-x0-9]+\)} 2 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*mf2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
/* { dg-final { scan-assembler-times {add\ta[0-7],a[0-7],a[0-7]\s+\.L[0-9][0-9][0-9]\:\s+vle32\.v\s+(?:v[0-9]|v[1-2][0-9]|v3[0-1]),0\s*\([a-x0-9]+\)} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" no-opts "-O1" } } } } */
