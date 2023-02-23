/* Subroutine for function pointer canonicalization on PA-RISC with ELF32.
   Copyright (C) 2002-2023 Free Software Foundation, Inc.
   Contributed by John David Anglin (dave.anglin@nrc.ca).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */


/* WARNING: The code is this function depends on internal and undocumented
   details of the GNU linker and dynamic loader as implemented for parisc
   linux.  */

/* This MUST match the defines sysdeps/hppa/dl-machine.h and
   bfd/elf32-hppa.c.  */
#define GOT_FROM_PLT_STUB (4*4)

/* List of byte offsets in _dl_runtime_resolve to search for "bl" branches.
   The first "bl" branch instruction found MUST be a call to fixup.  See
   the define for TRAMPOLINE_TEMPLATE in sysdeps/hppa/dl-machine.h.  If
   the trampoline template is changed, the list must be appropriately
   updated.  The offset of -4 allows for a magic branch at the start of
   the template should it be necessary to change the current branch
   position.  */
#define NOFFSETS 2
static int fixup_branch_offset[NOFFSETS] = { -4, 32 };

#define GET_FIELD(X, FROM, TO) \
  ((X) >> (31 - (TO)) & ((1 << ((TO) - (FROM) + 1)) - 1))
#define SIGN_EXTEND(VAL,BITS) \
  ((int) ((VAL) >> ((BITS) - 1) ? ((unsigned)(-1) << (BITS)) | (VAL) : (VAL)))

struct link_map;
typedef int (*fptr_t) (void);
typedef int (*fixup_t) (struct link_map *, unsigned int);
extern unsigned int _GLOBAL_OFFSET_TABLE_;

static inline int
_dl_read_access_allowed (unsigned int addr)
{
  int result;

  asm ("proberi (%1),3,%0" : "=r" (result) : "r" (addr) : );

  return result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"

/* __canonicalize_funcptr_for_compare must be hidden so that it is not
   placed in the dynamic symbol table.  Like millicode functions, it
   must be linked into all binaries in order access the got table of 
   that binary.  However, we don't use the millicode calling convention
   and the routine must be a normal function so that it can be compiled
   as pic code.  */
unsigned int __canonicalize_funcptr_for_compare (fptr_t)
      __attribute__ ((visibility ("hidden")));

unsigned int
__canonicalize_funcptr_for_compare (fptr_t fptr)
{
  static unsigned int fixup_plabel[2] __attribute__((used));
  fixup_t fixup;
  volatile unsigned int *plabel;
  unsigned int *got, *iptr, reloc_offset;
  int i;

  /* -1 and page 0 are special.  -1 is used in crtend to mark the end of
     a list of function pointers.  Also return immediately if the plabel
     bit is not set in the function pointer.  In this case, the function
     pointer points directly to the function.  */
  if ((int) fptr == -1 || (unsigned int) fptr < 4096 || !((int) fptr & 2))
    return (unsigned int) fptr;

  /* The function pointer points to a function descriptor (plabel).  If
     the plabel hasn't been resolved, the first word of the plabel points
     to the entry of the PLT stub just before the global offset table.
     The second word in the plabel contains the relocation offset for the
     function.  */
  plabel = (volatile unsigned int *) ((unsigned int) fptr & ~3);
  if (!_dl_read_access_allowed ((unsigned int)plabel))
    return (unsigned int) fptr;

  /* Load first word of candidate descriptor.  It should be a pointer
     with word alignment and point to memory that can be read.  */
  got = (unsigned int *) plabel[0];
  if (((unsigned int) got & 3) != 0
      || !_dl_read_access_allowed ((unsigned int)got))
    return (unsigned int) fptr;

  /* We need to load the relocation offset before the function address.  */
  reloc_offset = plabel[1];
  __sync_synchronize();
  got = (unsigned int *) (plabel[0] + GOT_FROM_PLT_STUB);

  /* Return the address of the function if the plabel has been resolved.  */
  if (got !=  &_GLOBAL_OFFSET_TABLE_)
    return plabel[0];

  /* Find the first "bl" branch in the offset search list.  This is a
     call to _dl_fixup or a magic branch to fixup at the beginning of the
     trampoline template.  The fixup function does the actual runtime
     resolution of function descriptors.  We only look for "bl" branches
     with a 17-bit pc-relative displacement.  */
  for (i = 0; i < NOFFSETS; i++)
    {
      iptr = (unsigned int *) (got[-2] + fixup_branch_offset[i]);
      if ((*iptr & 0xfc00e000) == 0xe8000000)
	break;
    }

  /* This should not happen... */
  if (i == NOFFSETS)
    return ~0;

  /* Extract the 17-bit displacement from the instruction.  */
  iptr += SIGN_EXTEND (GET_FIELD (*iptr, 19, 28) |
		       GET_FIELD (*iptr, 29, 29) << 10 |
		       GET_FIELD (*iptr, 11, 15) << 11 |
		       GET_FIELD (*iptr, 31, 31) << 16, 17);

  /* Build a plabel for an indirect call to _dl_fixup.  */
  fixup_plabel[0] = (unsigned int) iptr + 8;	/* address of fixup */
  fixup_plabel[1] = got[-1];			/* ltp for fixup */
  fixup = (fixup_t) ((int) fixup_plabel | 2);

  /* Call fixup to resolve the function address.  got[1] contains the
     link_map pointer and plabel[1] the relocation offset.  */
  fixup ((struct link_map *) got[1], reloc_offset);

  return plabel[0];
}

#pragma GCC diagnostic pop
