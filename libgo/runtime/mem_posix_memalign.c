#include <errno.h>

#include "runtime.h"
#include "malloc.h"

void*
SysAlloc(uintptr n)
{
	void *p;

	mstats.sys += n;
	errno = posix_memalign(&p, PageSize, n);
	if (errno > 0) {
		perror("posix_memalign");
		exit(2);
	}
	return p;
}

void
SysUnused(void *v, uintptr n)
{
	USED(v);
	USED(n);
	// TODO(rsc): call madvise MADV_DONTNEED
}

void
SysFree(void *v, uintptr n)
{
	USED(v);
	USED(n);
	// TODO(rsc): call munmap
}

