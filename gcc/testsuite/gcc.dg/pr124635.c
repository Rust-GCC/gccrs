/* { dg-do compile } */
/* { dg-options "-std=c23 -g" } */

#pragma scalar_storage_order big-endian

struct S {
  struct S *s[1];
  char c;
};

