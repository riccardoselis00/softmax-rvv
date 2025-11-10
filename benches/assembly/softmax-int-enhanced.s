	.file	"softmax-int-enhanced.c"
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
.LFB40:
	.cfi_startproc
	addi	sp,sp,-256
	.cfi_def_cfa_offset 256
	sd	ra,248(sp)
	sd	a3,64(sp)
	sd	a4,72(sp)
	.cfi_offset 1, -8
	ble	a1,zero,.L56
	sd	s2,224(sp)
	addiw	a5,a2,-1
	li	a4,29
	.cfi_offset 18, -32
	mv	s2,a2
	bgtu	a5,a4,.L57
	sd	s6,192(sp)
	sd	s8,176(sp)
	sd	s9,168(sp)
	fsd	fs0,136(sp)
	.cfi_offset 22, -64
	.cfi_offset 24, -80
	.cfi_offset 25, -88
	.cfi_offset 40, -120
	mv	s6,a1
	sd	s0,240(sp)
	sd	s3,216(sp)
	li	a1,1
	fmv.d	fs0,fa0
	lw	s8,0(a0)
	mv	s9,a0
	.cfi_offset 8, -16
	.cfi_offset 19, -40
	beq	s6,a1,.L58
	addiw	a3,s6,-2
	slli	ra,a3,32
	addi	t1,a0,8
	srli	t0,ra,30
	addi	a5,a0,4
	add	a2,t0,t1
	sub	t2,a2,a5
	addi	s0,t2,-4
	srli	a0,s0,2
	addi	a6,a0,1
	andi	a7,a6,7
	mv	s0,s8
	beq	a7,zero,.L5
	beq	a7,a1,.L183
	li	s3,2
	beq	a7,s3,.L184
	li	t3,3
	beq	a7,t3,.L185
	li	t4,4
	beq	a7,t4,.L186
	li	t5,5
	beq	a7,t5,.L187
	li	t6,6
	beq	a7,t6,.L188
	lw	a4,0(a5)
	sext.w	s0,a4
	bge	a4,s8,.L148
	sext.w	s0,s8
.L148:
	addi	a5,a5,4
.L188:
	lw	a1,0(a5)
	mv	a3,a1
	blt	a1,s0,.L240
.L151:
	sext.w	s0,a3
	addi	a5,a5,4
.L187:
	lw	ra,0(a5)
	mv	t0,ra
	blt	ra,s0,.L241
.L154:
	sext.w	s0,t0
	addi	a5,a5,4
.L186:
	lw	t1,0(a5)
	mv	t2,t1
	bge	t1,s0,.L157
	mv	t2,s0
.L157:
	sext.w	s0,t2
	addi	a5,a5,4
.L185:
	lw	a0,0(a5)
	mv	a6,a0
	blt	a0,s0,.L242
.L160:
	sext.w	s0,a6
	addi	a5,a5,4
.L184:
	lw	a7,0(a5)
	mv	s3,a7
	blt	a7,s0,.L243
.L163:
	sext.w	s0,s3
	addi	a5,a5,4
.L183:
	lw	t3,0(a5)
	mv	t4,t3
	blt	t3,s0,.L244
	addi	a5,a5,4
	sext.w	s0,t4
	beq	a2,a5,.L3
.L5:
	lw	t5,0(a5)
	sext.w	t6,t5
	blt	t5,s0,.L245
	lw	s0,4(a5)
	addi	a5,a5,4
	sext.w	a3,s0
	blt	s0,t6,.L246
.L169:
	lw	a1,4(a5)
	sext.w	a4,a1
	blt	a1,a3,.L247
.L171:
	lw	ra,8(a5)
	sext.w	t0,ra
	blt	ra,a4,.L248
.L173:
	lw	t1,12(a5)
	sext.w	t2,t1
	blt	t1,t0,.L249
.L175:
	lw	a0,16(a5)
	sext.w	a6,a0
	blt	a0,t2,.L250
.L177:
	lw	a7,20(a5)
	sext.w	s3,a7
	blt	a7,a6,.L251
.L179:
	lw	t3,24(a5)
	sext.w	s0,t3
	blt	t3,s3,.L252
.L181:
	addi	a5,a5,28
	bne	a2,a5,.L5
.L3:
	slli	s3,s6,2
	mv	a0,s3
	call	malloc@plt
	sd	a0,56(sp)
	mv	a3,a0
	beq	a0,zero,.L59
	sd	s1,232(sp)
	.cfi_offset 9, -24
	addi	s1,s3,-4
	sd	s4,208(sp)
	.cfi_offset 20, -48
	srli	s4,s1,2
	sd	s5,200(sp)
	.cfi_offset 21, -56
	addi	s5,s4,1
	sd	s7,184(sp)
	sd	s10,160(sp)
	sd	s11,152(sp)
	.cfi_offset 23, -72
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	andi	s7,s5,7
	mv	a4,s9
	mv	s1,a0
	add	s10,s3,s9
	beq	s7,zero,.L6
	li	s11,1
	beq	s7,s11,.L189
	li	a2,2
	beq	s7,a2,.L190
	li	t4,3
	beq	s7,t4,.L191
	li	t5,4
	beq	s7,t5,.L192
	li	t6,5
	beq	s7,t6,.L193
	li	a1,6
	bne	s7,a1,.L253
