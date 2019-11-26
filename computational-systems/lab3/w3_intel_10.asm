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
A DD 9907.08
B DD 13448.23
result DT 0

boxtitle DB "Результат работы программы", 0
msg DB "S = ", 0

.code
start:
lea ESI, A+3
inc byte ptr[ESI]
fld A
fsub B
fldln2
fdiv
fstp result

invoke FpuFLtoA, addr result, 16, addr msg+3, SRC1_REAL or SRC2_DIMM or STR_REG
invoke MessageBox, 0, addr msg, addr boxtitle, MB_OK

invoke ExitProcess, NULL
end start