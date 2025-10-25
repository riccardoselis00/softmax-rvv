	.file	"softmax.c"
	.option pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section	.rodata
	.align	3
.LC1:
	.string	"%.6f\n"
	.align	3
.LC0:
	.word	0
	.word	1072693248
	.word	0
	.word	1073741824
	.word	0
	.word	1074266112
	.word	0
	.word	1074790400
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	addi	sp,sp,-112
	.cfi_def_cfa_offset 112
	sd	ra,104(sp)
	sd	s0,96(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	addi	s0,sp,112
	.cfi_def_cfa 8, 0
	la	a5,__stack_chk_guard
	ld	a4, 0(a5)
	sd	a4, -24(s0)
	li	a4, 0
	lla	a5,.LC0
	ld	a2,0(a5)
	ld	a3,8(a5)
	ld	a4,16(a5)
	ld	a5,24(a5)
	sd	a2,-88(s0)
	sd	a3,-80(s0)
	sd	a4,-72(s0)
	sd	a5,-64(s0)
	fmv.d.x	fa5,zero
	fsd	fa5,-96(s0)
	sw	zero,-104(s0)
	j	.L2
.L3:
	lw	a5,-104(s0)
	slli	a5,a5,3
	addi	a5,a5,-16
	add	a5,a5,s0
	fld	fa5,-72(a5)
	fmv.d	fa0,fa5
	call	exp@plt
	fmv.d	fa5,fa0
	lw	a5,-104(s0)
	slli	a5,a5,3
	addi	a5,a5,-16
	add	a5,a5,s0
	fsd	fa5,-40(a5)
	lw	a5,-104(s0)
	slli	a5,a5,3
	addi	a5,a5,-16
	add	a5,a5,s0
	fld	fa5,-40(a5)
	fld	fa4,-96(s0)
	fadd.d	fa5,fa4,fa5
	fsd	fa5,-96(s0)
	lw	a5,-104(s0)
	addiw	a5,a5,1
	sw	a5,-104(s0)
.L2:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,3
	ble	a4,a5,.L3
	sw	zero,-100(s0)
	j	.L4
.L5:
	lw	a5,-100(s0)
	slli	a5,a5,3
	addi	a5,a5,-16
	add	a5,a5,s0
	fld	fa4,-40(a5)
	fld	fa5,-96(s0)
	fdiv.d	fa5,fa4,fa5
	lw	a5,-100(s0)
	slli	a5,a5,3
	addi	a5,a5,-16
	add	a5,a5,s0
	fsd	fa5,-40(a5)
	lw	a5,-100(s0)
	slli	a5,a5,3
	addi	a5,a5,-16
	add	a5,a5,s0
	fld	fa5,-40(a5)
	fmv.x.d	a1,fa5
	lla	a0,.LC1
	call	printf@plt
	lw	a5,-100(s0)
	addiw	a5,a5,1
	sw	a5,-100(s0)
.L4:
	lw	a5,-100(s0)
	sext.w	a4,a5
	li	a5,3
	ble	a4,a5,.L5
	li	a5,0
	mv	a4,a5
	la	a5,__stack_chk_guard
	ld	a3, -24(s0)
	ld	a5, 0(a5)
	xor	a5, a3, a5
	li	a3, 0
	beq	a5,zero,.L7
	call	__stack_chk_fail@plt
.L7:
	mv	a0,a4
	ld	ra,104(sp)
	.cfi_restore 1
	ld	s0,96(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 112
	addi	sp,sp,112
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
