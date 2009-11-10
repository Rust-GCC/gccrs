// arm.h -- ELF definitions specific to EM_ARM  -*- C++ -*-

// Copyright 2009, Free Software Foundation, Inc.
// Written by Doug Kwan <dougkwan@google.com>.

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
// distribution when not linked into a combined executable.)

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.

// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
// 02110-1301, USA.

#ifndef ELFCPP_ARM_H
#define ELFCPP_ARM_H

// The relocation type information is taken from:
//
//   ELF for the ARM Architecture
//   Document number: ARM IHI 0044C, current through ABI release 2.07
//   Date of Issue: 10th October, 2008
//

namespace elfcpp
{

//
// ARM Relocations Codes
//

// Operation notes: 
//   S: Address of the symbol.
//   A: Addend for relocation.
//   P: Address of the place being relocated.
//   Pa: Adjusted address of the place being relocated (P & 0xfffffffc)
//   T: is 1 if S has type STT_FUNC and the symbol addresses a Thumb
//      instruction.Thumb-bit; it is 0 otherwise.
//   B(S): Addressing origin of the output segment defining S.
//   GOT_ORG: Addressing origin of the Global Offset Table.
//   GOT(S): Address of the GOT entry for S.
//

enum
{
				// Type		Class	Operation		
				// ------------------------------
  R_ARM_NONE = 0,		// Static	Misc
  R_ARM_PC24 = 1,		// Deprecated	ARM	((S + A) | T) - P
  R_ARM_ABS32 = 2,		// Static	Data	(S + A) | T
  R_ARM_REL32 = 3,		// Static	Data	((S + A) | T) - P
  R_ARM_LDR_PC_G0 = 4,		// Static	ARM	S + A - P
  R_ARM_ABS16 = 5,		// Static	Data	S + A
  R_ARM_ABS12 = 6,		// Static	ARM	S + A
  R_ARM_THM_ABS5 = 7,		// Static	Thumb16	S + A
  R_ARM_ABS8 = 8,		// Static	Data	S + A
  R_ARM_SBREL32 = 9,		// Static	Data	((S + A) | T) - B(S)
  R_ARM_THM_CALL = 10,		// Static	Thumb32	((S + A) | T) - P
  R_ARM_THM_PC8 = 11,		// Static	Thumb16
  R_ARM_BREL_ADJ = 12,		// Dynamic	Data	DeltaB(S) + A
  R_ARM_TLS_DESC = 13,		// Dynamic	Data
  R_ARM_THM_SWI8 = 14,		// Obsolete
  R_ARM_XPC25 = 15,		// Obsolete
  R_ARM_THM_XPC22 = 16,		// Obsolete
  R_ARM_TLS_DTPMOD32 = 17,	// Dynamic	Data	Module(S)
  R_ARM_TLS_DTPOFF32 = 18,	// Dynamic	Data	S + A - TLS
  R_ARM_TLS_TPOFF32 = 19,	// Dynamic	Data	S + A - tp
  R_ARM_COPY = 20,		// Dynamic	Misc
  R_ARM_GLOB_DAT = 21,		// Dynamic	Data	(S + A) | T
  R_ARM_JUMP_SLOT = 22,		// Dynamic	Data	(S + A) | T
  R_ARM_RELATIVE = 23,		// Dynamic	Data	B(S) + A
  R_ARM_GOTOFF32 = 24,		// Static	Data	(((S + A) | T) - GOT_ORG
  R_ARM_BASE_PREL = 25,		// Static	Data	B(S) + A - P
  R_ARM_GOT_BREL = 26,		// Static	Data	GOT(S) + A - GOT_ORG
  R_ARM_PLT32 = 27,		// Deprecated	ARM	((S + A) | T) - P
  R_ARM_CALL = 28,		// Static	ARM	((S + A) | T) - P
  R_ARM_JUMP24 = 29,		// Static	ARM	((S + A) | T) - P
  R_ARM_THM_JUMP24 = 30,	// Static	Thumb32	((S + A) | T) - P
  R_ARM_BASE_ABS = 31,		// Static	Data	B(S) + A
  R_ARM_ALU_PCREL_7_0 = 32,	// Obsolete
  R_ARM_ALU_PCREL_15_8 = 33,	// Obsolete
  R_ARM_ALU_PCREL_23_15 = 34,	// Obsolete
  R_ARM_LDR_SBREL_11_0_NC = 35,	// Deprecated	ARM	S + A - B(S)
  R_ARM_ALU_SBREL_19_12_NC = 36,// Deprecated	ARM	S + A - B(S)
  R_ARM_ALU_SBREL_27_20_CK = 37,// Deprecated	ARM	S + A - B(S)
  R_ARM_TARGET1 = 38,		// Data		Misc	(S + A) | T or
				//			((S + A) | T) - P
  R_ARM_SBREL31 = 39,		// Deprecated	Data	((S + A) | T) - B(S)
  R_ARM_V4BX = 40,		// Static	Misc 
  R_ARM_TARGET2 = 41,		// Static	Misc
  R_ARM_PREL31 = 42,		// Static	Data	((S + A) | T) - P
  R_ARM_MOVW_ABS_NC = 43,	// Static	ARM	(S + A) | T
  R_ARM_MOVT_ABS = 44,		// Static	ARM	S + A
  R_ARM_MOVW_PREL_NC = 45,	// Static	ARM	((S + A) | T) - P
  R_ARM_MOVT_PREL = 46,		// Static	ARM	S + A - P
  R_ARM_THM_MOVW_ABS_NC = 47,	// Static	Thumb32	(S + A) | T
  R_ARM_THM_MOVT_ABS = 48,	// Static	Thumb32	S + A - P
  R_ARM_THM_MOVW_PREL_NC =  49,	// Static	Thumb32	((S + A) | T) - P
  R_ARM_THM_MOVT_PREL = 50,	// Static	Thumb32	S + A - P
  R_ARM_THM_JUMP19 = 51,	// Static	Thumb32	((S + A) | T) - P
  R_ARM_THM_JUMP6 = 52,		// Static	Thumb16	S + A - P
  R_ARM_THM_ALU_PREL_11_0 = 53,	// Static	Thumb32	((S + A) | T) - Pa
  R_ARM_THM_PC12 = 54,		// Static	Thumb32	S + A - Pa
  R_ARM_ABS32_NOI = 55,		// Static	Data	S + A
  R_ARM_REL32_NOI = 56,		// Static	Data	S + A - P
  R_ARM_ALU_PC_G0_NC = 57,	// Static	ARM	((S + A) | T) - P
  R_ARM_ALU_PC_G0 = 58,		// Static	ARM	((S + A) | T) - P
  R_ARM_ALU_PC_G1_NC = 59,	// Static	ARM	((S + A) | T) - P
  R_ARM_ALU_PC_G1 = 60,		// Static	ARM	((S + A) | T) - P
  R_ARM_ALU_PC_G2 = 61,		// Static	ARM	((S + A) | T) - P
  R_ARM_LDR_PC_G1 = 62,		// Static	ARM	S + A - P
  R_ARM_LDR_PC_G2 = 63,		// Static	ARM	S + A - P
  R_ARM_LDRS_PC_G0 = 64,	// Static	ARM	S + A - P
  R_ARM_LDRS_PC_G1 = 65,	// Static	ARM	S + A - P
  R_ARM_LDRS_PC_G2 = 66,	// Static	ARM	S + A - P
  R_ARM_LDC_PC_G0 = 67,		// Static	ARM	S + A - P
  R_ARM_LDC_PC_G1 = 68,		// Static	ARM	S + A - P
  R_ARM_LDC_PC_G2 = 69,		// Static	ARM	S + A - P
  R_ARM_ALU_SB_G0_NC = 70,	// Static	ARM	((S + A) | T) - B(S)
  R_ARM_ALU_SB_G0 = 71,		// Static	ARM	((S + A) | T) - B(S)
  R_ARM_ALU_SB_G1_NC = 72,	// Static	ARM	((S + A) | T) - B(S)
  R_ARM_ALU_SB_G1 = 73,		// Static	ARM	((S + A) | T) - B(S)
  R_ARM_ALU_SB_G2 = 74,		// Static	ARM	((S + A) | T) - B(S)
  R_ARM_LDR_SB_G0 = 75,		// Static	ARM	S + A - B(S)
  R_ARM_LDR_SB_G1 = 76,		// Static	ARM	S + A - B(S)
  R_ARM_LDR_SB_G2 = 77,		// Static	ARM	S + A - B(S)
  R_ARM_LDRS_SB_G0 = 78,	// Static	ARM	S + A - B(S)
  R_ARM_LDRS_SB_G1 = 79,	// Static	ARM	S + A - B(S)
  R_ARM_LDRS_SB_G2 = 80,	// Static	ARM	S + A - B(S)
  R_ARM_LDC_SB_G0 = 81,		// Static	ARM	S + A - B(S)
  R_ARM_LDC_SB_G1 = 82,		// Static	ARM	S + A - B(S)
  R_ARM_LDC_SB_G2 = 83,		// Static	ARM	S + A - B(S)
  R_ARM_MOVW_BREL_NC = 84,	// Static	ARM	((S + A) | T) - B(S)
  R_ARM_MOVT_BREL = 85,		// Static	ARM	S + A - B(S)
  R_ARM_MOVW_BREL = 86,		// Static	ARM	((S + A) | T) - B(S)
  R_ARM_THM_MOVW_BREL_NC = 87,	// Static	Thumb32	((S + A) | T) - B(S)
  R_ARM_THM_MOVT_BREL = 88,	// Static	Thumb32	S + A - B(S)
  R_ARM_THM_MOVW_BREL = 89,	// Static	Thumb32	((S + A) | T) - B(S)
  R_ARM_TLS_GOTDESC = 90,	// Static	Data
  R_ARM_TLS_CALL = 91,		// Static	ARM
  R_ARM_TLS_DESCSEQ = 92,	// Static	ARM	TLS relaxation
  R_ARM_THM_TLS_CALL = 93,	// Static	Thumb32
  R_ARM_PLT32_ABS =  94,	// Static	Data	PLT(S) + A
  R_ARM_GOT_ABS =  95,		// Static	Data	GOT(S) + A
  R_ARM_GOT_PREL = 96,		// Static	Data	GOT(S) + A - P
  R_ARM_GOT_BREL12 =  97,	// Static	ARM	GOT(S) + A - GOT_ORG
  R_ARM_GOTOFF12 =  98,		// Static	ARM	S + A - GOT_ROG
  R_ARM_GOTRELAX =  99,		// Static	Misc
  R_ARM_GNU_VTENTRY = 100,	// Deprecated	Data
  R_ARM_GNU_VTINHERIT = 101,	// Deprecated	Data
  R_ARM_THM_JUMP11 = 102,	// Static	Thumb16	S + A - P
  R_ARM_THM_JUMP8 = 103,	// Static	Thumb16	S + A - P
  R_ARM_TLS_GD32 = 104,		// Static	Data	GOT(S) + A - P
  R_ARM_TLS_LDM32 = 105,	// Static	Data	GOT(S) + A - P
  R_ARM_TLS_LDO32 = 106,	// Static	Data	S + A - TLS
  R_ARM_TLS_IE32 = 107,		// Static	Data	GOT(S) + A - P
  R_ARM_TLS_LE32 = 108,		// Static	Data	S + A - tp
  R_ARM_TLS_LDO12 = 109,	// Static	ARM	S + A - TLS
  R_ARM_TLS_LE12 = 110,		// Static	ARM	S + A - tp
  R_ARM_TLS_IE12GP = 111,	// Static	ARM	GOT(S) + A - GOT_ORG
  R_ARM_PRIVATE_0 = 112,	// Private (n = 0, 1, ... 15)
  R_ARM_PRIVATE_1 = 113,
  R_ARM_PRIVATE_2 = 114,
  R_ARM_PRIVATE_3 = 115,
  R_ARM_PRIVATE_4 = 116,
  R_ARM_PRIVATE_5 = 117,
  R_ARM_PRIVATE_6 = 118,
  R_ARM_PRIVATE_7 = 119,
  R_ARM_PRIVATE_8 = 120,
  R_ARM_PRIVATE_9 = 121,
  R_ARM_PRIVATE_10 = 122,
  R_ARM_PRIVATE_11 = 123,
  R_ARM_PRIVATE_12 = 124,
  R_ARM_PRIVATE_13 = 125,
  R_ARM_PRIVATE_14 = 126,
  R_ARM_PRIVATE_15 = 127,
  R_ARM_ME_TOO = 128,		// Obsolete
  R_ARM_THM_TLS_DESCSEQ16 = 129,// Static	Thumb16	
  R_ARM_THM_TLS_DESCSEQ32 = 130,// Static	Thumb32
  // 131 - 139			Unallocated
  // 140 - 159			Dynamic		Reserved for future allocation
  // 160 - 255			Unallocated
};

// e_flags values used for ARM.  We only support flags defined in AAELF.

enum
{
  EF_ARM_BE8 = 0x00800000,

  // Mask to extract EABI version, not really a flag value.
  EF_ARM_EABIMASK = 0xFF000000,

  EF_ARM_EABI_UNKNOWN = 0x00000000,
  EF_ARM_EABI_VER1 = 0x01000000,
  EF_ARM_EABI_VER2 = 0x02000000,
  EF_ARM_EABI_VER3 = 0x03000000,
  EF_ARM_EABI_VER4 = 0x04000000,
  EF_ARM_EABI_VER5 = 0x05000000,
};

// Extract EABI version from flags.

inline Elf_Word
arm_eabi_version(Elf_Word flags)
{ return flags & EF_ARM_EABIMASK; }

} // End namespace elfcpp.

#endif // !defined(ELFCPP_ARM_H)
