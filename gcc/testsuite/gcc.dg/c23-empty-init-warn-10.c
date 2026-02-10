/* Test that no C23 warnings are produced (despite being enabled for unions)
 * about initializers that might not zero padding bits (when configured to zero
 * only padding bits of unions).
 */
/* { dg-do run } */
/* { dg-options "-Wzero-init-padding-bits=unions -fzero-init-padding-bits=unions" } */

#include "c23-empty-init-warn-4.c"
