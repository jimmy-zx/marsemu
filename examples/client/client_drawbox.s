.extern	plot_init
.extern	plot_deinit
.extern	plot_draw
.extern	plot_mem

.text

.global	main
main:
	move	$s0,$ra
	jal	plot_init
	li	$a0,1
	li	$a1,1
	li	$a2,0xffffff
	jal	plot_draw
	jal	plot_deinit
	move	$ra,$s0
	jr	$ra

# vim: set noet ts=16 sts=16 sw=16:
