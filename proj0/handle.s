	.file	"handle.c"
	.section	.rodata
.LC0:
	.string	"%i\n"
	.text
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
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movq	$0, -32(%rbp)
	movq	$1000000000, -24(%rbp)
	call	getpid
	movl	%eax, -4(%rbp)
	movl	$.LC0, %eax
	movl	-4(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$alarm_handler, %esi
	movl	$14, %edi
	call	Signal
	movl	$int_handler, %esi
	movl	$2, %edi
	call	Signal
	movl	$usr1_handler, %esi
	movl	$10, %edi
	call	Signal
	movl	$1, %edi
	call	alarm
.L2:
	leaq	-32(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	nanosleep
	jmp	.L2
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
.LC1:
	.string	"Still here.\n"
	.text
	.globl	alarm_handler
	.type	alarm_handler, @function
alarm_handler:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	$1, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$12, %edx
	movl	$.LC1, %esi
	movl	%eax, %edi
	call	write
	movq	%rax, -16(%rbp)
	cmpq	$12, -16(%rbp)
	je	.L4
	movl	$-999, %edi
	call	exit
.L4:
	movl	$1, %edi
	call	alarm
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	alarm_handler, .-alarm_handler
	.section	.rodata
.LC2:
	.string	"Nice try.\n"
	.text
	.globl	int_handler
	.type	int_handler, @function
int_handler:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	$1, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$10, %edx
	movl	$.LC2, %esi
	movl	%eax, %edi
	call	write
	movq	%rax, -16(%rbp)
	cmpq	$10, -16(%rbp)
	je	.L6
	movl	$-999, %edi
	call	exit
.L6:
	movl	$1, %edi
	call	alarm
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	int_handler, .-int_handler
	.section	.rodata
.LC3:
	.string	"exiting\n"
	.text
	.globl	usr1_handler
	.type	usr1_handler, @function
usr1_handler:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	$1, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$8, %edx
	movl	$.LC3, %esi
	movl	%eax, %edi
	call	write
	movq	%rax, -16(%rbp)
	cmpq	$8, -16(%rbp)
	je	.L8
	movl	$1, %edi
	call	exit
.L8:
	movl	$1, %edi
	call	exit
	.cfi_endproc
.LFE3:
	.size	usr1_handler, .-usr1_handler
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
