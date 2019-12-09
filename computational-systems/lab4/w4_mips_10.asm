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
	li $s0, 0 # ����
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
	li $s7, 0 # �������
	sub_loop:
		li $s6, 0 # ��� �������� �������������� ���������� ���������
		
		lbu $t1, 0($s1) # ��������� 2 ����� ������������
		lbu $t2, 0($s2) # ��������� 2 ����� �����������
		
		and $t1, $t1, 0x0F # ��������� ������� ������� ������������
		and $t2, $t2, 0x0F # ��������� ������� ������� �����������
		
		addu $t2, $t2, $s0 # ���������� ���� � ����������
		
		li $s0, 0 # �������� ����
		
		bge $t1, $t2, nocorr # ���������, ����� �� �������
			li $s0, 1 # ���������� ����
			addu $t1, $t1, 10  # ���� +16, ��������� -6, � ����� +10
		nocorr:
		subu $t3, $t1, $t2 # ���������
		# ������ � $t3 ���������� ����� ������� �������, � � $s0 ���� �� �������
		or $s6, $s6, $t3 # ��������� ���������� ����� ������� ������� � $s6

		# �������� �� ��������� ������...
		lbu $t1, 0($s1)
		lbu $t2, 0($s2)
				
		ror $t1, $t1, 4
		and $t1, $t1, 0x0F
		ror $t2, $t2, 4
		and $t2, $t2, 0x0F
		
		addu $t2, $t2, $s0 # ���������� ���� � ����������
		
		li $s0, 0 # �������� ����
		
		bge $t1, $t2, nocorr_2 # ���������, ����� �� �������
			li $s0, 1
			addu $t1, $t1, 10  # ���� +16, ��������� -6, � ����� +10
		nocorr_2:
		subu $t3, $t1, $t2 # ���������
		# ������ � $t3 ���������� ����� ������� �������, � � $s0 ���� �� ���������		
		rol $t3, $t3, 4 # ����� ����� ������� ������� �� �����
		or $s6, $s6, $t3 # ��������� ���������� ����� ������� ������� � $s6				
										
		# ���������� ���������� ��������� ���������
		sb $s6, 0($s3)
		
		# ����������� ������ �� 1 ����
		addu $s1, $s1, 1
		addu $s2, $s2, 1
		addu $s3, $s3, 1
		
		# ����������� �������
		addu $s7, $s7, 1
	bne $s7, 4, sub_loop
	
	add $t7, $t7, 1
	# second_step ��� ��������� bcd3
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
