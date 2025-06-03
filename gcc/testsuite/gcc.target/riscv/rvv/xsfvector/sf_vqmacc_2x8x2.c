/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_xsfvqmaccdod -mabi=lp64d -O3" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "riscv_vector.h"

/*
** test_sf_vqmacc_2x8x2_i32m1_vint32m1_t:
** ...
** vsetivli\s+zero+,0+,e32+,m1,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m1_t
test_sf_vqmacc_2x8x2_i32m1_vint32m1_t (vint32m1_t vd, vint8m1_t vs1,
				       vint8m1_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m1 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_i32m2_vint32m2_t:
** ...
** vsetivli\s+zero+,0+,e32+,m2,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m2_t
test_sf_vqmacc_2x8x2_i32m2_vint32m2_t (vint32m2_t vd, vint8m1_t vs1,
				       vint8m2_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m2 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_i32m4_vint32m4_t:
** ...
** vsetivli\s+zero+,0+,e32+,m4,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m4_t
test_sf_vqmacc_2x8x2_i32m4_vint32m4_t (vint32m4_t vd, vint8m1_t vs1,
				       vint8m4_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m4 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_i32m8_vint32m8_t:
** ...
** vsetivli\s+zero+,0+,e32+,m8,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m8_t
test_sf_vqmacc_2x8x2_i32m8_vint32m8_t (vint32m8_t vd, vint8m1_t vs1,
				       vint8m8_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m8 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_vint32m1_t:
** ...
** vsetivli\s+zero+,0+,e32+,m1,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m1_t
test_sf_vqmacc_2x8x2_vint32m1_t (vint32m1_t vd, vint8m1_t vs1, vint8m1_t vs2,
				 size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_vint32m2_t:
** ...
** vsetivli\s+zero+,0+,e32+,m2,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m2_t
test_sf_vqmacc_2x8x2_vint32m2_t (vint32m2_t vd, vint8m1_t vs1, vint8m2_t vs2,
				 size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_vint32m4_t:
** ...
** vsetivli\s+zero+,0+,e32+,m4,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m4_t
test_sf_vqmacc_2x8x2_vint32m4_t (vint32m4_t vd, vint8m1_t vs1, vint8m4_t vs2,
				 size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_vint32m8_t:
** ...
** vsetivli\s+zero+,0+,e32+,m8,ta,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m8_t
test_sf_vqmacc_2x8x2_vint32m8_t (vint32m8_t vd, vint8m1_t vs1, vint8m8_t vs2,
				 size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2 (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_i32m1_tu_vint32m1_t:
** ...
** vsetivli\s+zero+,0+,e32+,m1,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m1_t
test_sf_vqmacc_2x8x2_i32m1_tu_vint32m1_t (vint32m1_t vd, vint8m1_t vs1,
					  vint8m1_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m1_tu (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_i32m2_tu_vint32m2_t:
** ...
** vsetivli\s+zero+,0+,e32+,m2,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m2_t
test_sf_vqmacc_2x8x2_i32m2_tu_vint32m2_t (vint32m2_t vd, vint8m1_t vs1,
					  vint8m2_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m2_tu (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_i32m4_tu_vint32m4_t:
** ...
** vsetivli\s+zero+,0+,e32+,m4,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m4_t
test_sf_vqmacc_2x8x2_i32m4_tu_vint32m4_t (vint32m4_t vd, vint8m1_t vs1,
					  vint8m4_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m4_tu (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_i32m8_tu_vint32m8_t:
** ...
** vsetivli\s+zero+,0+,e32+,m8,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m8_t
test_sf_vqmacc_2x8x2_i32m8_tu_vint32m8_t (vint32m8_t vd, vint8m1_t vs1,
					  vint8m8_t vs2, size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_i32m8_tu (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_tu_vint32m1_t:
** ...
** vsetivli\s+zero+,0+,e32+,m1,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m1_t
test_sf_vqmacc_2x8x2_tu_vint32m1_t (vint32m1_t vd, vint8m1_t vs1, vint8m1_t vs2,
				    size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_tu (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_tu_vint32m2_t:
** ...
** vsetivli\s+zero+,0+,e32+,m2,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m2_t
test_sf_vqmacc_2x8x2_tu_vint32m2_t (vint32m2_t vd, vint8m1_t vs1, vint8m2_t vs2,
				    size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_tu (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_tu_vint32m4_t:
** ...
** vsetivli\s+zero+,0+,e32+,m4,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m4_t
test_sf_vqmacc_2x8x2_tu_vint32m4_t (vint32m4_t vd, vint8m1_t vs1, vint8m4_t vs2,
				    size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_tu (vd, vs1, vs2, vl);
}

/*
** test_sf_vqmacc_2x8x2_tu_vint32m8_t:
** ...
** vsetivli\s+zero+,0+,e32+,m8,tu,ma+
** sf\.vqmacc\.2x8x2\tv[0-9]+,v[0-9]+,v[0-9]+
** ...
*/
vint32m8_t
test_sf_vqmacc_2x8x2_tu_vint32m8_t (vint32m8_t vd, vint8m1_t vs1, vint8m8_t vs2,
				    size_t vl)
{
  return __riscv_sf_vqmacc_2x8x2_tu (vd, vs1, vs2, vl);
}
