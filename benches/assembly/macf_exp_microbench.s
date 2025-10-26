	.file	"macf_exp_microbench.c"
	.option pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section	.rodata
	.align	2
	.type	c0, @object
	.size	c0, 4
c0:
	.word	1065353216
	.align	2
	.type	c1, @object
	.size	c1, 4
c1:
	.word	1060205080
	.align	2
	.type	c2, @object
	.size	c2, 4
c2:
	.word	1047920112
	.align	2
	.type	c3, @object
	.size	c3, 4
c3:
	.word	1029920839
	.align	2
	.type	c4, @object
	.size	c4, 4
c4:
	.word	1008571739
	.align	2
	.type	c5, @object
	.size	c5, 4
c5:
	.word	984531967
	.align	2
	.type	LOG2E, @object
	.size	LOG2E, 4
LOG2E:
	.word	1069066811
	.local	sink
	.comm	sink,4,4
	.text
	.align	1
	.globl	bench_exp_horner_word_ilp
	.type	bench_exp_horner_word_ilp, @function
bench_exp_horner_word_ilp:
.LFB8:
	.cfi_startproc
	addi	sp,sp,-320
	.cfi_def_cfa_offset 320
	sd	ra,312(sp)
	sd	s0,304(sp)
	sd	s1,296(sp)
	fsd	fs0,280(sp)
	fsd	fs1,272(sp)
	fsd	fs2,264(sp)
	fsd	fs3,256(sp)
	fsd	fs4,248(sp)
	fsd	fs5,240(sp)
	fsd	fs6,232(sp)
	fsd	fs8,224(sp)
	fsd	fs9,216(sp)
	fsd	fs10,208(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	.cfi_offset 9, -24
	.cfi_offset 40, -40
	.cfi_offset 41, -48
	.cfi_offset 50, -56
	.cfi_offset 51, -64
	.cfi_offset 52, -72
	.cfi_offset 53, -80
	.cfi_offset 54, -88
	.cfi_offset 56, -96
	.cfi_offset 57, -104
	.cfi_offset 58, -112
	addi	s0,sp,320
	.cfi_def_cfa 8, 0
	sd	a0,-296(s0)
	sd	a1,-304(s0)
	sd	a2,-312(s0)
	mv	a5,a3
	sw	a5,-316(s0)
	la	a5,__stack_chk_guard
	ld	a4, 0(a5)
	sd	a4, -120(s0)
	li	a4, 0
	lw	a5,-316(s0)
	sext.w	a5,a5
	bgt	a5,zero,.L2
	li	a5,1
	sw	a5,-316(s0)
.L2:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,8
	ble	a4,a5,.L3
	li	a5,8
	sw	a5,-316(s0)
.L3:
	sd	zero,-224(s0)
	j	.L4
.L85:
	sw	zero,-272(s0)
	j	.L5
.L10:
	lw	a4,-272(s0)
	ld	a5,-224(s0)
	add	a5,a4,a5
	slli	a5,a5,2
	ld	a4,-296(s0)
	add	a5,a4,a5
	flw	fa5,0(a5)
	addi	a4,s0,-184
	lw	a5,-272(s0)
	slli	a5,a5,2
	add	a4,a4,a5
	addi	a3,s0,-152
	lw	a5,-272(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	fsw	fa5,-264(s0)
	sd	a4,-216(s0)
	sd	a5,-208(s0)
	lla	a5,.LC0
	flw	fa5,0(a5)
	flw	fa4,-264(s0)
	fmul.s	fa5,fa4,fa5
	fsw	fa5,-260(s0)
	flw	fa5,-260(s0)
	fsw	fa5,-256(s0)
	flw	fa5,-256(s0)
	fmv.s.x	fa4,zero
	fge.s	a5,fa5,fa4
	beq	a5,zero,.L100
	flw	fa4,-256(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fadd.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
	j	.L8
.L100:
	flw	fa4,-256(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fsub.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
.L8:
	sw	a5,-252(s0)
	lw	a5,-252(s0)
	fcvt.s.w	fa5,a5
	flw	fa4,-260(s0)
	fsub.s	fa5,fa4,fa5
	fsw	fa5,-248(s0)
	ld	a5,-216(s0)
	lw	a4,-252(s0)
	sw	a4,0(a5)
	ld	a5,-208(s0)
	flw	fa5,-248(s0)
	fsw	fa5,0(a5)
	nop
	lw	a5,-272(s0)
	addiw	a5,a5,1
	sw	a5,-272(s0)
.L5:
	lw	a5,-272(s0)
	mv	a4,a5
	lw	a5,-316(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L10
	lw	a5,-316(s0)
	sext.w	a5,a5
	ble	a5,zero,.L11
	flw	fa5,-152(s0)
	j	.L12
.L11:
	fmv.s.x	fa5,zero
.L12:
	fmv.s	fs0,fa5
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,1
	ble	a4,a5,.L13
	flw	fa5,-148(s0)
	j	.L14
.L13:
	fmv.s.x	fa5,zero
.L14:
	fmv.s	fs1,fa5
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,2
	ble	a4,a5,.L15
	flw	fa5,-144(s0)
	j	.L16
.L15:
	fmv.s.x	fa5,zero
.L16:
	fmv.s	fa0,fa5
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,3
	ble	a4,a5,.L17
	flw	fa5,-140(s0)
	j	.L18
.L17:
	fmv.s.x	fa5,zero
.L18:
	fmv.s	fa1,fa5
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,4
	ble	a4,a5,.L19
	flw	fa5,-136(s0)
	j	.L20
.L19:
	fmv.s.x	fa5,zero
.L20:
	fmv.s	fa2,fa5
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,5
	ble	a4,a5,.L21
	flw	fa5,-132(s0)
	j	.L22
.L21:
	fmv.s.x	fa5,zero
.L22:
	fmv.s	fs8,fa5
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,6
	ble	a4,a5,.L23
	flw	fa5,-128(s0)
	j	.L24
.L23:
	fmv.s.x	fa5,zero
.L24:
	fmv.s	fs9,fa5
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,7
	ble	a4,a5,.L25
	flw	fa5,-124(s0)
	j	.L26
.L25:
	fmv.s.x	fa5,zero
.L26:
	fmv.s	fs10,fa5
	lla	a5,.LC2
	flw	fa5,0(a5)
	lla	a5,.LC2
	flw	fa4,0(a5)
	fmv.s	fa6,fa4
	lla	a5,.LC2
	flw	fa4,0(a5)
	fmv.s	fa7,fa4
	lla	a5,.LC2
	flw	fa4,0(a5)
	fmv.s	fs2,fa4
	lla	a5,.LC2
	flw	fa4,0(a5)
	fmv.s	fs3,fa4
	lla	a5,.LC2
	flw	fa4,0(a5)
	fmv.s	fs4,fa4
	lla	a5,.LC2
	flw	fa4,0(a5)
	fmv.s	fs5,fa4
	lla	a5,.LC2
	flw	fa4,0(a5)
	fmv.s	fs6,fa4
	lla	a5,.LC3
	flw	fa4,0(a5)
	fmv.s	fa3,fa4
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,8
	bgtu	a4,a5,.L94
	lwu	a5,-316(s0)
	slli	a4,a5,2
	lla	a5,.L29
	add	a5,a4,a5
	lw	a5,0(a5)
	sext.w	a4,a5
	lla	a5,.L29
	add	a5,a4,a5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L29:
	.word	.L94-.L29
	.word	.L36-.L29
	.word	.L35-.L29
	.word	.L34-.L29
	.word	.L33-.L29
	.word	.L32-.L29
	.word	.L31-.L29
	.word	.L30-.L29
	.word	.L28-.L29
	.text
.L28:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x69AB7B5B
# 0 "" 2
#NO_APP
.L30:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x699AFADB
# 0 "" 2
#NO_APP
.L31:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x698A7A5B
# 0 "" 2
#NO_APP
.L32:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68C9F9DB
# 0 "" 2
#NO_APP
.L33:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68B9795B
# 0 "" 2
#NO_APP
.L34:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68A8F8DB
# 0 "" 2
#NO_APP
.L35:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6898785B
# 0 "" 2
#NO_APP
.L36:
#APP
# 136 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6887F7DB
# 0 "" 2
#NO_APP
.L94:
	lla	a5,.LC4
	flw	fa4,0(a5)
	fmv.s	fa3,fa4
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,8
	bgtu	a4,a5,.L95
	lwu	a5,-316(s0)
	slli	a4,a5,2
	lla	a5,.L39
	add	a5,a4,a5
	lw	a5,0(a5)
	sext.w	a4,a5
	lla	a5,.L39
	add	a5,a4,a5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L39:
	.word	.L95-.L39
	.word	.L46-.L39
	.word	.L45-.L39
	.word	.L44-.L39
	.word	.L43-.L39
	.word	.L42-.L39
	.word	.L41-.L39
	.word	.L40-.L39
	.word	.L38-.L39
	.text
.L38:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x69AB7B5B
# 0 "" 2
#NO_APP
.L40:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x699AFADB
# 0 "" 2
#NO_APP
.L41:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x698A7A5B
# 0 "" 2
#NO_APP
.L42:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68C9F9DB
# 0 "" 2
#NO_APP
.L43:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68B9795B
# 0 "" 2
#NO_APP
.L44:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68A8F8DB
# 0 "" 2
#NO_APP
.L45:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6898785B
# 0 "" 2
#NO_APP
.L46:
#APP
# 137 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6887F7DB
# 0 "" 2
#NO_APP
.L95:
	lla	a5,.LC5
	flw	fa4,0(a5)
	fmv.s	fa3,fa4
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,8
	bgtu	a4,a5,.L96
	lwu	a5,-316(s0)
	slli	a4,a5,2
	lla	a5,.L49
	add	a5,a4,a5
	lw	a5,0(a5)
	sext.w	a4,a5
	lla	a5,.L49
	add	a5,a4,a5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L49:
	.word	.L96-.L49
	.word	.L56-.L49
	.word	.L55-.L49
	.word	.L54-.L49
	.word	.L53-.L49
	.word	.L52-.L49
	.word	.L51-.L49
	.word	.L50-.L49
	.word	.L48-.L49
	.text
.L48:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x69AB7B5B
# 0 "" 2
#NO_APP
.L50:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x699AFADB
# 0 "" 2
#NO_APP
.L51:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x698A7A5B
# 0 "" 2
#NO_APP
.L52:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68C9F9DB
# 0 "" 2
#NO_APP
.L53:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68B9795B
# 0 "" 2
#NO_APP
.L54:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68A8F8DB
# 0 "" 2
#NO_APP
.L55:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6898785B
# 0 "" 2
#NO_APP
.L56:
#APP
# 138 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6887F7DB
# 0 "" 2
#NO_APP
.L96:
	lla	a5,.LC6
	flw	fa4,0(a5)
	fmv.s	fa3,fa4
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,8
	bgtu	a4,a5,.L97
	lwu	a5,-316(s0)
	slli	a4,a5,2
	lla	a5,.L59
	add	a5,a4,a5
	lw	a5,0(a5)
	sext.w	a4,a5
	lla	a5,.L59
	add	a5,a4,a5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L59:
	.word	.L97-.L59
	.word	.L66-.L59
	.word	.L65-.L59
	.word	.L64-.L59
	.word	.L63-.L59
	.word	.L62-.L59
	.word	.L61-.L59
	.word	.L60-.L59
	.word	.L58-.L59
	.text
.L58:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x69AB7B5B
# 0 "" 2
#NO_APP
.L60:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x699AFADB
# 0 "" 2
#NO_APP
.L61:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x698A7A5B
# 0 "" 2
#NO_APP
.L62:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68C9F9DB
# 0 "" 2
#NO_APP
.L63:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68B9795B
# 0 "" 2
#NO_APP
.L64:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68A8F8DB
# 0 "" 2
#NO_APP
.L65:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6898785B
# 0 "" 2
#NO_APP
.L66:
#APP
# 139 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6887F7DB
# 0 "" 2
#NO_APP
.L97:
	lla	a5,.LC7
	flw	fa4,0(a5)
	fmv.s	fa3,fa4
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,8
	bgtu	a4,a5,.L98
	lwu	a5,-316(s0)
	slli	a4,a5,2
	lla	a5,.L69
	add	a5,a4,a5
	lw	a5,0(a5)
	sext.w	a4,a5
	lla	a5,.L69
	add	a5,a4,a5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L69:
	.word	.L98-.L69
	.word	.L76-.L69
	.word	.L75-.L69
	.word	.L74-.L69
	.word	.L73-.L69
	.word	.L72-.L69
	.word	.L71-.L69
	.word	.L70-.L69
	.word	.L68-.L69
	.text
.L68:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x69AB7B5B
# 0 "" 2
#NO_APP
.L70:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x699AFADB
# 0 "" 2
#NO_APP
.L71:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x698A7A5B
# 0 "" 2
#NO_APP
.L72:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68C9F9DB
# 0 "" 2
#NO_APP
.L73:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68B9795B
# 0 "" 2
#NO_APP
.L74:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x68A8F8DB
# 0 "" 2
#NO_APP
.L75:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6898785B
# 0 "" 2
#NO_APP
.L76:
#APP
# 140 "benches/codes/macf_exp_microbench.c" 1
	.word 0x6887F7DB
# 0 "" 2
#NO_APP
.L98:
	lw	a5,-316(s0)
	sext.w	a5,a5
	ble	a5,zero,.L77
	fmv.x.s	a2,fa5
	lw	a3,-184(s0)
	ld	a5,-224(s0)
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L77:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,1
	ble	a4,a5,.L78
	fmv.x.s	a2,fa6
	lw	a3,-180(s0)
	ld	a5,-224(s0)
	addi	a5,a5,1
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L78:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,2
	ble	a4,a5,.L79
	fmv.x.s	a2,fa7
	lw	a3,-176(s0)
	ld	a5,-224(s0)
	addi	a5,a5,2
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L79:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,3
	ble	a4,a5,.L80
	fmv.x.s	a2,fs2
	lw	a3,-172(s0)
	ld	a5,-224(s0)
	addi	a5,a5,3
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L80:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,4
	ble	a4,a5,.L81
	fmv.x.s	a2,fs3
	lw	a3,-168(s0)
	ld	a5,-224(s0)
	addi	a5,a5,4
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L81:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,5
	ble	a4,a5,.L82
	fmv.x.s	a2,fs4
	lw	a3,-164(s0)
	ld	a5,-224(s0)
	addi	a5,a5,5
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L82:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,6
	ble	a4,a5,.L83
	fmv.x.s	a2,fs5
	lw	a3,-160(s0)
	ld	a5,-224(s0)
	addi	a5,a5,6
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L83:
	lw	a5,-316(s0)
	sext.w	a4,a5
	li	a5,7
	ble	a4,a5,.L84
	fmv.x.s	a2,fs6
	lw	a3,-156(s0)
	ld	a5,-224(s0)
	addi	a5,a5,7
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	fmv.s.x	fa0,a2
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
.L84:
	lw	a5,-316(s0)
	ld	a4,-224(s0)
	add	a5,a4,a5
	sd	a5,-224(s0)
.L4:
	lw	a4,-316(s0)
	ld	a5,-224(s0)
	add	a5,a4,a5
	ld	a4,-312(s0)
	bgeu	a4,a5,.L85
	j	.L86
.L91:
	ld	a5,-224(s0)
	slli	a5,a5,2
	ld	a4,-296(s0)
	add	a5,a4,a5
	flw	fa5,0(a5)
	fsw	fa5,-244(s0)
	addi	a5,s0,-280
	sd	a5,-200(s0)
	addi	a5,s0,-276
	sd	a5,-192(s0)
	lla	a5,.LC0
	flw	fa5,0(a5)
	flw	fa4,-244(s0)
	fmul.s	fa5,fa4,fa5
	fsw	fa5,-240(s0)
	flw	fa5,-240(s0)
	fsw	fa5,-236(s0)
	flw	fa5,-236(s0)
	fmv.s.x	fa4,zero
	fge.s	a5,fa5,fa4
	beq	a5,zero,.L101
	flw	fa4,-236(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fadd.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
	j	.L89
.L101:
	flw	fa4,-236(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fsub.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
.L89:
	sw	a5,-232(s0)
	lw	a5,-232(s0)
	fcvt.s.w	fa5,a5
	flw	fa4,-240(s0)
	fsub.s	fa5,fa4,fa5
	fsw	fa5,-228(s0)
	ld	a5,-200(s0)
	lw	a4,-232(s0)
	sw	a4,0(a5)
	ld	a5,-192(s0)
	flw	fa5,-228(s0)
	fsw	fa5,0(a5)
	nop
	lla	a5,.LC2
	flw	fa4,0(a5)
	flw	fa5,-276(s0)
	fmul.s	fa4,fa4,fa5
	lla	a5,.LC3
	flw	fa5,0(a5)
	fadd.s	fa4,fa4,fa5
	flw	fa5,-276(s0)
	fmul.s	fa4,fa4,fa5
	lla	a5,.LC4
	flw	fa5,0(a5)
	fadd.s	fa4,fa4,fa5
	flw	fa5,-276(s0)
	fmul.s	fa4,fa4,fa5
	lla	a5,.LC5
	flw	fa5,0(a5)
	fadd.s	fa4,fa4,fa5
	flw	fa5,-276(s0)
	fmul.s	fa4,fa4,fa5
	lla	a5,.LC6
	flw	fa5,0(a5)
	fadd.s	fa4,fa4,fa5
	flw	fa5,-276(s0)
	fmul.s	fa4,fa4,fa5
	lla	a5,.LC7
	flw	fa5,0(a5)
	fadd.s	fa5,fa4,fa5
	fsw	fa5,-268(s0)
	lw	a3,-280(s0)
	ld	a5,-224(s0)
	slli	a5,a5,2
	ld	a4,-304(s0)
	add	s1,a4,a5
	mv	a0,a3
	flw	fa0,-268(s0)
	call	scalbnf@plt
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
	ld	a5,-224(s0)
	addi	a5,a5,1
	sd	a5,-224(s0)
.L86:
	ld	a4,-224(s0)
	ld	a5,-312(s0)
	bltu	a4,a5,.L91
	nop
	la	a5,__stack_chk_guard
	ld	a4, -120(s0)
	ld	a5, 0(a5)
	xor	a5, a4, a5
	li	a4, 0
	beq	a5,zero,.L92
	call	__stack_chk_fail@plt
.L92:
	ld	ra,312(sp)
	.cfi_restore 1
	ld	s0,304(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 320
	ld	s1,296(sp)
	.cfi_restore 9
	fld	fs0,280(sp)
	.cfi_restore 40
	fld	fs1,272(sp)
	.cfi_restore 41
	fld	fs2,264(sp)
	.cfi_restore 50
	fld	fs3,256(sp)
	.cfi_restore 51
	fld	fs4,248(sp)
	.cfi_restore 52
	fld	fs5,240(sp)
	.cfi_restore 53
	fld	fs6,232(sp)
	.cfi_restore 54
	fld	fs8,224(sp)
	.cfi_restore 56
	fld	fs9,216(sp)
	.cfi_restore 57
	fld	fs10,208(sp)
	.cfi_restore 58
	addi	sp,sp,320
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE8:
	.size	bench_exp_horner_word_ilp, .-bench_exp_horner_word_ilp
	.align	1
	.type	lcg32f, @function
lcg32f:
.LFB9:
	.cfi_startproc
	addi	sp,sp,-48
	.cfi_def_cfa_offset 48
	sd	s0,40(sp)
	.cfi_offset 8, -8
	addi	s0,sp,48
	.cfi_def_cfa 8, 0
	sd	a0,-40(s0)
	ld	a5,-40(s0)
	lw	a4,0(a5)
	li	a5,1662976
	addiw	a5,a5,1549
	mulw	a5,a4,a5
	sext.w	a4,a5
	li	a5,1013903360
	addiw	a5,a5,863
	addw	a5,a4,a5
	sext.w	a4,a5
	ld	a5,-40(s0)
	sw	a4,0(a5)
	ld	a5,-40(s0)
	lw	a5,0(a5)
	fcvt.s.wu	fa4,a5
	lla	a5,.LC8
	flw	fa5,0(a5)
	fmul.s	fa5,fa4,fa5
	fsw	fa5,-20(s0)
	flw	fa4,-20(s0)
	lla	a5,.LC9
	flw	fa5,0(a5)
	fmul.s	fa4,fa4,fa5
	lla	a5,.LC10
	flw	fa5,0(a5)
	fsub.s	fa5,fa4,fa5
	fmv.s	fa0,fa5
	ld	s0,40(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 48
	addi	sp,sp,48
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE9:
	.size	lcg32f, .-lcg32f
	.section	.rodata
	.align	3
.LC11:
	.string	"Usage: %s <N_elems> [K_lanes (1..8, default 8)]\n"
	.align	3
.LC12:
	.string	"alloc failed\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	addi	sp,sp,-112
	.cfi_def_cfa_offset 112
	sd	ra,104(sp)
	sd	s0,96(sp)
	sd	s1,88(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	.cfi_offset 9, -24
	addi	s0,sp,112
	.cfi_def_cfa 8, 0
	mv	a5,a0
	sd	a1,-112(s0)
	sw	a5,-100(s0)
	la	a5,__stack_chk_guard
	ld	a4, 0(a5)
	sd	a4, -40(s0)
	li	a4, 0
	lw	a5,-100(s0)
	sext.w	a4,a5
	li	a5,1
	bgt	a4,a5,.L105
	la	a5,stderr
	ld	a4,0(a5)
	ld	a5,-112(s0)
	ld	a5,0(a5)
	mv	a2,a5
	lla	a1,.LC11
	mv	a0,a4
	call	fprintf@plt
	li	a5,1
	j	.L118
.L105:
	ld	a5,-112(s0)
	addi	a5,a5,8
	ld	a5,0(a5)
	li	a2,10
	li	a1,0
	mv	a0,a5
	call	strtoull@plt
	sd	a0,-64(s0)
	lw	a5,-100(s0)
	sext.w	a4,a5
	li	a5,2
	ble	a4,a5,.L107
	ld	a5,-112(s0)
	addi	a5,a5,16
	ld	a5,0(a5)
	mv	a0,a5
	call	atoi@plt
	mv	a5,a0
	j	.L108
.L107:
	li	a5,8
.L108:
	sw	a5,-84(s0)
	lw	a5,-84(s0)
	sext.w	a5,a5
	bgt	a5,zero,.L109
	li	a5,1
	sw	a5,-84(s0)
.L109:
	lw	a5,-84(s0)
	sext.w	a4,a5
	li	a5,8
	ble	a4,a5,.L110
	li	a5,8
	sw	a5,-84(s0)
.L110:
	ld	a5,-64(s0)
	slli	a5,a5,2
	mv	a1,a5
	li	a0,64
	call	aligned_alloc@plt
	mv	a5,a0
	sd	a5,-56(s0)
	ld	a5,-64(s0)
	slli	a5,a5,2
	mv	a1,a5
	li	a0,64
	call	aligned_alloc@plt
	mv	a5,a0
	sd	a5,-48(s0)
	ld	a5,-56(s0)
	beq	a5,zero,.L111
	ld	a5,-48(s0)
	bne	a5,zero,.L112
.L111:
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,13
	li	a1,1
	lla	a0,.LC12
	call	fwrite@plt
	li	a5,2
	j	.L118
.L112:
	li	a5,1
	sw	a5,-92(s0)
	sd	zero,-80(s0)
	j	.L113
.L114:
	ld	a5,-80(s0)
	slli	a5,a5,2
	ld	a4,-56(s0)
	add	s1,a4,a5
	addi	a5,s0,-92
	mv	a0,a5
	call	lcg32f
	fmv.s	fa5,fa0
	fsw	fa5,0(s1)
	ld	a5,-80(s0)
	addi	a5,a5,1
	sd	a5,-80(s0)
.L113:
	ld	a4,-80(s0)
	ld	a5,-64(s0)
	bltu	a4,a5,.L114
	lw	a5,-84(s0)
	mv	a3,a5
	ld	a2,-64(s0)
	ld	a1,-48(s0)
	ld	a0,-56(s0)
	call	bench_exp_horner_word_ilp
	fmv.s.x	fa5,zero
	fsw	fa5,-88(s0)
	sd	zero,-72(s0)
	j	.L115
.L117:
	ld	a5,-72(s0)
	slli	a5,a5,2
	ld	a4,-48(s0)
	add	a5,a4,a5
	flw	fa4,0(a5)
	lla	a5,.LC13
	flw	fa5,0(a5)
	fmul.s	fa4,fa4,fa5
	flw	fa5,-88(s0)
	fadd.s	fa5,fa4,fa5
	fsw	fa5,-88(s0)
	ld	a5,-72(s0)
	addi	a5,a5,1
	sd	a5,-72(s0)
.L115:
	ld	a5,-64(s0)
	li	a4,1024
	bleu	a5,a4,.L116
	li	a5,1024
.L116:
	ld	a4,-72(s0)
	bltu	a4,a5,.L117
	ld	a0,-56(s0)
	call	free@plt
	ld	a0,-48(s0)
	call	free@plt
	li	a5,0
.L118:
	mv	a4,a5
	la	a5,__stack_chk_guard
	ld	a3, -40(s0)
	ld	a5, 0(a5)
	xor	a5, a3, a5
	li	a3, 0
	beq	a5,zero,.L119
	call	__stack_chk_fail@plt
.L119:
	mv	a0,a4
	ld	ra,104(sp)
	.cfi_restore 1
	ld	s0,96(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 112
	ld	s1,88(sp)
	.cfi_restore 9
	addi	sp,sp,112
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.section	.rodata
	.align	2
.LC0:
	.word	1069066811
	.align	2
.LC1:
	.word	1056964608
	.align	2
.LC2:
	.word	984531967
	.align	2
.LC3:
	.word	1008571739
	.align	2
.LC4:
	.word	1029920839
	.align	2
.LC5:
	.word	1047920112
	.align	2
.LC6:
	.word	1060205080
	.align	2
.LC7:
	.word	1065353216
	.align	2
.LC8:
	.word	796917760
	.align	2
.LC9:
	.word	1101004800
	.align	2
.LC10:
	.word	1092616192
	.align	2
.LC13:
	.word	7136238
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
