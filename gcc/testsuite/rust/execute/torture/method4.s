	.file	"method4.rs"
	.text
	.globl	_ZN21_$LT$method4..Bar$GT$6foobar17h97f851c20cc4f331E
	.type	_ZN21_$LT$method4..Bar$GT$6foobar17h97f851c20cc4f331E, @function
_ZN21_$LT$method4..Bar$GT$6foobar17h97f851c20cc4f331E:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_ZN21_$LT$method4..Bar$GT$6foobar17h97f851c20cc4f331E, .-_ZN21_$LT$method4..Bar$GT$6foobar17h97f851c20cc4f331E
	.section	.rodata
.LC0:
	.string	"mut_deref\n"
	.zero	1
	.text
	.type	_ZN61_$LT$method4..Foo..$LT$T$GT$$u20$as$u20$method4..DerefMut$GT$9deref_mut17hf136b71d2680718dE, @function
_ZN61_$LT$method4..Foo..$LT$T$GT$$u20$as$u20$method4..DerefMut$GT$9deref_mut17hf136b71d2680718dE:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	$.LC0, -48(%rbp)
	movq	$11, -40(%rbp)
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-56(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	_ZN61_$LT$method4..Foo..$LT$T$GT$$u20$as$u20$method4..DerefMut$GT$9deref_mut17hf136b71d2680718dE, .-_ZN61_$LT$method4..Foo..$LT$T$GT$$u20$as$u20$method4..DerefMut$GT$9deref_mut17hf136b71d2680718dE
	.section	.rodata
.LC1:
	.string	"foobar: %i\n"
	.zero	1
	.text
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
	subq	$80, %rsp
	movl	$123, -36(%rbp)
	leaq	-36(%rbp), %rax
	movq	%rax, -32(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN61_$LT$method4..Foo..$LT$T$GT$$u20$as$u20$method4..DerefMut$GT$9deref_mut17hf136b71d2680718dE
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	_ZN21_$LT$method4..Bar$GT$6foobar17h97f851c20cc4f331E
	movl	%eax, -4(%rbp)
	movq	$.LC1, -80(%rbp)
	movq	$12, -72(%rbp)
	movq	-80(%rbp), %rax
	movq	-72(%rbp), %rdx
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movq	-64(%rbp), %rax
	movq	%rax, -16(%rbp)
	movl	-4(%rbp), %edx
	movq	-16(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %edx
	movl	-4(%rbp), %eax
	subl	$123, %eax
	jno	.L6
	movl	$1, %edx
.L6:
	movl	%eax, -20(%rbp)
	movl	%edx, %eax
	andl	$1, %eax
	testb	%al, %al
	je	.L8
	call	abort
.L8:
	movl	-20(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.0.1 20240309 (experimental)"
	.section	.note.GNU-stack,"",@progbits
