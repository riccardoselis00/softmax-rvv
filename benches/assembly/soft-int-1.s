	.file	"soft-int-1.c"
	.option pic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	__udivti3
	.globl	__divti3
	.align	1
	.globl	int_softmax_mac_ilp
	.type	int_softmax_mac_ilp, @function
int_softmax_mac_ilp:
.LFB41:
	.cfi_startproc
	addi	sp,sp,-288
	.cfi_def_cfa_offset 288
	sd	ra,280(sp)
	sd	a3,88(sp)
	sd	a4,96(sp)
	.cfi_offset 1, -8
	ble	a1,zero,.L53
	sd	s2,256(sp)
	addiw	a5,a2,-1
	li	a4,29
	.cfi_offset 18, -32
	mv	s2,a2
	bgtu	a5,a4,.L54
	sd	s9,200(sp)
	sd	s11,184(sp)
	fsd	fs0,168(sp)
	.cfi_offset 25, -88
	.cfi_offset 27, -104
	.cfi_offset 40, -120
	mv	s9,a1
	sd	s0,272(sp)
	sd	s3,248(sp)
	li	a1,1
	fmv.d	fs0,fa0
	lw	s11,0(a0)
	mv	a7,a0
	.cfi_offset 8, -16
	.cfi_offset 19, -40
	beq	s9,a1,.L55
	addiw	a3,s9,-2
	slli	ra,a3,32
	addi	t1,a0,8
	srli	t0,ra,30
	addi	a5,a0,4
	add	a2,t0,t1
	sub	t2,a2,a5
	addi	s0,t2,-4
	srli	a0,s0,2
	addi	a6,a0,1
	andi	s3,a6,7
	mv	s0,s11
	beq	s3,zero,.L5
	beq	s3,a1,.L177
	li	t3,2
	beq	s3,t3,.L178
	li	t4,3
	beq	s3,t4,.L179
	li	t5,4
	beq	s3,t5,.L180
	li	t6,5
	beq	s3,t6,.L181
	li	a4,6
	beq	s3,a4,.L182
	lw	a1,0(a5)
	sext.w	s0,a1
	bge	a1,s11,.L142
	sext.w	s0,s11
.L142:
	addi	a5,a5,4
.L182:
	lw	ra,0(a5)
	mv	a3,ra
	blt	ra,s0,.L234
.L145:
	sext.w	s0,a3
	addi	a5,a5,4
.L181:
	lw	t0,0(a5)
	mv	t1,t0
	blt	t0,s0,.L235
.L148:
	sext.w	s0,t1
	addi	a5,a5,4
.L180:
	lw	t2,0(a5)
	mv	a0,t2
	bge	t2,s0,.L151
	mv	a0,s0
.L151:
	sext.w	s0,a0
	addi	a5,a5,4
.L179:
	lw	a6,0(a5)
	mv	s3,a6
	blt	a6,s0,.L236
.L154:
	sext.w	s0,s3
	addi	a5,a5,4
.L178:
	lw	t3,0(a5)
	mv	t4,t3
	blt	t3,s0,.L237
.L157:
	sext.w	s0,t4
	addi	a5,a5,4
.L177:
	lw	t5,0(a5)
	mv	t6,t5
	blt	t5,s0,.L238
	addi	a5,a5,4
	sext.w	s0,t6
	beq	a2,a5,.L3
.L5:
	lw	a1,0(a5)
	sext.w	a4,a1
	blt	a1,s0,.L239
	lw	s0,4(a5)
	addi	a5,a5,4
	sext.w	ra,s0
	blt	s0,a4,.L240
.L163:
	lw	a3,4(a5)
	sext.w	t0,a3
	blt	a3,ra,.L241
.L165:
	lw	t1,8(a5)
	sext.w	t2,t1
	blt	t1,t0,.L242
.L167:
	lw	a0,12(a5)
	sext.w	a6,a0
	blt	a0,t2,.L243
.L169:
	lw	s3,16(a5)
	sext.w	t3,s3
	blt	s3,a6,.L244
.L171:
	lw	t4,20(a5)
	sext.w	t5,t4
	blt	t4,t3,.L245
.L173:
	lw	t6,24(a5)
	sext.w	s0,t6
	blt	t6,t5,.L246
.L175:
	addi	a5,a5,28
	bne	a2,a5,.L5
.L3:
	slli	s3,s9,2
	mv	a0,s3
	sd	a7,0(sp)
	call	malloc@plt
	sd	a0,80(sp)
	mv	a3,a0
	beq	a0,zero,.L56
	sd	s1,264(sp)
	.cfi_offset 9, -24
	addi	s1,s3,-4
	sd	s4,240(sp)
	sd	s7,216(sp)
	.cfi_offset 20, -48
	.cfi_offset 23, -72
	srli	s4,s1,2
	ld	s7,0(sp)
	sd	s5,232(sp)
	.cfi_offset 21, -56
	addi	s5,s4,1
	sd	s6,224(sp)
	sd	s8,208(sp)
	sd	s10,192(sp)
	.cfi_offset 22, -64
	.cfi_offset 24, -80
	.cfi_offset 26, -96
	andi	s6,s5,7
	mv	a4,s7
	mv	s1,a0
	add	s8,s3,s7
	beq	s6,zero,.L6
	li	s10,1
	beq	s6,s10,.L183
	li	a7,2
	beq	s6,a7,.L184
	li	a2,3
	beq	s6,a2,.L185
	li	a1,4
	beq	s6,a1,.L186
	li	ra,5
	beq	s6,ra,.L187
	li	t0,6
	bne	s6,t0,.L247
.L188:
	lw	a6,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	t3,a6,s0
	sw	t3,-4(a3)
.L187:
	lw	t4,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	t5,t4,s0
	sw	t5,-4(a3)
.L186:
	lw	t6,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	a5,t6,s0
	sw	a5,-4(a3)
.L185:
	lw	s3,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	s4,s3,s0
	sw	s4,-4(a3)
.L184:
	lw	s5,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	s6,s5,s0
	sw	s6,-4(a3)
.L183:
	lw	s10,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	a7,s10,s0
	sw	a7,-4(a3)
	beq	s8,a4,.L217
.L6:
	lw	ra,0(a4)
	lw	t0,4(a4)
	lw	t1,8(a4)
	lw	t2,12(a4)
	lw	a0,16(a4)
	lw	a1,20(a4)
	lw	a2,24(a4)
	lw	t5,28(a4)
	subw	t4,ra,s0
	subw	t3,t0,s0
	subw	t6,t1,s0
	subw	a6,t2,s0
	subw	s3,a0,s0
	subw	s4,a1,s0
	subw	s5,a2,s0
	subw	a5,t5,s0
	sw	t4,0(a3)
	sw	t3,4(a3)
	sw	t6,8(a3)
	sw	a6,12(a3)
	sw	s3,16(a3)
	sw	s4,20(a3)
	sw	s5,24(a3)
	sw	a5,28(a3)
	addi	a4,a4,32
	addi	a3,a3,32
	bne	s8,a4,.L6
.L217:
	fld	fa5,.LC0,a5
	fdiv.d	ft0,fa5,fs0
	fcvt.l.d s4,ft0,rtz
	bgt	s4,zero,.L7
	li	s4,1
.L7:
	slliw	s0,s2,1
	addiw	s8,s0,-64
	sd	s0,48(sp)
	slli	s6,s8,32
	blt	s6,zero,.L8
	li	s10,1
	sll	a1,s10,s8
	li	a0,0
.L9:
	mv	a2,s4
	srai	a3,s4,63
	sd	s7,0(sp)
	call	__divti3@plt
	or	s7,a0,a1
	ld	a7,0(sp)
	mv	s0,a0
	mv	s5,a1
	beq	s7,zero,.L248
.L10:
	slli	ra,s9,3
	mv	a0,ra
	sd	a7,8(sp)
	sd	ra,0(sp)
	call	malloc@plt
	ld	a2,0(sp)
	ld	a7,8(sp)
	mv	s10,a0
	beq	a0,zero,.L249
	sraiw	t0,s9,2
	slliw	t1,t0,2
	srai	t4,s9,2
	sd	t1,64(sp)
	mv	t2,t1
	beq	t4,zero,.L58
	ld	s2,48(sp)
	lla	t5,.LC3
	li	t4,44740608
	addi	t6,t4,-1365
	ld	a6,0(t5)
	li	t3,1118208
	addi	s3,t3,273
	slli	a4,t6,12
	li	s7,1
	li	a3,63
	addiw	a5,s2,-64
	li	s8,1
	slli	s6,s3,13
	subw	t0,a3,s2
	addi	t1,a4,-1366
	slli	t4,s7,39
	sw	a5,36(sp)
	mv	ra,a0
	sd	a5,8(sp)
	sd	a6,72(sp)
	sd	zero,56(sp)
	sd	zero,0(sp)
	sw	t0,144(sp)
	li	a5,0
	slli	a1,s8,40
	sd	t1,16(sp)
	addi	a0,s6,546
	sd	t4,24(sp)
	sd	a2,104(sp)
	sd	s11,112(sp)
	sd	s10,120(sp)
	sd	a7,128(sp)
	sd	s9,136(sp)
	sw	t2,148(sp)
	sd	t5,152(sp)
	j	.L32
.L251:
	ld	s9,56(sp)
	not	s7,s8
	srai	t6,s7,63
	sd	s6,0(ra)
	and	s3,s8,t6
	ld	s6,64(sp)
	srl	t2,t2,s3
	addiw	t4,s9,4
	sd	t3,8(ra)
	sd	t1,16(ra)
	sd	t2,24(ra)
	sd	t4,56(sp)
	addi	s1,s1,16
	addi	ra,ra,32
	ble	s6,t4,.L250
.L32:
	lw	s10,0(s1)
	ld	s2,8(sp)
	lw	t6,4(s1)
	srai	s11,s10,63
	mul	s9,s11,s0
	lw	a7,8(s1)
	lw	a6,12(s1)
	mul	a2,s5,s10
	mulhu	t2,s10,s0
	add	t5,s9,a2
	mul	t3,s10,s0
	add	s3,t5,t2
	blt	s2,zero,.L14
	lw	s8,36(sp)
	sra	t4,s3,s8
.L15:
	srai	t1,t6,63
	ld	s3,8(sp)
	mul	s11,t1,s0
	mul	s9,s5,t6
	mulhu	t2,t6,s0
	add	a2,s11,s9
	mul	t5,t6,s0
	add	t3,a2,t2
	blt	s3,zero,.L16
	lw	s2,36(sp)
	sra	a2,t3,s2
.L17:
	srai	t0,a7,63
	ld	s3,8(sp)
	mul	s11,t0,s0
	mul	t1,s5,a7
	mulhu	s9,a7,s0
	add	t2,s11,t1
	mul	t5,a7,s0
	add	t3,t2,s9
	blt	s3,zero,.L18
	lw	s2,36(sp)
	sra	a3,t3,s2
.L19:
	srai	t0,a6,63
	ld	s3,8(sp)
	mul	s11,t0,s0
	mul	s9,s5,a6
	mulhu	t1,a6,s0
	add	t2,s11,s9
	mul	t5,a6,s0
	add	t3,t2,t1
	blt	s3,zero,.L20
	lw	s2,36(sp)
	sra	s8,t3,s2
	sd	s8,40(sp)
.L21:
	ld	a4,72(sp)
	ld	s8,16(sp)
	mul	t5,s4,t4
	mv	t3,a5
	mv	t1,a5
	mv	s6,a5
	mv	s11,a5
	mv	t0,a5
	mv	s9,a5
	mv	s3,a5
	mv	s2,a5
	mv	t2,a5
	mul	s7,s4,a2
	sub	s10,s10,t5
	srai	t5,s10,8
	ld	s10,24(sp)
#APP
# 56 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t3, t5, s8
# 0 "" 2
# 61 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s6, t5, a0
# 0 "" 2
#NO_APP
	add	s6,s6,a4
#APP
# 44 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s11, t5, t5
# 0 "" 2
# 66 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t0, s11, s6
# 0 "" 2
#NO_APP
	add	t3,t3,s10
	sub	t6,t6,s7
	srai	s7,t6,8
#APP
# 56 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t1, s7, s8
# 0 "" 2
#NO_APP
	ld	s8,40(sp)
	mul	s6,s4,a3
	add	t1,t1,s10
	srli	t0,t0,40
	add	t3,t3,t0
#APP
# 44 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s9, s7, s7
# 0 "" 2
# 61 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s3, s7, a0
# 0 "" 2
#NO_APP
	add	s3,s3,a4
#APP
# 66 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s2, s9, s3
# 0 "" 2
#NO_APP
	srli	s2,s2,40
	add	t1,t1,s2
	mul	s10,s4,s8
	mv	s8,a5
#APP
# 74 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s8, s11, t3
# 0 "" 2
#NO_APP
	srli	s11,s8,40
	ld	s8,24(sp)
	sub	a7,a7,s6
	srai	a7,a7,8
	ld	t3,16(sp)
	mv	t0,a5
	mv	s6,a5
#APP
# 51 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t0, s7, a1
# 0 "" 2
# 44 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s6, a7, a7
# 0 "" 2
#NO_APP
	add	t6,t0,a1
	sub	a6,a6,s10
	mv	s7,a5
	mv	s3,a5
#APP
# 74 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s7, s9, t1
# 0 "" 2
# 56 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s3, a7, t3
# 0 "" 2
#NO_APP
	srli	s9,s7,40
	add	t1,s3,s8
	mv	s10,a5
	mv	s2,a5
#APP
# 61 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s10, a7, a0
# 0 "" 2
#NO_APP
	add	s7,s10,a4
#APP
# 66 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s2, s6, s7
# 0 "" 2
#NO_APP
	srli	s3,s2,40
	add	s7,t6,s9
	add	s9,t1,s3
	mv	s10,a5
#APP
# 51 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s10, a7, a1
# 0 "" 2
#NO_APP
	mv	a7,a5
#APP
# 74 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, a7, s6, s9
# 0 "" 2
#NO_APP
	srli	s9,a7,40
	ld	a7,24(sp)
	ld	s6,16(sp)
#APP
# 51 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t2, t5, a1
# 0 "" 2
#NO_APP
	add	t5,t2,a1
	srai	a6,a6,8
	add	s8,t5,s11
	add	t1,s10,a1
	mv	s11,a5
	mv	s2,a5
#APP
# 44 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s11, a6, a6
# 0 "" 2
# 56 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s2, a6, s6
# 0 "" 2
#NO_APP
	mv	s3,a5
	add	s2,s2,a7
#APP
# 61 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s3, a6, a0
# 0 "" 2
#NO_APP
	mv	t3,a5
	add	a4,s3,a4
#APP
# 66 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t3, s11, a4
# 0 "" 2
#NO_APP
	srli	t3,t3,40
	add	s6,t1,s9
	sltu	t2,t5,t2
	add	s9,s2,t3
	sltu	t5,s8,t5
	sltu	t0,t6,t0
	mv	s3,a5
	sltu	t6,s7,t6
#APP
# 51 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, s3, a6, a1
# 0 "" 2
#NO_APP
	mv	a6,a5
	add	a7,s3,a1
#APP
# 74 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, a6, s11, s9
# 0 "" 2
#NO_APP
	srli	s11,a6,40
	add	a4,a7,s11
	add	s2,t5,t2
	add	t3,t6,t0
	sltu	s10,t1,s10
	sltu	t1,s6,t1
	add	t6,t1,s10
	slli	t2,s2,24
	slli	t0,t3,24
	sltu	s9,a4,a7
	srli	s8,s8,40
	srli	s7,s7,40
	sltu	s3,a7,s3
	or	t5,t2,s8
	or	s11,t0,s7
	slli	a6,t6,24
	srli	s6,s6,40
	add	s2,s9,s3
	ld	t0,0(sp)
	or	a7,a6,s6
	slli	t3,s2,24
	srli	a4,a4,40
	xor	s10,s11,t5
	or	t2,t3,a4
	xor	t1,s10,a7
	xor	s8,t1,t2
	xor	s7,t0,s8
	sd	s7,0(sp)
	li	t6,62
	li	s6,0
	bgt	t4,t6,.L22
	not	s3,t4
	srai	s9,s3,63
	and	t4,t4,s9
	srl	s6,t5,t4
.L22:
	li	t5,62
	li	t3,0
	bgt	a2,t5,.L24
	not	a6,a2
	srai	s2,a6,63
	and	a2,a2,s2
	srl	t3,s11,a2
.L24:
	li	s11,62
	li	t1,0
	bgt	a3,s11,.L26
	not	a4,a3
	srai	s10,a4,63
	and	a3,a3,s10
	srl	t1,a7,a3
