/* PR debug/59418 */
/* Reported by Ryan Mansfield <rmansfield@qnx.com> */

/* { dg-do compile } */
/* { dg-options "-Os -g" } */
/* { dg-options "-mcpu=unset -march=armv7-a+fp -mfloat-abi=hard -Os -g" { target { arm*-*-* && { ! arm_thumb1 } } } } */

extern int printf (const char *__format, ...);
double bar (const char *, int);

void
foo (const char *pptr, int caplen)
{
  int type;
  const char *tptr;
  if (caplen < 4)
    {
      (void) printf ("foo");
      return;
    }
  while (tptr < pptr)
    {
      switch (type)
	{
	case 0x01:
	  printf ("");
	case 0x0b:
	  printf ("");
	case 0x0e:
	  printf ("");
	case 0x10:
	  printf ("%1.2fW", bar (tptr, caplen) / 1000.0);
	}
    }
  printf ("foo");
}
