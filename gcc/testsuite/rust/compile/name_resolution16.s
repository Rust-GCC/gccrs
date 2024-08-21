	.file	"name_resolution16.rs"
	.text
	.globl	_ZN3Foo17h78829da7d2de9479E
	.type	_ZN3Foo17h78829da7d2de9479E, @function
_ZN3Foo17h78829da7d2de9479E:
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
	.size	_ZN3Foo17h78829da7d2de9479E, .-_ZN3Foo17h78829da7d2de9479E
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
	subq	$16, %rsp
	call	_ZN3Foo17h78829da7d2de9479E
	movl	$15, -4(%rbp)
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.0.0 20240109 (experimental)"
	.section	.note.GNU-stack,"",@progbits