.L26:
	ld	s8,40(sp)
	li	a7,62
	ble	s8,a7,.L251
	ld	s9,56(sp)
	sd	s6,0(ra)
	ld	s6,64(sp)
	addiw	t4,s9,4
	sd	t3,8(ra)
	sd	t1,16(ra)
	sd	zero,24(ra)
	sd	t4,56(sp)
	addi	s1,s1,16
	addi	ra,ra,32
	bgt	s6,t4,.L32
.L250:
	li	a5,4
	ld	a2,104(sp)
	ld	s11,112(sp)
	ld	s10,120(sp)
	ld	a7,128(sp)
	ld	s9,136(sp)
	lw	a0,148(sp)
	ld	a1,152(sp)
	blt	s6,a5,.L252
.L33:
	sext.w	t4,a0
	bge	t4,s9,.L34
.L13:
	li	t5,44740608
	li	a6,1118208
	addi	s2,t5,-1365
	ld	t0,80(sp)
	addi	t3,a6,273
	ld	s1,48(sp)
	slli	t1,s2,12
	slli	t2,t3,13
	lw	s7,36(sp)
	li	s6,1
	ld	s8,0(a1)
	slli	a4,t4,3
	slli	a1,t4,2
	li	a3,1
	li	s3,63
	addi	a5,t1,-1366
	addi	ra,t2,546
	add	t5,t0,a1
	slli	s2,s6,39
	sd	a2,8(sp)
	sd	a7,16(sp)
	add	t6,s10,a4
	subw	s3,s3,s1
	li	a6,0
	slli	t0,a3,40
	li	s6,62
	mv	a2,a5
	mv	a7,ra
.L41:
	lw	a5,0(t5)
	srai	a0,a5,63
	mul	t3,a0,s0
	mul	a1,s5,a5
	mulhu	a3,a5,s0
	add	a4,t3,a1
	add	t1,a4,a3
	slli	t2,t1,1
	mul	ra,a5,s0
	sll	s1,t2,s3
	blt	s7,zero,.L35
	lw	a0,36(sp)
	sra	a4,t1,a0
.L36:
	mv	ra,a6
	mv	a1,a6
	mv	t2,a6
	mul	s1,s4,a4
	mv	t3,a6
	mv	t1,a6
	mv	a3,a6
	not	a0,a4
	srai	a0,a0,63
	and	a0,a4,a0
	sub	a5,a5,s1
	srai	s1,a5,8
#APP
# 44 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, ra, s1, s1
# 0 "" 2
# 56 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, a1, s1, a2
# 0 "" 2
# 61 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t2, s1, a7
# 0 "" 2
#NO_APP
	add	a1,a1,s2
	add	t2,t2,s8
#APP
# 66 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t3, ra, t2
# 0 "" 2
#NO_APP
	srli	t3,t3,40
#APP
# 51 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, t1, s1, t0
# 0 "" 2
#NO_APP
	add	s1,a1,t3
	add	a5,t1,t0
#APP
# 74 "benches/code/soft-int-1.c" 1
	.insn r 0x5B, 0x02, 0x00, a3, ra, s1
# 0 "" 2
#NO_APP
	srli	ra,a3,40
	add	a3,a5,ra
	sltu	a1,a3,a5
	sltu	t1,a5,t1
	ld	ra,0(sp)
	add	t2,a1,t1
	srli	s1,a3,40
	slli	t3,t2,24
	or	a5,t3,s1
	xor	a3,ra,a5
	sd	a3,0(sp)
	srl	a0,a5,a0
	bgt	a4,s6,.L37
	sd	a0,0(t6)
	addiw	t4,t4,1
	addi	t5,t5,4
	addi	t6,t6,8
	blt	t4,s9,.L41
.L229:
	ld	a2,8(sp)
	ld	a7,16(sp)
.L34:
	addi	s4,a2,-8
	srli	s0,s4,3
	addi	s5,s0,1
	andi	s8,s5,3
	mv	s4,s10
	add	s5,a2,s10
	mv	a1,s10
	li	a5,0
	li	ra,0
	beq	s8,zero,.L42
	li	s7,1
	beq	s8,s7,.L189
	li	s3,2
	beq	s8,s3,.L190
	ld	a5,0(s10)
	addi	a1,s10,8
	srai	ra,a5,63
.L190:
	ld	a6,0(a1)
	addi	a1,a1,8
	add	t0,a5,a6
	srai	s2,a6,63
	sltu	s6,t0,a5
	add	a4,ra,s2
	mv	a5,t0
	add	ra,s6,a4
.L189:
	ld	t1,0(a1)
	addi	a1,a1,8
	add	s8,a5,t1
	srai	t2,t1,63
	sltu	t3,s8,a5
	add	s1,ra,t2
	add	s0,t3,s1
	mv	a5,s8
	mv	ra,s0
	beq	s5,a1,.L216
.L42:
	ld	t4,0(a1)
	ld	a0,8(a1)
	ld	t5,16(a1)
	add	a3,a5,t4
	srai	t6,t4,63
	sltu	s7,a3,a5
	add	a2,ra,t6
	add	s3,a3,a0
	add	a6,s7,a2
	srai	t0,a0,63
	ld	s2,24(a1)
	sltu	s6,s3,a3
	add	a4,a6,t0
	add	t1,s3,t5
	add	s8,s6,a4
	srai	t2,t5,63
	add	s1,s8,t2
	sltu	t3,t1,s3
	add	s8,t1,s2
	add	s0,t3,s1
	srai	a5,s2,63
	sltu	ra,s8,t1
	add	t4,s0,a5
	add	s0,ra,t4
	addi	a1,a1,32
	mv	a5,s8
	mv	ra,s0
	bne	s5,a1,.L42
