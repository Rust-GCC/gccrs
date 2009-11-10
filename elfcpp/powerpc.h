// powerpc.h -- ELF definitions specific to EM_PPC and EM_PPC64  -*- C++ -*-

// Copyright 2008, Free Software Foundation, Inc.
// Written by David S. Miller <davem@davemloft.net>.

// This file is part of elfcpp.
   
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation; either version 2, or
// (at your option) any later version.

// In addition to the permissions in the GNU Library General Public
// License, the Free Software Foundation gives you unlimited
// permission to link the compiled version of this file into
// combinations with other programs, and to distribute those
// combinations without any restriction coming from the use of this
// file.  (The Library Public License restrictions do apply in other
// respects; for example, they cover modification of the file, and
/// distribution when not linked into a combined executable.)

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.

// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
// 02110-1301, USA.

#ifndef ELFCPP_POWERPC_H
#define ELFCPP_POWERPC_H

namespace elfcpp
{

// The relocation numbers for 32-bit and 64-bit powerpc are nearly
// identical.  Therefore I've adopted the convention of using
// R_POWERPC_foo for values which are the same in R_PPC_* and R_PPC64_*.
// For relocations which are specific to the word size I will use
// R_PPC_foo or R_PPC64_foo.
enum
{
  R_POWERPC_NONE = 0,
  R_POWERPC_ADDR32 = 1,
  R_POWERPC_ADDR24 = 2,
  R_POWERPC_ADDR16 = 3,
  R_POWERPC_ADDR16_LO = 4,
  R_POWERPC_ADDR16_HI = 5,
  R_POWERPC_ADDR16_HA = 6,
  R_POWERPC_ADDR14 = 7,
  R_POWERPC_ADDR14_BRTAKEN = 8,
  R_POWERPC_ADDR14_BRNTAKEN = 9,
  R_POWERPC_REL24 = 10,
  R_POWERPC_REL14 = 11,
  R_POWERPC_REL14_BRTAKEN = 12,
  R_POWERPC_REL14_BRNTAKEN = 13,
  R_POWERPC_GOT16 = 14,
  R_POWERPC_GOT16_LO = 15,
  R_POWERPC_GOT16_HI = 16,
  R_POWERPC_GOT16_HA = 17,
  R_PPC_PLTREL24 = 18,
  R_POWERPC_COPY = 19,
  R_POWERPC_GLOB_DAT = 20,
  R_POWERPC_JMP_SLOT = 21,
  R_POWERPC_RELATIVE = 22,
  R_PPC_LOCAL24PC = 23,
  R_POWERPC_UADDR32 = 24,
  R_POWERPC_UADDR16 = 25,
  R_POWERPC_REL32 = 26,
  R_POWERPC_PLT32 = 27,
  R_POWERPC_PLTREL32 = 28,
  R_POWERPC_PLT16_LO = 29,
  R_POWERPC_PLT16_HI = 30,
  R_POWERPC_PLT16_HA = 31,
  R_PPC_SDAREL16 = 32,
  R_POWERPC_SECTOFF = 33,
  R_POWERPC_SECTOFF_LO = 34,
  R_POWERPC_SECTOFF_HI = 35,
  R_POWERPC_SECTOFF_HA = 36,
  R_POWERPC_ADDR30 = 37,
  R_PPC64_ADDR64 = 38,
  R_PPC64_ADDR16_HIGHER = 39,
  R_PPC64_ADDR16_HIGHERA = 40,
  R_PPC64_ADDR16_HIGHEST = 41,
  R_PPC64_ADDR16_HIGHESTA = 42,
  R_PPC64_UADDR64 = 43,
  R_PPC64_REL64 = 44,
  R_PPC64_PLT64 = 45,
  R_PPC64_PLTREL64 = 46,
  R_PPC64_TOC16 = 47,
  R_PPC64_TOC16_LO = 48,
  R_PPC64_TOC16_HI = 49,
  R_PPC64_TOC16_HA = 50,
  R_PPC64_TOC = 51,
  R_PPC64_PLTGOT16 = 52,
  R_PPC64_PLTGOT16_LO = 53,
  R_PPC64_PLTGOT16_HI = 54,
  R_PPC64_PLTGOT16_HA = 55,
  R_PPC64_ADDR16_DS = 56,
  R_PPC64_ADDR16_LO_DS = 57,
  R_PPC64_GOT16_DS = 58,
  R_PPC64_GOT16_LO_DS = 59,
  R_PPC64_PLT16_LO_DS = 60,
  R_PPC64_SECTOFF_DS = 61,
  R_PPC64_SECTOFF_LO_DS = 62,
  R_PPC64_TOC16_DS = 63,
  R_PPC64_TOC16_LO_DS = 64,
  R_PPC64_PLTGOT16_DS = 65,
  R_PPC64_PLTGOT16_LO_DS = 66,
  R_POWERPC_TLS = 67,
  R_POWERPC_DTPMOD = 68,
  R_POWERPC_TPREL16 = 69,
  R_POWERPC_TPREL16_LO = 70,
  R_POWERPC_TPREL16_HI = 71,
  R_POWERPC_TPREL16_HA = 72,
  R_POWERPC_TPREL = 73,
  R_POWERPC_DTPREL16 = 74,
  R_POWERPC_DTPREL16_LO = 75,
  R_POWERPC_DTPREL16_HI = 76,
  R_POWERPC_DTPREL16_HA = 77,
  R_POWERPC_DTPREL = 78,
  R_POWERPC_GOT_TLSGD16 = 79,
  R_POWERPC_GOT_TLSGD16_LO = 80,
  R_POWERPC_GOT_TLSGD16_HI = 81,
  R_POWERPC_GOT_TLSGD16_HA = 82,
  R_POWERPC_GOT_TLSLD16 = 83,
  R_POWERPC_GOT_TLSLD16_LO = 84,
  R_POWERPC_GOT_TLSLD16_HI = 85,
  R_POWERPC_GOT_TLSLD16_HA = 86,
  R_POWERPC_GOT_TPREL16 = 87,
  R_POWERPC_GOT_TPREL16_LO = 88,
  R_POWERPC_GOT_TPREL16_HI = 89,
  R_POWERPC_GOT_TPREL16_HA = 90,
  R_POWERPC_GOT_DTPREL16 = 91,
  R_POWERPC_GOT_DTPREL16_LO = 92,
  R_POWERPC_GOT_DTPREL16_HI = 93,
  R_POWERPC_GOT_DTPREL16_HA = 94,
  R_PPC64_TPREL16_DS = 95,
  R_PPC64_TPREL16_LO_DS = 96,
  R_PPC64_TPREL16_HIGHER = 97,
  R_PPC64_TPREL16_HIGHERA = 98,
  R_PPC64_TPREL16_HIGHEST = 99,
  R_PPC64_TPREL16_HIGHESTA = 100,
  R_PPC64_DTPREL16_DS = 101,
  R_PPC64_DTPREL16_LO_DS = 102,
  R_PPC64_DTPREL16_HIGHER = 103,
  R_PPC64_DTPREL16_HIGHERA = 104,
  R_PPC64_DTPREL16_HIGHEST = 105,
  R_PPC64_DTPREL16_HIGHESTA = 106,
  R_PPC_EMB_NADDR32 = 101,
  R_PPC_EMB_NADDR16 = 102,
  R_PPC_EMB_NADDR16_LO = 103,
  R_PPC_EMB_NADDR16_HI = 104,
  R_PPC_EMB_NADDR16_HA = 105,
  R_PPC_EMB_SDAI16 = 106,
  R_PPC_EMB_SDA2I16 = 107,
  R_PPC_EMB_SDA2REL = 108,
  R_PPC_EMB_SDA21 = 109,
  R_PPC_EMB_MRKREF = 110,
  R_PPC_EMB_RELSEC16 = 111,
  R_PPC_EMB_RELST_LO = 112,
  R_PPC_EMB_RELST_HI = 113,
  R_PPC_EMB_RELST_HA = 114,
  R_PPC_EMB_BIT_FLD = 115,
  R_PPC_EMB_RELSDA = 116,
  R_PPC_RELAX32 = 245,
  R_PPC_RELAX32PC = 246,
  R_PPC_RELAX32_PLT = 247,
  R_PPC_RELAX32PC_PLT = 248,
  R_PPC_REL16 = 249,
  R_PPC_REL16_LO = 250,
  R_PPC_REL16_HI = 251,
  R_PPC_REL16_HA = 252,
  R_POWERPC_GNU_VTINHERIT = 253,
  R_POWERPC_GNU_VTENTRY = 254,
  R_PPC_TOC16 = 255,
};

// e_flags values defined for powerpc
enum
{
  EF_PPC_EMB = 0x80000000,             // PowerPC embedded flag.
  EF_PPC_RELOCATABLE = 0x00010000,     // PowerPC -mrelocatable flag.  */
  EF_PPC_RELOCATABLE_LIB = 0x00008000, // PowerPC -mrelocatable-lib flag.  */
};

} // End namespace elfcpp.

#endif // !defined(ELFCPP_POWERPC_H)
