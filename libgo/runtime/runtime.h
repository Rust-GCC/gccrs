/* runtime.h -- runtime support for Go.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "config.h"

#define _GNU_SOURCE
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#include "go-alloc.h"
#include "go-panic.h"
#include "go-string.h"

typedef struct __go_string String;

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
typedef	struct	Lock		Lock;

/* We use mutexes for locks.  6g uses futexes directly, and perhaps
   someday we will do that too.  */

struct	Lock
{
	pthread_mutex_t	mutex;
};

/* A Note.  */

typedef	struct	Note		Note;

struct Note {
	int32 woken;
};

/* Per CPU declarations.  */

#ifdef __rtems__
#define __thread
#endif

extern __thread		M* 	m;

#ifdef __rtems__
#undef __thread
#endif

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
	int32	locks;
	int32	nomemprof;
	MCache	*mcache;

	/* For the list of all threads.  */
	struct __go_thread_id *list_entry;

	/* For the garbage collector.  */
	void	*gc_sp;
	size_t	gc_len;
	void	*gc_next_segment;
	void	*gc_next_sp;
	void	*gc_initial_sp;
	struct __go_panic_defer_struct *gc_panic_defer;
};

/* Macros.  */
#define	nelem(x)	(sizeof(x)/sizeof((x)[0]))
#define	nil		((void*)0)
#define USED(v)		((void) v)

/* We map throw to assert.  */
#define throw(s) assert(s == 0)

void*	mal(uintptr);
void	mallocinit(void);
void	siginit(void);
bool	__go_sigsend(int32 sig);
int64	nanotime(void);

void	stoptheworld(void);
void	starttheworld(void);
void	__go_go(void (*pfn)(void*), void*);
void	__go_gc_goroutine_init(void*);
void	__go_enable_gc(void);
int	__go_run_goroutine_gc(int);
void	__go_scanstacks(void (*scan)(int32, byte *, int64));
void	__go_stealcache(void);

/*
 * mutual exclusion locks.  in the uncontended case,
 * as fast as spin locks (just a few user-level instructions),
 * but on the contention path they sleep in the kernel.
 */
#define	LOCK_INITIALIZER	{ PTHREAD_MUTEX_INITIALIZER }
void	initlock(Lock*);
void	lock(Lock*);
void	unlock(Lock*);
void	destroylock(Lock*);
bool	trylock(Lock*);

void semacquire (uint32 *) asm ("libgo_runtime.runtime.Semacquire");
void semrelease (uint32 *) asm ("libgo_runtime.runtime.Semrelease");

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
#define runtime_memclr(buf, size) __builtin_memset((buf), 0, (size))
#define mcmp(a, b, s) __builtin_memcmp((a), (b), (s))
MCache*	allocmcache(void);
void	free(void *v);
struct __go_func_type;
void	addfinalizer(void*, void(*fn)(void*), const struct __go_func_type *);
void	walkfintab(void (*fn)(void*));
#define runtime_mmap mmap
#define cas(pval, old, new) __sync_bool_compare_and_swap (pval, old, new)

struct __go_func_type;
void reflect_call(const struct __go_func_type *, const void *, _Bool, void **,
		  void **)
  asm ("libgo_reflect.reflect.call");

#ifdef __rtems__
void __wrap_rtems_task_variable_add(void **);
#endif
