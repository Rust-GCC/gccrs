/* { dg-do compile { target { *-*-linux* && lp64 } } } */
/* { dg-options "-O2 -fno-pic -mtune=generic -mgeneral-regs-only -mapxf -mtune-ctrl=prologue_using_move,epilogue_using_move" } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target "*-*-*" } {^\t?\.}  } } */

/* start must save and restore all caller saved registers.  */

/*
**start:
**.LFB[0-9]+:
**	.cfi_startproc
**	subq	\$248, %rsp
**...
**	movq	%rax, 8\(%rsp\)
**	movq	%rdx, 16\(%rsp\)
**	movq	%rcx, 24\(%rsp\)
**	movq	%rbx, 32\(%rsp\)
**	movq	%rsi, 40\(%rsp\)
**	movq	%rdi, 48\(%rsp\)
**	movq	%r8, 56\(%rsp\)
**	movq	%r9, 64\(%rsp\)
**	movq	%r10, 72\(%rsp\)
**	movq	%r11, 80\(%rsp\)
**	movq	%r12, 88\(%rsp\)
**	movq	%r13, 96\(%rsp\)
**	movq	%r14, 104\(%rsp\)
**	movq	%r15, 112\(%rsp\)
**	movq	%r16, 120\(%rsp\)
**	movq	%r17, 128\(%rsp\)
**	movq	%r18, 136\(%rsp\)
**	movq	%r19, 144\(%rsp\)
**	movq	%r20, 152\(%rsp\)
**	movq	%r21, 160\(%rsp\)
**	movq	%r22, 168\(%rsp\)
**	movq	%r23, 176\(%rsp\)
**	movq	%r24, 184\(%rsp\)
**	movq	%r25, 192\(%rsp\)
**	movq	%r26, 200\(%rsp\)
**	movq	%r27, 208\(%rsp\)
**	movq	%r28, 216\(%rsp\)
**	movq	%r29, 224\(%rsp\)
**	movq	%r30, 232\(%rsp\)
**	movq	%r31, 240\(%rsp\)
**...
**	call	\*code\(%rip\)
**	movq	8\(%rsp\), %rax
**	movq	16\(%rsp\), %rdx
**	movq	24\(%rsp\), %rcx
**	movq	32\(%rsp\), %rbx
**	movq	40\(%rsp\), %rsi
**	movq	48\(%rsp\), %rdi
**	movq	56\(%rsp\), %r8
**	movq	64\(%rsp\), %r9
**	movq	72\(%rsp\), %r10
**	movq	80\(%rsp\), %r11
**	movq	88\(%rsp\), %r12
**	movq	96\(%rsp\), %r13
**	movq	104\(%rsp\), %r14
**	movq	112\(%rsp\), %r15
**	movq	120\(%rsp\), %r16
**	movq	128\(%rsp\), %r17
**	movq	136\(%rsp\), %r18
**	movq	144\(%rsp\), %r19
**	movq	152\(%rsp\), %r20
**	movq	160\(%rsp\), %r21
**	movq	168\(%rsp\), %r22
**	movq	176\(%rsp\), %r23
**	movq	184\(%rsp\), %r24
**	movq	192\(%rsp\), %r25
**	movq	200\(%rsp\), %r26
**	movq	208\(%rsp\), %r27
**	movq	216\(%rsp\), %r28
**	movq	224\(%rsp\), %r29
**	movq	232\(%rsp\), %r30
**	movq	240\(%rsp\), %r31
**	addq	\$248, %rsp
**...
**	ret
**	.cfi_endproc
**...
*/

#define DONT_SAVE_REGS __attribute__((no_callee_saved_registers))
#define SAVE_REGS __attribute__((no_caller_saved_registers))

typedef DONT_SAVE_REGS void (*op_t)(void); 

extern op_t code[];

SAVE_REGS void start()
{
  code[0]();
}
