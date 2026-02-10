/* Test that no C23 warnings are produced (when explicitly disabled) about
 * initializers that might not zero padding bits (when configured to zero
 * only padding bits of unions).
 */
/* { dg-do run } */
/* { dg-options "-Wzero-init-padding-bits=standard -fzero-init-padding-bits=unions" } */

#include "c23-empty-init-warn-4.c"
