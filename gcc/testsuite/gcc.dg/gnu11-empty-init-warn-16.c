/* Test that no GNU11 warnings are produced (by default) about
 * initializers that might not zero padding bits (when configured to zero
 * padding bits of structs and unions).
 */
/* { dg-do run } */
/* { dg-options "-fzero-init-padding-bits=all" } */

#include "gnu11-empty-init-warn-4.c"
