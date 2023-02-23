;; Constraint definitions for Renesas / SuperH SH.
;; Copyright (C) 2007-2023 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Overview of uppercase letter constraints:
;; Axx: atomic memory operand constraints
;;  Ara: Same as Sra but disallows r15
;;  Add: Same as Sdd but disallows r15
;; Bxx: miscellaneous constraints
;;  Bsc: SCRATCH - for the scratch register in movsi_ie in the
;;       fldi0 / fldi0 cases
;; Cxx: Constants other than only CONST_INT
;;  Ccl: call site label
;;  Csy: label or symbol
;;  Cpg: non-explicit constants that can be directly loaded into a general
;;       purpose register in PIC code.  Like 's' except we don't allow
;;       PIC_ADDR_P
;; IJKLMNOP: CONT_INT constants
;;  Ixx: signed xx bit
;;  Jmb: 0x000000FF
;;  Jmw: 0x0000FFFF
;;  Jhb: 0x80000000
;;  Kxx: unsigned xx bit
;;  M: 1
;;  N: 0
;;  P27: 1 | 2 | 8 | 16
;;  Pso: 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128
;;  Psz: ~1 | ~2 | ~4 | ~8 | ~16 | ~32 | ~64 | ~128
;; G: Floating point 0
;; H: Floating point 1
;; Q: pc relative load operand
;; Rxx: reserved for exotic register classes.
;; Sxx: extra memory constraints
;;  Sua: unaligned memory address
;;  Sbv: QImode address without displacement
;;  Sbw: QImode address with 12 bit displacement
;;  Snd: address without displacement
;;  Sdd: address with displacement
;;  Sra: simple register address
;; W: vector
;; Z: zero in any mode
;;
;; unused CONST_INT constraint letters: LO
;; unused "extra" constraint letters: D T U Y

;; Register constraints
(define_register_constraint "a" "ALL_REGS"
  "@internal")

(define_register_constraint "c" "FPSCR_REGS"
  "Floating-point status register.")

(define_register_constraint "d" "DF_REGS"
  "Double precision floating-point register.")

(define_register_constraint "e" "TARGET_FMOVD ? NO_REGS : FP_REGS"
  "Floating-point register.")

(define_register_constraint "f" "FP_REGS"
  "Floating-point register.")

(define_register_constraint "k" "SIBCALL_REGS"
  "@internal")

(define_register_constraint "l" "PR_REGS"
  "PR register.")

(define_register_constraint "t" "T_REGS"
  "T register.")

(define_register_constraint "u" "NON_SP_REGS"
  "Non-stack-pointer register.")

(define_register_constraint "w" "FP0_REGS"
  "Floating-point register 0.")

(define_register_constraint "x" "MAC_REGS"
  "MACH and MACL registers.")

(define_register_constraint "y" "FPUL_REGS"
  "FPUL register.")

(define_register_constraint "z" "R0_REGS"
  "R0 register.")

;; Integer constraints
(define_constraint "I08"
  "A signed 8-bit constant, as used in add, sub, etc."
  (and (match_code "const_int")
       (match_test "ival >= -128 && ival <= 127")))

(define_constraint "I16"
  "A signed 16-bit constant."
  (and (match_code "const_int")
       (match_test "ival >= -32768 && ival <= 32767")))

(define_constraint "I20"
  "A signed 20-bit constant, as used in SH2A movi20."
  (and (match_code "const_int")
       (match_test "ival >= -524288 && ival <= 524287")
       (match_test "TARGET_SH2A")))

(define_constraint "I28"
  "A signed 28-bit constant, as used in SH2A movi20s."
  (and (match_code "const_int")
       (match_test "ival >=  -134217728 && ival <= 134217727")
       (match_test "(ival & 255) == 0")
       (match_test "TARGET_SH2A")))

(define_constraint "Jmb"
  "Low byte mask constant 0x000000FF"
  (and (match_code "const_int")
       (match_test "ival == 0xFF")))

(define_constraint "Jmw"
  "Low word mask constant 0x0000FFFF"
  (and (match_code "const_int")
       (match_test "ival == 0xFFFF")))

(define_constraint "Jhb"
  "Highest bit constant"
  (and (match_code "const_int")
       (match_test "(ival & 0xFFFFFFFF) == 0x80000000")))

