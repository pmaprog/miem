.586
.xmm
.model flat, stdcall
option casemap :none

include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib

.data
data dd 87361.8641, 2.58361594, 0.00008, 753.85638163,
         85731.1111, 869.0, 3030.1927, 88.0

bcd1 db 4 dup(0)
bcd2 db 4 dup(0)
bcd3 db 4 dup(0)
res db 4 dup(0)
bcd4 db 4 dup(0)
sign db 0

boxtitle DB "Результат работы программы", 0
msg DB "BCD1 = XXXXXXXX", 10, "BCD2 = XXXXXXXX", 10, "BCD3 = XXXXXXXX", 10, "BCD4 = SXXXXXXXX", 0

.code
start:

mov dword ptr[bcd1], 47022080h
mov dword ptr[bcd2], 40300000h
mov dword ptr[bcd3], 40014000h

jmp here


movaps XMM0, data
movaps XMM1, data+16

andps XMM0, XMM1 ; XMM0 = XMM0 & XMM1

movss bcd1, XMM0

; i = 3
shufps XMM0, XMM0, 00100111b
movss bcd2, XMM0

; j = 1
shufps XMM0, XMM0, 11100001b
movss bcd3, XMM0
;jmp here
; процесс обнуления запрещенных комбинаций
mov ecx, 12
xor esi, esi
zeroing:
    mov dh, bcd1[esi]
    mov dl, bcd1[esi]

    and dh, 11110000b
    and dl, 00001111b

    cmp dl, 9
    jle dl_less
    xor dl, dl
    dl_less:

    ror dh, 4 ; сдвигаем вправо на 4 бита
    cmp dh, 9 ; сравниваем с 9
    jle dh_less
    xor dh, dh ; обнуляем, т.к. значение > 9
    dh_less:
    rol dh, 4 ; сдвигаем влево на 4 бита

    or dh, dl ; делаем слияние
    mov bcd1[esi], dh ; результат обработки помещаем обратно в наше bcd число

    inc esi ; переходим к следующей цифре
    loop zeroing

here:

; сравниваем bcd1 и bcd2
mov esi, 3
cmp_bcd1_bcd2:
    mov dl, bcd1[esi]
    mov dh, bcd2[esi]
    cmp dl, dh
    jg bcd1_greater
    jl bcd1_less
    dec esi
    jmp cmp_bcd1_bcd2

; если bcd2 больше, то меняем переменные местами
bcd1_less:
mov sign, 1
mov eax, dword ptr[bcd1]
mov ebx, dword ptr[bcd2]
mov dword ptr[bcd1], ebx
mov dword ptr[bcd2], eax

; вычитаем от bcd1 число bcd2
bcd1_greater:
mov ecx, 4
xor esi, esi
clc
bcd1_minus_bcd2:
    mov al, bcd1[esi]
    sbb al, bcd2[esi]
    das
    mov res[esi], al
    inc esi
    loop bcd1_minus_bcd2

; теперь от результа прошлой операции отнимаем bcd3
; выполняем такие же действия
mov esi, 3
cmp_res_bcd3:
    mov dl, res[esi]
    mov dh, bcd3[esi]
    cmp dl, dh
    jg res_greater
    jl res_less
    dec esi
    jmp cmp_res_bcd3

res_less:
mov sign, 1
mov eax, dword ptr[res]
mov ebx, dword ptr[bcd3]
mov dword ptr[res], ebx
mov dword ptr[bcd3], eax

res_greater:
mov ecx, 4
xor esi, esi
clc
res_minus_bcd3:
    mov al, res[esi]
    sbb al, bcd3[esi]
    das
    mov bcd4[esi], al
    inc esi
    loop res_minus_bcd3

; вывод на экран результата вычислений

mov ecx, 4
xor eax, eax
print_bcd1:
    mov dl, bcd1[ecx-1]
    mov dh, bcd1[ecx-1]
    and dl, 00001111b
    and dh, 11110000b
    ror dh, 4
    or dl, 30h
    or dh, 30h
    mov msg[7+eax], dh
    mov msg[8+eax], dl
    add eax, 2
    dec esi
    loop print_bcd1

mov ecx, 4
xor eax, eax
print_bcd2:
    mov dl, bcd2[ecx-1]
    mov dh, bcd2[ecx-1]
    and dl, 00001111b
    and dh, 11110000b
    ror dh, 4
    or dl, 30h
    or dh, 30h
    mov msg[23+eax], dh
    mov msg[24+eax], dl
    add eax, 2
    dec esi
    loop print_bcd2

mov ecx, 4
xor eax, eax
print_bcd3:
    mov dl, res[ecx-1]
    mov dh, res[ecx-1]
    and dl, 00001111b
    and dh, 11110000b
    ror dh, 4
    or dl, 30h
    or dh, 30h
    mov msg[39+eax], dh
    mov msg[40+eax], dl
    add eax, 2
    dec esi
    loop print_bcd3

mov msg[55], 2Bh
cmp sign, 1
jne plus
mov msg[55], 2Dh
plus:

mov ecx, 4
xor eax, eax
print_bcd4:
    mov dl, bcd4[ecx-1]
    mov dh, bcd4[ecx-1]
    and dl, 00001111b
    and dh, 11110000b
    ror dh, 4
    or dl, 30h
    or dh, 30h
    mov msg[56+eax], dh
    mov msg[57+eax], dl
    add eax, 2
    dec esi
    loop print_bcd4

invoke MessageBox, 0, addr msg, addr boxtitle, MB_OK

invoke ExitProcess, NULL
end start