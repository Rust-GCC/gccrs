/* Test that no GNU11 warnings are produced (when explicitly disabled) about
 * initializers that might not zero padding bits (when configured to zero
 * only padding bits of unions).
 */
/* { dg-do run } */
/* { dg-options "-Wzero-init-padding-bits=standard -fzero-init-padding-bits=unions" } */

#include "gnu11-empty-init-warn-4.c"
