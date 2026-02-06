/* { dg-do compile } */
/* { dg-options "-march=armv8-a -O1 -fno-schedule-insns" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include <arm_acle.h>

/* Access kind specifiers.  */
#define PLD 0
#define PST 1
/* Retention policies.  */
#define KEEP 0
#define STRM 1

/* This test is a bit awkward as we need to test the constants that get passed
   into x1/w1. This may be a bit fragile.  */

/*
** pldx_range:
**...
**	mov	[xw]1, 0
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PSTKEEP, [xw]1, \[x0\]
**	rprfm	PLDSTRM, [xw]1, \[x0\]
**	rprfm	PSTSTRM, [xw]1, \[x0\]
**	mov	[xw]1, 1
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 4194303
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 2097152
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 2097151
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 0
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 4194304
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 274873712640
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 576460752303423488
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 576460477425516544
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, -1152921504606846976
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, -2305843009213693952
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, -3458764513820540928
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 2305843009213693952
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 1152921504606846976
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	mov	[xw]1, 0
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**	rprfm	PLDKEEP, [xw]1, \[x0\]
**...
*/
int pldx_range (void *a) {
  __pldx_range (PLD, KEEP, 0, 1, 0, 0, a);
  __pldx_range (PST, KEEP, 0, 1, 0, 0, a);
  __pldx_range (PLD, STRM, 0, 1, 0, 0, a);
  __pldx_range (PST, STRM, 0, 1, 0, 0, a);
  __pldx_range (PLD, KEEP, 1, 1, 0, 0, a);
  __pldx_range (PLD, KEEP, -1, 1, 0, 0, a);
  __pldx_range (PLD, KEEP, -(1<<21), 1, 0, 0, a);
  __pldx_range (PLD, KEEP, (1<<21)-1, 1, 0, 0, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, 0, a);
  __pldx_range (PLD, KEEP, 0, 2, 0, 0, a);
  __pldx_range (PLD, KEEP, 0, 65536, 0, 0, a);
  __pldx_range (PLD, KEEP, 0, 1, -(1<<21), 0, a);
  __pldx_range (PLD, KEEP, 0, 1, (1<<21)-1, 0, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, 1ULL, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 15) - 1, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, 1ULL << 15, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 16) - 1, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, 1ULL << 16, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 16) + 1, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, 1ULL << 28, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 28) + 1, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 29) - 1, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, 1ULL << 29, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 29) + 1, a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 30), a);
  __pldx_range (PLD, KEEP, 0, 1, 0, (1ULL << 31), a);
}

/*
** pld_range:
**...
**	rprfm	PLDKEEP, x1, \[x0\]
**	rprfm	PSTKEEP, x1, \[x0\]
**	rprfm	PLDSTRM, x1, \[x0\]
**	rprfm	PSTSTRM, x1, \[x0\]
**...
*/
int pld_range (void *a, uint64_t m) {
  __pld_range (PLD, KEEP, m, a);
  __pld_range (PST, KEEP, m, a);
  __pld_range (PLD, STRM, m, a);
  __pld_range (PST, STRM, m, a);
}
