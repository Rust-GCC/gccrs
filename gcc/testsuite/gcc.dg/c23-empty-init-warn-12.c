/* Test that no C23 warnings are produced (by default) about
 * initializers that might not zero padding bits (when configured to zero
 * only padding bits of unions).
 */
/* { dg-do run } */
/* { dg-options "-fzero-init-padding-bits=unions" } */

#include "c23-empty-init-warn-4.c"