.L216:
	or	a1,s8,s0
	bne	a1,zero,.L43
	li	s5,1
	beq	s9,s5,.L62
	lw	s0,4(a7)
	addiw	s6,s9,-2
	addi	s3,a7,4
	li	a1,0
	andi	s2,s6,7
	li	t5,0
	blt	s11,s0,.L253
.L70:
	li	a4,2
	addi	a5,s3,4
	ble	s9,a4,.L212
	sext.w	s1,s2
	beq	s2,zero,.L47
	li	a3,1
	beq	s1,a3,.L192
	beq	s1,a4,.L193
	li	ra,3
	beq	s1,ra,.L194
	li	t4,4
	beq	s1,t4,.L195
	li	a7,5
	beq	s1,a7,.L196
	li	a0,6
	beq	s1,a0,.L197
	lw	t5,0(a5)
	bge	s11,t5,.L72
	li	a1,2
	mv	s11,t5
.L72:
	addi	a4,a4,1
	addi	a5,a5,4
.L197:
	lw	t6,0(a5)
	blt	s11,t6,.L254
.L75:
	addi	a4,a4,1
	addi	a5,a5,4
.L196:
	lw	t0,0(a5)
	bge	s11,t0,.L78
	sext.w	a1,a4
	mv	s11,t0
.L78:
	addi	a4,a4,1
	addi	a5,a5,4
.L195:
	lw	a6,0(a5)
	bge	s11,a6,.L81
	sext.w	a1,a4
	mv	s11,a6
.L81:
	addi	a4,a4,1
	addi	a5,a5,4
.L194:
	lw	t1,0(a5)
	blt	s11,t1,.L255
.L84:
	addi	a4,a4,1
	addi	a5,a5,4
.L193:
	lw	a2,0(a5)
	blt	s11,a2,.L256
.L87:
	addi	a4,a4,1
	addi	a5,a5,4
.L192:
	lw	t2,0(a5)
	mv	t5,a1
	bge	s11,t2,.L90
	sext.w	a1,a4
	mv	s11,t2
	mv	t5,a4
.L90:
	addi	a4,a4,1
	sext.w	t3,a4
	addi	a5,a5,4
	ble	s9,t3,.L212
.L47:
	lw	s4,0(a5)
	bge	s11,s4,.L46
	sext.w	a1,a4
	mv	s11,s4
.L46:
	lw	s7,4(a5)
	addi	a4,a4,1
	addi	a5,a5,4
	bge	s11,s7,.L93
	sext.w	a1,a4
	mv	s11,s7
.L93:
	lw	s5,4(a5)
	addi	s8,a4,1
	bge	s11,s5,.L95
	sext.w	a1,s8
	mv	s11,s5
.L95:
	lw	s0,8(a5)
	addi	s6,a4,2
	bge	s11,s0,.L97
	sext.w	a1,s6
	mv	s11,s0
.L97:
	lw	s3,12(a5)
	addi	s2,a4,3
	bge	s11,s3,.L99
	sext.w	a1,s2
	mv	s11,s3
.L99:
	lw	s1,16(a5)
	addi	a3,a4,4
	bge	s11,s1,.L101
	sext.w	a1,a3
	mv	s11,s1
.L101:
	lw	ra,20(a5)
	addi	t4,a4,5
	bge	s11,ra,.L103
	sext.w	a1,t4
	mv	s11,ra
.L103:
	lw	a7,24(a5)
	addi	a0,a4,6
	mv	t5,a1
	bge	s11,a7,.L105
	sext.w	a1,a0
	mv	s11,a7
	mv	t5,a0
.L105:
	addi	a4,a4,7
	sext.w	t6,a4
	addi	a5,a5,28
	bgt	s9,t6,.L47
.L212:
	ld	a0,88(sp)
	add	s8,a0,t5
.L44:
	slli	s9,s9,32
	srli	a2,s9,32
	li	a1,0
	call	memset@plt
	li	t0,-1
	sb	t0,0(s8)
.L48:
	ld	a6,96(sp)
	beq	a6,zero,.L52
	ld	t1,0(sp)
	sd	t1,0(a6)
.L52:
	ld	a0,80(sp)
	call	free@plt
	mv	a0,s10
	call	free@plt
	li	a0,0
.L230:
	ld	s0,272(sp)
	.cfi_restore 8
	ld	s1,264(sp)
	.cfi_restore 9
	ld	s2,256(sp)
	.cfi_restore 18
	ld	s3,248(sp)
	.cfi_restore 19
	ld	s4,240(sp)
	.cfi_restore 20
	ld	s5,232(sp)
	.cfi_restore 21
	ld	s6,224(sp)
	.cfi_restore 22
	ld	s7,216(sp)
	.cfi_restore 23
	ld	s8,208(sp)
	.cfi_restore 24
	ld	s9,200(sp)
	.cfi_restore 25
	ld	s10,192(sp)
	.cfi_restore 26
	ld	s11,184(sp)
	.cfi_restore 27
	fld	fs0,168(sp)
	.cfi_restore 40
.L2:
	ld	ra,280(sp)
	.cfi_restore 1
	addi	sp,sp,288
	.cfi_def_cfa_offset 0
	jr	ra
.L20:
	.cfi_def_cfa_offset 288
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	.cfi_offset 9, -24
	.cfi_offset 18, -32
	.cfi_offset 19, -40
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 22, -64
	.cfi_offset 23, -72
	.cfi_offset 24, -80
	.cfi_offset 25, -88
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	.cfi_offset 40, -120
	lw	s7,144(sp)
	ld	t0,48(sp)
	slli	s6,t3,1
	sll	a4,s6,s7
	srl	s11,t5,t0
	or	s9,a4,s11
	sd	s9,40(sp)
	j	.L21
