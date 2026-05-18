/* { dg-do compile } */
/* { dg-options "-march=rv64gc -mabi=lp64d -mzilsd-word-align" } */

int i;

/* { dg-error "only supported for RV32" "" { target *-*-* } 0 } */
