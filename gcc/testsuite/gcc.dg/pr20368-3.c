/* ICE with -Wstrict-prototypes and typeof an undeclared function.
   Bug 20368.  Test with -Wmissing-declarations.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu17 -Wmissing-declarations" } */

extern __typeof (f) g; /* { dg-error "'f' undeclared here \\(not in a function\\)" } */

int
f (x) /* { dg-warning "no previous declaration for 'f'" } */
     float x;
{
}