.L18:
	lw	s6,144(sp)
	ld	a4,48(sp)
	slli	s8,t3,1
	sll	s7,s8,s6
	srl	a3,t5,a4
	or	a3,s7,a3
	j	.L19
.L16:
	lw	s6,144(sp)
	ld	a4,48(sp)
	slli	s8,t3,1
	sll	s7,s8,s6
	srl	a3,t5,a4
	or	a2,s7,a3
	j	.L17
.L14:
	lw	a4,144(sp)
	ld	a3,48(sp)
	slli	s6,s3,1
	sll	s7,s6,a4
	srl	t0,t3,a3
	or	t4,s7,t0
	j	.L15
.L37:
	sd	zero,0(t6)
	addiw	t4,t4,1
	addi	t5,t5,4
	addi	t6,t6,8
	bgt	s9,t4,.L41
	j	.L229
.L35:
	ld	t3,48(sp)
	srl	a1,ra,t3
	or	a4,s1,a1
	j	.L36
.L248:
	li	s0,1
	li	s5,0
	j	.L10
.L246:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 26
	addi	a5,a5,28
	sext.w	s0,t5
	bne	a2,a5,.L5
	j	.L3
.L245:
	lw	t6,24(a5)
	sext.w	t5,t3
	sext.w	s0,t6
	bge	t6,t5,.L175
	j	.L246
.L244:
	lw	t4,20(a5)
	sext.w	t3,a6
	sext.w	t5,t4
	bge	t4,t3,.L173
	j	.L245
.L243:
	lw	s3,16(a5)
	sext.w	a6,t2
	sext.w	t3,s3
	bge	s3,a6,.L171
	j	.L244
.L242:
	lw	a0,12(a5)
	sext.w	t2,t0
	sext.w	a6,a0
	bge	a0,t2,.L169
	j	.L243
.L241:
	lw	t1,8(a5)
	sext.w	t0,ra
	sext.w	t2,t1
	bge	t1,t0,.L167
	j	.L242
.L240:
	lw	a3,4(a5)
	sext.w	ra,a4
	sext.w	t0,a3
	bge	a3,ra,.L165
	j	.L241
.L239:
	sext.w	a4,s0
	lw	s0,4(a5)
	addi	a5,a5,4
	sext.w	ra,s0
	bge	s0,a4,.L163
	j	.L240
.L43:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 22, -64
	.cfi_offset 23, -72
	.cfi_offset 24, -80
	.cfi_offset 26, -96
	sub	s11,s5,s10
	addi	s9,s11,-8
	srli	a7,s9,3
	slli	a0,s0,63
	srli	t5,s8,1
	andi	a3,a7,1
	ld	s7,88(sp)
	or	s6,a0,t5
	srai	s3,s0,1
	li	s2,255
	bne	a3,zero,.L51
	ld	s4,0(s10)
	mv	a2,s8
	mv	a3,s0
	srai	t6,s4,63
	slli	t0,s4,8
	srli	a4,s4,56
	slli	a6,t6,8
	sub	t1,t0,s4
	or	t2,a4,a6
	sgtu	t3,t1,t0
	sub	s1,t2,t6
	add	a0,t1,s6
	sub	ra,s1,t3
	sltu	a5,a0,t1
	add	t4,ra,s3
	add	a1,a5,t4
	call	__divti3@plt
	bgt	a0,s2,.L213
	not	a1,a0
	srai	s11,a1,63
	and	a2,a0,s11
.L214:
	sb	a2,0(s7)
	addi	s4,s10,8
	addi	s7,s7,1
	bne	s4,s5,.L51
	j	.L48
.L257:
	li	ra,255
.L50:
	ld	t4,8(s4)
	sb	ra,0(s7)
	mv	a2,s8
	srai	s11,t4,63
	slli	s9,t4,8
	srli	a7,t4,56
	slli	a0,s11,8
	sub	a1,s9,t4
	or	t5,a7,a0
	sgtu	t6,a1,s9
	sub	t0,t5,s11
	add	a0,a1,s6
	sub	a6,t0,t6
	sltu	a4,a0,a1
	add	t1,a6,s3
	mv	a3,s0
	add	a1,a4,t1
	call	__divti3@plt
	not	a2,a0
	addi	s4,s4,8
	addi	s7,s7,1
	srai	t2,a2,63
	ble	a0,s2,.L110
	li	t3,255
.L215:
	sb	t3,0(s7)
	addi	s4,s4,8
	addi	s7,s7,1
	beq	s4,s5,.L48
.L51:
	ld	s9,0(s4)
	mv	a3,s0
	mv	a2,s8
	srai	a7,s9,63
	slli	a0,s9,8
	srli	t5,s9,56
	slli	t6,a7,8
	sub	t0,a0,s9
	or	a6,t5,t6
	sgtu	a4,t0,a0
	sub	t1,a6,a7
	add	a0,t0,s6
	sub	t2,t1,a4
	sltu	t3,a0,t0
	add	s1,t2,s3
	add	a1,t3,s1
	call	__divti3@plt
	not	a3,a0
	srai	a5,a3,63
	bgt	a0,s2,.L257
	and	ra,a0,a5
	j	.L50
.L252:
	li	a0,4
	j	.L33
.L238:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 26
	mv	t6,s0
	addi	a5,a5,4
	sext.w	s0,t6
	bne	a2,a5,.L5
	j	.L3
.L237:
	mv	t4,s0
	j	.L157
.L236:
	mv	s3,s0
	j	.L154
.L110:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 22, -64
	.cfi_offset 23, -72
	.cfi_offset 24, -80
	.cfi_offset 26, -96
	and	t3,a0,t2
	j	.L215
