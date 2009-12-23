/* go-signal.c -- signal handling for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <assert.h>
#include <signal.h>
#include <stdlib.h>

#include "go-signal.h"

#include "runtime.h"

#undef int 

#ifndef SA_ONSTACK
#define SA_ONSTACK 0
#endif

/* What to do for a signal.  */

struct sigtab
{
  /* Signal number.  */
  int sig;
  /* Nonzero if the signal should be ignored.  */
  _Bool ignore;
};

/* What to do for signals.  */

static struct sigtab signals[] =
{
  { SIGHUP, 0 },
  { SIGINT, 0 },
  { SIGALRM, 1 },
  { SIGTERM, 0 },
#ifdef SIGUSR1
  { SIGUSR1, 1 },
#endif
#ifdef SIGUSR2
  { SIGUSR2, 1 },
#endif
#ifdef SIGTSTP
  { SIGTSTP, 1 },
#endif
#ifdef SIGTTIN
  { SIGTTIN, 1 },
#endif
#ifdef SIGTTOU
  { SIGTTOU, 1 },
#endif
#ifdef SIGURG
  { SIGURG, 1 },
#endif
#ifdef SIGXCPU
  { SIGXCPU, 1 },
#endif
#ifdef SIGXFSZ
  { SIGXFSZ, 1 },
#endif
#ifdef SIGVTARLM
  { SIGVTALRM, 1 },
#endif
#ifdef SIGPROF
  { SIGPROF, 1 },
#endif
#ifdef SIGWINCH
  { SIGWINCH, 1 },
#endif
#ifdef SIGIO
  { SIGIO, 1 },
#endif
#ifdef SIGPWR
  { SIGPWR, 1 },
#endif
  { -1, 0 }
};

/* The Go signal handler.  */

static void
sighandler (int sig)
{
  int i;

  if (sigsend (sig))
    return;
  for (i = 0; signals[i].sig != -1; ++i)
    {
      if (signals[i].sig == sig)
	{
	  struct sigaction sa;

	  if (signals[i].ignore)
	    return;

	  memset (&sa, 0, sizeof sa);

	  sa.sa_handler = SIG_DFL;

	  i = sigemptyset (&sa.sa_mask);
	  assert (i == 0);

	  if (sigaction (sig, &sa, NULL) != 0)
	    abort ();

	  raise (sig);
	  exit (2);
	}
    }
  abort ();
}

/* Initialize signal handling for Go.  This is called when the program
   starts.  */

void
__initsig ()
{
  struct sigaction sa;
  int i;

  siginit ();

  memset (&sa, 0, sizeof sa);

  sa.sa_handler = sighandler;

  i = sigfillset (&sa.sa_mask);
  assert (i == 0);

  for (i = 0; signals[i].sig != -1; ++i)
    if (sigaction (signals[i].sig, &sa, NULL) != 0)
      assert (0);
}
