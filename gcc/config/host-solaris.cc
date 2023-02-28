/* Solaris host-specific hook definitions.
   Copyright (C) 2004-2023 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "hosthooks.h"
#include "hosthooks-def.h"


#undef HOST_HOOKS_GT_PCH_GET_ADDRESS
#define HOST_HOOKS_GT_PCH_GET_ADDRESS sol_gt_pch_get_address
#undef HOST_HOOKS_GT_PCH_USE_ADDRESS
#define HOST_HOOKS_GT_PCH_USE_ADDRESS sol_gt_pch_use_address

/* Before Solaris 11, the mmap ADDR parameter is mostly ignored without
   MAP_FIXED set.  Before we give up, search the desired address space with
   mincore to see if the space is really free.  */

static void *
mmap_fixed (void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
  void *base;

  base = mmap ((caddr_t) addr, len, prot, flags, fd, off);
  
  if (base != addr)
    {
      size_t page_size = getpagesize();
      char one_byte;
      size_t i;

      if (base != (void *) MAP_FAILED)
	munmap ((caddr_t) base, len);

      errno = 0;
      for (i = 0; i < len; i += page_size)
	if (mincore ((char *)addr + i, page_size, (char *) &one_byte) == -1
	    && errno == ENOMEM)
	  continue; /* The page is not mapped.  */
	else
	  break;

      if (i >= len)
	base = mmap ((caddr_t) addr, len, prot, flags | MAP_FIXED, fd, off);
    }

  return base;
}

/* For various ports, try to guess a fixed spot in the vm space
   that's probably free.  Based on McDougall, Mauro, Solaris Internals, 2nd
   ed., p.460-461, fig. 9-3, 9-4, 9-5.  */
#if defined(__sparcv9__)
/* This low to avoid VA hole on UltraSPARC I/II.  */
# define TRY_EMPTY_VM_SPACE	0x70000000000
#elif defined(__sparc__)
# define TRY_EMPTY_VM_SPACE	0x80000000
#elif defined(__x86_64__)
# define TRY_EMPTY_VM_SPACE	0x80000000000
#elif defined(__i386__)
# define TRY_EMPTY_VM_SPACE	0xB0000000
#else
# define TRY_EMPTY_VM_SPACE	0
#endif

/* Determine a location where we might be able to reliably allocate
   SIZE bytes.  FD is the PCH file, though we should return with the
   file unmapped.  */

static void *
sol_gt_pch_get_address (size_t size, int fd)
{
  void *addr;

  addr = mmap_fixed ((caddr_t) TRY_EMPTY_VM_SPACE, size,
		     PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

  /* If we failed the map, that means there's *no* free space.  */
  if (addr == (void *) MAP_FAILED)
    return NULL;
  /* Unmap the area before returning.  */
  munmap ((caddr_t) addr, size);

  return addr;
}

/* Map SIZE bytes of FD+OFFSET at BASE.  Return 1 if we succeeded at 
   mapping the data at BASE, -1 if we couldn't.  */

static int
sol_gt_pch_use_address (void *&base, size_t size, int fd, size_t offset)
{
  void *addr;

  /* We're called with size == 0 if we're not planning to load a PCH
     file at all.  This allows the hook to free any static space that
     we might have allocated at link time.  */
  if (size == 0)
    return -1;

  addr = mmap_fixed ((caddr_t) base, size,
		     PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, offset);

  return addr == base ? 1 : -1;
}


const struct host_hooks host_hooks = HOST_HOOKS_INITIALIZER;
