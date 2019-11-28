.data
one: .float 1
a: .float 9909.23
S: .float 0
str: .asciiz "S = "

.text
main:
lwc1 $f1, one # загружаем в $f1 единицу

lwc1 $f0, a # загружаем в $f0 наше исходное число A
sqrt.s $f0, $f0 # вычисляем sqrt(A)
add.s $f0, $f0, $f1 # добавляем 1
swc1 $f0, S # загружаем в переменную S наш результат

# выводим "S = "
li $v0, 4
la $a0, str
syscall

# выводим число
lwc1 $f12, S
li $v0, 2
syscall
