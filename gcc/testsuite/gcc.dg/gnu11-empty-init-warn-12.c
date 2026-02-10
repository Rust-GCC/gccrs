/* Test that no GNU11 warnings are produced (by default) about
 * initializers that might not zero padding bits (when configured to zero
 * only padding bits of unions).
 */
/* { dg-do run } */
/* { dg-options "-fzero-init-padding-bits=unions" } */

#include "gnu11-empty-init-warn-4.c"
