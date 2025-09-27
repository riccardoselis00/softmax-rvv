	.file	"sum.c"
	.option pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section	.rodata
	.align	3
.LC2:
	.string	"the ciaoooo sum of %.1f and %.1f is %.1f\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	addi	sp,sp,-48
	.cfi_def_cfa_offset 48
	sd	ra,40(sp)
	sd	s0,32(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	addi	s0,sp,48
	.cfi_def_cfa 8, 0
	lla	a5,.LC0
	fld	fa5,0(a5)
	fsd	fa5,-40(s0)
	lla	a5,.LC1
	fld	fa5,0(a5)
	fsd	fa5,-32(s0)
	fld	fa4,-40(s0)
	fld	fa5,-32(s0)
	fadd.d	fa5,fa4,fa5
	fsd	fa5,-24(s0)
	ld	a3,-24(s0)
	ld	a2,-32(s0)
	ld	a1,-40(s0)
	lla	a0,.LC2
	call	printf@plt
	li	a5,0
	mv	a0,a5
	ld	ra,40(sp)
	.cfi_restore 1
	ld	s0,32(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 48
	addi	sp,sp,48
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align	3
.LC0:
	.word	0
	.word	1076101120
	.align	3
.LC1:
	.word	0
	.word	1075052544
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
