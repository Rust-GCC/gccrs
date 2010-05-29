// Copyright 2010 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "runtime.h"

void
initlock(Lock *l)
{
	if(pthread_mutex_init(&l->mutex, NULL) != 0)
		throw("pthread_mutex_init failed");
}

void
lock(Lock *l)
{
	if(pthread_mutex_lock(&l->mutex) != 0)
		throw("lock failed");
}

void
unlock(Lock *l)
{
	if(pthread_mutex_unlock(&l->mutex) != 0)
		throw("unlock failed");
}

void
destroylock(Lock *l)
{
	pthread_mutex_destroy(&l->mutex);
}
