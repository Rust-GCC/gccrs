/* Test that no GNU11 warnings are produced (when explicitly disabled) about
 * initializers that might not zero padding bits (when configured not to zero
 * padding bits unless mandated by the language standard).
 */
/* { dg-do run } */
/* { dg-options "-Wzero-init-padding-bits=standard -fzero-init-padding-bits=standard" } */

#include "gnu11-empty-init-warn-4.c"
