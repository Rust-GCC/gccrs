/* { dg-do compile } */
/* { dg-require-ifunc "" } */
/* { dg-options "-O0" } */

__attribute__ ((target_clones ("default","arch=123456"))) void
test (void)	/* { dg-warning "in attribute \\\'arch=123456\\\' you need to set a legal value for \\\"arch\\\"" } */
{}
