/* { dg-do compile } */
/* { dg-options "-O3 -march=rv32gc_zve32x -mabi=ilp32d" } */

#include "riscv_vector.h"

/* To support target attribute, the vfloat*m*x*_t need to be registered
   in advance.  We add type and args/return value check during the
   set current function but cannot cover below cases.  It is the same
   behavior compared to aarch64 sve.  */
void foo0 () {__rvv_bool64_t t;}
void foo1 () {__rvv_int8mf8_t t;}
void foo2 () {__rvv_uint8mf8_t t;}
void foo3 () {__rvv_int16mf4_t t;}
void foo4 () {__rvv_uint16mf4_t t;}
void foo5 () {__rvv_int32mf2_t t;}
void foo6 () {__rvv_uint32mf2_t t;}
void foo7 () {__rvv_int64m1_t t;}
void foo8 () {__rvv_uint64m1_t t;}
void foo9 () {__rvv_int64m2_t t;}
void foo10 () {__rvv_uint64m2_t t;}
void foo11 () {__rvv_int64m4_t t;}
void foo12 () {__rvv_uint64m4_t t;}
void foo13 () {__rvv_int64m8_t t;}
void foo14 () {__rvv_uint64m8_t t;}

void new_foo0 (__rvv_bool64_t t) { }    /* { dg-error {argument type '__rvv_bool64_t' requires the minimal vector length '64' but '32' is given} } */
void new_foo1 (__rvv_int8mf8_t t) { }   /* { dg-error {argument type '__rvv_int8mf8_t' requires the minimal vector length '64' but '32' is given} } */
void new_foo2 (__rvv_uint8mf8_t t) { }  /* { dg-error {argument type '__rvv_uint8mf8_t' requires the minimal vector length '64' but '32' is given} } */
void new_foo3 (__rvv_int16mf4_t t) { }  /* { dg-error {argument type '__rvv_int16mf4_t' requires the minimal vector length '64' but '32' is given} } */
void new_foo4 (__rvv_uint16mf4_t t) { } /* { dg-error {argument type '__rvv_uint16mf4_t' requires the minimal vector length '64' but '32' is given} } */
void new_foo5 (__rvv_int32mf2_t t) { }  /* { dg-error {argument type '__rvv_int32mf2_t' requires the minimal vector length '64' but '32' is given} } */
void new_foo6 (__rvv_uint32mf2_t t) { } /* { dg-error {argument type '__rvv_uint32mf2_t' requires the minimal vector length '64' but '32' is given} } */
void new_foo7 (__rvv_int64m1_t t) { }   /* { dg-error {argument type '__rvv_int64m1_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
void new_foo8 (__rvv_uint64m1_t t) { }  /* { dg-error {argument type '__rvv_uint64m1_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
void new_foo9 (__rvv_int64m2_t t) { }   /* { dg-error {argument type '__rvv_int64m2_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
void new_foo10 (__rvv_uint64m2_t t) { } /* { dg-error {argument type '__rvv_uint64m2_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
void new_foo11 (__rvv_int64m4_t t) { }  /* { dg-error {argument type '__rvv_int64m4_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
void new_foo12 (__rvv_uint64m4_t t) { } /* { dg-error {argument type '__rvv_uint64m4_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
void new_foo13 (__rvv_int64m8_t t) { }  /* { dg-error {argument type '__rvv_int64m8_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
void new_foo14 (__rvv_uint64m8_t t) { } /* { dg-error {argument type '__rvv_uint64m8_t' requires the zve64x, zve64f, zve64d or v ISA extension} } */
