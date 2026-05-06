/* PR target/124908 */
/* { dg-options "-O2 -msve-vector-bits=128" } */

#include <arm_sve.h>

/* Verify that folding svld1rq does not ICE with -msve-vector-bits=128.  */

svuint8_t
f_u8 (const uint8_t *p)
{
  return svld1rq_u8 (svptrue_b8 (), p);
}

svint8_t
f_s8 (const int8_t *p)
{
  return svld1rq_s8 (svptrue_b8 (), p);
}

svuint16_t
f_u16 (const uint16_t *p)
{
  return svld1rq_u16 (svptrue_b16 (), p);
}

svuint32_t
f_u32 (const uint32_t *p)
{
  return svld1rq_u32 (svptrue_b32 (), p);
}

svfloat64_t
f_f64 (const float64_t *p)
{
  return svld1rq_f64 (svptrue_b64 (), p);
}

