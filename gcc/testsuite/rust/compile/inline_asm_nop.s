	.file	"inline_asm_nop.rs"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	pushq	%rbp
.LCFI0:
	movq	%rsp, %rbp
.LCFI1:
	popq	%rbp
.LCFI2:
	ret
.LFE0:
	.size	main, .-main
	.section	.eh_frame,"aw",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0
	.byte	0x3
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.uleb128 0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0
	.byte	0x4
	.long	.LCFI0-.LFB0
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.align 8
.LEFDE1:
	.ident	"GCC: (GNU) 14.0.1 20240309 (experimental)"
	.section	.note.GNU-stack,"",@progbits
