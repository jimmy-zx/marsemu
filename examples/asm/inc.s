.macro	fprologue
	addi	$sp,$sp,-24	# stack (24)
	sw	$fp,16($sp)	# backup old $fp
	addi	$fp,$sp,0	# setup new $fp
	sw	$a3,36($fp)	# 36($fp) = $a3
	sw	$a2,32($fp)	# 32($fp) = $a2
	sw	$a1,28($fp)	# 28($fp) = $a1
	sw	$a0,24($fp)	# 24($fp) = $a0
	sw	$ra,20($fp)	# 20($fp) = $ra
			# 16($fp) = old $fp
.endm

.macro	fepilogue
	lw	$ra,20($fp)
	lw	$fp,16($fp)
	addi	$sp,$sp,24
.endm


# vim: set noet ts=16 sts=16 sw=16:
