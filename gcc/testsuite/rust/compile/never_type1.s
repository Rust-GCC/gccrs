	.file	"never_type1.rs"
	.text
	.type	_ZN11never_type14test17h3f05998eb63478baE, @function
_ZN11never_type14test17h3f05998eb63478baE:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_ZN11never_type14test17h3f05998eb63478baE, .-_ZN11never_type14test17h3f05998eb63478baE
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	_ZN11never_type14test17h3f05998eb63478baE
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.0.0 20240109 (experimental)"
	.section	.note.GNU-stack,"",@progbits
