.macro	fprol
	addiu	$sp,$sp,-24	# allocate a minimal stack (16+2*4)
	sw	$ra,20($sp)
	sw	$fp,16($sp)
	move	$fp,$sp
	sw	$a3,36($sp)	# save $a*
	sw	$a2,32($sp)
	sw	$a1,28($sp)
	sw	$a0,24($sp)
.endm

.macro	fepil
	move	$sp,$fp
	lw	$ra,20($sp)
	lw	$fp,16($sp)
	addiu	$sp,$sp,24
.endm

.macro	savesr
	addiu	$sp,$sp,32	# allocate stack for $s* (8*4)
	move	$fp,$sp
	sw	$s7,44($sp)
	sw	$s6,40($sp)
	sw	$s5,36($sp)
	sw	$s4,32($sp)
	sw	$s3,28($sp)
	sw	$s2,24($sp)
	sw	$s1,20($sp)
	sw	$s0,16($sp)
.endm

.macro	loadsr
	lw	$s7,44($sp)
	lw	$s6,40($sp)
	lw	$s5,36($sp)
	lw	$s4,32($sp)
	lw	$s3,28($sp)
	lw	$s2,24($sp)
	lw	$s1,20($sp)
	lw	$s0,16($sp)
	addiu	$sp,$sp,-32
	move	$fp,$sp
.endm


# vim: set noet ts=16 sts=16 sw=16:
