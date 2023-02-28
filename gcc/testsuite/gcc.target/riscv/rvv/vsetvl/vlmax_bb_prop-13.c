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
    vint32mf2_t v = *(vint32mf2_t*)(in + 500);
    *(vint32mf2_t*)(out + 500) = v;
    break;
  }
  case 1:{
    vint32mf2_t v = *(vint32mf2_t*)(in + 500);
    *(vint32mf2_t*)(out + 500) = v;
    break;
  }
  default:{
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
    vint32mf2_t v = *(vint32mf2_t*)(in + 900 + i);
    *(vint32mf2_t*)(out + 900 + i) = v;
  }
}

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf8,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf4,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*mf4,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e16,\s*mf2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e32,\s*mf2,\s*t[au],\s*m[au]} 3 { target { no-opts "-O0"  no-opts "-Os" no-opts "-funroll-loops" no-opts "-g" } } } } */
