/* Test that no GNU11 warnings are produced (despite being enabled for structs
 * and unions) about initializers that might not zero padding bits (when
 * configured to zero padding bits of structs and unions).
 */
/* { dg-do run } */
/* { dg-options "-Wzero-init-padding-bits=all -fzero-init-padding-bits=all" } */

#include "gnu11-empty-init-warn-4.c"
