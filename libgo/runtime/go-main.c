/* go-main.c -- the main function for a Go program.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include <stdlib.h>
#include <time.h>

#include "go-alloc.h"
#include "array.h"
#include "go-string.h"

/* The main function for a Go program.  This records the command line
   parameters, calls the real main function, and returns a zero status
   if the real main function returns.  */

extern char **environ;

extern struct __go_open_array Args asm ("os.Args");

extern struct __go_open_array Envs asm ("os.Envs");

extern void real_main () asm ("main.main");

int
main (int argc, char **argv)
{
  int i;
  struct __go_string **values;

  Args.__count = argc;
  Args.__capacity = argc;
  values = __go_alloc (argc * sizeof (struct __go_string *));
  for (i = 0; i < argc; ++i)
    {
      size_t len;
      struct __go_string *s;

      len = __builtin_strlen (argv[i]);
      s = __go_alloc (sizeof (struct __go_string) + len);
      s->__length = len;
      __builtin_memcpy (&s->__data[0], argv[i], len);
      values[i] = s;
    }
  Args.__values = values;

  for (i = 0; environ[i] != NULL; ++i)
    ;
  Envs.__count = i;
  Envs.__capacity = i;
  values = __go_alloc (i * sizeof (struct __go_string *));
  for (i = 0; environ[i] != NULL; ++i)
    {
      size_t len;
      struct __go_string *s;

      len = __builtin_strlen (environ[i]);
      s = __go_alloc (sizeof (struct __go_string) + len);
      s->__length = len;
      __builtin_memcpy (&s->__data[0], environ[i], len);
      values[i] = s;
    }
  Envs.__values = values;

  srandom ((unsigned int) time (NULL));

  real_main ();

  return 0;
}
