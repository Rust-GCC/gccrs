/* { dg-options "--coverage -fpath-coverage -g -O2" } */
/* { dg-do compile } */
/* { dg-require-effective-target sigsetjmp } */

#include <setjmp.h>

extern sigjmp_buf jmpbuf;
typedef void (*sfun) (void);
extern sfun getfn (int);

/* This distilled srunner setup/teardown functions in check-0.15.2.  The
   combination of sigsetjmp, optimization, and debug statements causes a
   problem if the gimple statement iterator is not positioned correctely before
   adding instrumentation code.  This was originally setjmp, but a sigjmp_buf
   can't generally be passed to setjmp.  */

extern void
debug_after_labels (int *itr)
{
  for (; *itr; ++itr)
    {
      sfun fn = getfn (*itr);
      if (sigsetjmp (jmpbuf, 0) == 0)
	fn ();
    }
}
