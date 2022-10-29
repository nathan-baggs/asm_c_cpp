;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;         Distributed under the Boost Software License, Version 1.0.         ;;
;;            (See accompanying file LICENSE or copy at                       ;;
;;                 https://www.boost.org/LICENSE_1_0.txt)                     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .text

global _start

extern print
extern print_num
extern XOpenDisplay

_start:
    lea rdi, [hello_world]
    call print

    mov rdi, 0xabc
    call print_num

    mov rax, 0x3c
    mov rdi, 0x0
    syscall

section .rodata
    hello_world: db "hello world",0xa
