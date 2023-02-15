/* Definitions for PA_RISC with ELF format on 64-bit Linux
   Copyright (C) 1999-2023 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#if 0 /* needs some work :-( */
/* If defined, this macro specifies a table of register pairs used to
   eliminate unneeded registers that point into the stack frame.  */

#define ELIMINABLE_REGS							\
{									\
  {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},				\
  {ARG_POINTER_REGNUM,	 STACK_POINTER_REGNUM},				\
  {ARG_POINTER_REGNUM,	 FRAME_POINTER_REGNUM},				\
}

/* This macro returns the initial difference between the specified pair
   of registers.  */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
  do								\
    {								\
      int fsize;						\
								\
      fsize = pa_compute_frame_size (get_frame_size (), 0);	\
      if ((TO) == FRAME_POINTER_REGNUM				\
	  && (FROM) == ARG_POINTER_REGNUM)			\
	{							\
	  (OFFSET) = -16;					\
	  break;						\
	}							\
								\
      gcc_assert ((TO) == STACK_POINTER_REGNUM);		\
								\
      switch (FROM)						\
	{							\
	case FRAME_POINTER_REGNUM:				\
	  (OFFSET) = - fsize;					\
	  break;						\
								\
	case ARG_POINTER_REGNUM:				\
	  (OFFSET) = - fsize - 16;				\
	  break;						\
								\
	default:						\
	  gcc_unreachable ();					\
	}							\
    } while (0)
#endif
