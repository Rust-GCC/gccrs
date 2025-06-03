
; real
; GAS defines r0..r7 as aliases for real registers; we want the saddr
; forms here.
r_0	=	0xffef8
r_1	=	0xffef9
r_2	=	0xffefa
r_3	=	0xffefb
r_4	=	0xffefc
r_5	=	0xffefd
r_6	=	0xffefe
r_7	=	0xffeff

#ifdef __RL78_G10__

; clobberable
r8	=	0xffec8
r9	=	0xffec9
r10	=	0xffeca
r11	=	0xffecb
r12	=	0xffecc
r13	=	0xffecd
r14	=	0xffece
r15	=	0xffecf
; preserved
r16	=	0xffed0
r17	=	0xffed1
r18	=	0xffed2
r19	=	0xffed3
r20	=	0xffed4
r21	=	0xffed5
r22	=	0xffed6
r23	=	0xffed7

#else

; clobberable
r8	=	0xffef0
r9	=	0xffef1
r10	=	0xffef2
r11	=	0xffef3
r12	=	0xffef4
r13	=	0xffef5
r14	=	0xffef6
r15	=	0xffef7
; preserved
r16	=	0xffee8
r17	=	0xffee9
r18	=	0xffeea
r19	=	0xffeeb
r20	=	0xffeec
r21	=	0xffeed
r22	=	0xffeee
r23	=	0xffeef

#endif

.macro START_ANOTHER_FUNC name
	.global \name
	.type \name , @function
\name:
.endm

    /* Start a function in its own section, so that it
       can be subject to linker garbage collection.  */
.macro START_FUNC name
	.pushsection .text.\name,"ax",@progbits
	START_ANOTHER_FUNC \name
.endm

.macro END_ANOTHER_FUNC name
	.size \name , . - \name
.endm

    /* End the function.  Set the size.  */
.macro END_FUNC name
	END_ANOTHER_FUNC \name
	.popsection
.endm
