	.file	"macf_s_copy.c"
	.option pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.local	sink
	.comm	sink,4,4
	.align	1
	.type	macf_s, @function
macf_s:
.LFB0:
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sd	s0,24(sp)
	.cfi_offset 8, -8
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
	fsw	fa0,-20(s0)
	fsw	fa1,-24(s0)
	fsw	fa2,-28(s0)
	flw	fa3,-28(s0)
	flw	fa4,-24(s0)
	flw	fa5,-20(s0)
#APP
# 10 "benches/codes/macf_s_copy.c" 1
	.word 0x68E7F7DB
# 0 "" 2
#NO_APP
	fmv.s	fa0,fa5
	ld	s0,24(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 32
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE0:
	.size	macf_s, .-macf_s
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sd	ra,24(sp)
	sd	s0,16(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
	lla	a5,.LC0
	flw	fa5,0(a5)
	fsw	fa5,-32(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fsw	fa5,-28(s0)
	lla	a5,.LC2
	flw	fa5,0(a5)
	fsw	fa5,-24(s0)
	flw	fa2,-24(s0)
	flw	fa1,-28(s0)
	flw	fa0,-32(s0)
	call	macf_s
	fsw	fa0,-20(s0)
	lla	a5,sink
	flw	fa5,-20(s0)
	fsw	fa5,0(a5)
	li	a5,0
	mv	a0,a5
	ld	ra,24(sp)
	.cfi_restore 1
	ld	s0,16(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 32
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.section	.rodata
	.align	2
.LC0:
	.word	1073741824
	.align	2
.LC1:
	.word	1077936128
	.align	2
.LC2:
	.word	1082130432
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
