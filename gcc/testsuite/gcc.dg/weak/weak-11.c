/* PR 19031 */
/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-require-alias "" } */
/* { dg-options "-funit-at-a-time" } */

/* { dg-final { scan-weak "xyzzy" } } */

static const int local = 1; 
#pragma weak xyzzy = local
