	.file	"macf_exp_no-mac.c"
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
	.text
	.align	1
	.globl	bench_exp_horner_nofma
	.type	bench_exp_horner_nofma, @function
bench_exp_horner_nofma:
.LFB12:
	.cfi_startproc
	addi	sp,sp,-832
	.cfi_def_cfa_offset 832
	sd	ra,824(sp)
	sd	s0,816(sp)
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	addi	s0,sp,832
	.cfi_def_cfa 8, 0
	sd	a0,-808(s0)
	sd	a1,-816(s0)
	sd	a2,-824(s0)
	mv	a5,a3
	sw	a5,-828(s0)
	la	a5,__stack_chk_guard
	ld	a4, 0(a5)
	sd	a4, -24(s0)
	li	a4, 0
	lw	a5,-828(s0)
	sext.w	a5,a5
	bgt	a5,zero,.L2
	li	a5,1
	sw	a5,-828(s0)
.L2:
	lw	a5,-828(s0)
	sext.w	a4,a5
	li	a5,16
	ble	a4,a5,.L3
	li	a5,16
	sw	a5,-828(s0)
.L3:
	sd	zero,-256(s0)
	j	.L4
.L45:
	sw	zero,-780(s0)
	j	.L5
.L10:
	lw	a4,-780(s0)
	ld	a5,-256(s0)
	add	a5,a4,a5
	slli	a5,a5,2
	ld	a4,-808(s0)
	add	a5,a4,a5
	flw	fa5,0(a5)
	addi	a4,s0,-216
	lw	a5,-780(s0)
	slli	a5,a5,2
	add	a4,a4,a5
	addi	a3,s0,-152
	lw	a5,-780(s0)
	slli	a5,a5,2
	add	a5,a3,a5
	fsw	fa5,-744(s0)
	sd	a4,-248(s0)
	sd	a5,-240(s0)
	lla	a5,.LC0
	flw	fa5,0(a5)
	flw	fa4,-744(s0)
	fmul.s	fa5,fa4,fa5
	fsw	fa5,-740(s0)
	flw	fa5,-740(s0)
	fsw	fa5,-736(s0)
	flw	fa5,-736(s0)
	fmv.s.x	fa4,zero
	fge.s	a5,fa5,fa4
	beq	a5,zero,.L75
	flw	fa4,-736(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fadd.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
	j	.L8
.L75:
	flw	fa4,-736(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fsub.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
.L8:
	sw	a5,-732(s0)
	lw	a5,-732(s0)
	fcvt.s.w	fa5,a5
	flw	fa4,-740(s0)
	fsub.s	fa5,fa4,fa5
	fsw	fa5,-728(s0)
	ld	a5,-248(s0)
	lw	a4,-732(s0)
	sw	a4,0(a5)
	ld	a5,-240(s0)
	flw	fa5,-728(s0)
	fsw	fa5,0(a5)
	nop
	lw	a5,-780(s0)
	addiw	a5,a5,1
	sw	a5,-780(s0)
.L5:
	lw	a5,-780(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L10
	sw	zero,-776(s0)
	j	.L11
.L12:
	lla	a5,.LC2
	flw	fa5,0(a5)
	lw	a5,-776(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	fsw	fa5,-72(a5)
	lw	a5,-776(s0)
	addiw	a5,a5,1
	sw	a5,-776(s0)
.L11:
	lw	a5,-776(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L12
	sw	zero,-772(s0)
	j	.L13
.L17:
	lw	a5,-772(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa3,-72(a5)
	lw	a5,-772(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa4,-136(a5)
	lla	a5,.LC3
	flw	fa5,0(a5)
	fsw	fa3,-724(s0)
	fsw	fa4,-720(s0)
	fsw	fa5,-716(s0)
	flw	fa5,-724(s0)
	fsw	fa5,-712(s0)
	flw	fa5,-720(s0)
	fsw	fa5,-708(s0)
	flw	fa5,-712(s0)
	flw	fa4,-708(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-704(s0)
	flw	fa5,-704(s0)
	fsw	fa5,-700(s0)
	flw	fa5,-700(s0)
	fsw	fa5,-696(s0)
	flw	fa5,-716(s0)
	fsw	fa5,-692(s0)
	flw	fa5,-696(s0)
	flw	fa4,-692(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-688(s0)
	flw	fa5,-688(s0)
	nop
	lw	a5,-772(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	fsw	fa5,-72(a5)
	lw	a5,-772(s0)
	addiw	a5,a5,1
	sw	a5,-772(s0)
.L13:
	lw	a5,-772(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L17
	sw	zero,-768(s0)
	j	.L18
.L22:
	lw	a5,-768(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa3,-72(a5)
	lw	a5,-768(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa4,-136(a5)
	lla	a5,.LC4
	flw	fa5,0(a5)
	fsw	fa3,-684(s0)
	fsw	fa4,-680(s0)
	fsw	fa5,-676(s0)
	flw	fa5,-684(s0)
	fsw	fa5,-672(s0)
	flw	fa5,-680(s0)
	fsw	fa5,-668(s0)
	flw	fa5,-672(s0)
	flw	fa4,-668(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-664(s0)
	flw	fa5,-664(s0)
	fsw	fa5,-660(s0)
	flw	fa5,-660(s0)
	fsw	fa5,-656(s0)
	flw	fa5,-676(s0)
	fsw	fa5,-652(s0)
	flw	fa5,-656(s0)
	flw	fa4,-652(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-648(s0)
	flw	fa5,-648(s0)
	nop
	lw	a5,-768(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	fsw	fa5,-72(a5)
	lw	a5,-768(s0)
	addiw	a5,a5,1
	sw	a5,-768(s0)
.L18:
	lw	a5,-768(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L22
	sw	zero,-764(s0)
	j	.L23
.L27:
	lw	a5,-764(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa3,-72(a5)
	lw	a5,-764(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa4,-136(a5)
	lla	a5,.LC5
	flw	fa5,0(a5)
	fsw	fa3,-644(s0)
	fsw	fa4,-640(s0)
	fsw	fa5,-636(s0)
	flw	fa5,-644(s0)
	fsw	fa5,-632(s0)
	flw	fa5,-640(s0)
	fsw	fa5,-628(s0)
	flw	fa5,-632(s0)
	flw	fa4,-628(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-624(s0)
	flw	fa5,-624(s0)
	fsw	fa5,-620(s0)
	flw	fa5,-620(s0)
	fsw	fa5,-616(s0)
	flw	fa5,-636(s0)
	fsw	fa5,-612(s0)
	flw	fa5,-616(s0)
	flw	fa4,-612(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-608(s0)
	flw	fa5,-608(s0)
	nop
	lw	a5,-764(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	fsw	fa5,-72(a5)
	lw	a5,-764(s0)
	addiw	a5,a5,1
	sw	a5,-764(s0)
.L23:
	lw	a5,-764(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L27
	sw	zero,-760(s0)
	j	.L28
.L32:
	lw	a5,-760(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa3,-72(a5)
	lw	a5,-760(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa4,-136(a5)
	lla	a5,.LC6
	flw	fa5,0(a5)
	fsw	fa3,-604(s0)
	fsw	fa4,-600(s0)
	fsw	fa5,-596(s0)
	flw	fa5,-604(s0)
	fsw	fa5,-592(s0)
	flw	fa5,-600(s0)
	fsw	fa5,-588(s0)
	flw	fa5,-592(s0)
	flw	fa4,-588(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-584(s0)
	flw	fa5,-584(s0)
	fsw	fa5,-580(s0)
	flw	fa5,-580(s0)
	fsw	fa5,-576(s0)
	flw	fa5,-596(s0)
	fsw	fa5,-572(s0)
	flw	fa5,-576(s0)
	flw	fa4,-572(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-568(s0)
	flw	fa5,-568(s0)
	nop
	lw	a5,-760(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	fsw	fa5,-72(a5)
	lw	a5,-760(s0)
	addiw	a5,a5,1
	sw	a5,-760(s0)
.L28:
	lw	a5,-760(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L32
	sw	zero,-756(s0)
	j	.L33
.L37:
	lw	a5,-756(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa3,-72(a5)
	lw	a5,-756(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa4,-136(a5)
	lla	a5,.LC7
	flw	fa5,0(a5)
	fsw	fa3,-564(s0)
	fsw	fa4,-560(s0)
	fsw	fa5,-556(s0)
	flw	fa5,-564(s0)
	fsw	fa5,-552(s0)
	flw	fa5,-560(s0)
	fsw	fa5,-548(s0)
	flw	fa5,-552(s0)
	flw	fa4,-548(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-544(s0)
	flw	fa5,-544(s0)
	fsw	fa5,-540(s0)
	flw	fa5,-540(s0)
	fsw	fa5,-536(s0)
	flw	fa5,-556(s0)
	fsw	fa5,-532(s0)
	flw	fa5,-536(s0)
	flw	fa4,-532(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-528(s0)
	flw	fa5,-528(s0)
	nop
	lw	a5,-756(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	fsw	fa5,-72(a5)
	lw	a5,-756(s0)
	addiw	a5,a5,1
	sw	a5,-756(s0)
.L33:
	lw	a5,-756(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L37
	sw	zero,-752(s0)
	j	.L38
.L44:
	lw	a5,-752(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	flw	fa5,-72(a5)
	lw	a5,-752(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-200(a5)
	lw	a3,-752(s0)
	ld	a5,-256(s0)
	add	a5,a3,a5
	slli	a5,a5,2
	ld	a3,-816(s0)
	add	a5,a3,a5
	fsw	fa5,-524(s0)
	sw	a4,-520(s0)
	flw	fa5,-524(s0)
	fsw	fa5,-784(s0)
	lw	a4,-784(s0)
	mv	a3,a4
	li	a4,-2147483648
	and	a4,a3,a4
	sw	a4,-516(s0)
	lw	a4,-784(s0)
	srliw	a4,a4,23
	sext.w	a4,a4
	andi	a4,a4,255
	sw	a4,-512(s0)
	lw	a4,-784(s0)
	mv	a3,a4
	li	a4,8388608
	addi	a4,a4,-1
	and	a4,a3,a4
	sw	a4,-508(s0)
	lw	a4,-512(s0)
	sext.w	a4,a4
	bne	a4,zero,.L39
	lw	a4,-516(s0)
	sw	a4,-784(s0)
	flw	fa5,-784(s0)
	j	.L43
.L39:
	lw	a4,-512(s0)
	addiw	a4,a4,-127
	sext.w	a4,a4
	lw	a3,-520(s0)
	addw	a4,a3,a4
	sw	a4,-504(s0)
	lw	a4,-504(s0)
	sext.w	a4,a4
	bgt	a4,zero,.L41
	lw	a4,-516(s0)
	sw	a4,-784(s0)
	flw	fa5,-784(s0)
	j	.L43
.L41:
	lw	a4,-504(s0)
	sext.w	a3,a4
	li	a4,127
	ble	a3,a4,.L42
	lw	a4,-516(s0)
	mv	a3,a4
	li	a4,2139095040
	or	a4,a3,a4
	sext.w	a4,a4
	sw	a4,-784(s0)
	flw	fa5,-784(s0)
	j	.L43
.L42:
	lw	a4,-504(s0)
	addiw	a4,a4,127
	sext.w	a4,a4
	sext.w	a4,a4
	slliw	a4,a4,23
	sext.w	a4,a4
	lw	a3,-516(s0)
	or	a4,a3,a4
	sext.w	a4,a4
	lw	a3,-508(s0)
	or	a4,a3,a4
	sext.w	a4,a4
	sw	a4,-784(s0)
	flw	fa5,-784(s0)
.L43:
	fsw	fa5,0(a5)
	lw	a5,-752(s0)
	addiw	a5,a5,1
	sw	a5,-752(s0)
.L38:
	lw	a5,-752(s0)
	mv	a4,a5
	lw	a5,-828(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L44
	lw	a5,-828(s0)
	ld	a4,-256(s0)
	add	a5,a4,a5
	sd	a5,-256(s0)
.L4:
	lw	a4,-828(s0)
	ld	a5,-256(s0)
	add	a5,a4,a5
	ld	a4,-824(s0)
	bgeu	a4,a5,.L45
	j	.L46
.L71:
	ld	a5,-256(s0)
	slli	a5,a5,2
	ld	a4,-808(s0)
	add	a5,a4,a5
	flw	fa5,0(a5)
	fsw	fa5,-276(s0)
	addi	a5,s0,-788
	sd	a5,-232(s0)
	addi	a5,s0,-784
	sd	a5,-224(s0)
	lla	a5,.LC0
	flw	fa5,0(a5)
	flw	fa4,-276(s0)
	fmul.s	fa5,fa4,fa5
	fsw	fa5,-272(s0)
	flw	fa5,-272(s0)
	fsw	fa5,-268(s0)
	flw	fa5,-268(s0)
	fmv.s.x	fa4,zero
	fge.s	a5,fa5,fa4
	beq	a5,zero,.L76
	flw	fa4,-268(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fadd.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
	j	.L49
.L76:
	flw	fa4,-268(s0)
	lla	a5,.LC1
	flw	fa5,0(a5)
	fsub.s	fa5,fa4,fa5
	fcvt.w.s a5,fa5,rtz
	sext.w	a5,a5
.L49:
	sw	a5,-264(s0)
	lw	a5,-264(s0)
	fcvt.s.w	fa5,a5
	flw	fa4,-272(s0)
	fsub.s	fa5,fa4,fa5
	fsw	fa5,-260(s0)
	ld	a5,-232(s0)
	lw	a4,-264(s0)
	sw	a4,0(a5)
	ld	a5,-224(s0)
	flw	fa5,-260(s0)
	fsw	fa5,0(a5)
	nop
	lla	a5,.LC2
	flw	fa5,0(a5)
	fsw	fa5,-748(s0)
	flw	fa4,-784(s0)
	lla	a5,.LC3
	flw	fa5,0(a5)
	flw	fa3,-748(s0)
	fsw	fa3,-316(s0)
	fsw	fa4,-312(s0)
	fsw	fa5,-308(s0)
	flw	fa5,-316(s0)
	fsw	fa5,-304(s0)
	flw	fa5,-312(s0)
	fsw	fa5,-300(s0)
	flw	fa5,-304(s0)
	flw	fa4,-300(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-296(s0)
	flw	fa5,-296(s0)
	fsw	fa5,-292(s0)
	flw	fa5,-292(s0)
	fsw	fa5,-288(s0)
	flw	fa5,-308(s0)
	fsw	fa5,-284(s0)
	flw	fa5,-288(s0)
	flw	fa4,-284(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-280(s0)
	flw	fa5,-280(s0)
	nop
	fsw	fa5,-748(s0)
	flw	fa4,-784(s0)
	lla	a5,.LC4
	flw	fa5,0(a5)
	flw	fa3,-748(s0)
	fsw	fa3,-356(s0)
	fsw	fa4,-352(s0)
	fsw	fa5,-348(s0)
	flw	fa5,-356(s0)
	fsw	fa5,-344(s0)
	flw	fa5,-352(s0)
	fsw	fa5,-340(s0)
	flw	fa5,-344(s0)
	flw	fa4,-340(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-336(s0)
	flw	fa5,-336(s0)
	fsw	fa5,-332(s0)
	flw	fa5,-332(s0)
	fsw	fa5,-328(s0)
	flw	fa5,-348(s0)
	fsw	fa5,-324(s0)
	flw	fa5,-328(s0)
	flw	fa4,-324(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-320(s0)
	flw	fa5,-320(s0)
	nop
	fsw	fa5,-748(s0)
	flw	fa4,-784(s0)
	lla	a5,.LC5
	flw	fa5,0(a5)
	flw	fa3,-748(s0)
	fsw	fa3,-396(s0)
	fsw	fa4,-392(s0)
	fsw	fa5,-388(s0)
	flw	fa5,-396(s0)
	fsw	fa5,-384(s0)
	flw	fa5,-392(s0)
	fsw	fa5,-380(s0)
	flw	fa5,-384(s0)
	flw	fa4,-380(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-376(s0)
	flw	fa5,-376(s0)
	fsw	fa5,-372(s0)
	flw	fa5,-372(s0)
	fsw	fa5,-368(s0)
	flw	fa5,-388(s0)
	fsw	fa5,-364(s0)
	flw	fa5,-368(s0)
	flw	fa4,-364(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-360(s0)
	flw	fa5,-360(s0)
	nop
	fsw	fa5,-748(s0)
	flw	fa4,-784(s0)
	lla	a5,.LC6
	flw	fa5,0(a5)
	flw	fa3,-748(s0)
	fsw	fa3,-436(s0)
	fsw	fa4,-432(s0)
	fsw	fa5,-428(s0)
	flw	fa5,-436(s0)
	fsw	fa5,-424(s0)
	flw	fa5,-432(s0)
	fsw	fa5,-420(s0)
	flw	fa5,-424(s0)
	flw	fa4,-420(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-416(s0)
	flw	fa5,-416(s0)
	fsw	fa5,-412(s0)
	flw	fa5,-412(s0)
	fsw	fa5,-408(s0)
	flw	fa5,-428(s0)
	fsw	fa5,-404(s0)
	flw	fa5,-408(s0)
	flw	fa4,-404(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-400(s0)
	flw	fa5,-400(s0)
	nop
	fsw	fa5,-748(s0)
	flw	fa4,-784(s0)
	lla	a5,.LC7
	flw	fa5,0(a5)
	flw	fa3,-748(s0)
	fsw	fa3,-476(s0)
	fsw	fa4,-472(s0)
	fsw	fa5,-468(s0)
	flw	fa5,-476(s0)
	fsw	fa5,-464(s0)
	flw	fa5,-472(s0)
	fsw	fa5,-460(s0)
	flw	fa5,-464(s0)
	flw	fa4,-460(s0)
#APP
# 74 "benches/codes/macf_exp_no-mac.c" 1
	fmul.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-456(s0)
	flw	fa5,-456(s0)
	fsw	fa5,-452(s0)
	flw	fa5,-452(s0)
	fsw	fa5,-448(s0)
	flw	fa5,-468(s0)
	fsw	fa5,-444(s0)
	flw	fa5,-448(s0)
	flw	fa4,-444(s0)
#APP
# 80 "benches/codes/macf_exp_no-mac.c" 1
	fadd.s fa5, fa5, fa4
# 0 "" 2
#NO_APP
	fsw	fa5,-440(s0)
	flw	fa5,-440(s0)
	nop
	fsw	fa5,-748(s0)
	lw	a4,-788(s0)
	ld	a5,-256(s0)
	slli	a5,a5,2
	ld	a3,-816(s0)
	add	a5,a3,a5
	flw	fa5,-748(s0)
	fsw	fa5,-500(s0)
	sw	a4,-496(s0)
	flw	fa5,-500(s0)
	fsw	fa5,-792(s0)
	lw	a4,-792(s0)
	mv	a3,a4
	li	a4,-2147483648
	and	a4,a3,a4
	sw	a4,-492(s0)
	lw	a4,-792(s0)
	srliw	a4,a4,23
	sext.w	a4,a4
	andi	a4,a4,255
	sw	a4,-488(s0)
	lw	a4,-792(s0)
	mv	a3,a4
	li	a4,8388608
	addi	a4,a4,-1
	and	a4,a3,a4
	sw	a4,-484(s0)
	lw	a4,-488(s0)
	sext.w	a4,a4
	bne	a4,zero,.L66
	lw	a4,-492(s0)
	sw	a4,-792(s0)
	flw	fa5,-792(s0)
	j	.L70
.L66:
	lw	a4,-488(s0)
	addiw	a4,a4,-127
	sext.w	a4,a4
	lw	a3,-496(s0)
	addw	a4,a3,a4
	sw	a4,-480(s0)
	lw	a4,-480(s0)
	sext.w	a4,a4
	bgt	a4,zero,.L68
	lw	a4,-492(s0)
	sw	a4,-792(s0)
	flw	fa5,-792(s0)
	j	.L70
.L68:
	lw	a4,-480(s0)
	sext.w	a3,a4
	li	a4,127
	ble	a3,a4,.L69
	lw	a4,-492(s0)
	mv	a3,a4
	li	a4,2139095040
	or	a4,a3,a4
	sext.w	a4,a4
	sw	a4,-792(s0)
	flw	fa5,-792(s0)
	j	.L70
.L69:
	lw	a4,-480(s0)
	addiw	a4,a4,127
	sext.w	a4,a4
	sext.w	a4,a4
	slliw	a4,a4,23
	sext.w	a4,a4
	lw	a3,-492(s0)
	or	a4,a3,a4
	sext.w	a4,a4
	lw	a3,-484(s0)
	or	a4,a3,a4
	sext.w	a4,a4
	sw	a4,-792(s0)
	flw	fa5,-792(s0)
.L70:
	fsw	fa5,0(a5)
	ld	a5,-256(s0)
	addi	a5,a5,1
	sd	a5,-256(s0)
.L46:
	ld	a4,-256(s0)
	ld	a5,-824(s0)
	bltu	a4,a5,.L71
	nop
	la	a5,__stack_chk_guard
	ld	a4, -24(s0)
	ld	a5, 0(a5)
	xor	a5, a4, a5
	li	a4, 0
	beq	a5,zero,.L72
	call	__stack_chk_fail@plt
.L72:
	ld	ra,824(sp)
	.cfi_restore 1
	ld	s0,816(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 832
	addi	sp,sp,832
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE12:
	.size	bench_exp_horner_nofma, .-bench_exp_horner_nofma
	.align	1
	.type	lcg32f, @function
lcg32f:
.LFB13:
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
.LFE13:
	.size	lcg32f, .-lcg32f
	.section	.rodata
	.align	3
.LC11:
	.string	"Usage: %s <N_elems> [K_lanes (default 8)]\n"
	.align	3
.LC12:
	.string	"alloc failed\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB14:
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
	bgt	a4,a5,.L80
	la	a5,stderr
	ld	a4,0(a5)
	ld	a5,-112(s0)
	ld	a5,0(a5)
	mv	a2,a5
	lla	a1,.LC11
	mv	a0,a4
	call	fprintf@plt
	li	a5,1
	j	.L93
.L80:
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
	ble	a4,a5,.L82
	ld	a5,-112(s0)
	addi	a5,a5,16
	ld	a5,0(a5)
	mv	a0,a5
	call	atoi@plt
	mv	a5,a0
	j	.L83
.L82:
	li	a5,8
.L83:
	sw	a5,-84(s0)
	lw	a5,-84(s0)
	sext.w	a5,a5
	bgt	a5,zero,.L84
	li	a5,1
	sw	a5,-84(s0)
.L84:
	lw	a5,-84(s0)
	sext.w	a4,a5
	li	a5,16
	ble	a4,a5,.L85
	li	a5,16
	sw	a5,-84(s0)
.L85:
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
	beq	a5,zero,.L86
	ld	a5,-48(s0)
	bne	a5,zero,.L87
.L86:
	la	a5,stderr
	ld	a5,0(a5)
	mv	a3,a5
	li	a2,13
	li	a1,1
	lla	a0,.LC12
	call	fwrite@plt
	li	a5,2
	j	.L93
.L87:
	li	a5,1
	sw	a5,-92(s0)
	sd	zero,-80(s0)
	j	.L88
.L89:
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
.L88:
	ld	a4,-80(s0)
	ld	a5,-64(s0)
	bltu	a4,a5,.L89
	lw	a5,-84(s0)
	mv	a3,a5
	ld	a2,-64(s0)
	ld	a1,-48(s0)
	ld	a0,-56(s0)
	call	bench_exp_horner_nofma
	fmv.s.x	fa5,zero
	fsw	fa5,-88(s0)
	sd	zero,-72(s0)
	j	.L90
.L92:
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
.L90:
	ld	a5,-64(s0)
	li	a4,1024
	bleu	a5,a4,.L91
	li	a5,1024
.L91:
	ld	a4,-72(s0)
	bltu	a4,a5,.L92
	ld	a0,-56(s0)
	call	free@plt
	ld	a0,-48(s0)
	call	free@plt
	li	a5,0
.L93:
	mv	a4,a5
	la	a5,__stack_chk_guard
	ld	a3, -40(s0)
	ld	a5, 0(a5)
	xor	a5, a3, a5
	li	a3, 0
	beq	a5,zero,.L94
	call	__stack_chk_fail@plt
.L94:
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
.LFE14:
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
