#include <stdlib.h>
#include "analyzer-decls.h"

extern void do_something (const char *p)
  __attribute__((nonnull (1)));

/* Verify that the analyzer bifurcates on getenv, considering
   both NULL and non-NULL outcomes.  */

void test_null_deref (void)
{
  char *p = getenv ("HOME"); /* { dg-message "when 'getenv' returns NULL" } */
  *p = 'a'; /* { dg-warning "dereference of NULL 'p'" } */
}

void test_checked (void)
{
  char *p = getenv ("HOME");
  if (p)
    do_something (p); /* no warning: p is non-NULL here.  */
}

void test_unchecked_use (void)
{
  char *p = getenv ("PATH"); /* { dg-message "when 'getenv' returns NULL" } */
  do_something (p); /* { dg-warning "use of NULL 'p' where non-null expected" } */
}

void test_bifurcation (void)
{
  char *p = getenv ("EDITOR");
  if (p)
    __analyzer_eval (p != NULL); /* { dg-warning "TRUE" } */
}

void test_getenv_returns_nonnull (void)
{
  char *p = getenv ("TERM");
  if (!p)
    return;
  __analyzer_eval (p != NULL); /* { dg-warning "TRUE" } */
}

char *test_passthrough (const char *name)
{
  return getenv (name);
}

void test_unterminated (void)
{
  char buf[3] = "abc";
  getenv (buf); /* { dg-warning "stack-based buffer over-read" } */
  /* { dg-message "while looking for null terminator for argument 1 \\('&buf'\\) of 'getenv'..." "event" { target *-*-* } .-1 } */
}
