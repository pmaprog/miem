.data	
	bcd1: .byte 0x80 0x20 0x02 0x47
	bcd2: .byte 0x00 0x00 0x30 0x40
	bcd3: .byte 0x00 0x40 0x01 0x40
	bcd4: .byte 0 0 0 0
	
	strbcd1: .asciiz "BCD1 = "
	strbcd2: .asciiz ", BCD2 = "
	strbcd3: .asciiz ", BCD3 = "
	msg: .asciiz "\nBCD4 = BCD1 - BCD2 - BCD3 = "

.text
	li $s0, 0 # заем
	li $t7, 0
	la $s1, bcd1
	la $s2, bcd2
	la $s3, bcd4
	j first_step
	
	second_step:
	la $s1, bcd3
	la $s2, bcd4
	la $s3, bcd4
	
	first_step:
	# $s1 - $s2 = $s3
	li $s7, 0 # счетчик
	sub_loop:
		li $s6, 0 # для хранения промежуточного результата вычитания
		
		lbu $t1, 0($s1) # загружаем 2 цифры уменьшаемого
		lbu $t2, 0($s2) # загружаем 2 цифры вычитаемого
		
		and $t1, $t1, 0x0F # оставляем младшую тетраду уменьшаемого
		and $t2, $t2, 0x0F # оставляем младшую тетраду вычитаемого
		
		addu $t2, $t2, $s0 # складываем заем и вычитаемое
		
		li $s0, 0 # обнуляем заем
		
		bge $t1, $t2, nocorr # проверяем, можно ли вычесть
			li $s0, 1 # запоминаем заем
			addu $t1, $t1, 10  # заем +16, коррекция -6, в итоге +10
		nocorr:
		subu $t3, $t1, $t2 # вычитание
		# теперь в $t3 правильная цифра младшей тетрады, а в $s0 заем из старшей
		or $s6, $s6, $t3 # перепишем правильную цифру младшей тетрады в $s6

		# работаем со следующей цифрой...
		lbu $t1, 0($s1)
		lbu $t2, 0($s2)
				
		ror $t1, $t1, 4
		and $t1, $t1, 0x0F
		ror $t2, $t2, 4
		and $t2, $t2, 0x0F
		
		addu $t2, $t2, $s0 # складываем заем и вычитаемое
		
		li $s0, 0 # обнуляем заем
		
		bge $t1, $t2, nocorr_2 # проверяем, можно ли вычесть
			li $s0, 1
			addu $t1, $t1, 10  # заем +16, коррекция -6, в итоге +10
		nocorr_2:
		subu $t3, $t1, $t2 # вычитание
		# теперь в $t3 правильная цифра старшей тетрады, а в $s0 заем из следующей		
		rol $t3, $t3, 4 # сдвиг цифры старшей тетрады на место
		or $s6, $s6, $t3 # перепишем правильную цифру старшей тетрады в $s6				
										
		# записываем правильный результат вычитания
		sb $s6, 0($s3)
		
		# увеличиваем адреса на 1 байт
		addu $s1, $s1, 1
		addu $s2, $s2, 1
		addu $s3, $s3, 1
		
		# увеличиваем счетчик
		addu $s7, $s7, 1
	bne $s7, 4, sub_loop
	
	add $t7, $t7, 1
	# second_step это вычитание bcd3
	beq $t7, 1, second_step

 	# bcd1
	li $v0, 4
	la $a0, strbcd1
	syscall
	li $v0, 11
	li $s7, 3
	print_bcd1:
		lb $a0, bcd1($s7)
		ror $a0, $a0, 4
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall
		lb $a0, bcd1($s7)
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall		
		subu $s7, $s7, 1
	bgez $s7, print_bcd1
	
	# bcd2
	li $v0, 4
	la $a0, strbcd2
	syscall
	li $v0, 11
	li $s7, 3
	print_bcd2:
		lb $a0, bcd2($s7)
		ror $a0, $a0, 4
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall
		lb $a0, bcd2($s7)
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall		
		subu $s7, $s7, 1
	bgez $s7, print_bcd2
		
	# bcd3
	li $v0, 4
	la $a0, strbcd3
	syscall
	li $v0, 11
	li $s7, 3
	print_bcd3:
		lb $a0, bcd3($s7)
		ror $a0, $a0, 4
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall
		lb $a0, bcd3($s7)
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall		
		subu $s7, $s7, 1
	bgez $s7, print_bcd3

	# bcd4
	li $v0, 4
	la $a0, msg
	syscall
	li $v0, 11
	li $a0, 0x2d
	syscall
	li $s7, 3
	print_bcd4:
		lb $a0, bcd4($s7)
		ror $a0, $a0, 4
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall
		lb $a0, bcd4($s7)
		and $a0, $a0, 0x0f
		or $a0, $a0, 0x30
		syscall		
		subu $s7, $s7, 1
	bgez $s7, print_bcd4
