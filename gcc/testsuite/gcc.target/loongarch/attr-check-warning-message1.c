/* { dg-do compile } */
/* { dg-require-ifunc "" } */
/* { dg-options "-O0" } */

__attribute__ ((target_clones ("default","123456"))) void
test (void)	/* { dg-warning "\\\'123456\\\' is not supported in target attribute" } */
{}
