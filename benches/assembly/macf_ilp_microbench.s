	.file	"macf_ilp_microbench.c"
	.option pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.local	sink
	.comm	sink,4,4
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	addi	sp,sp,-128
	.cfi_def_cfa_offset 128
	sd	ra,120(sp)
	sd	s0,112(sp)
	fsd	fs2,104(sp)
	fsd	fs3,96(sp)
	fsd	fs4,88(sp)
	fsd	fs5,80(sp)
	fsd	fs6,72(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	.cfi_offset 50, -24
	.cfi_offset 51, -32
	.cfi_offset 52, -40
	.cfi_offset 53, -48
	.cfi_offset 54, -56
	addi	s0,sp,128
	.cfi_def_cfa 8, 0
	mv	a5,a0
	sd	a1,-128(s0)
	sw	a5,-116(s0)
	lw	a5,-116(s0)
	sext.w	a4,a5
	li	a5,1
	ble	a4,a5,.L2
	ld	a5,-128(s0)
	addi	a5,a5,8
	ld	a5,0(a5)
	li	a2,10
	li	a1,0
	mv	a0,a5
	call	strtoull@plt
	mv	a5,a0
	j	.L3
.L2:
	li	a5,7999488
	addi	a5,a5,512
.L3:
	sd	a5,-80(s0)
	lw	a5,-116(s0)
	sext.w	a4,a5
	li	a5,2
	ble	a4,a5,.L4
	ld	a5,-128(s0)
	addi	a5,a5,16
	ld	a5,0(a5)
	mv	a0,a5
	call	atoi@plt
	mv	a5,a0
	j	.L5
.L4:
	li	a5,8
.L5:
	sw	a5,-104(s0)
	lw	a5,-104(s0)
	sext.w	a5,a5
	bgt	a5,zero,.L6
	li	a5,1
	sw	a5,-104(s0)
.L6:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,8
	ble	a4,a5,.L7
	li	a5,8
	sw	a5,-104(s0)
.L7:
	lla	a5,.LC0
	flw	fa5,0(a5)
	fsw	fa5,-96(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fsw	fa5,-92(s0)
	flw	fa3,-92(s0)
	flw	fa4,-96(s0)
	fmv.s.x	fa5,zero
	lla	a5,.LC2
	flw	fa6,0(a5)
	lla	a5,.LC3
	flw	fa7,0(a5)
	lla	a5,.LC4
	flw	fs2,0(a5)
	lla	a5,.LC5
	flw	fs3,0(a5)
	lla	a5,.LC6
	flw	fs4,0(a5)
	lla	a5,.LC7
	flw	fs5,0(a5)
	lla	a5,.LC8
	flw	fs6,0(a5)
	lw	a5,-104(s0)
	ld	a4,-80(s0)
	divu	a5,a4,a5
	sd	a5,-72(s0)
	sd	zero,-88(s0)
	j	.L8
.L19:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,8
	bgtu	a4,a5,.L9
	lwu	a5,-104(s0)
	slli	a4,a5,2
	lla	a5,.L11
	add	a5,a4,a5
	lw	a5,0(a5)
	sext.w	a4,a5
	lla	a5,.L11
	add	a5,a4,a5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L11:
	.word	.L9-.L11
	.word	.L18-.L11
	.word	.L17-.L11
	.word	.L16-.L11
	.word	.L15-.L11
	.word	.L14-.L11
	.word	.L13-.L11
	.word	.L12-.L11
	.word	.L10-.L11
	.text
.L10:
#APP
# 59 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68EB7B5B
# 0 "" 2
#NO_APP
.L12:
#APP
# 60 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68EAFADB
# 0 "" 2
#NO_APP
.L13:
#APP
# 61 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68EA7A5B
# 0 "" 2
#NO_APP
.L14:
#APP
# 62 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68E9F9DB
# 0 "" 2
#NO_APP
.L15:
#APP
# 63 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68E9795B
# 0 "" 2
#NO_APP
.L16:
#APP
# 64 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68E8F8DB
# 0 "" 2
#NO_APP
.L17:
#APP
# 65 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68E8785B
# 0 "" 2
#NO_APP
.L18:
#APP
# 66 "benches/codes/macf_ilp_microbench.c" 1
	.word 0x68E7F7DB
# 0 "" 2
#NO_APP
.L9:
	ld	a5,-88(s0)
	addi	a5,a5,1
	sd	a5,-88(s0)
.L8:
	ld	a4,-88(s0)
	ld	a5,-72(s0)
	bltu	a4,a5,.L19
	fmv.s.x	fa4,zero
	fsw	fa4,-100(s0)
	lw	a5,-104(s0)
	sext.w	a5,a5
	ble	a5,zero,.L20
	fmv.s	fa4,fa5
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L20:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,1
	ble	a4,a5,.L21
	fmv.s	fa4,fa6
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L21:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,2
	ble	a4,a5,.L22
	fmv.s	fa4,fa7
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L22:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,3
	ble	a4,a5,.L23
	fmv.s	fa4,fs2
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L23:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,4
	ble	a4,a5,.L24
	fmv.s	fa4,fs3
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L24:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,5
	ble	a4,a5,.L25
	fmv.s	fa4,fs4
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L25:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,6
	ble	a4,a5,.L26
	fmv.s	fa4,fs5
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L26:
	lw	a5,-104(s0)
	sext.w	a4,a5
	li	a5,7
	ble	a4,a5,.L27
	fmv.s	fa4,fs6
	flw	fa5,-100(s0)
	fadd.s	fa5,fa5,fa4
	fsw	fa5,-100(s0)
.L27:
	lla	a5,sink
	flw	fa5,-100(s0)
	fsw	fa5,0(a5)
	li	a5,0
	mv	a0,a5
	ld	ra,120(sp)
	.cfi_restore 1
	ld	s0,112(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 128
	fld	fs2,104(sp)
	.cfi_restore 50
	fld	fs3,96(sp)
	.cfi_restore 51
	fld	fs4,88(sp)
	.cfi_restore 52
	fld	fs5,80(sp)
	.cfi_restore 53
	fld	fs6,72(sp)
	.cfi_restore 54
	addi	sp,sp,128
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align	2
.LC0:
	.word	1065361605
	.align	2
.LC1:
	.word	1065336439
	.align	2
.LC2:
	.word	1065353216
	.align	2
.LC3:
	.word	1073741824
	.align	2
.LC4:
	.word	1077936128
	.align	2
.LC5:
	.word	1082130432
	.align	2
.LC6:
	.word	1084227584
	.align	2
.LC7:
	.word	1086324736
	.align	2
.LC8:
	.word	1088421888
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
