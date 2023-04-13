/* See e.g. https://en.cppreference.com/w/c/io/fprintf
   and https://www.man7.org/linux/man-pages/man3/sprintf.3.html */

extern int
sprintf(char* dst, const char* fmt, ...)
  __attribute__((__nothrow__));

#define NULL ((void *)0)

int
test_passthrough (char* dst, const char* fmt)
{
  /* This assumes that fmt doesn't have any arguments.  */
  return sprintf (dst, fmt);
}

void
test_known (void)
{
  char buf[10];
  int res = sprintf (buf, "foo");
  /* TODO: ideally we would know the value of "res" is 3,
     and known the content and strlen of "buf" after the call */
}

int
test_null_dst (void)
{
  return sprintf (NULL, "hello world"); /* { dg-warning "use of NULL where non-null expected" } */
}

int
test_null_fmt (char *dst)
{
  return sprintf (dst, NULL);  /* { dg-warning "use of NULL where non-null expected" } */
}

int
test_uninit_dst (void)
{
  char *dst;
  return sprintf (dst, "hello world"); /* { dg-warning "use of uninitialized value 'dst'" } */
}

int
test_uninit_fmt_ptr (char *dst)
{
  const char *fmt;
  return sprintf (dst, fmt); /* { dg-warning "use of uninitialized value 'fmt'" } */
}

int
test_uninit_fmt_buf (char *dst)
{
  const char fmt[10];
  return sprintf (dst, fmt); // TODO (PR analyzer/105899): complain about "fmt" not being initialized
}

int
test_fmt_not_terminated (char *dst)
{
  const char fmt[3] = "foo";
  return sprintf (dst, fmt); // TODO (PR analyzer/105899): complain about "fmt" not being terminated
}