(define_constraint "K03"
  "An unsigned 3-bit constant, as used in SH2A bclr, bset, etc."
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 7")))

(define_constraint "K04"
  "An unsigned 4-bit constant, as used in mov.b displacement addressing."
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 15")))

(define_constraint "K05"
  "An unsigned 5-bit constant, as used in mov.w displacement addressing."
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 31")))

(define_constraint "K08"
  "An unsigned 8-bit constant, as used in and, or, etc."
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 255")))
 
(define_constraint "K12"
  "An unsigned 12-bit constant, as used in SH2A 12-bit mov.b displacement
   addressing."
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 4095")))

(define_constraint "K13"
  "An unsigned 13-bit constant, as used in SH2A 12-bit mov.w displacement
   addressing."
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 8191")))

(define_constraint "P27"
  "A constant for shift operand 1,2,8 or 16."
  (and (match_code "const_int")
       (match_test "ival == 1 || ival == 2 || ival == 8 || ival == 16")))

(define_constraint "M"
  "Integer constant 1."
  (and (match_code "const_int")
       (match_test "ival == 1")))

(define_constraint "N"
  "Integer constant 0."
  (and (match_code "const_int")
       (match_test "ival == 0")))

;; Floating-point constraints
(define_constraint "G"
  "Double constant 0."
  (and (match_code "const_double")
       (match_test "fp_zero_operand (op)")))

(define_constraint "H"
  "Double constant 1."
  (and (match_code "const_double")
       (match_test "fp_one_operand (op)")))

;; Extra constraints
(define_constraint "Q"
  "A pc relative load operand."
  (and (match_code "mem")
       (match_test "GET_MODE (op) != QImode")
       (match_test "IS_PC_RELATIVE_LOAD_ADDR_P (XEXP (op, 0))")))

(define_constraint "Bsc"
  "Constraint for selecting FLDI0 or FLDI1 instruction.  If the clobber
   operand is not SCRATCH (i.e. REG) then R0 is probably being used,
   hence mova is being used, hence do not select this pattern."
  (match_code "scratch"))

(define_constraint "Ccl"
  "A call site label, for bsrf."
  (and (match_code "unspec")
       (match_test "XINT (op, 1) == UNSPEC_CALLER")))

(define_constraint "Csy"
  "A label or a symbol."
  (ior (match_test "NON_PIC_REFERENCE_P (op)")
       (match_test "PIC_ADDR_P (op)")))

(define_constraint "Z"
  "A zero in any shape or form."
  (match_test "op == CONST0_RTX (GET_MODE (op))"))

(define_constraint "W"
  "Any vector constant we can handle."
  (and (match_code "const_vector")
       (ior (match_test "sh_rep_vec (op, VOIDmode)")
	    (match_test "HOST_BITS_PER_WIDE_INT >= 64
			 ? sh_const_vec (op, VOIDmode)
			 : sh_1el_vec (op, VOIDmode)"))))

(define_constraint "Cpg"
  "A non-explicit constant that can be loaded directly into a general
   purpose register.  This is like 's' except we don't allow
   PIC_ADDR_P."
  (match_test "IS_NON_EXPLICIT_CONSTANT_P (op)"))

(define_constraint "Pso"
  "Integer constant with a single bit set in its lower 8-bit."
  (and (match_code "const_int")
       (ior (match_test "ival == 1")
	    (match_test "ival == 2")
	    (match_test "ival == 4")
	    (match_test "ival == 8")
	    (match_test "ival == 16")
	    (match_test "ival == 32")
	    (match_test "ival == 64")
	    (match_test "ival == 128"))))

(define_constraint "Psz"
  "Integer constant with a single zero bit in the lower 8-bit."
  (and (match_code "const_int")
       (ior (match_test "~ival == 1")
	    (match_test "~ival == 2")
	    (match_test "~ival == 4")
	    (match_test "~ival == 8")
	    (match_test "~ival == 16")
	    (match_test "~ival == 32")
	    (match_test "~ival == 64")
	    (match_test "~ival == 128"))))

(define_memory_constraint "Sua"
  "@internal"
  (and (match_test "memory_operand (op, GET_MODE (op))")
       (match_test "GET_CODE (XEXP (op, 0)) != PLUS")))

(define_memory_constraint "Sdd"
  "A memory reference that uses displacement addressing."
  (and (match_code "mem")
       (match_code "plus" "0")
       (match_code "reg" "00")
       (match_code "const_int" "01")))

(define_memory_constraint "Snd"
  "A memory reference that excludes displacement addressing."
  (and (match_code "mem")
       (match_test "! satisfies_constraint_Sdd (op)")))

(define_memory_constraint "Sid"
  "A memory reference that uses index addressing."
  (and (match_code "mem")
       (match_code "plus" "0")
       (match_code "reg" "00")
       (match_code "reg" "01")))

(define_memory_constraint "Ssd"
  "A memory reference that excludes index and displacement addressing."
  (and (match_code "mem")
       (match_test "! satisfies_constraint_Sid (op)")
       (match_test "! satisfies_constraint_Sdd (op)")))

(define_memory_constraint "Sbv"
  "A memory reference, as used in SH2A bclr.b, bset.b, etc."
  (and (match_test "MEM_P (op) && GET_MODE (op) == QImode")
       (match_test "REG_P (XEXP (op, 0))")))

(define_memory_constraint "Sbw"
  "A memory reference, as used in SH2A bclr.b, bset.b, etc."
  (and (match_test "satisfies_constraint_Sdd (op)")
       (match_test "GET_MODE (op) == QImode")
       (match_test "satisfies_constraint_K12 (XEXP (XEXP (op, 0), 1))")))

(define_memory_constraint "Sra"
  "A memory reference that uses simple register addressing."
  (and (match_code "mem")
       (match_code "reg" "0")))

(define_memory_constraint "Ara"
  "A memory reference that uses simple register addressing suitable for
   gusa atomic operations."
  (and (match_code "mem")
       (match_code "reg" "0")
       (match_test "REGNO (XEXP (op, 0)) != SP_REG")))

(define_memory_constraint "Add"
  "A memory reference that uses displacement addressing suitable for
   gusa atomic operations."
  (and (match_code "mem")
       (match_test "GET_MODE (op) == SImode")
       (match_code "plus" "0")
       (match_code "reg" "00")
       (match_code "const_int" "01")
       (match_test "REGNO (XEXP (XEXP (op, 0), 0)) != SP_REG")))
