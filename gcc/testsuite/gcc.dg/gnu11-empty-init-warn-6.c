/* Test that GNU11 warnings are only produced (when enabled for unions) about
 * initializers that might not zero padding bits of unions (when configured not
 * to zero padding bits unless mandated by the language standard).
 */
/* { dg-do run } */
/* { dg-options "-fzero-init-padding-bits=standard" } */

#include "gnu11-empty-init-warn-2.c"
