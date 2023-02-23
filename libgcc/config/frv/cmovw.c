/* Move word library function.
   Copyright (C) 2000-2023 Free Software Foundation, Inc.
   Contributed by Red Hat, Inc.
  
   This file is part of GCC.
  
   GCC is free software ; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.
  
   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY ; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

void
__cmovw (int *dest, const int *src, unsigned len)
{
  unsigned i;
  unsigned num = len >> 2;
  unsigned xlen = len & ~3;
  char *dest_byte = (char *)dest;
  const char *src_byte = (const char *)src;

  if (dest_byte < src_byte || dest_byte > src_byte+len)
    {
      for (i = 0; i < num; i++)
	dest[i] = src[i];

      while (len > xlen)
	{
	  dest_byte[xlen] = src_byte[xlen];
	  xlen++;
	}
    }
  else
    {
      while (len-- > 0)
	dest_byte[len] = src_byte[len];
    }
}
