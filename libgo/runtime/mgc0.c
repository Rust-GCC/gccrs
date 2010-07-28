// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Garbage collector -- step 0.
//
// Stop the world, mark and sweep garbage collector.
// NOT INTENDED FOR PRODUCTION USE.
//
// A mark and sweep collector provides a way to exercise
// and test the memory allocator and the stack walking machinery
// without also needing to get reference counting
// exactly right.

#include "runtime.h"
#include "malloc.h"

enum {
	Debug = 0
};

static bool finstarted;
static Lock finqlock = LOCK_INITIALIZER;
static pthread_cond_t finqcond = PTHREAD_COND_INITIALIZER;
static Finalizer *finq;
static int32 fingwait;

static void runfinq(void*);

enum {
	PtrSize = sizeof(void*)
};

static void
scanblock(int32 depth, byte *b, int64 n)
{
	int32 off;
	void *obj;
	uintptr size;
	uint32 *refp, ref;
	void **vp;
	int64 i;

	if(Debug > 1)
		printf("%d scanblock %p %lld\n", depth, b, (long long) n);
	off = (uint32)(uintptr)b & (PtrSize-1);
	if(off) {
		b += PtrSize - off;
		n -= PtrSize - off;
	}

	vp = (void**)b;
	n /= PtrSize;
	for(i=0; i<n; i++) {
		obj = vp[i];
		if(obj == nil)
			continue;
		if(mheap.closure_min != nil && mheap.closure_min <= (byte*)obj && (byte*)obj < mheap.closure_max) {
			if((((uintptr)obj) & 63) != 0)
				continue;

			// Looks like a Native Client closure.
			// Actual pointer is pointed at by address in first instruction.
			// Embedded pointer starts at byte 2.
			// If it is f4f4f4f4 then that space hasn't been
			// used for a closure yet (f4 is the HLT instruction).
			// See nacl/386/closure.c for more.
			void **pp;
			pp = *(void***)((byte*)obj+2);
			if(pp == (void**)0xf4f4f4f4)	// HLT... - not a closure after all
				continue;
			obj = *pp;
		}
		if(mheap.min <= (byte*)obj && (byte*)obj < mheap.max) {
			if(mlookup(obj, (byte**)&obj, &size, nil, &refp)) {
				ref = *refp;
				switch(ref & ~RefFlags) {
				case RefNone:
					if(Debug > 1)
						printf("%d found at %p: ", depth, &vp[i]);
					*refp = RefSome | (ref & RefFlags);
					if(!(ref & RefNoPointers))
						scanblock(depth+1, obj, size);
					break;
				}
			}
		}
	}
}

static void
markfin(void *v)
{
	uintptr size;
	uint32 *refp;

	size = 0;
	refp = nil;
	if(!mlookup(v, (byte**)&v, &size, nil, &refp) || !(*refp & RefHasFinalizer))
		throw("mark - finalizer inconsistency");
	
	// do not mark the finalizer block itself.  just mark the things it points at.
	scanblock(1, v, size);
}

struct globals {
	byte *start;
	uintptr size;
};

// FIXME: This needs to grow as needed.
#define GLOBALS_ENTRIES 16

static struct globals globals[GLOBALS_ENTRIES];

// Called by runtime.
void
__go_register_mem(void *start, void *end)
{
	int i;

	if(start == nil || end == nil)
		throw("__go_register_mem");
	if(start == end)
		return;
	for(i = 0; i < GLOBALS_ENTRIES; ++i) {
		if(globals[i].start == nil) {
			globals[i].start = (byte*)start;
			globals[i].size = (byte*)end - (byte*)start;
			return;
		}
	}
	throw("__go_register_mem out of space");
}

// Called by runtime for dlclose.
void
__go_deregister_mem(void *start, void *end)
{
	int i;

	if(start == end)
		return;
	for(i = 0; i < GLOBALS_ENTRIES; ++i) {
		if(globals[i].start == (byte*)start
		   && globals[i].size == (size_t)((byte*)end - (byte*)start)) {
			globals[i].start = nil;
			return;
		}
	}
	throw("__go_deregister_mem not found");
}

static void
mark(void)
{
	int i;

	// mark data+bss.
	// skip mheap itself, which has no interesting pointers
	// and is mostly zeroed and would not otherwise be paged in.
	for(i = 0; i < GLOBALS_ENTRIES; ++i) {
		if (globals[i].start == nil)
			continue;
		if ((byte*)&mheap >= globals[i].start
		    && (byte*)&mheap < globals[i].start + globals[i].size) {
			scanblock(0, globals[i].start, (byte*)&mheap - globals[i].start);
			scanblock(0, (byte*)(&mheap+1),
				  globals[i].start + globals[i].size - (byte*)(&mheap+1));
		}
		else
			scanblock(0, globals[i].start, globals[i].size);
	}

	// mark stacks
	__go_scanstacks(scanblock);

	// mark things pointed at by objects with finalizers
	walkfintab(markfin);
}

