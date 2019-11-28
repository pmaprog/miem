.386
.model flat, stdcall
option casemap :none

include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
include \masm32\include\fpu.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\fpu.lib

.data
A DD 9909.23 ; 4-байтовое исходное число
result DT 0 ; 10-байтовое число, здесь будем хранить результат

boxtitle DB "Результат работы программы", 0
msg DB "S = ", 0

.code
start:
fld A ; Загружаем в стек FPU число A
fsqrt ; Вычисляем корень из верхушки стека, т.е. sqrt(А)
fld1 ; Загружаем в стек число 1
fadd ; Складываем ST + ST(1) (т.е. sqrt(A) + 1)
fstp result ; Выгружаем число из стека в переменную

; Функция для вывода ЧПЗ на экран. В результате работы функции,
; в строковую переменную msg поместится наш результат
invoke FpuFLtoA, addr result, 5, addr msg+3, SRC1_REAL or SRC2_DIMM or STR_REG

; Выводим на экран окно с сообщением
invoke MessageBox, 0, addr msg, addr boxtitle, MB_OK

invoke ExitProcess, NULL
end start