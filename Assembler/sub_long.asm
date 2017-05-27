extern scanf
extern putchar

%macro GET_CHAR_m 1.nolist
				jmp %%after_data
section .data
		%%fmt db "%c", 0 ; db - 1 байт
		%%read_tmp db 0
		%%tmp dq 0 ; dq - 8 байт
section .text
%%after_data:
				pushfq ; флаги на стек
				push rax
				push rcx
				push rdx
				push r8
				push r9
				push r10
				push r11
				mov rdx, %%read_tmp
				mov rcx, %%fmt
				call scanf
				pop r11
				pop r10
				pop r9
				pop r8
				pop rdx
				pop rcx
				pop rax
				popfq
				movzx %1, byte [%%read_tmp] ; Копирование с нулевым расширением
%endmacro


%macro PRINT_CHAR_m 1.nolist
				jmp %%after_data
section .data
		%%tmp dq 0
section .text
				%%after_data:
				mov qword[%%tmp], rax    
				movzx rax, %1
				pushfq
				push qword[%%tmp]
				push rcx
				push rdx
				push r8
				push r9
				push r10
				push r11
				and rax, 0xff
				mov rcx, rax
				call putchar
				pop r11
				pop r10
				pop r9
				pop r8
				pop rdx
				pop rcx
				pop rax
				popfq
%endmacro

section .text
    global main
main:
                mov             rbp, rsp; for correct debugging
                mov             r15, rsp

                sub             rsp, 2 * 128 * 8
                lea             rdi, [rsp + 128 * 8] ; mov + вычисления
                mov             rcx, 128
                call            read_long
                mov             rdi, rsp
                call            read_long
                lea             rsi, [rsp + 128 * 8]
                call            sub_long_long

                call            write_long

                mov             al, 0x0a ; '\n'
                call            write_char
                
                add             rsp, 2 * 128 * 8
                xor             rax, rax
				
                ret
exit:
                xor             rax, rax
				mov             rsp, r15
                ret

; subs two long number
;    rdi -- address of #1 (long number)
;    rsi -- address of #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sub is written to rdi
sub_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                sbb             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret
                
; adds two long number
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                adc             [rdi], rax
                lea             rdi, [rdi++++++++++ + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret
                
; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx, rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; inverse long number
;    rdi -- address of #1 (long number)
;    rcx -- length of long number in qwords
; result:
;    inv is written to rdi
inv_long:
                push            rdi
                push            rcx
                push            r8

.loop:
                mov             r8, [rdi]
                not             r8
                mov             [rdi], r8
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             r8
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx

                xor             rsi, rsi
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [rdi], rax
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep 			stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep 			scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short
                call            add_long_short
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop
                
                

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            r8
                
                mov             r8, [rdi + rcx * 8 - 8]
                cmp             r8, 0xffffffffffffffff ;flag = -1
                mov             r8, 1
                
                jnz             .minus
                mov             r8, 0
                mov             al, '-'
                call            write_char
                
                mov             rax, 1
                call            inv_long
                call            add_long_short
.minus:         
                call            write_unslong
                
                pop             r8
                pop             rax
                ret

; write unlong number to stdout
;    rdi -- argument (unlong number)
;    rcx -- length of unlong number in qwords
write_unslong:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret
                
; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                GET_CHAR_m        rax
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                PRINT_CHAR_m      al
                ret
                
; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push             r8
                push             rax
                push             rsi
                
                mov              r8, rdx
.loop1:
                
                mov              al, [rsi]
                call             write_char
                dec              r8
                inc              rsi
                cmp              r8, 0
                JNE              .loop1
                
                pop              rsi
                pop              rax
                pop              r8
                ret


                section         .rodata
invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg