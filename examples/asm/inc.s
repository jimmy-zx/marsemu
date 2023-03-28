.macro	fprologue
	addiu	$sp,$sp,-24	# allocate a minimal stack (16+2*4)
	sw	$ra,20($sp)
	sw	$fp,16($sp)
	move	$fp,$sp
	sw	$a3,36($sp)	# save $a*
	sw	$a2,32($sp)
	sw	$a1,28($sp)
	sw	$a0,24($sp)
.endm

.macro	fepilogue
	move	$sp,$fp
	lw	$ra,20($sp)
	lw	$fp,16($sp)
	addiu	$sp,$sp,24
.endm


# vim: set noet ts=16 sts=16 sw=16:
