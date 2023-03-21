.text

.globl	main
main:
	li	$t0,0
loop:
	addi	$t0,$t0,1
	blt	$t0,0x10000,loop
	li	$v0,0
	li	$v1,0
	# jr	$ra
	li	$v0,10
	syscall