.L194:
	lw	t2,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	a0,t2,s0
	sw	a0,-4(a3)
.L193:
	lw	a6,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	a7,a6,s0
	sw	a7,-4(a3)
.L192:
	lw	t3,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	a5,t3,s0
	sw	a5,-4(a3)
.L191:
	lw	s3,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	s4,s3,s0
	sw	s4,-4(a3)
.L190:
	lw	s5,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	s7,s5,s0
	sw	s7,-4(a3)
.L189:
	lw	s11,0(a4)
	addi	a3,a3,4
	addi	a4,a4,4
	subw	a2,s11,s0
	sw	a2,-4(a3)
	beq	s10,a4,.L224
.L6:
	lw	t4,0(a4)
	lw	t5,4(a4)
	lw	t6,8(a4)
	lw	ra,12(a4)
	lw	t0,16(a4)
	lw	a1,20(a4)
	lw	t2,24(a4)
	lw	a5,28(a4)
	subw	t3,t4,s0
	subw	t1,t5,s0
	subw	a7,t6,s0
	subw	a6,ra,s0
	subw	a0,t0,s0
	subw	s3,a1,s0
	subw	s4,t2,s0
	subw	s5,a5,s0
	sw	t3,0(a3)
	sw	t1,4(a3)
	sw	a7,8(a3)
	sw	a6,12(a3)
	sw	a0,16(a3)
	sw	s3,20(a3)
	sw	s4,24(a3)
	sw	s5,28(a3)
	addi	a4,a4,32
	addi	a3,a3,32
	bne	s10,a4,.L6
.L224:
	fld	fa5,.LC0,a5
	fdiv.d	ft0,fa5,fs0
	fcvt.l.d s3,ft0,rtz
	bgt	s3,zero,.L7
	li	s3,1
.L7:
	slliw	s0,s2,1
	addiw	s10,s0,-64
	sd	s0,32(sp)
	slli	s7,s10,32
	blt	s7,zero,.L8
	li	s11,1
	sll	a1,s11,s10
	li	a0,0
.L9:
	mv	a2,s3
	srai	a3,s3,63
	call	__divti3@plt
	or	a2,a0,a1
	mv	s0,a0
	mv	s2,a1
	beq	a2,zero,.L254
.L10:
	fld	fa4,.LC1,a5
	fdiv.d	ft1,fa4,fs0
	fld	ft2,.LC2,a5
	fmul.d	ft3,fs0,ft2
	fmv.d.x	fa3,zero
	li	t4,83984384
	addi	t5,t4,-9
	slli	t6,t5,12
	sd	t6,8(sp)
	fmul.d	ft4,ft3,fs0
	fle.d	ra,ft4,fa3
	fcvt.l.d s4,ft1,rtz
	bne	ra,zero,.L13
	fgt.d	t0,ft4,fa3
	beq	t0,zero,.L213
	fld	ft5,.LC3,a5
	fdiv.d	ft6,ft5,ft4
	fcvt.l.d a1,ft6,rtz
	sd	a1,8(sp)
.L13:
	slli	t2,s6,3
	mv	a0,t2
	sd	t2,0(sp)
	call	malloc@plt
	ld	a7,0(sp)
	mv	s7,a0
	beq	a0,zero,.L255
	sraiw	a5,s6,2
	slliw	t3,a5,2
	srai	a1,s6,2
	sd	t3,48(sp)
	beq	a1,zero,.L63
	ld	a0,32(sp)
	li	a4,63
	li	s5,40960
	addiw	s11,a0,-64
	li	a3,53248
	li	t4,32768
	li	a2,8192
	subw	t5,a4,a0
	sw	s11,20(sp)
	sd	s11,0(sp)
	mv	t0,s7
	sd	zero,40(sp)
	li	s10,0
	sw	t5,120(sp)
	addi	s5,s5,-457
	addi	ra,a3,695
	addi	a1,t4,-182
	addi	a2,a2,-794
	li	s11,62
	sd	a7,80(sp)
	sd	s8,88(sp)
	sd	s7,96(sp)
	sd	s9,104(sp)
	sd	s6,112(sp)
	sw	t3,124(sp)
.L35:
	lw	s8,0(s1)
	ld	a0,0(sp)
	lw	a7,4(s1)
	srai	s6,s8,63
	mul	s9,s6,s0
	lw	a6,8(s1)
	lw	t2,12(s1)
	mul	t6,s2,s8
	mulhu	s7,s8,s0
	add	a5,s9,t6
	mul	t3,s8,s0
	add	t1,a5,s7
	blt	a0,zero,.L17
	lw	a3,20(sp)
	sra	t6,t1,a3
.L18:
	srai	s7,a7,63
	ld	a4,0(sp)
	mul	t3,s7,s0
	mul	t1,s2,a7
	mulhu	a5,a7,s0
	add	a0,t3,t1
	mul	a3,a7,s0
	add	t4,a0,a5
	blt	a4,zero,.L19
	lw	t5,20(sp)
	sra	t5,t4,t5
.L20:
	srai	a5,a6,63
	ld	s7,0(sp)
	mul	a0,a5,s0
	mul	a3,s2,a6
	mulhu	s6,a6,s0
	add	t4,a0,a3
	mul	a4,a6,s0
	add	s9,t4,s6
	blt	s7,zero,.L21
	lw	t3,20(sp)
	sra	t4,s9,t3
.L22:
	srai	a4,t2,63
	ld	a5,0(sp)
	mul	s9,a4,s0
	mul	s7,s2,t2
	mulhu	t1,t2,s0
	add	t3,s9,s7
	mul	a0,t2,s0
	add	a3,t3,t1
	blt	a5,zero,.L23
	lw	s6,20(sp)
	sra	t3,a3,s6
.L24:
	li	a4,0
	li	a0,0
	li	a3,0
	mul	s6,s3,t6
	li	a5,0
	sd	zero,24(sp)
	mul	s7,s3,t5
	sub	s8,s8,s6
	add	t1,s4,s8
	xor	s6,t1,ra
	xor	s8,t1,s5
#APP
# 113 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a4, s8, s8
# 0 "" 2
# 113 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a4, s6, s6
# 0 "" 2
#NO_APP
	xor	s9,t1,a1
#APP
# 113 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a4, s9, s9
# 0 "" 2
#NO_APP
	xor	t1,t1,a2
	sub	a7,a7,s7
	add	a7,s4,a7
	xor	s6,a7,s5
#APP
# 114 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a0, s6, s6
# 0 "" 2
#NO_APP
	mul	s6,s3,t4
	xor	s8,a7,ra
	xor	s7,a7,a1
#APP
# 114 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a0, s8, s8
# 0 "" 2
#NO_APP
	xor	a7,a7,a2
#APP
# 113 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a4, t1, t1
# 0 "" 2
# 114 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a0, s7, s7
# 0 "" 2
# 114 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a0, a7, a7
# 0 "" 2
#NO_APP
	xor	s8,a4,a0
	mul	s9,s3,t3
	sub	a6,a6,s6
	add	a6,s4,a6
	xor	a7,a6,s5
	xor	t1,a6,a1
	xor	s6,a6,a2
#APP
# 115 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a3, a7, a7
# 0 "" 2
#NO_APP
	sub	t2,t2,s9
	add	s7,s4,t2
	xor	s9,a6,ra
#APP
# 115 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a3, s9, s9
# 0 "" 2
# 115 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a3, t1, t1
# 0 "" 2
# 115 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a3, s6, s6
# 0 "" 2
#NO_APP
	xor	s8,s8,a3
	xor	t2,s7,s5
	xor	a7,s7,ra
	xor	a6,s7,a1
#APP
# 116 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, t2, t2
# 0 "" 2
#NO_APP
	xor	s7,s7,a2
#APP
# 116 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, a7, a7
# 0 "" 2
# 116 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, a6, a6
# 0 "" 2
# 116 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, s7, s7
# 0 "" 2
#NO_APP
	xor	s9,s8,a5
	xor	s10,s10,s9
	bgt	t6,s11,.L25
	ld	s8,8(sp)
	not	t1,t6
	srai	s6,t1,63
	add	a4,a4,s8
	and	t6,t6,s6
	srl	t2,a4,t6
	sd	t2,24(sp)
.L25:
	li	s9,0
	bgt	t5,s11,.L27
	ld	s7,8(sp)
	not	a7,t5
	srai	a6,a7,63
	add	a0,a0,s7
	and	t5,t5,a6
	srl	s9,a0,t5
.L27:
	li	a4,0
	bgt	t4,s11,.L29
	ld	s8,8(sp)
	not	t1,t4
	srai	s6,t1,63
	add	a3,a3,s8
	and	t4,t4,s6
	srl	a4,a3,t4
.L29:
	bgt	t3,s11,.L31
	ld	a7,8(sp)
	not	t6,t3
	ld	a0,24(sp)
	srai	t2,t6,63
	add	a5,a5,a7
	and	a6,t3,t2
	srl	s7,a5,a6
	sd	a0,0(t0)
	sd	s9,8(t0)
	sd	a4,16(t0)
	sd	s7,24(t0)
.L238:
	ld	t5,40(sp)
	ld	t1,48(sp)
	addi	s1,s1,16
	addiw	s9,t5,4
	sd	s9,40(sp)
	addi	t0,t0,32
	bgt	t1,s9,.L35
	li	ra,4
	ld	a7,80(sp)
	ld	s8,88(sp)
	ld	s7,96(sp)
	ld	s9,104(sp)
	ld	s6,112(sp)
	lw	a1,124(sp)
	blt	t1,ra,.L256
.L36:
	sext.w	a1,a1
	bge	a1,s6,.L37
.L16:
	ld	t6,56(sp)
	ld	a5,32(sp)
	lw	s5,20(sp)
	slli	a2,a1,2
	slli	s11,a1,3
	li	a3,40960
	li	a4,53248
	li	t3,32768
	li	t4,8192
	li	t2,63
	add	a6,t6,a2
	add	a0,s7,s11
	subw	t2,t2,a5
	addi	t0,a3,-457
	li	s1,0
	addi	t6,a4,695
	addi	t5,t3,-182
	addi	t4,t4,-794
	li	ra,62
.L44:
	lw	s11,0(a6)
	srai	t1,s11,63
	mul	a3,t1,s0
	mul	a2,s2,s11
	mulhu	a4,s11,s0
	add	t3,a3,a2
	add	t1,t3,a4
	slli	a5,t1,1
	mul	a2,s11,s0
	sll	a4,a5,t2
	blt	s5,zero,.L38
	lw	a3,20(sp)
	sra	a3,t1,a3
