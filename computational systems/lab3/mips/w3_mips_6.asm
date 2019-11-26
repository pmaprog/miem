.data
one: .float 1
a: .float 9909.23
S: .float 0
str: .asciiz "S = "

.text
main:
l.s $f1, one

lwc1 $f0, a
sqrt.s $f0, $f0      # sqrt(A)
add.s $f0, $f0, $f1  # sqrt(A) + 1
swc1 $f0, S

# print "S = "
li $v0, 4
la $a0, str
syscall

# print $f12 value
lwc1 $f12, S
li $v0, 2
syscall