/* { dg-additional-options "-Wno-nonnull" } */
#include "analyzer-decls.h"


extern int strcasecmp (const char *s1, const char *s2);

int
test_passthrough (const char *s1, const char *s2)
{
  return strcasecmp (s1, s2);
}

void
test_literals (void)
{
  strcasecmp ("string", "STRING");
}

void
test_unterminated (void)
{
  char buf[3] = {'a', 'b', 'c' };
  strcasecmp (buf, "abc"); /* { dg-warning "stack-based buffer over-read" } */
}

void
test_unterminated_2 (void)
{
  char buf[3] = { 'a', 'b', 'c' };
  strcasecmp ("abc", buf); /* { dg-warning "stack-based buffer over-read" } */
}

void
test_null_1 (void)
{
  strcasecmp (NULL, "abc"); /* { dg-warning "use of NULL where non-null expected" } */
}

void
test_null_2 (void)
{
  strcasecmp("abc", NULL); /* { dg-warning "use of NULL where non-null expected" } */
}
