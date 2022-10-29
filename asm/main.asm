;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;         Distributed under the Boost Software License, Version 1.0.         ;;
;;            (See accompanying file LICENSE or copy at                       ;;
;;                 https://www.boost.org/LICENSE_1_0.txt)                     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .text

global _start

_start:
    mov rax, 0x1
    mov rdi, 0x1;
    lea rsi, [hello_world]
    mov rdx, 0xc
    syscall

    mov rax, 0x3c
    mov rdi, 0x0
    syscall

section .rodata
    hello_world: db "hello world",0xa
