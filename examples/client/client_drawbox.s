.extern	plot_init
.extern	plot_deinit
.extern	plot_draw
.extern	plot_mem

.text

.global	main
main:
	addi	$sp,$sp,-4
	sw	$ra,0($sp)
	addi	$sp,$sp,-16
	jal	plot_init
	li	$a0,1
	li	$a1,1
	li	$a2,0xffffff
	jal	plot_draw
	jal	plot_deinit
	addi	$sp,$sp,16
	lw	$ra,0($sp)
	addi	$sp,$sp,4
	jr	$ra

# vim: set noet ts=16 sts=16 sw=16:
