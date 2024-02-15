	.file	"issue-2847-b.rs"
	.text
	.globl	_ZN12issue_2847_b4test17h911112d7b004734fE
	.type	_ZN12issue_2847_b4test17h911112d7b004734fE, @function
_ZN12issue_2847_b4test17h911112d7b004734fE:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movabsq	$-4294967296, %rdx
	andq	%rdx, %rax
	orq	$1, %rax
	movl	%eax, %edx
	movabsq	$8589934592, %rax
	orq	%rdx, %rax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_ZN12issue_2847_b4test17h911112d7b004734fE, .-_ZN12issue_2847_b4test17h911112d7b004734fE
	.ident	"GCC: (GNU) 13.0.1 20230417 (experimental)"
	.section	.note.GNU-stack,"",@progbits