.L8:
	ld	a3,48(sp)
	li	a4,1
	li	a1,0
	sll	a0,a4,a3
	j	.L9
.L253:
	li	a1,1
	mv	s11,s0
	li	t5,1
	j	.L70
.L235:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 26
	mv	t1,s0
	j	.L148
.L256:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 22, -64
	.cfi_offset 23, -72
	.cfi_offset 24, -80
	.cfi_offset 26, -96
	sext.w	a1,a4
	mv	s11,a2
	j	.L87
.L247:
	lw	t2,0(s7)
	addi	a3,a0,4
	addi	a4,s7,4
	subw	a0,t2,s0
	sw	a0,0(s1)
	j	.L188
.L213:
	li	a2,255
	j	.L214
.L255:
	sext.w	a1,a4
	mv	s11,t1
	j	.L84
.L234:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 26
	mv	a3,s0
	j	.L145
.L55:
	mv	s0,s11
	j	.L3
.L58:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 22, -64
	.cfi_offset 23, -72
	.cfi_offset 24, -80
	.cfi_offset 26, -96
	ld	s1,48(sp)
	sd	zero,0(sp)
	lla	a1,.LC3
	addiw	a0,s1,-64
	sw	a0,36(sp)
	j	.L13
.L254:
	sext.w	a1,a4
	mv	s11,t6
	j	.L75
.L62:
	ld	s8,88(sp)
	mv	a0,s8
	j	.L44
.L54:
	.cfi_restore 8
	.cfi_restore 9
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 27
	.cfi_restore 40
	ld	s2,256(sp)
	.cfi_restore 18
	li	a0,-2
	j	.L2
.L53:
	li	a0,-1
	j	.L2
.L249:
	.cfi_offset 8, -16
	.cfi_offset 9, -24
	.cfi_offset 18, -32
	.cfi_offset 19, -40
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 22, -64
	.cfi_offset 23, -72
	.cfi_offset 24, -80
	.cfi_offset 25, -88
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	.cfi_offset 40, -120
	ld	a0,80(sp)
	call	free@plt
	li	a0,-4
	j	.L230
.L56:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 26
	ld	s0,272(sp)
	.cfi_restore 8
	ld	s2,256(sp)
	.cfi_restore 18
	ld	s3,248(sp)
	.cfi_restore 19
	ld	s9,200(sp)
	.cfi_restore 25
	ld	s11,184(sp)
	.cfi_restore 27
	fld	fs0,168(sp)
	.cfi_restore 40
	li	a0,-3
	j	.L2
	.cfi_endproc
.LFE41:
	.size	int_softmax_mac_ilp, .-int_softmax_mac_ilp
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC4:
	.string	"alloc_fail"
	.align	3
.LC6:
	.string	"error %d\n"
	.align	3
.LC7:
	.string	"N=%zu LANES=%d QCOEF=%d RSHIFT=%d mac_hash=%llu out_sum=%llu\n"
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB42:
	.cfi_startproc
	addi	sp,sp,-64
	.cfi_def_cfa_offset 64
	sd	s0,48(sp)
	.cfi_offset 8, -16
	la	s0,__stack_chk_guard
	ld	a5, 0(s0)
	sd	a5, 8(sp)
	li	a5, 0
	sd	s2,32(sp)
	sd	ra,56(sp)
	sd	s1,40(sp)
	sd	s3,24(sp)
	sd	s4,16(sp)
	.cfi_offset 18, -32
	.cfi_offset 1, -8
	.cfi_offset 9, -24
	.cfi_offset 19, -40
	.cfi_offset 20, -48
	li	a5,1
	li	s2,1048576
	ble	a0,a5,.L259
	ld	a0,8(a1)
	li	a2,10
	li	a1,0
	call	strtoull@plt
	mv	s2,a0
.L259:
	slli	s4,s2,2
	mv	a0,s4
	call	malloc@plt
	mv	s1,a0
	mv	a0,s2
	call	malloc@plt
	mv	s3,a0
	beq	s1,zero,.L260
	beq	a0,zero,.L260
	li	a4,-1640529920
	sext.w	a1,s2
	mv	a5,s1
	add	a0,s4,s1
	addi	a4,a4,-1607
	beq	s2,zero,.L337
	sub	a7,a0,s1
	addi	ra,a7,-4
	srli	t0,ra,2
	addi	t1,t0,1
	li	a6,1315422208
	andi	t2,t1,7
	li	a3,101
	addiw	s4,a6,1703
	beq	t2,zero,.L264
	li	a2,1
	beq	t2,a2,.L321
	li	t3,2
	beq	t2,t3,.L322
	li	t4,3
	beq	t2,t4,.L323
	li	t5,4
	beq	t2,t5,.L324
	li	t6,5
	beq	t2,t6,.L325
	li	a7,6
	bne	t2,a7,.L344
.L326:
	remuw	t1,a4,a3
	addw	a4,s4,a4
	addiw	a6,t1,-50
	sw	a6,0(a5)
	addi	a5,a5,4
.L325:
	remuw	t2,a4,a3
	addw	a4,s4,a4
	addiw	a2,t2,-50
	sw	a2,0(a5)
	addi	a5,a5,4
.L324:
	remuw	t3,a4,a3
	addw	a4,s4,a4
	addiw	t4,t3,-50
	sw	t4,0(a5)
	addi	a5,a5,4
.L323:
	remuw	t5,a4,a3
	addw	a4,s4,a4
	addiw	t6,t5,-50
	sw	t6,0(a5)
	addi	a5,a5,4
.L322:
	remuw	a7,a4,a3
	addi	a5,a5,4
	addw	a4,s4,a4
	addiw	ra,a7,-50
	sw	ra,-4(a5)
