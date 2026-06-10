// { dg-do compile }
// { dg-options "-O2 -fdump-tree-evrp" }

#include <stdint.h>

int8_t src(uint8_t v2_u8, int8_t v4_i8, int8_t v5_i8) {
  if (!(((uint8_t)6 <= v2_u8) && (v2_u8 <= (uint8_t)7))) __builtin_unreachable();
  if (!(((int8_t)65 <= v4_i8) && (v4_i8 <= (int8_t)80))) __builtin_unreachable();
  if (!(((int8_t)100 <= v5_i8) && (v5_i8 <= (int8_t)101))) __builtin_unreachable();
  int8_t i0_i8 = (int8_t)(v5_i8 % v4_i8);
  int8_t i1_i8 = (int8_t)((int8_t)i0_i8 >> (uint8_t)v2_u8);
  return i1_i8;
}
// { dg-final { scan-tree-dump "20, 36" "evrp" } }