.L39:
	not	a5,a3
	srai	a2,a5,63
	mv	a5,s1
	mul	a4,s3,a3
	and	a2,a3,a2
	sub	s11,s11,a4
	add	a4,s4,s11
	xor	s11,a4,t0
#APP
# 153 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, s11, s11
# 0 "" 2
#NO_APP
	ld	s11,8(sp)
	xor	t3,a4,t6
	xor	t1,a4,t5
#APP
# 153 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, t3, t3
# 0 "" 2
#NO_APP
	xor	a4,a4,t4
#APP
# 153 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, t1, t1
# 0 "" 2
# 153 "benches/code/softmax-int-enhanced.c" 1
	.insn r 0x5B, 0x02, 0x00, a5, a4, a4
# 0 "" 2
#NO_APP
	add	t3,a5,s11
	srl	a2,t3,a2
	xor	s10,s10,a5
	bgt	a3,ra,.L40
	sd	a2,0(a0)
	addiw	a1,a1,1
	addi	a6,a6,4
	addi	a0,a0,8
	bgt	s6,a1,.L44
.L37:
	addi	s3,a7,-8
	srli	s0,s3,3
	addi	s2,s0,1
	andi	t2,s2,3
	mv	s4,s7
	add	s5,a7,s7
	mv	t1,s7
	li	a1,0
	li	a2,0
	beq	t2,zero,.L45
	li	a7,1
	beq	t2,a7,.L195
	li	t0,2
	beq	t2,t0,.L196
	ld	a1,0(s7)
	addi	t1,s7,8
	srai	a2,a1,63
.L196:
	ld	s1,0(t1)
	addi	t1,t1,8
	add	t6,a1,s1
	srai	t5,s1,63
	sltu	t4,t6,a1
	add	ra,a2,t5
	mv	a1,t6
	add	a2,t4,ra
.L195:
	ld	a3,0(t1)
	addi	t1,t1,8
	add	s11,a1,a3
	srai	a4,a3,63
	sltu	a5,s11,a1
	add	t3,a2,a4
	add	s0,a5,t3
	mv	a1,s11
	mv	a2,s0
	beq	s5,t1,.L223
.L45:
	ld	s3,0(t1)
	ld	a0,8(t1)
	ld	a6,16(t1)
	add	s2,a1,s3
	srai	t2,s3,63
	sltu	a7,s2,a1
	add	t0,a2,t2
	add	s1,s2,a0
	srai	t5,a0,63
	add	t6,a7,t0
	ld	t4,24(t1)
	sltu	ra,s1,s2
	add	a3,t6,t5
	add	t3,s1,a6
	add	s11,ra,a3
	srai	a4,a6,63
	add	s0,s11,a4
	sltu	a5,t3,s1
	add	a1,a5,s0
	add	s11,t3,t4
	srai	s3,t4,63
	sltu	a2,s11,t3
	add	a0,a1,s3
	add	s0,a2,a0
	addi	t1,t1,32
	mv	a1,s11
	mv	a2,s0
	bne	s5,t1,.L45
.L223:
	or	t1,s11,s0
	bne	t1,zero,.L46
	li	s5,1
	beq	s6,s5,.L67
	lw	s0,4(s9)
	addiw	s1,s6,-2
	addi	s3,s9,4
	li	a0,0
	andi	a1,s1,7
	li	t0,0
	blt	s8,s0,.L257
.L76:
	li	a4,2
	addi	a5,s3,4
	ble	s6,a4,.L219
	sext.w	t1,a1
	beq	a1,zero,.L50
	li	s9,1
	beq	t1,s9,.L198
	beq	t1,a4,.L199
	li	a3,3
	beq	t1,a3,.L200
	li	s2,4
	beq	t1,s2,.L201
	li	t2,5
	beq	t1,t2,.L202
	li	a7,6
	beq	t1,a7,.L203
	lw	a6,0(a5)
	bge	s8,a6,.L78
	li	a0,2
	mv	s8,a6
.L78:
	addi	a4,a4,1
	addi	a5,a5,4
.L203:
	lw	t0,0(a5)
	blt	s8,t0,.L258
.L81:
	addi	a4,a4,1
	addi	a5,a5,4
.L202:
	lw	t6,0(a5)
	bge	s8,t6,.L84
	sext.w	a0,a4
	mv	s8,t6
.L84:
	addi	a4,a4,1
	addi	a5,a5,4
.L201:
	lw	t5,0(a5)
	bge	s8,t5,.L87
	sext.w	a0,a4
	mv	s8,t5
.L87:
	addi	a4,a4,1
	addi	a5,a5,4
.L200:
	lw	t4,0(a5)
	blt	s8,t4,.L259
.L90:
	addi	a4,a4,1
	addi	a5,a5,4
.L199:
	lw	ra,0(a5)
	blt	s8,ra,.L260
.L93:
	addi	a4,a4,1
	addi	a5,a5,4
.L198:
	lw	a2,0(a5)
	mv	t0,a0
	bge	s8,a2,.L96
	sext.w	a0,a4
	mv	s8,a2
	mv	t0,a4
.L96:
	addi	a4,a4,1
	sext.w	t3,a4
	addi	a5,a5,4
	ble	s6,t3,.L219
.L50:
	lw	s4,0(a5)
	bge	s8,s4,.L49
	sext.w	a0,a4
	mv	s8,s4
.L49:
	lw	s5,4(a5)
	addi	a4,a4,1
	addi	a5,a5,4
	bge	s8,s5,.L99
	sext.w	a0,a4
	mv	s8,s5
.L99:
	lw	s11,4(a5)
	addi	s0,a4,1
	bge	s8,s11,.L101
	sext.w	a0,s0
	mv	s8,s11
.L101:
	lw	s1,8(a5)
	addi	s3,a4,2
	bge	s8,s1,.L103
	sext.w	a0,s3
	mv	s8,s1
.L103:
	lw	a1,12(a5)
	addi	t1,a4,3
	bge	s8,a1,.L105
	sext.w	a0,t1
	mv	s8,a1
.L105:
	lw	s9,16(a5)
	addi	a3,a4,4
	bge	s8,s9,.L107
	sext.w	a0,a3
	mv	s8,s9
.L107:
	lw	s2,20(a5)
	addi	t2,a4,5
	bge	s8,s2,.L109
	sext.w	a0,t2
	mv	s8,s2
.L109:
	lw	a7,24(a5)
	addi	a6,a4,6
	mv	t0,a0
	bge	s8,a7,.L111
	sext.w	a0,a6
	mv	s8,a7
	mv	t0,a6
.L111:
	addi	a4,a4,7
	sext.w	t6,a4
	addi	a5,a5,28
	bgt	s6,t6,.L50
.L219:
	ld	a0,64(sp)
	add	s11,a0,t0
.L47:
	slli	s6,s6,32
	srli	a2,s6,32
	li	a1,0
	call	memset@plt
	li	t5,-1
	sb	t5,0(s11)
.L51:
	ld	t4,72(sp)
	beq	t4,zero,.L55
	sd	s10,0(t4)
.L55:
	ld	a0,56(sp)
	call	free@plt
	mv	a0,s7
	call	free@plt
	li	a0,0
.L236:
	ld	s0,240(sp)
	.cfi_restore 8
	ld	s1,232(sp)
	.cfi_restore 9
	ld	s2,224(sp)
	.cfi_restore 18
	ld	s3,216(sp)
	.cfi_restore 19
	ld	s4,208(sp)
	.cfi_restore 20
	ld	s5,200(sp)
	.cfi_restore 21
	ld	s6,192(sp)
	.cfi_restore 22
	ld	s7,184(sp)
	.cfi_restore 23
	ld	s8,176(sp)
	.cfi_restore 24
	ld	s9,168(sp)
	.cfi_restore 25
	ld	s10,160(sp)
	.cfi_restore 26
	ld	s11,152(sp)
	.cfi_restore 27
	fld	fs0,136(sp)
	.cfi_restore 40
.L2:
	ld	ra,248(sp)
	.cfi_restore 1
	addi	sp,sp,256
	.cfi_def_cfa_offset 0
	jr	ra
.L31:
	.cfi_def_cfa_offset 256
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
	ld	t3,24(sp)
	sd	s9,8(t0)
	sd	a4,16(t0)
	sd	t3,0(t0)
	sd	zero,24(t0)
	j	.L238
.L40:
	sd	zero,0(a0)
	addiw	a1,a1,1
	addi	a6,a6,4
	addi	a0,a0,8
	bgt	s6,a1,.L44
	j	.L37
.L38:
	ld	t3,32(sp)
	srl	t1,a2,t3
	or	a3,a4,t1
	j	.L39
.L23:
	lw	s9,120(sp)
	ld	t1,32(sp)
	slli	a4,a3,1
	sll	s7,a4,s9
	srl	t3,a0,t1
	or	t3,s7,t3
	j	.L24
.L21:
	lw	a5,120(sp)
	ld	a3,32(sp)
	slli	t1,s9,1
	sll	a0,t1,a5
	srl	s6,a4,a3
	or	t4,a0,s6
	j	.L22
.L19:
	lw	s9,120(sp)
	ld	t3,32(sp)
	slli	s6,t4,1
	sll	s7,s6,s9
	srl	t1,a3,t3
	or	t5,s7,t1
	j	.L20
.L17:
	lw	a4,120(sp)
	ld	s6,32(sp)
	slli	t4,t1,1
	sll	t5,t4,a4
	srl	s9,t3,s6
	or	t6,t5,s9
	j	.L18
.L254:
	li	s0,1
	li	s2,0
	j	.L10
.L252:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 23
	.cfi_restore 26
	.cfi_restore 27
	addi	a5,a5,28
	sext.w	s0,s3
	bne	a2,a5,.L5
	j	.L3
.L251:
	lw	t3,24(a5)
	sext.w	s3,a6
	sext.w	s0,t3
	bge	t3,s3,.L181
	j	.L252
.L250:
	lw	a7,20(a5)
	sext.w	a6,t2
	sext.w	s3,a7
	bge	a7,a6,.L179
	j	.L251
.L249:
	lw	a0,16(a5)
	sext.w	t2,t0
	sext.w	a6,a0
	bge	a0,t2,.L177
	j	.L250
