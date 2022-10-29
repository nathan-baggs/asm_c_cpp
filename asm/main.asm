;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;         Distributed under the Boost Software License, Version 1.0.         ;;
;;            (See accompanying file LICENSE or copy at                       ;;
;;                 https://www.boost.org/LICENSE_1_0.txt)                     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .text

global _start

extern print
extern print_num
extern assert_not_null
extern exit
extern XOpenDisplay

_start:
    lea rdi, [hello_world]
    call print

    mov rdi, 0x0
    call XOpenDisplay
    mov rdi, rax
    lea rsi, [x_open_display_failed]
    call assert_not_null
        
    lea rdi, [goodbye]
    call print

    mov rdi, 0x0
    call exit

section .rodata
    hello_world: db "hello world", 0xa, 0x0
    goodbye: db "goodbye", 0xa, 0x0
    x_open_display_failed: db "XOpenDisplay failed", 0xa, 0x0
