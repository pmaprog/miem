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
A DD 9909.23
result DT 0

boxtitle DB "Результат работы программы", 0
msg DB "S = ", 0

.code
start:
fld A
fsqrt
fld1
fadd
fstp result

invoke FpuFLtoA, addr result, 5, addr msg+3, SRC1_REAL or SRC2_DIMM or STR_REG
invoke MessageBox, 0, addr msg, addr boxtitle, MB_OK

invoke ExitProcess, NULL
end start