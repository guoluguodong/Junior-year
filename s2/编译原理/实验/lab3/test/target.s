	.data
	.text
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-280
	sw	s0,0(sp)
	sw	s1,4(sp)
	sw	s2,8(sp)
	sw	s3,12(sp)
	sw	s4,16(sp)
	sw	s5,20(sp)
	sw	s6,24(sp)
	sw	s7,28(sp)
	sw	s8,32(sp)
	sw	s9,36(sp)
	sw	s10,40(sp)
	sw	s11,44(sp)
	sw	ra,48(sp)
	fsw	fs0,52(sp)
	fsw	fs1,56(sp)
	fsw	fs2,60(sp)
	fsw	fs3,64(sp)
	fsw	fs4,68(sp)
	fsw	fs5,72(sp)
	fsw	fs6,76(sp)
	fsw	fs7,80(sp)
	fsw	fs8,84(sp)
	fsw	fs9,88(sp)
	fsw	fs10,92(sp)
	fsw	fs11,96(sp)
	
	call	getfloat
	fsw	fa0, 100(sp)
	flw	ft10, 100(sp)

	fmv.s	fa0,ft10
	call	putfloat
	li	a0,10
	call	putch
	
	
	li	s11,0x00000000
	fmv.w.x	ft11,s11
	sw	s11, 112(sp)
	li	s11,0x00000000
	fmv.w.x	ft11,s11
	fmv.s	fa0,ft11
	call	putfloat
	li	a0,10
	call	putch
	
	li s11,0
	sw	s11, 116(sp)
	lw	s11, 128(sp)
	#TODO
	flt.s s11,ft10,ft11

	mv	a0,s11
	call	putint
	li	a0,10
	call	putch
	
	bnez	s11,_ir_goto_label0
	j	_ir_goto_label1
_ir_goto_label0:
	li	a0,10
	call	putch
	li	s11,0x00000000
	fmv.w.x	ft11,s11
	# 这里的sw应该是fsw,且寄存器不对，fsub要改0-
	fsw	ft11, 132(sp)
	flw	ft11, 132(sp)
	# ft10没有赋值 108->100
	flw	ft10, 100(sp)

	fmv.s	fa0,ft10
	call	putfloat
	li	a0,10
	call	putch
	fmv.s	fa0,ft11
	call	putfloat
	li	a0,10
	call	putch

	fsub.s	ft9,ft11,ft10
	
	fsw	ft9, 136(sp)
	flw	ft10, 136(sp)
	fmv.s	ft9,ft10
	fsw	ft9, 112(sp)
_ir_goto_label1:
	nop
	flw	ft9, 112(sp)
	fmv.s	fa0,ft9
	addi	sp,sp,0
	call	putfloat
	addi	sp,sp,0
	addi	sp,sp,0
	call	getint
	addi	sp,sp,0
	sw	a0, 140(sp)
	lw	s10, 140(sp)
	mv	s11,s10
	sw	s11, 148(sp)
	li	s11,0
	sw	s11, 152(sp)
	li	s11,0
	sw	s11, 156(sp)
	lw	s11, 148(sp)
	lw	s10, 156(sp)
	slt	s9,s11,s10
	sw	s9, 160(sp)
	lw	s9, 160(sp)
	bnez	s9,_ir_goto_label2
	j	_ir_goto_label3
_ir_goto_label2:
	li	s9,0
	sw	s9, 164(sp)
	lw	s9, 164(sp)
	lw	s10, 148(sp)
	sub	s11,s9,s10
	sw	s11, 168(sp)
	lw	s10, 168(sp)
	mv	s11,s10
	sw	s11, 152(sp)
_ir_goto_label3:
	nop
	lw	s11, 152(sp)
	mv	a0,s11
	addi	sp,sp,0
	call	putint
	addi	sp,sp,0
	li	a0,0
	lw	s0,0(sp)
	lw	s1,4(sp)
	lw	s2,8(sp)
	lw	s3,12(sp)
	lw	s4,16(sp)
	lw	s5,20(sp)
	lw	s6,24(sp)
	lw	s7,28(sp)
	lw	s8,32(sp)
	lw	s9,36(sp)
	lw	s10,40(sp)
	lw	s11,44(sp)
	lw	ra,48(sp)
	addi	sp,sp,280
	ret
