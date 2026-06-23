/* { dg-do compile } */
/* { dg-options "-muser-provided-CPU=${srcdir}/gcc.target/aarch64/aarch64-json-tunings/gp2fp-2.json -O2 -fno-vect-cost-model -fweb" } */
/* { dg-warning "JSON tuning file does not contain version information" "" { target *-*-* } 0 } */

double d;
_Complex short cs;
void foo() { cs *= d; }