// free RefNone, free & queue finalizers for RefNone|RefHasFinalizer, reset RefSome
static void
sweepspan(MSpan *s)
{
	int32 n, npages, size;
	byte *p;
	uint32 ref, *gcrefp, *gcrefep;
	MCache *c;
	Finalizer *f;

	p = (byte*)(s->start << PageShift);
	if(s->sizeclass == 0) {
		// Large block.
		ref = s->gcref0;
		switch(ref & ~(RefFlags^RefHasFinalizer)) {
		case RefNone:
			// Free large object.
			mstats.alloc -= s->npages<<PageShift;
			runtime_memclr(p, s->npages<<PageShift);
			if(ref & RefProfiled)
				MProf_Free(p, s->npages<<PageShift);
			s->gcref0 = RefFree;
			MHeap_Free(&mheap, s, 1);
			break;
		case RefNone|RefHasFinalizer:
			f = getfinalizer(p, 1);
			if(f == nil)
				throw("finalizer inconsistency");
			f->arg = p;
			f->next = finq;
			finq = f;
			ref &= ~RefHasFinalizer;
			// fall through
		case RefSome:
		case RefSome|RefHasFinalizer:
			s->gcref0 = RefNone | (ref&RefFlags);
			break;
		}
		return;
	}

	// Chunk full of small blocks.
	MGetSizeClassInfo(s->sizeclass, &size, &npages, &n);
	gcrefp = s->gcref;
	gcrefep = s->gcref + n;
	for(; gcrefp < gcrefep; gcrefp++, p += size) {
		ref = *gcrefp;
		if(ref < RefNone)	// RefFree or RefStack
			continue;
		switch(ref & ~(RefFlags^RefHasFinalizer)) {
		case RefNone:
			// Free small object.
			if(ref & RefProfiled)
				MProf_Free(p, size);
			*gcrefp = RefFree;
			c = m->mcache;
			if(size > (int32)sizeof(uintptr))
				((uintptr*)p)[1] = 1;	// mark as "needs to be zeroed"
			mstats.alloc -= size;
			mstats.by_size[s->sizeclass].nfree++;
			MCache_Free(c, p, s->sizeclass, size);
			break;
		case RefNone|RefHasFinalizer:
			f = getfinalizer(p, 1);
			if(f == nil)
				throw("finalizer inconsistency");
			f->arg = p;
			f->next = finq;
			finq = f;
			ref &= ~RefHasFinalizer;
			// fall through
		case RefSome:
		case RefSome|RefHasFinalizer:
			*gcrefp = RefNone | (ref&RefFlags);
			break;
		}
	}
}

static void
sweep(void)
{
	MSpan *s;

	for(s = mheap.allspans; s != nil; s = s->allnext)
		if(s->state == MSpanInUse)
			sweepspan(s);
}

static Lock gcsema = LOCK_INITIALIZER;

// Initialized from $GOGC.  GOGC=off means no gc.
//
// Next gc is after we've allocated an extra amount of
// memory proportional to the amount already in use.
// If gcpercent=100 and we're using 4M, we'll gc again
// when we get to 8M.  This keeps the gc cost in linear
// proportion to the allocation cost.  Adjusting gcpercent
// just changes the linear constant (and also the amount of
// extra memory used).
static int32 gcpercent = -2;

void
gc(int32 force __attribute__ ((unused)))
{
	int64 t0, t1;
	char *p;
	Finalizer *fp;

	// The gc is turned off (via enablegc) until
	// the bootstrap has completed.
	// Also, malloc gets called in the guts
	// of a number of libraries that might be
	// holding locks.  To avoid priority inversion
	// problems, don't bother trying to run gc
	// while holding a lock.  The next mallocgc
	// without a lock will do the gc instead.
	if(!mstats.enablegc || m->locks > 0 /* || panicking */)
		return;

	if(gcpercent == -2) {	// first time through
		p = getenv("GOGC");
		if(p == nil || p[0] == '\0')
			gcpercent = 100;
		else if(strcmp(p, "off") == 0)
			gcpercent = -1;
		else
			gcpercent = atoi(p);
	}
	if(gcpercent < 0)
		return;

	lock(&finqlock);
	lock(&gcsema);
	m->locks++;	// disable gc during the mallocs in newproc
	t0 = nanotime();
	stoptheworld();
	if(force || mstats.heap_alloc >= mstats.next_gc) {
		mark();
		sweep();
		__go_stealcache();
		mstats.next_gc = mstats.heap_alloc+mstats.heap_alloc*gcpercent/100;
	}

	t1 = nanotime();
	mstats.numgc++;
	mstats.pause_ns += t1 - t0;
	if(mstats.debuggc)
		printf("pause %llu\n", (unsigned long long)t1-t0);
	unlock(&gcsema);
	starttheworld();

	// finqlock is still held.
	fp = finq;
	if(fp != nil) {
		// kick off or wake up goroutine to run queued finalizers
		if(!finstarted) {
			__go_go(runfinq, nil);
			finstarted = 1;
		}
		else if(fingwait) {
			fingwait = 0;
			pthread_cond_signal(&finqcond);
		}
	}
	m->locks--;
	unlock(&finqlock);
}

static void
runfinq(void* dummy)
{
	Finalizer *f, *next;

	USED(dummy);

	for(;;) {
		lock(&finqlock);
		f = finq;
		finq = nil;
		if(f == nil) {
			fingwait = 1;
			pthread_cond_wait(&finqcond, &finqlock.mutex);
			unlock(&finqlock);
			continue;
		}
		unlock(&finqlock);
		for(; f; f=next) {
			void *params[1];

			next = f->next;
			params[0] = &f->arg;
			reflect_call(f->ft, (void*)f->fn, 0, params, nil);
			f->fn = nil;
			f->arg = nil;
			f->next = nil;
			__go_free(f);
		}
		gc(1);	// trigger another gc to clean up the finalized objects, if possible
	}
}

void
__go_enable_gc()
{
  mstats.enablegc = 1;
}
