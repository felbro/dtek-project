  # labwork.S
  # Written 2015 by F Lundevall
  # Edited 2016 by Felix Broberg
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  # You can add your own code here
  #

	.data
	.align 2

	.text

.global hexasc
hexasc:
	move $v0, $a0		# copy value from a0 to v0
	andi $v0,$v0,0xf	# keeps the 4 least significant figures. Discards the rest
	ble $v0,9,if		# goes to if: if v0 <= 9
	nop
	addi $v0,$v0,0x37	# if v0 > 9, 0x37 is added

	j ret			# jumps to ret: if v0 > 9
	nop
if:	addi $v0, $v0, 0x30	# adds 0x30 to the value in v0

ret:	andi $v0,$v0,0x7f	# keeps the 7 lest significant figures. Discards the rest
	jr $ra			# returns to main
	nop


.global time2string
time2string:
	PUSH $ra		#Save $ra (return adress) and $s0
	PUSH $s0
	move	$s0, $a0

	move	$t1, $a1
	andi	$t1,0xffff
	beq	$t1,$0,hour


	srl	$a0,$a1,12	#First number (as in if $a0 = 0x1234, 0x1 is chosen)
	jal hexasc
	nop
	sb	$v0,0($s0)

	srl	$a0,$a1,8	#second number (as in if $a0 = 0x1234, 0x12 is chosen. hexasc makes this 0x2
	jal hexasc
	nop
	sb	$v0,1($s0)

	li	$t0, 0x3A	#colon :
	sb	$t0,2($s0)

	srl	$a0,$a1,4	#third number
	jal hexasc
	nop
	sb	$v0,3($s0)

	move	$a0,$a1		#fourth number
	jal hexasc
	nop
	sb	$v0,4($s0)

	li	$t0, 0x00	#null
	sb	$t0,5($s0)
	j theend

hour:	li 	$t0,0x48
	sb	$t0,0($s0)

	li	$t0,0x4f
	sb	$t0,1($s0)

	li	$t0,0x55
	sb	$t0,2($s0)

	li	$t0,0x52
	sb	$t0,3($s0)

	li	$t0,0x00
	sb	$t0,4($s0)

theend:
	POP	$s0
	POP	$ra
	jr	$ra
	nop

.global delay
delay:
	blez	$a0,jumpback
	nop
	PUSH $s0
	PUSH $s1

	move	$s0, $a0	# $s0 = $a0

delayloop:				#While loop
	ble	$s0,$0,delaycont	# $s0 != 0, do the while-loop
	nop
	addi	$s0,$s0,-1		# decrement $s0 by 1.

	li	$s1,0		#int i = 0
	li	$t0, 11500 	# i < 4711 (changed to i < 100 since that suited the $a0 = 1000 => wait-time = 1000ms

forloop:				#foorloop
	slt	$t1,$s1,$t0

	beq	$t1,$0,done
	nop
	addi	$s1,$s1,1
	j	forloop
	nop

done:
	j delayloop
	nop

delaycont:
	POP	$s1
	POP	$s0
jumpback:
	jr	$ra
	nop


.global enable_interrupt
enable_interrupt:
  ei
  jr $ra
  nop

# The effect of .global *label* is to make it visible to outside sources. This enables us to call on the assembler code functions through a c-lang program
