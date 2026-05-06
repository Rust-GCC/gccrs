/* { dg-do compile } */
/* { dg-require-ifunc "" } */
/* { dg-options "-O0" } */

__attribute__ ((target_clones ("default","cmodel=normal"))) void
test (void)	/* { dg-warning "attribute \\\'cmodel=normal\\\' is not supported in \\\'target_version\\\' or \\\'target_clones\\\'" } */
{}
