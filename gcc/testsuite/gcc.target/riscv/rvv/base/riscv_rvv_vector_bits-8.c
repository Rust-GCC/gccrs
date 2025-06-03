/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvl256b_zfh_zvfh -mabi=lp64 -mrvv-vector-bits=zvl -O3" } */

#include "riscv_rvv_vector_bits.h"

TEST_FIXED_TYPE_INT_ALL (vint8mf8_t,  32,   v4qi)
TEST_FIXED_TYPE_INT_ALL (vint8mf4_t,  64,   v8qi)
TEST_FIXED_TYPE_INT_ALL (vint8mf2_t, 128,  v16qi)
TEST_FIXED_TYPE_INT_ALL (vint8m1_t,  256,  v32qi)
TEST_FIXED_TYPE_INT_ALL (vint8m2_t,  512,  v64qi)
TEST_FIXED_TYPE_INT_ALL (vint8m4_t, 1024, v128qi)
TEST_FIXED_TYPE_INT_ALL (vint8m8_t, 2048, v256qi)

TEST_FIXED_TYPE_INT_ALL (vint16mf4_t,  64,   v4hi)
TEST_FIXED_TYPE_INT_ALL (vint16mf2_t, 128,   v8hi)
TEST_FIXED_TYPE_INT_ALL (vint16m1_t,  256,  v16hi)
TEST_FIXED_TYPE_INT_ALL (vint16m2_t,  512,  v32hi)
TEST_FIXED_TYPE_INT_ALL (vint16m4_t, 1024,  v64hi)
TEST_FIXED_TYPE_INT_ALL (vint16m8_t, 2048, v128hi)

TEST_FIXED_TYPE_INT_ALL (vint32mf2_t, 128,  v4si)
TEST_FIXED_TYPE_INT_ALL (vint32m1_t,  256,  v8si)
TEST_FIXED_TYPE_INT_ALL (vint32m2_t,  512, v16si)
TEST_FIXED_TYPE_INT_ALL (vint32m4_t, 1024, v32si)
TEST_FIXED_TYPE_INT_ALL (vint32m8_t, 2048, v64si)

TEST_FIXED_TYPE_INT_ALL (vint64m1_t,  256,  v4di)
TEST_FIXED_TYPE_INT_ALL (vint64m2_t,  512,  v8di)
TEST_FIXED_TYPE_INT_ALL (vint64m4_t, 1024, v16di)
TEST_FIXED_TYPE_INT_ALL (vint64m8_t, 2048, v32di)

TEST_FIXED_TYPE_INT_ALL (vuint8mf4_t,  64,   v8uqi)
TEST_FIXED_TYPE_INT_ALL (vuint8mf2_t, 128,  v16uqi)
TEST_FIXED_TYPE_INT_ALL (vuint8m1_t,  256,  v32uqi)
TEST_FIXED_TYPE_INT_ALL (vuint8m2_t,  512,  v64uqi)
TEST_FIXED_TYPE_INT_ALL (vuint8m4_t, 1024, v128uqi)
TEST_FIXED_TYPE_INT_ALL (vuint8m8_t, 2048, v256uqi)

TEST_FIXED_TYPE_INT_ALL (vuint16mf4_t,  64,   v4uhi)
TEST_FIXED_TYPE_INT_ALL (vuint16mf2_t, 128,   v8uhi)
TEST_FIXED_TYPE_INT_ALL (vuint16m1_t,  256,  v16uhi)
TEST_FIXED_TYPE_INT_ALL (vuint16m2_t,  512,  v32uhi)
TEST_FIXED_TYPE_INT_ALL (vuint16m4_t, 1024,  v64uhi)
TEST_FIXED_TYPE_INT_ALL (vuint16m8_t, 2048, v128uhi)

TEST_FIXED_TYPE_INT_ALL (vuint32mf2_t, 128,  v4usi)
TEST_FIXED_TYPE_INT_ALL (vuint32m1_t,  256,  v8usi)
TEST_FIXED_TYPE_INT_ALL (vuint32m2_t,  512, v16usi)
TEST_FIXED_TYPE_INT_ALL (vuint32m4_t, 1024, v32usi)
TEST_FIXED_TYPE_INT_ALL (vuint32m8_t, 2048, v64usi)

TEST_FIXED_TYPE_INT_ALL (vuint64m1_t,  256,  v4udi)
TEST_FIXED_TYPE_INT_ALL (vuint64m2_t,  512,  v8udi)
TEST_FIXED_TYPE_INT_ALL (vuint64m4_t, 1024, v16udi)
TEST_FIXED_TYPE_INT_ALL (vuint64m8_t, 2048, v32udi)

TEST_FIXED_TYPE_FLOAT_ALL (vfloat16mf4_t,  64,   v4hi)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat16mf2_t, 128,   v8hi)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat16m1_t,  256,  v16hi)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat16m2_t,  512,  v32hi)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat16m4_t, 1024,  v64hi)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat16m8_t, 2048, v128hi)

TEST_FIXED_TYPE_FLOAT_ALL (vfloat32mf2_t, 128,  v4si)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat32m1_t,  256,  v8si)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat32m2_t,  512, v16si)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat32m4_t, 1024, v32si)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat32m8_t, 2048, v64si)

TEST_FIXED_TYPE_FLOAT_ALL (vfloat64m1_t,  256,  v4di)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat64m2_t,  512,  v8di)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat64m4_t, 1024, v16di)
TEST_FIXED_TYPE_FLOAT_ALL (vfloat64m8_t, 2048, v32di)

/* { dg-final { scan-assembler-not {csrr\s+[atx][0-9]+,\s*vlenb} } } */
