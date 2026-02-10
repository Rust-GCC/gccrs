/* Test that no C23 warnings are produced (when explicitly disabled) about
 * initializers that might not zero padding bits.
 */
/* { dg-do run } */
/* { dg-options "-Wzero-init-padding-bits=standard" } */

#include "c23-empty-init-warn-4.c"
