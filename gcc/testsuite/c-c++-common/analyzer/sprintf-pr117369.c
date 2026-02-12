/* { dg-additional-options "-O" } */

extern int
sprintf (char *__restrict __s,
	 const char *__restrict __format, ...)
  __attribute__ ((__nothrow__));

int
main()
{
  char buf[16];
  sprintf(buf + 1, "."); /* { dg-bogus "buffer overflow" } */
}
