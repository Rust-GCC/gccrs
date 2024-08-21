	.file	"issue-1436.rs"
	.text
	.type	_ZN34_$LT$$BP$const$u20$$u5b$T$u5d$$GT$6as_ptr17h8a715795e7d22b79E, @function
_ZN34_$LT$$BP$const$u20$$u5b$T$u5d$$GT$6as_ptr17h8a715795e7d22b79E:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, %rax
	movq	%rsi, %rcx
	movq	%rax, %rax
	movl	$0, %edx
	movq	%rcx, %rdx
	movq	%rax, -16(%rbp)
	movq	%rdx, -8(%rbp)
	movq	-16(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	_ZN34_$LT$$BP$const$u20$$u5b$T$u5d$$GT$6as_ptr17h8a715795e7d22b79E, .-_ZN34_$LT$$BP$const$u20$$u5b$T$u5d$$GT$6as_ptr17h8a715795e7d22b79E
	.type	_ZN6offset17hfcce61add4369095E, @function
_ZN6offset17hfcce61add4369095E:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	leaq	0(,%rax,4), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	_ZN6offset17hfcce61add4369095E, .-_ZN6offset17hfcce61add4369095E
	.type	_ZN24_$LT$$BP$const$u20$T$GT$6offset17hdd98c187de4a90fcE, @function
_ZN24_$LT$$BP$const$u20$T$GT$6offset17hdd98c187de4a90fcE:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN6offset17hfcce61add4369095E
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	_ZN24_$LT$$BP$const$u20$T$GT$6offset17hdd98c187de4a90fcE, .-_ZN24_$LT$$BP$const$u20$T$GT$6offset17hdd98c187de4a90fcE
	.type	_ZN24_$LT$$BP$const$u20$T$GT$3add17ha0021fa3514bb587E, @function
_ZN24_$LT$$BP$const$u20$T$GT$3add17ha0021fa3514bb587E:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN24_$LT$$BP$const$u20$T$GT$6offset17hdd98c187de4a90fcE
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	_ZN24_$LT$$BP$const$u20$T$GT$3add17ha0021fa3514bb587E, .-_ZN24_$LT$$BP$const$u20$T$GT$3add17ha0021fa3514bb587E
	.type	_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$13get_unchecked17h380626c4db66b3dbE, @function
_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$13get_unchecked17h380626c4db66b3dbE:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, %rax
	movq	%rdx, %rcx
	movq	%rax, %rax
	movl	$0, %edx
	movq	%rcx, %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rdi
	movq	%rax, %rsi
	call	_ZN34_$LT$$BP$const$u20$$u5b$T$u5d$$GT$6as_ptr17h8a715795e7d22b79E
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	_ZN24_$LT$$BP$const$u20$T$GT$3add17ha0021fa3514bb587E
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$13get_unchecked17h380626c4db66b3dbE, .-_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$13get_unchecked17h380626c4db66b3dbE
	.type	_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$5index17hd4d43ff651bdb67eE, @function
_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$5index17hd4d43ff651bdb67eE:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, %rax
	movq	%rdx, %rcx
	movq	%rax, %rax
	movl	$0, %edx
	movq	%rcx, %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$13get_unchecked17h380626c4db66b3dbE
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$5index17hd4d43ff651bdb67eE, .-_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$5index17hd4d43ff651bdb67eE
	.section	.rodata
.LC0:
	.string	"slice-index\n"
	.zero	1
	.text
	.type	_ZN60_$LT$$u5b$T$u5d$$u20$as$u20$issue_1436..Index..$LT$I$GT$$GT$5index17h213005ab2b4ec746E, @function
_ZN60_$LT$$u5b$T$u5d$$u20$as$u20$issue_1436..Index..$LT$I$GT$$GT$5index17h213005ab2b4ec746E:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$88, %rsp
	.cfi_offset 3, -24
	movq	%rdi, %rax
	movq	%rax, %rcx
	movl	$0, %ebx
	movq	%rsi, %rbx
	movq	%rcx, -80(%rbp)
	movq	%rbx, -72(%rbp)
	movq	%rdx, -88(%rbp)
	movq	$.LC0, -64(%rbp)
	movq	$13, -56(%rbp)
	movq	-64(%rbp), %rax
	movq	-56(%rbp), %rdx
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-80(%rbp), %rcx
	movq	-72(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	_ZN69_$LT$usize$u20$as$u20$issue_1436..SliceIndex..$LT$$u5b$T$u5d$$GT$$GT$5index17hd4d43ff651bdb67eE
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_ZN60_$LT$$u5b$T$u5d$$u20$as$u20$issue_1436..Index..$LT$I$GT$$GT$5index17h213005ab2b4ec746E, .-_ZN60_$LT$$u5b$T$u5d$$u20$as$u20$issue_1436..Index..$LT$I$GT$$GT$5index17h213005ab2b4ec746E
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	$1, -64(%rbp)
	movl	$2, -60(%rbp)
	movl	$3, -56(%rbp)
	movl	$4, -52(%rbp)
	movl	$5, -48(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	$5, -24(%rbp)
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rax
	movl	$1, %edx
	movq	%rcx, %rdi
	movq	%rax, %rsi
	call	_ZN60_$LT$$u5b$T$u5d$$u20$as$u20$issue_1436..Index..$LT$I$GT$$GT$5index17h213005ab2b4ec746E
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movl	$0, %edx
	movl	-4(%rbp), %eax
	subl	$2, %eax
	jno	.L16
	movl	$1, %edx
.L16:
	movl	%eax, -8(%rbp)
	movl	%edx, %eax
	andl	$1, %eax
	testb	%al, %al
	je	.L18
	call	abort
.L18:
	movl	-8(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.0.1 20240309 (experimental)"
	.section	.note.GNU-stack,"",@progbits
