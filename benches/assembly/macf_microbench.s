	.file	"macf_microbench.c"
	.option pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.local	sink
	.comm	sink,4,4
	.align	1
	.type	macf_s_word, @function
macf_s_word:
.LFB6:
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
# 15 "benches/codes/macf_microbench.c" 1
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
.LFE6:
	.size	macf_s_word, .-macf_s_word
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	addi	sp,sp,-64
	.cfi_def_cfa_offset 64
	sd	ra,56(sp)
	sd	s0,48(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	addi	s0,sp,64
	.cfi_def_cfa 8, 0
	mv	a5,a0
	sd	a1,-64(s0)
	sw	a5,-52(s0)
	lw	a5,-52(s0)
	sext.w	a4,a5
	li	a5,1
	ble	a4,a5,.L4
	ld	a5,-64(s0)
	addi	a5,a5,8
	ld	a5,0(a5)
	li	a2,10
	li	a1,0
	mv	a0,a5
	call	strtoull@plt
	mv	a5,a0
	j	.L5
.L4:
	li	a5,9998336
	addi	a5,a5,1664
.L5:
	sd	a5,-24(s0)
	fmv.s.x	fa5,zero
	fsw	fa5,-44(s0)
	lla	a5,.LC0
	flw	fa5,0(a5)
	fsw	fa5,-40(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fsw	fa5,-36(s0)
	sd	zero,-32(s0)
	j	.L6
.L7:
	flw	fa2,-36(s0)
	flw	fa1,-40(s0)
	flw	fa0,-44(s0)
	call	macf_s_word
	fsw	fa0,-44(s0)
	ld	a5,-32(s0)
	addi	a5,a5,1
	sd	a5,-32(s0)
.L6:
	ld	a4,-32(s0)
	ld	a5,-24(s0)
	bltu	a4,a5,.L7
	lla	a5,sink
	flw	fa5,-44(s0)
	fsw	fa5,0(a5)
	li	a5,0
	mv	a0,a5
	ld	ra,56(sp)
	.cfi_restore 1
	ld	s0,48(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 64
	addi	sp,sp,64
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.section	.rodata
	.align	2
.LC0:
	.word	1065361605
	.align	2
.LC1:
	.word	1065336439
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
