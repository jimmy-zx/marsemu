.macro	fprol ssize=24
	addiu	$sp,$sp,-\ssize
	sw	$a3,\ssize+12($sp)
	sw	$a2,\ssize+8($sp)
	sw	$a1,\ssize+4($sp)
	sw	$a0,\ssize+0($sp)
	sw	$ra,\ssize-4($sp)
	sw	$fp,\ssize-8($sp)
	move	$fp,$sp
.endm

.macro	fepil ssize=24
	move	$sp,$fp
	lw	$ra,\ssize-4($sp)
	lw	$fp,\ssize-8($sp)
	addiu	$sp,$sp,\ssize
.endm

.macro	savesr ssize
	sw	$s7,\ssize-12($sp)
	sw	$s6,\ssize-16($sp)
	sw	$s5,\ssize-20($sp)
	sw	$s4,\ssize-24($sp)
	sw	$s3,\ssize-28($sp)
	sw	$s2,\ssize-32($sp)
	sw	$s1,\ssize-36($sp)
	sw	$s0,\ssize-40($sp)
.endm

.macro	loadsr ssize
	lw	$s7,\ssize-12($sp)
	lw	$s6,\ssize-16($sp)
	lw	$s5,\ssize-20($sp)
	lw	$s4,\ssize-24($sp)
	lw	$s3,\ssize-28($sp)
	lw	$s2,\ssize-32($sp)
	lw	$s1,\ssize-36($sp)
	lw	$s0,\ssize-40($sp)
.endm

# vim: set noet ts=8 sts=8 sw=8:
