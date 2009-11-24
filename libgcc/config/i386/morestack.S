# x86/x86_64 support for -fsplit-stack.
# Copyright (C) 2009 Free Software Foundation, Inc.
# Contributed by Ian Lance Taylor <iant@google.com>.

# This file is part of GCC.

# GCC is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3, or (at your option) any later
# version.

# GCC is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.

# Under Section 7 of GPL version 3, you are granted additional
# permissions described in the GCC Runtime Library Exception, version
# 3.1, as published by the Free Software Foundation.

# You should have received a copy of the GNU General Public License and
# a copy of the GCC Runtime Library Exception along with this program;
# see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
# <http://www.gnu.org/licenses/>.


# Support for allocating more stack space when using -fsplit-stack.
# When a function discovers that it needs more stack space, it will
# call __morestack with the size of the stack frame and the size of
# the parameters to copy from the old stack frame to the new one.
# The __morestack function preserves the parameter registers and
# calls __generic_morestack to actually allocate the stack space.

# When this is called stack space is very low, but we ensure that
# there is enough space to push the parameter registers and to call
# __generic_morestack.

# When calling __generic_morestack, FRAME_SIZE points to the size of
# the desired frame when the function is called, and the function
# sets it to the size of the allocated stack.  OLD_STACK points to
# the parameters on the old stack and PARAM_SIZE is the number of
# bytes of parameters to copy to the new stack.  These are the
# parameters of the function that called __morestack.  The
# __generic_morestack function returns the new stack pointer,
# pointing to the address of the first copied parameter.  The return
# value minus the returned *FRAME_SIZE will be the first address on
# the stack which we should not use.

# void *__generic_morestack (size_t *frame_size, void *old_stack,
#			      size_t param_size);

# The __morestack routine arranges for the caller to return to a stub
# on the new stack.  The stub is responsible for restoring the old
# stack pointer and returning to the caller's caller.  This calls
# __generic_releasestack to retrieve the old stack pointer and
# release the newly allocated stack.

# void *__generic_releasestack (size_t *available);

# We do a little dance so that the processor's call/return return
# address prediction works out.  The compiler arranges for the caller
# to look like this:
#   call __generic_morestack
#   ret
#  L:
#   // carry on with function
# After we allocate more stack, we call L, which is in our caller.
# When that returns (to the predicted instruction), we release the
# stack segment and reset the stack pointer.  We then return to the
# predicted instruction, namely the ret instruction immediately after
# the call to __generic_morestack.  That then returns to the caller of
# the original caller.

# This entry point is for split-stack code which calls non-split-stack
# code.  When the linker sees this case, it converts the call to
# __morestack to call __morestack_non_split instead.  We just bump the
# requested stack space by 16K.

	.global __morestack_non_split

#ifdef __ELF__
       .type	__morestack_non_split,@function
#endif

__morestack_non_split:

#ifndef __x86_64__
	addl	$0x4000,4(%esp)
#else
	addq	$0x4000,%r10
#endif

#ifdef __ELF__
	.size	__morestack_non_split, . - __morestack_non_split
#endif

# __morestack_non_split falls through into __morestack.

	.global	__morestack

#ifdef __ELF__
	.type	__morestack,@function
#endif

__morestack:
	.cfi_startproc

#ifndef __x86_64__

	# Set up a normal backtrace.
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp

	# In 32-bit mode the parameters are pushed on the stack.  The
	# argument size is pushed then the new stack frame size is
	# pushed.

	# In 32-bit mode the registers %eax, %edx, and %ecx may be
	# used for parameters, depending on the regparm and fastcall
	# attributes.

	pushl	%eax
	pushl	%edx
	pushl	%ecx

	pushl	12(%ebp)		# The size of the parameters.
	leal	20(%ebp),%eax		# Address of caller's parameters.
	pushl	%eax
	addl	$512,8(%ebp)		# Ask for backoff bytes.
	leal	8(%ebp),%eax		# The address of the new frame size.
	pushl	%eax

	# Note that %esp is exactly 32 bytes below the CFA -- perfect for
	# a 16-byte aligned stack.  That said, we still ought to compile
	# generic-morestack.c with -mpreferred-stack-boundary=2.  FIXME.
	call	__generic_morestack

	movl	%eax,%esp		# Switch to the new stack.
	subl	8(%ebp),%eax		# The end of the stack space.
	addl	$512,%eax		# Back off 512 bytes.

	# FIXME: The offset must match
	# TARGET_THREAD_SPLIT_STACK_OFFSET in
	# gcc/config/i386/linux.h.
	movl	%eax,%gs:0x30		# Save the new stack boundary.

	movl	-8(%ebp),%edx		# Restore registers.
	movl	-12(%ebp),%ecx

	movl	4(%ebp),%eax		# Increment the return address
	inc	%eax			# to skip the ret instruction;
					# see above.
	
	movl	%eax,-8(%ebp)		# Store return address in an
					# unused slot.

	movl	-4(%ebp),%eax		# Restore the last register.

	call	*-8(%ebp)		# Call our caller!

	# The caller will return here, as predicted.

	# Save the registers which may hold a return value.  We
	# assume that __generic_releasestack does not touch any
	# floating point or vector registers.
	pushl	%eax
	pushl	%edx

	pushl	$0			# Where the available space is returned.
	leal	0(%esp),%eax		# Push its address.
	push	%eax

	call	__generic_releasestack

	subl	4(%esp),%eax		# Subtract available space.
	addl	$512,%eax		# Back off 512 bytes.
	movl	%eax,%gs:0x30		# Save the new stack boundary.

	addl	$8,%esp			# Remove values from stack.

	popl	%edx			# Restore possible return value
	popl	%eax

	# Switch back to the old stack via copy back from %ebp.
	leave
	.cfi_restore %ebp
	.cfi_def_cfa %esp, 4
	ret	$8			# Return to caller, which will
					# immediately return.  Pop
					# arguments as we go.

#else /* defined(__x86_64__) */

	# Set up a normal backtrace.
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp

	# In 64-bit mode the new stack frame size is passed in r10
        # and the argument size is passed in r11.

	addq	$512,%r10		# Ask for backoff bytes.
	pushq	%r10			# Save new frame size.

	# In 64-bit mode the registers %rdi, %rsi, %rdx, %rcx, %r8,
	# and %r9 may be used for parameters.

	pushq	%rdi
	pushq	%rsi
	pushq	%rdx
	pushq	%rcx
	pushq	%r8
	pushq	%r9

	# An extra push to maintain 16-byte stack alignment.  After this,
	# we'll be 80 bytes below the (presumably aligned) CFA.
	pushq	%rdi

	leaq	-8(%rbp),%rdi		# Address of new frame size.
	leaq	24(%rbp),%rsi		# The caller's parameters.
	movq	%r11,%rdx		# The size of the parameters.

	call	__generic_morestack

	popq	%rdi			# Restore registers
	popq	%r9
	popq	%r8
	popq	%rcx
	popq	%rdx
	popq	%rsi
	popq	%rdi

	popq	%r10			# Reload modified frame size
	movq	%rax,%rsp		# Switch to the new stack.
	subq	%r10,%rax		# The end of the stack space.
	addq	$512,%rax		# Back off 512 bytes.

	# FIXME: The offset must match
	# TARGET_THREAD_SPLIT_STACK_OFFSET in
	# gcc/config/i386/linux64.h.
	movq	%rax,%fs:0x70		# Save the new stack boundary.

	movq	8(%rbp),%r10		# Increment the return address
	incq	%r10			# to skip the ret instruction;
					# see above.

	call	*%r10			# Call our caller!

	# The caller will return here, as predicted.

	# Save the registers which may hold a return value.  We
	# assume that __generic_releasestack does not touch any
	# floating point or vector registers.
	pushq	%rax
	pushq	%rdx

	pushq	$0			# Where the available space is returned.
	leaq	0(%rsp),%rdi		# Pass its address.

	call	__generic_releasestack

	subq	0(%rsp),%rax		# Subtract available space.
	addq	$512,%rax		# Back off 512 bytes.
	movq	%rax,%fs:0x70		# Save the new stack boundary.

	addq	$8,%rsp			# Remove value from stack.

	popq	%rdx			# Restore possible return value
	popq	%rax

	# Switch back to the old stack via copy back from %rbp.
	leave
	.cfi_restore %rbp
	.cfi_def_cfa %rsp, 8
	ret				# Return to caller, which will
					# immediately return.

#endif /* defined(__x86_64__) */

	.cfi_endproc
#ifdef __ELF__
	.size	__morestack, . - __morestack
#endif

# Initialize the stack test value when the program starts or when a
# new thread starts.  We don't know how large the main stack is, so we
# guess conservatively.  We might be able to use getrlimit here.

	.global	__stack_split_initialize

#ifdef __ELF__
	.type	__stack_split_initialize, @function
#endif

__stack_split_initialize:

#ifndef __x86_64__

	leal	-16000(%esp),%eax	# We should have at least 16K.
	movl	%eax,%gs:0x30
	ret

#else /* defined(__x86_64__) */

	leaq	-16000(%rsp),%rax	# We should have at least 16K.
	movq	%rax,%fs:0x70
	ret

#endif /* defined(__x86_64__) */

#ifdef __ELF__
	.size	__stack_split_initialize, . - __stack_split_initialize
#endif

# Make __stack_split_initialize a high priority constructor.  FIXME:
# This is ELF specific.

	.section	.ctors.65535,"aw",@progbits

#ifndef __x86_64__
	.align	4
	.long	__stack_split_initialize
#else
	.align	8
	.quad	__stack_split_initialize
#endif

#ifdef __ELF__
	.section	.note.GNU-stack,"",@progbits
	.section	.note.GNU-split-stack,"",@progbits
	.section	.note.GNU-no-split-stack,"",@progbits
#endif
