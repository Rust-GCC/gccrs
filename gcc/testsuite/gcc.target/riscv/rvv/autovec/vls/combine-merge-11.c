/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvl4096b -mabi=lp64d -O3 -mrvv-max-lmul=m8" } */

#include "def.h"

DEF_REPEAT (v16di, int64_t, 16, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var0, var0, var0, var0, var0, var0)
DEF_REPEAT (v32di, int64_t, 32, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var0, var0, var0, var0, var0, var0)
DEF_REPEAT (v64di, int64_t, 64, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var0, var0, var0, var0)
DEF_REPEAT (v128di, int64_t, 128, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
	    var1, var1, var1, var1, var1, var1, var1, var1, var0, var0, var0, var0, var0, var0)
DEF_REPEAT (
  v256di, int64_t, 256, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var0, var0, var0, var0, var0, var0)
DEF_REPEAT (
  v512di, int64_t, 512, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1, var1,
  var1, var1, var1, var1, var1, var1, var1, var0, var0, var0, var0, var0, var0)


/* { dg-final { scan-assembler-times {vmv\.v\.x} 6 } } */
/* { dg-final { scan-assembler-times {vid\.v} 6 } } */
/* { dg-final { scan-assembler-times {vmsgtu} 6 } } */
/* { dg-final { scan-assembler-times {vmerge\.vxm} 6 } } */
