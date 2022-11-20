;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;         Distributed under the Boost Software License, Version 1.0.          ;;
;;            (See accompanying file LICENSE or copy at                        ;;
;;                 https://www.boost.org/LICENSE_1_0.txt)                      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global assert_not_null
global assert_null
global exit
global game_malloc
global game_mmap
global get_time
global print
global print_num
global sleep_ms

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This file contains various utilities.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Print a string to STDOUT.
;
; @param rdi
;   Address of null terminated string to print.
;
; @returns
;   Number of bytes output.
;
print:
    push rbp
    mov rbp, rsp

    mov r10, rdi ; save off string address
    mov r9, rdi ; iterator register for string
    movzx rax, byte [r9] ; load first byte
    mov rcx, 0x0 ; accumulator for string length

count_null_start:
    cmp rax, 0x0 ; are we at the null byte?
    je count_null_end

    inc rcx ; increment accumulator
    inc r9 ; move to next byte
    movzx rax, byte [r9] ; load byte
    jmp count_null_start 

count_null_end:
    ; syscall to write string to stdout
    mov rax, 0x1
    mov rdi, 0x1;
    mov rsi, r10
    mov rdx, rcx
    syscall

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Print an integer to STDOUT (with a new line).
;
; @param rdi
;   Unsigned number to print.
;
print_num:
    push rbp
    mov rbp, rsp
    sub rsp, 0x15 ; get stack space for two buffers, one to convert the number into a string (in reverse order)]
                  ; and another to reverse the string into

    mov r9, rdi ; r9 will be the mutable copy of the input number
    mov rdx, rdi
    mov rcx, 0x0 ; accumulator for string length

    mov rdi, rsp ; set destination as start of first buffer

write_loop_start:

    mov rax, r9 ; load number to be divided
    mov rdx, 0 ; zero out remainder
    mov r10, 0xa ; divisor by 10
    div r10 ; rdx = remainder, rax = quotient

    mov r11, 0x30
    add rdx, r11 ; add 0x30 to remainder to get ASCII code for number

    mov [rdi], dl ; write ASCII byte to buffer
    inc rcx ; increment how many characters have been written

    cmp rax, 0x0 ; check if we have reached the end
    je write_loop_end
    
    inc rdi
    mov r9, rax ; load remainder into r9 so it can be divided again
    jmp write_loop_start

write_loop_end:

    mov rsi, rdi ; move destination into source so we can reverse it
    mov rdi, rsp
    add rdi, 0xa ; set destination to start of second buffer

reverse_loop_start:
    cmp rcx, 0x0
    je reverse_loop_end

    movzx rax, byte[rsi]
    mov [rdi], al ; write ascii character into second buffer
    dec rsi
    inc rdi

    dec rcx; ; keep track of how many characters we've reversed
    jmp reverse_loop_start
reverse_loop_end:

    mov rax, 0xa
    mov [rdi], al ; write newline character into the end

    mov rdi, rsp
    add rdi, 0xa ; print second buffer
    call print

    leave
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Assert the input is not null.
;
; @param rdi
;   Value to check is not null.
;
; @param rsi
;   Pointer to error message string.
;
assert_not_null:
    push rbp
    mov rbp, rsp

    cmp rdi, 0x0
    jne assert_not_null_end

    mov rdi, rsi
    call print

    mov rdi, 0x1
    call exit

assert_not_null_end:

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Assert the input is null.
;
; @param rdi
;   Value to check is null.
;
; @param rsi
;   Pointer to error message string.
;
assert_null:
    push rbp
    mov rbp, rsp

    cmp rdi, 0x0
    je assert_null_end

    mov rdi, rsi
    call print

    mov rdi, 0x1
    call exit

assert_null_end:

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Exit the program.
;
; @param rdi
;   Exit code.
;
exit:
    mov rax, 0x3c
    syscall

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Sleep the current process for the supplied number of milliseconds.
;
; @param rdi
;   Number of milliseconds to sleep for.
;
sleep_ms:
    push rbp
    mov rbp, rsp

    imul rdi, rdi, 1000000 ; convert supplied ms to ns
    xor rax, rax 
    ; recreate struct timepec on the stack
    push rdi ; tv_nsec
    push rax ; tv_sec

    ; nanosleep syscall
    mov rax, 0x23
    mov rdi, rsp
    mov rsi, 0x0
    syscall

    add rsp, 16

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Get the time since epoch in milliseconds.
;
; @returns
;   Milliseconds since epoch.
;
get_time:
    push rbp
    mov rbp, rsp

    ; create empty timeval struct on the stack
    push 0x0 ; tv_usec
    push 0x0 ; tv_sec

    ; gettimeofday syscall
    mov rax, 0x60
    mov rdi, rsp
    mov rsi, 0x0
    syscall

    pop rax ; seconds
    pop rbx ; microseconds

    imul rax, rax, 1000000 ; convert seconds to microseconds
    add rax, rbx ; add microseconds
    mov rdx, 0x0
    mov rbx, 1000
    div rbx ; convert to milliseconds

    pop rbp
    ret
