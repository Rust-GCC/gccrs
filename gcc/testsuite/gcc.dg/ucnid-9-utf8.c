/* Test __func__ with extended identifiers and character set
   conversions.  */
/* { dg-do run } */
/* { dg-xfail-if "" { "powerpc-ibm-aix*" } } */
/* { dg-skip-if "" { ! ucn } } */
/* { dg-options "-std=c99 -fexec-charset=ISO-8859-1 -g" } */
/* { dg-require-iconv "ISO-8859-1" } */

extern int strcmp (const char *, const char *);
extern void abort (void);
extern void exit (int);

void
é (void)
{
  if (strcmp (__func__, "é") != 0)
    abort ();
}

int
main (void)
{
  é ();
  exit (0);
}
