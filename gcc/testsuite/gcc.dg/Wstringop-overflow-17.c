/* Test to verify that -Wstringop-overflow mentions the referenced object
   { dg-do compile }
   { dg-options "-O2 -Wall" } */

static void copy_n (char *d, const char *s, int n)
{
  while (n--)
    *d++ = *s++;    // { dg-warning "writing 1 byte into a region of size 0" }
  *d = 0;           // { dg-warning "writing 1 byte into a region of size 0" }
}

void sink (void*);

void call_copy_n (const char *s)
{
  char a[3];        // { dg-message "destination object declared here" }
  copy_n (a, "1234567", 7);
  sink (a);
}
