/* Definitions for option handling for OpenRISC.
   Copyright (C) 2021-2023 Free Software Foundation, Inc.
   Contributed by Stafford Horne.

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

#ifndef GCC_OR1K_OPTS_H
#define GCC_OR1K_OPTS_H

/* The OpenRISC code generation models available.  */
enum or1k_cmodel_type {
  CMODEL_SMALL,
  CMODEL_LARGE
};

#endif /* GCC_OR1K_OPTS_H */
