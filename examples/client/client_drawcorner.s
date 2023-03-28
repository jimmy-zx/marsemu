# an example to use client/plot

.text

.globl	main
main:
	# Initialize a stack frame of 24.
	# Note that the last 16 bytes must be reserved according to SysV ABI
	addi	$sp,$sp,-24
	sw	$ra,20($sp)
	sw	$fp,16($sp)
	move	$fp,$sp

	# plot_init() - Initialize the client library
	jal	plot_init

	# draw the top edge
	# plot_draw(1, 1, 0xffffffff)
	li	$a0,1
	li	$a1,1
	li	$a2,0xffffff
	jal	plot_draw

	# plot_draw(2, 1, 0xffffffff)
	li	$a0,2
	li	$a1,1
	li	$a2,0xffffff
	jal	plot_draw
	# plot_draw(3, 1, 0xffffffff)
	li	$a0,3
	li	$a1,1
	li	$a2,0xffffff
	jal	plot_draw
	# plot_draw(4, 1, 0xffffffff)
	li	$a0,4
	li	$a1,1
	li	$a2,0xffffff
	jal	plot_draw

	# draw the other edge
	li	$a0,1
	li	$a1,2
	li	$a2,0xffffff
	jal	plot_draw
	li	$a0,1
	li	$a1,3
	li	$a2,0xffffff
	jal	plot_draw
	li	$a0,1
	li	$a1,4
	li	$a2,0xffffff
	jal	plot_draw

	# plot_deinit() - Deinitialize the client library
	jal	plot_deinit

	# Restore the stack frame and return
	lw	$fp,16($sp)
	lw	$ra,20($sp)
	addi	$sp,$sp,24
	jr	$ra
