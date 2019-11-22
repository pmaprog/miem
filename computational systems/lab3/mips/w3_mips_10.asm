	.data
aa:	.float 9907.08
bb:	.float 13448.23
ln2:	.float 0.693147180559945
four:	.float 4.0
res:	.float 0.0
head_A:	.asciiz "\n A = \0"
head_B: .asciiz "\n B = \0"
head_RES:	.asciiz "\n S = A*4-B/ln2 = \0"


	.text
		lwc1 $f1, aa
		lwc1 $f12, aa
		
		la $a0, head_A
		li $v0, 4
		syscall
		li $v0, 2
		syscall
		
		lwc1 $f2, bb
		lwc1 $f12, bb
		
		la $a0, head_B
		li $v0, 4
		syscall
		li $v0, 2
		syscall
		
		lwc1 $f3, four
		lwc1 $f4, ln2
		mul.s $f11, $f1, $f3
		sub.s $f10, $f11, $f2
		div.s $f12, $f10, $f4
		swc1 $f12, res
		
		la $a0, head_RES
		li $v0, 4
		syscall
		
		li $v0, 2
		syscall
		
		
