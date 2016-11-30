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
.global enable_interrupt
enable_interrupt:
ei
jr $ra
nop

# The effect of .global *label* is to make it visible to outside sources. This enables us to call on the assembler code functions through a c-lang program
