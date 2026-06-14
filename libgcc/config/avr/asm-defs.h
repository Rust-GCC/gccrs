/*  -*- Mode: Asm -*-  */
/* Copyright (C) 1998-2026 Free Software Foundation, Inc.
   Contributed by Denis Chertykov <chertykov@gmail.com>

   This file is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Definitions for common SFRs and fixed registers.

#if defined (__AVR_TINY__)
#define __zero_reg__ r17
#define __tmp_reg__ r16
#else
#define __zero_reg__ r1
#define __tmp_reg__ r0
#endif

#if defined (__AVR_HAVE_SPH__)
#define __SP_H__ 0x3e
#endif

#define __SP_L__  0x3d
#define __RAMPZ__ 0x3B
#define __EIND__  0x3C
#define __SREG__  0x3f


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Helper macros.

;;; REGNO sets symbol SYM to the register number of REG.
;;; Valid REGs are: r0..r31, R0..R31 and XL, yh, ZH etc.
;;; Expressions that evaluate to a number can also be used.
.macro  REGNO sym, reg
    ..regno.done = 0
    ..regno = 0
    .irp  name,                                                 \
             r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7,  r8,  r9,   \
            r10, r11, r12, r13, r14, r15, r16, r17, r18, r19,   \
            r20, r21, r22, r23, r24, r25, r26, r27, r28, r29,   \
            r30, r31,                                           \
             R0,  R1,  R2,  R3,  R4,  R5,  R6,  R7,  R8,  R9,   \
            R10, R11, R12, R13, R14, R15, R16, R17, R18, R19,   \
            R20, R21, R22, R23, R24, R25, R26, R27, R28, R29,   \
            R30, R31
        .ifc  \name,\reg
            \sym = ..regno % 32
            ..regno.done = 1
        .endif
        ..regno = ..regno + 1
    .endr

    ..regno = 0
    .irp  name,                       \
            xl, xh, yl, yh, zl, zh,   \
            Xl, Xh, Yl, Yh, Zl, Zh,   \
            xL, xH, yL, yH, zL, zH,   \
            XL, XH, YL, YH, ZL, ZH,   \
            x,  xh, y,  yh, z,  zh,   \
            X,  XH, Y,  YH, Z,  ZH
        .ifc  \name,\reg
            \sym = 26 + ..regno % 6
            ..regno.done = 1
        .endif
        ..regno = ..regno + 1
    .endr

    .if ! ..regno.done
        ;; Assume that \reg is some valid numeric expression.
        \sym = \reg
    .endif
.endm


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Definitions to factor out different ISAs.

#if defined (__AVR_HAVE_JMP_CALL__)
#define XCALL call
#define XJMP  jmp
#else
#define XCALL rcall
#define XJMP  rjmp
#endif

#if defined (__AVR_HAVE_EIJMP_EICALL__)
#define XICALL eicall
#define XIJMP  eijmp
#define PC_SIZE 3
#else
#define XICALL icall
#define XIJMP  ijmp
#define PC_SIZE 2
#endif


;;; Like MOVW
#ifdef __AVR_HAVE_MOVW__
#define wmov movw
#else
.macro  wmov  dst, src
    REGNO ..wmov.dst, \dst
    REGNO ..wmov.src, \src
    mov   ..wmov.dst+0, ..wmov.src+0
    mov   ..wmov.dst+1, ..wmov.src+1
.endm
#endif /* __AVR_HAVE_MOVW__ */


;;; Like SBIW, ADIW
#ifdef __AVR_HAVE_ADIW__
#define waddi adiw
#define wsubi sbiw
#else
.macro  wsubi  reg, arg
    REGNO ..wsubi.reg, \reg
    subi  ..wsubi.reg+0, lo8 (\arg)
    sbci  ..wsubi.reg+1, hi8 (\arg)
.endm

.macro  waddi  reg, arg
    REGNO ..waddi.reg, \reg
    subi  ..waddi.reg+0, lo8 (-(\arg))
    sbci  ..waddi.reg+1, hi8 (-(\arg))
.endm
#endif /* __AVR_HAVE_ADIW__ */


;;; [R]JMP to label LABL when REG is positive (REG.7 = 0).
;;; Otherwise, fallthrough.
.macro  .branch_plus  reg, labl
#ifdef __AVR_ERRATA_SKIP_JMP_CALL__
    ;; Some cores have a problem skipping 2-word instructions.
    tst     \reg
    brmi    .L.branch_plus.\@
#else
    sbrs    \reg, 7
#endif /* skip erratum */
    XJMP    \labl
.L.branch_plus.\@:
.endm ; .branch_plus


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Macros for convenience.

;; Skip the next instruction, typically a jump target.
#define skip cpse 16,16


.macro  mov4  dst, src
    REGNO ..mov4.dst, \dst
    REGNO ..mov4.src, \src
    .if ..mov4.dst < ..mov4.src
        wmov    ..mov4.dst+0, ..mov4.src+0
        wmov    ..mov4.dst+2, ..mov4.src+2
    .else
        wmov    ..mov4.dst+2, ..mov4.src+2
        wmov    ..mov4.dst+0, ..mov4.src+0
    .endif
.endm


;; Negate a 2-byte value held in consecutive registers.
.macro  NEG2  reg
    com     \reg+1
    neg     \reg
    sbci    \reg+1, -1
.endm


;; Negate a 4-byte value held in consecutive registers.
;; Sets the V flag for signed overflow tests if REG >= 16.
.macro  NEG4  reg
    com     \reg+3
    com     \reg+2
    com     \reg+1
.if \reg >= 16
    neg     \reg
    sbci    \reg+1, -1
    sbci    \reg+2, -1
    sbci    \reg+3, -1
.else
    com     \reg
    adc     \reg,   __zero_reg__
    adc     \reg+1, __zero_reg__
    adc     \reg+2, __zero_reg__
    adc     \reg+3, __zero_reg__
.endif
.endm


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Function entry and exit.

.macro DEFUN name
    .global \name
    .func \name
    \name:
.endm

.macro ENDF name
    .size \name, .-\name
    .endfunc
.endm

.macro FALIAS name
    .global \name
    .func \name
    \name:
    .size \name, .-\name
    .endfunc
.endm


#ifndef __AVR_TINY__

;;; Convenience macro for easy use of __prologue_saves__.
;;; Push the N_PUSHED callee-saved registers  Y, R17, R16, R15, ...
;;; with 0 <= N_PUSHED <= 18.  The frame pointer (Y) is set up according
;;; to a frame size of N_FRAME.  Clobbers TMP_REG.  Does not clobber T.
.macro do_prologue_saves n_pushed n_frame=0
    ldi     r26, lo8 (\n_frame)
    ldi     r27, hi8 (\n_frame)
    ldi     r30, lo8 (gs(.L_prologue_saves.\@))
    ldi     r31, hi8 (gs(.L_prologue_saves.\@))
    XJMP __prologue_saves__ + ((18 - (\n_pushed)) * 2)
.L_prologue_saves.\@:
.endm

;;; Convenience macro for easy use of __epilogue_restores__.
;;; Undo the effect of __prologue_saves__.  Clobbers TMP_REG.
.macro do_epilogue_restores n_pushed n_frame=0
    in      r28, __SP_L__
#ifdef __AVR_HAVE_SPH__
    in      r29, __SP_H__
.if \n_frame > 63
    subi    r28, lo8 (-(\n_frame))
    sbci    r29, hi8 (-(\n_frame))
.elseif \n_frame > 0
    adiw    r28, \n_frame
.endif
#else
    clr     r29
.if \n_frame > 0
    subi    r28, lo8 (-(\n_frame))
.endif
#endif /* HAVE SPH */
    ldi     r30, \n_pushed
    XJMP __epilogue_restores__ + ((18 - (\n_pushed)) * 2)
.endm

#endif /* AVR_TINY */
