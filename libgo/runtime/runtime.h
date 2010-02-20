/* runtime.h -- runtime support for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>

#include "go-alloc.h"
#include "go-panic.h"
#include "go-string.h"

typedef struct __go_string *String;

/* This file supports C files copied from the 6g runtime library.
   This is a version of the 6g runtime.h rewritten for gccgo's version
   of the code.  */

typedef signed int   int8    __attribute__ ((mode (QI)));
typedef unsigned int uint8   __attribute__ ((mode (QI)));
typedef signed int   int16   __attribute__ ((mode (HI)));
typedef unsigned int uint16  __attribute__ ((mode (HI)));
typedef signed int   int32   __attribute__ ((mode (SI)));
typedef unsigned int uint32  __attribute__ ((mode (SI)));
typedef signed int   int64   __attribute__ ((mode (DI)));
typedef unsigned int uint64  __attribute__ ((mode (DI)));
typedef float        float32 __attribute__ ((mode (SF)));
typedef double       float64 __attribute__ ((mode (DF)));
typedef unsigned int uintptr __attribute__ ((mode (pointer)));

/* Defined types.  */

typedef	uint8			bool;
typedef	uint8			byte;
typedef	struct	M		M;
typedef	struct	MCache		MCache;

/* We use mutexes for locks.  6g uses futexes directly, and perhaps
   someday we will do that too.  */

typedef pthread_mutex_t		Lock;

/* A Note.  */

typedef	struct	Note		Note;

struct Note {
	int32 woken;
};

/* Per CPU declarations.  */

extern __thread		M* 	m;

/* Constants.  */

enum
{
	true	= 1,
	false	= 0,
};

/* Structures.  */

struct	M
{
	int32	mallocing;
	int32	gcing;
	MCache	*mcache;
};

/* Macros.  */
#define	nelem(x)	(sizeof(x)/sizeof((x)[0]))
#define	nil		((void*)0)
#define USED(v)		((void) v)

/* We map throw to panic.  */
#define throw(s) __go_panic_msg (s)

/* Mutual exclusion locks.  */
#define lock(p) \
  (pthread_mutex_lock(p) == 0 || (__go_panic_msg ("lock failed"), 0))
#define unlock(p) \
  (pthread_mutex_unlock(p) == 0 || (__go_panic_msg ("unlock failed"), 0))

void	mallocinit(void);
void	siginit(void);
bool	sigsend(int32 sig);

/*
 * sleep and wakeup on one-time events.
 * before any calls to notesleep or notewakeup,
 * must call noteclear to initialize the Note.
 * then, any number of threads can call notesleep
 * and exactly one thread can call notewakeup (once).
 * once notewakeup has been called, all the notesleeps
 * will return.  future notesleeps will return immediately.
 */
void	noteclear(Note*);
void	notesleep(Note*);
void	notewakeup(Note*);

/* Functions.  */
#define runtime_memclr(buf, size) __builtin_memset(buf, 0, size)
#define runtime_mmap mmap
MCache*	allocmcache(void);
void	free(void *v);
void	addfinalizer(void*, void(*fn)(void*), int32);

#define cas(pval, old, new) __sync_bool_compare_and_swap (pval, old, new)
