/* Test that no C23 warnings are produced (by default) about
 * initializers that might not zero padding bits (when configured not to zero
 * padding bits unless mandated by the language standard).
 */
/* { dg-do run } */
/* { dg-options "-fzero-init-padding-bits=standard" } */

#include "c23-empty-init-warn-4.c"