.L321:
	remuw	t0,a4,a3
	addi	a5,a5,4
	addw	a4,s4,a4
	addiw	t1,t0,-50
	sw	t1,-4(a5)
	beq	a0,a5,.L341
.L264:
	addw	a6,s4,a4
	addw	t2,s4,a6
	addw	t4,s4,t2
	addw	t3,s4,t4
	addw	ra,s4,t3
	addw	a7,s4,ra
	remuw	t5,a4,a3
	addw	a2,s4,a7
	addw	a4,s4,a2
	addi	a5,a5,32
	remuw	t6,a6,a3
	addiw	t0,t5,-50
	sw	t0,-32(a5)
	remuw	t1,t2,a3
	addiw	a6,t6,-50
	sw	a6,-28(a5)
	remuw	t2,t4,a3
	addiw	t4,t1,-50
	sw	t4,-24(a5)
	remuw	t3,t3,a3
	addiw	t5,t2,-50
	sw	t5,-20(a5)
	remuw	ra,ra,a3
	addiw	t6,t3,-50
	sw	t6,-16(a5)
	remuw	a7,a7,a3
	addiw	t0,ra,-50
	sw	t0,-12(a5)
	remuw	a2,a2,a3
	addiw	t1,a7,-50
	sw	t1,-8(a5)
	addiw	a6,a2,-50
	sw	a6,-4(a5)
	bne	a0,a5,.L264
.L341:
	mv	a4,sp
	mv	a3,s3
	fld	fa0,.LC5,a5
	li	a2,8
	mv	a0,s1
	sd	zero,0(sp)
	call	int_softmax_mac_ilp
	bne	a0,zero,.L268
	andi	a1,s2,7
	mv	a5,s3
	add	a3,s2,s3
	li	a7,0
	beq	a1,zero,.L267
	li	a0,1
	beq	a1,a0,.L327
	li	s4,2
	beq	a1,s4,.L328
	li	a4,3
	beq	a1,a4,.L329
	li	t2,4
	beq	a1,t2,.L330
	li	t4,5
	beq	a1,t4,.L331
	li	t3,6
	beq	a1,t3,.L332
	lbu	a7,0(s3)
	addi	a5,s3,1
.L332:
	lbu	t5,0(a5)
	addi	a5,a5,1
	add	a7,a7,t5
.L331:
	lbu	ra,0(a5)
	addi	a5,a5,1
	add	a7,a7,ra
.L330:
	lbu	t6,0(a5)
	addi	a5,a5,1
	add	a7,a7,t6
.L329:
	lbu	t0,0(a5)
	addi	a5,a5,1
	add	a7,a7,t0
.L328:
	lbu	a2,0(a5)
	addi	a5,a5,1
	add	a7,a7,a2
.L327:
	lbu	t1,0(a5)
	addi	a5,a5,1
	add	a7,a7,t1
	beq	a3,a5,.L269
.L267:
	lbu	a6,0(a5)
	lbu	a1,1(a5)
	lbu	s4,2(a5)
	lbu	a4,3(a5)
	add	a7,a7,a6
	lbu	a0,4(a5)
	add	t2,a7,a1
	lbu	t4,5(a5)
	add	t3,t2,s4
	lbu	t5,6(a5)
	add	ra,t3,a4
	lbu	t6,7(a5)
	add	t0,ra,a0
	add	a2,t0,t4
	add	t1,a2,t5
	addi	a5,a5,8
	add	a7,t1,t6
	bne	a3,a5,.L267
.L269:
	ld	a6,0(sp)
	li	a5,8
	li	a4,40
	li	a3,4
	mv	a2,s2
	lla	a1,.LC7
	li	a0,2
	call	__printf_chk@plt
	mv	a0,s1
	call	free@plt
	mv	a0,s3
	call	free@plt
	li	a0,0
.L266:
	ld	a4, 8(sp)
	ld	s0, 0(s0)
	xor	s0, a4, s0
	li	a4, 0
	bne	s0,zero,.L345
	ld	ra,56(sp)
	.cfi_remember_state
	.cfi_restore 1
	ld	s0,48(sp)
	.cfi_restore 8
	ld	s1,40(sp)
	.cfi_restore 9
	ld	s2,32(sp)
	.cfi_restore 18
	ld	s3,24(sp)
	.cfi_restore 19
	ld	s4,16(sp)
	.cfi_restore 20
	addi	sp,sp,64
	.cfi_def_cfa_offset 0
	jr	ra
.L268:
	.cfi_restore_state
	mv	a2,a0
	lla	a1,.LC6
	li	a0,2
	call	__printf_chk@plt
	li	a0,1
	j	.L266
.L344:
	remuw	ra,a4,a3
	addi	a5,s1,4
	addw	a4,s4,a4
	addiw	t0,ra,-50
	sw	t0,0(s1)
	j	.L326
.L337:
	mv	a4,sp
	mv	a3,s3
	fld	fa0,.LC5,a5
	li	a2,8
	li	a1,0
	mv	a0,s1
	sd	zero,0(sp)
	call	int_softmax_mac_ilp
	bne	a0,zero,.L268
	li	a7,0
	j	.L269
.L260:
	lla	a0,.LC4
	call	puts@plt
	li	a0,1
	j	.L266
.L345:
	call	__stack_chk_fail@plt
	.cfi_endproc
.LFE42:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align	3
.LC0:
	.word	-17155601
	.word	1072049730
	.section	.rodata.cst16,"aM",@progbits,16
	.align	4
.LC3:
	.dword	45812984490
	.dword	0
	.section	.rodata.cst8
	.align	3
.LC5:
	.word	1202590843
	.word	1066695393
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