.L248:
	lw	t1,12(a5)
	sext.w	t0,a4
	sext.w	t2,t1
	bge	t1,t0,.L175
	j	.L249
.L247:
	lw	ra,8(a5)
	sext.w	a4,a3
	sext.w	t0,ra
	bge	ra,a4,.L173
	j	.L248
.L246:
	lw	a1,4(a5)
	sext.w	a3,t6
	sext.w	a4,a1
	bge	a1,a3,.L171
	j	.L247
.L245:
	sext.w	t6,s0
	lw	s0,4(a5)
	addi	a5,a5,4
	sext.w	a3,s0
	bge	s0,t6,.L169
	j	.L246
.L46:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 23, -72
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	sub	s6,s5,s7
	addi	s8,s6,-8
	srli	s9,s8,3
	slli	a6,s0,63
	srli	s2,s11,1
	andi	t2,s9,1
	ld	s6,64(sp)
	or	s1,a6,s2
	srai	s3,s0,1
	li	s8,255
	bne	t2,zero,.L54
	ld	s4,0(s7)
	mv	a2,s11
	mv	a3,s0
	srai	a7,s4,63
	slli	t0,s4,8
	srli	t6,s4,56
	slli	t5,a7,8
	sub	t4,t0,s4
	or	ra,t6,t5
	sgtu	t3,t4,t0
	sub	a4,ra,a7
	sub	a1,a4,t3
	add	a0,t4,s1
	sltu	a5,a0,t4
	add	t1,a1,s3
	add	a1,a5,t1
	call	__divti3@plt
	bgt	a0,s8,.L220
	not	a2,a0
	srai	s9,a2,63
	and	a3,a0,s9
.L221:
	sb	a3,0(s6)
	addi	s4,s7,8
	addi	s6,s6,1
	bne	s4,s5,.L54
	j	.L51
.L261:
	li	t1,255
.L53:
	ld	s9,8(s4)
	sb	t1,0(s6)
	mv	a2,s11
	srai	a3,s9,63
	slli	a0,a3,8
	slli	s2,s9,8
	srli	t2,s9,56
	sub	a7,s2,s9
	or	a6,t2,a0
	sub	t6,a6,a3
	sgtu	t0,a7,s2
	add	a0,a7,s1
	sub	t5,t6,t0
	sltu	t4,a0,a7
	add	ra,t5,s3
	mv	a3,s0
	add	a1,t4,ra
	call	__divti3@plt
	not	a2,a0
	addi	s4,s4,8
	addi	s6,s6,1
	srai	t3,a2,63
	ble	a0,s8,.L116
	li	a4,255
.L222:
	sb	a4,0(s6)
	addi	s4,s4,8
	addi	s6,s6,1
	beq	s4,s5,.L51
.L54:
	ld	s2,0(s4)
	mv	a2,s11
	mv	a3,s0
	srai	a0,s2,63
	slli	t2,s2,8
	slli	a6,a0,8
	srli	a7,s2,56
	sub	t0,t2,s2
	or	t6,a7,a6
	sub	t4,t6,a0
	sgtu	t5,t0,t2
	add	a0,t0,s1
	sub	ra,t4,t5
	sltu	t3,a0,t0
	add	a4,ra,s3
	add	a1,t3,a4
	call	__divti3@plt
	not	a1,a0
	srai	a5,a1,63
	bgt	a0,s8,.L261
	and	t1,a0,a5
	j	.L53
.L256:
	li	a1,4
	j	.L36
.L244:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 23
	.cfi_restore 26
	.cfi_restore 27
	mv	t4,s0
	addi	a5,a5,4
	sext.w	s0,t4
	bne	a2,a5,.L5
	j	.L3
.L243:
	mv	s3,s0
	j	.L163
.L242:
	mv	a6,s0
	j	.L160
.L213:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 23, -72
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	sd	zero,8(sp)
	j	.L13
.L116:
	and	a4,a0,t3
	j	.L222
.L8:
	ld	a3,32(sp)
	li	a4,1
	li	a1,0
	sll	a0,a4,a3
	j	.L9
.L257:
	li	a0,1
	mv	s8,s0
	li	t0,1
	j	.L76
.L241:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 23
	.cfi_restore 26
	.cfi_restore 27
	mv	t0,s0
	j	.L154
.L260:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 23, -72
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	sext.w	a0,a4
	mv	s8,ra
	j	.L93
.L253:
	lw	t0,0(s9)
	addi	a4,s9,4
	addi	a3,a0,4
	subw	t1,t0,s0
	sw	t1,0(a0)
	j	.L194
.L220:
	li	a3,255
	j	.L221
.L259:
	sext.w	a0,a4
	mv	s8,t4
	j	.L90
.L240:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 23
	.cfi_restore 26
	.cfi_restore 27
	mv	a3,s0
	j	.L151
.L63:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 23, -72
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	ld	s1,32(sp)
	li	s10,0
	addiw	a6,s1,-64
	sw	a6,20(sp)
	j	.L16
.L58:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 23
	.cfi_restore 26
	.cfi_restore 27
	mv	s0,s8
	j	.L3
.L258:
	.cfi_offset 9, -24
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	.cfi_offset 23, -72
	.cfi_offset 26, -96
	.cfi_offset 27, -104
	sext.w	a0,a4
	mv	s8,t0
	j	.L81
.L67:
	ld	s11,64(sp)
	mv	a0,s11
	j	.L47
.L57:
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
	ld	s2,224(sp)
	.cfi_restore 18
	li	a0,-2
	j	.L2
.L56:
	li	a0,-1
	j	.L2
.L255:
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
	ld	a0,56(sp)
	call	free@plt
	li	a0,-4
	j	.L236
.L59:
	.cfi_restore 9
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 23
	.cfi_restore 26
	.cfi_restore 27
	ld	s0,240(sp)
	.cfi_restore 8
	ld	s2,224(sp)
	.cfi_restore 18
	ld	s3,216(sp)
	.cfi_restore 19
	ld	s6,192(sp)
	.cfi_restore 22
	ld	s8,176(sp)
	.cfi_restore 24
	ld	s9,168(sp)
	.cfi_restore 25
	fld	fs0,136(sp)
	.cfi_restore 40
	li	a0,-3
	j	.L2
	.cfi_endproc
.LFE40:
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
	.string	"N=%zu LANES=%d KMAC=%d mac_hash=%llu out_sum=%llu\n"
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
.LFB41:
	.cfi_startproc
	addi	sp,sp,-80
	.cfi_def_cfa_offset 80
	sd	s2,48(sp)
	.cfi_offset 18, -32
	la	s2,__stack_chk_guard
	ld	a5, 0(s2)
	sd	a5, 8(sp)
	li	a5, 0
	sd	s3,40(sp)
	sd	ra,72(sp)
	sd	s0,64(sp)
	sd	s1,56(sp)
	sd	s5,24(sp)
	li	a5,1
	.cfi_offset 19, -40
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	.cfi_offset 9, -24
	.cfi_offset 21, -56
	li	s3,1048576
	ble	a0,a5,.L263
	ld	a0,8(a1)
	li	a2,10
	li	a1,0
	call	strtoull@plt
	mv	s3,a0
.L263:
	slli	s5,s3,2
	mv	a0,s5
	call	malloc@plt
	mv	s0,a0
	mv	a0,s3
	call	malloc@plt
	mv	s1,a0
	beq	s0,zero,.L264
	beq	a0,zero,.L264
	sd	s4,32(sp)
	li	a4,-1640529920
	.cfi_offset 20, -48
	sext.w	s4,s3
	mv	a5,s0
	add	a0,s5,s0
	addi	a4,a4,-1607
	beq	s3,zero,.L341
	sub	a1,a0,s0
	addi	ra,a1,-4
	srli	t0,ra,2
	addi	t1,t0,1
	li	a3,1315422208
	andi	t2,t1,7
	li	a2,101
	addiw	s5,a3,1703
	beq	t2,zero,.L268
	li	a6,1
	beq	t2,a6,.L325
	li	a7,2
	beq	t2,a7,.L326
	li	t3,3
	beq	t2,t3,.L327
	li	t4,4
	beq	t2,t4,.L328
	li	t5,5
	beq	t2,t5,.L329
	li	t6,6
	bne	t2,t6,.L348
.L330:
	remuw	t0,a4,a2
	addw	a4,s5,a4
	addiw	t1,t0,-50
	sw	t1,0(a5)
	addi	a5,a5,4
.L329:
	remuw	a3,a4,a2
	addw	a4,s5,a4
	addiw	t2,a3,-50
	sw	t2,0(a5)
	addi	a5,a5,4
.L328:
	remuw	a6,a4,a2
	addw	a4,s5,a4
	addiw	a7,a6,-50
	sw	a7,0(a5)
	addi	a5,a5,4
.L327:
	remuw	t3,a4,a2
	addw	a4,s5,a4
	addiw	t4,t3,-50
	sw	t4,0(a5)
	addi	a5,a5,4
.L326:
	remuw	t5,a4,a2
	addi	a5,a5,4
	addw	a4,s5,a4
	addiw	t6,t5,-50
	sw	t6,-4(a5)
.L325:
	remuw	a1,a4,a2
	addi	a5,a5,4
	addw	a4,s5,a4
	addiw	ra,a1,-50
	sw	ra,-4(a5)
	beq	a0,a5,.L345
.L268:
	addw	t0,s5,a4
	addw	a3,s5,t0
	addw	t2,s5,a3
	addw	t1,s5,t2
	addw	a7,s5,t1
	addw	a6,s5,a7
	remuw	t3,a4,a2
	addw	a1,s5,a6
	addw	a4,s5,a1
	addi	a5,a5,32
	remuw	t5,t0,a2
	addiw	t4,t3,-50
	sw	t4,-32(a5)
	remuw	t6,a3,a2
	addiw	ra,t5,-50
	sw	ra,-28(a5)
	remuw	t0,t2,a2
	addiw	a3,t6,-50
	sw	a3,-24(a5)
	remuw	t2,t1,a2
	addiw	t1,t0,-50
	sw	t1,-20(a5)
	remuw	a7,a7,a2
	addiw	t3,t2,-50
	sw	t3,-16(a5)
	remuw	a6,a6,a2
	addiw	t5,a7,-50
	sw	t5,-12(a5)
	remuw	a1,a1,a2
	addiw	t4,a6,-50
	sw	t4,-8(a5)
	addiw	t6,a1,-50
	sw	t6,-4(a5)
	bne	a0,a5,.L268
.L345:
	li	a1,0
	li	a0,0
	sd	zero,0(sp)
	call	m5_reset_stats@plt
	mv	a1,s4
	mv	a4,sp
	mv	a3,s1
	fld	fa0,.LC5,a5
	li	a2,8
	mv	a0,s0
	call	int_softmax_mac_ilp
	mv	s5,a0
	li	a1,0
	li	a0,0
	call	m5_dump_stats@plt
	bne	s5,zero,.L272
	andi	s4,s3,7
	mv	a5,s1
	add	s5,s3,s1
	li	a6,0
	beq	s4,zero,.L271
	li	a0,1
	beq	s4,a0,.L331
	li	a2,2
	beq	s4,a2,.L332
	li	a4,3
	beq	s4,a4,.L333
	li	ra,4
	beq	s4,ra,.L334
	li	t0,5
	beq	s4,t0,.L335
	li	a3,6
	beq	s4,a3,.L336
	lbu	a6,0(s1)
	addi	a5,s1,1
.L336:
	lbu	t2,0(a5)
	addi	a5,a5,1
	add	a6,a6,t2
.L335:
	lbu	t1,0(a5)
	addi	a5,a5,1
	add	a6,a6,t1
.L334:
	lbu	a7,0(a5)
	addi	a5,a5,1
	add	a6,a6,a7
.L333:
	lbu	t3,0(a5)
	addi	a5,a5,1
	add	a6,a6,t3
.L332:
	lbu	t5,0(a5)
	addi	a5,a5,1
	add	a6,a6,t5
.L331:
	lbu	a1,0(a5)
	addi	a5,a5,1
	add	a6,a6,a1
	beq	s5,a5,.L273
.L271:
	lbu	t4,0(a5)
	lbu	t6,1(a5)
	lbu	s4,2(a5)
	lbu	a4,3(a5)
	add	a6,a6,t4
	lbu	a0,4(a5)
	add	a2,a6,t6
	lbu	ra,5(a5)
	add	t0,a2,s4
	lbu	a3,6(a5)
	add	t2,t0,a4
	lbu	t1,7(a5)
	add	a7,t2,a0
	add	t3,a7,ra
	add	t5,t3,a3
	addi	a5,a5,8
	add	a6,t5,t1
	bne	s5,a5,.L271
.L273:
	ld	a5,0(sp)
	li	a4,4
	li	a3,4
	mv	a2,s3
	lla	a1,.LC7
	li	a0,2
	call	__printf_chk@plt
	mv	a0,s0
	call	free@plt
	mv	a0,s1
	call	free@plt
	ld	s4,32(sp)
	.cfi_restore 20
	li	a0,0
.L270:
	ld	a4, 8(sp)
	ld	s2, 0(s2)
	xor	s2, a4, s2
	li	a4, 0
	bne	s2,zero,.L349
	ld	ra,72(sp)
	.cfi_restore 1
	ld	s0,64(sp)
	.cfi_restore 8
	ld	s1,56(sp)
	.cfi_restore 9
	ld	s2,48(sp)
	.cfi_restore 18
	ld	s3,40(sp)
	.cfi_restore 19
	ld	s5,24(sp)
	.cfi_restore 21
	addi	sp,sp,80
	.cfi_def_cfa_offset 0
	jr	ra
.L272:
	.cfi_def_cfa_offset 80
	.cfi_offset 1, -8
	.cfi_offset 8, -16
	.cfi_offset 9, -24
	.cfi_offset 18, -32
	.cfi_offset 19, -40
	.cfi_offset 20, -48
	.cfi_offset 21, -56
	mv	a2,s5
	lla	a1,.LC6
	li	a0,2
	call	__printf_chk@plt
	li	a0,1
	ld	s4,32(sp)
	.cfi_remember_state
	.cfi_restore 20
	j	.L270
.L348:
	.cfi_restore_state
	remuw	a1,a4,a2
	addi	a5,s0,4
	addw	a4,s5,a4
	addiw	ra,a1,-50
	sw	ra,0(s0)
	j	.L330
.L341:
	li	a1,0
	li	a0,0
	sd	zero,0(sp)
	call	m5_reset_stats@plt
	li	a1,0
	mv	a4,sp
	mv	a3,s1
	fld	fa0,.LC5,a5
	li	a2,8
	mv	a0,s0
	call	int_softmax_mac_ilp
	mv	s5,a0
	li	a1,0
	li	a0,0
	call	m5_dump_stats@plt
	bne	s5,zero,.L272
	li	a6,0
	j	.L273
.L264:
	.cfi_restore 20
	lla	a0,.LC4
	call	puts@plt
	li	a0,1
	j	.L270
.L349:
	sd	s4,32(sp)
	.cfi_offset 20, -48
	call	__stack_chk_fail@plt
	.cfi_endproc
.LFE41:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align	3
.LC0:
	.word	-17155601
	.word	1072049730
	.align	3
.LC1:
	.word	1408749273
	.word	1073063395
	.align	3
.LC2:
	.word	-68719477
	.word	1071051177
	.align	3
.LC3:
	.word	-1821066134
	.word	1070990360
	.align	3
.LC5:
	.word	1202590843
	.word	1066695393
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
