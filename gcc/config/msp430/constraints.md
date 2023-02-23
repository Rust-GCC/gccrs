;;  Machine Description for TI MSP43* processors
;;  Copyright (C) 2013-2023 Free Software Foundation, Inc.
;;  Contributed by Red Hat.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_register_constraint "R12" "R12_REGS"
  "Register R12.")

(define_register_constraint "R13" "R13_REGS"
  "Register R13.")

(define_constraint "K"
  "Integer constant 1-19."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 1, 19)")))

(define_constraint "L"
  "Integer constant -1^20..1^19."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, HOST_WIDE_INT_M1U << 20, 1 << 19)")))

;; Valid shift amount for RRUM, RRAM, RLAM, RRCM.
(define_constraint "M"
  "Integer constant 1-4."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 1, 4)")))

(define_constraint "N"
  "Integer constant 0-255."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 255)")))

(define_constraint "O"
  "Integer constant 256-65535."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 256, 65535)")))

(define_constraint "P"
  "Integer constant 1-16."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 1, 16)")))

;; We do not allow arbitrary constants, eg symbols or labels,
;; because their address may be above the 16-bit address limit
;; supported by the offset used in the MOVA instruction.
(define_constraint "Ya"
  "Memory reference, any type, but restricted range of constants"
  (and (match_code "mem")
       (ior (match_code "reg" "0")
	    (and (match_code "plus" "0")
		 (match_code "reg" "00")
		 (match_test ("CONST_INT_P (XEXP (XEXP (op, 0), 1))")))
	    (match_test "CONSTANT_P (XEXP (op, 0))")
	    (match_code "post_inc" "0")
	    )))

(define_constraint "Yl"
  "Memory reference, labels only."
  (and (match_code "mem")
       (match_code "label_ref" "0")))


;; These are memory references that are safe to use without the X suffix,
;; because we know/assume they need not index across the 64K boundary.
;; Note that for a PSImode memory operand, we always need to use the X suffix,
;; regardless of what this constraint decides.
(define_constraint "Ys"
  "Memory reference, indexed or indirect register addressing modes."
  (and (match_code "mem")
       (ior
	(and (match_code "plus" "0")
	     (and (match_code "reg" "00")
		  (match_test ("CONST_INT_P (XEXP (XEXP (op, 0), 1))"))
		  (match_test ("IN_RANGE (INTVAL (XEXP (XEXP (op, 0), 1)), HOST_WIDE_INT_M1U << 15, (1 << 15)-1)"))))
	(match_code "reg" "0")
	(match_code "post_inc" "0")
	)))

(define_constraint "Yc"
  "Memory reference, for CALL - we can't use SP."
  (and (match_code "mem")
       (match_code "mem" "0")
       (not (ior
	     (and (match_code "plus" "00")
		  (and (match_code "reg" "000")
		       (match_test ("REGNO (XEXP (XEXP (op, 0), 0)) != SP_REGNO"))))
	     (and (match_code "reg" "0")
		  (match_test ("REGNO (XEXP (XEXP (op, 0), 0)) != SP_REGNO")))
	     ))))

(define_constraint "Yx"
  "Memory reference, in lower memory below address 0x10000."
  (and (match_code "mem")
       (match_test "msp430_op_not_in_high_mem (op)")))
