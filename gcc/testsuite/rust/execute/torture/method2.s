	.file	"method2.rs"
	.text
	.type	_ZN21_$LT$method2..Bar$GT$6foobar17hef2a7cef07c80038E, @function
_ZN21_$LT$method2..Bar$GT$6foobar17hef2a7cef07c80038E:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_ZN21_$LT$method2..Bar$GT$6foobar17hef2a7cef07c80038E, .-_ZN21_$LT$method2..Bar$GT$6foobar17hef2a7cef07c80038E
	.section	.rodata
.LC0:
	.string	"foo_deref\n"
	.zero	1
	.text
	.type	_ZN58_$LT$method2..Foo..$LT$T$GT$$u20$as$u20$method2..Deref$GT$5deref17h2a5903e4427cfb70E, @function
_ZN58_$LT$method2..Foo..$LT$T$GT$$u20$as$u20$method2..Deref$GT$5deref17h2a5903e4427cfb70E:
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
	.size	_ZN58_$LT$method2..Foo..$LT$T$GT$$u20$as$u20$method2..Deref$GT$5deref17h2a5903e4427cfb70E, .-_ZN58_$LT$method2..Foo..$LT$T$GT$$u20$as$u20$method2..Deref$GT$5deref17h2a5903e4427cfb70E
	.section	.rodata
.LC1:
	.string	"imm_deref\n"
	.zero	1
	.text
	.type	_ZN45_$LT$$RF$$u20$T$u20$as$u20$method2..Deref$GT$5deref17h59990cb857f9fb5fE, @function
_ZN45_$LT$$RF$$u20$T$u20$as$u20$method2..Deref$GT$5deref17h59990cb857f9fb5fE:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	$.LC1, -48(%rbp)
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
	movq	(%rax), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	_ZN45_$LT$$RF$$u20$T$u20$as$u20$method2..Deref$GT$5deref17h59990cb857f9fb5fE, .-_ZN45_$LT$$RF$$u20$T$u20$as$u20$method2..Deref$GT$5deref17h59990cb857f9fb5fE
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
	subq	$32, %rsp
	movl	$123, -20(%rbp)
	leaq	-20(%rbp), %rax
	movq	%rax, -16(%rbp)
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN58_$LT$method2..Foo..$LT$T$GT$$u20$as$u20$method2..Deref$GT$5deref17h2a5903e4427cfb70E
	movq	%rax, %rdi
	call	_ZN45_$LT$$RF$$u20$T$u20$as$u20$method2..Deref$GT$5deref17h59990cb857f9fb5fE
	movl	(%rax), %eax
	movl	%eax, %edi
	call	_ZN21_$LT$method2..Bar$GT$6foobar17hef2a7cef07c80038E
	movl	%eax, -4(%rbp)
	movl	$0, %edx
	movl	-4(%rbp), %eax
	subl	$123, %eax
	jno	.L8
	movl	$1, %edx
.L8:
	movl	%eax, -8(%rbp)
	movl	%edx, %eax
	andl	$1, %eax
	testb	%al, %al
	je	.L10
	call	abort
.L10:
	movl	-8(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.0.1 20240309 (experimental)"
	.section	.note.GNU-stack,"",@progbits
