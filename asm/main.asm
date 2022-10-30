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
extern assert_null
extern exit
extern XOpenDisplay
extern XDefaultScreen
extern XBlackPixel
extern XCreateSimpleWindow
extern XDefaultRootWindow
extern XSelectInput
extern XMapWindow
extern XCreateGC
extern XNextEvent

_start:

    lea rdi, [hello_world]
    call print

    mov rdi, 0x0
    call XOpenDisplay
    mov [display], rax
    mov rdi, rax
    lea rsi, [x_open_display_failed]
    call assert_not_null

    call XDefaultScreen
    mov [screen_number], rax
    mov rdi, rax
    call print_num

    mov rdi, [display]
    mov rsi, [screen_number]
    call XBlackPixel
    mov [black_colour], rax

    mov rdi, [display]
    call XDefaultRootWindow
    mov [default_root_window], rax

    mov rdi, [display]
    mov rsi, [default_root_window]
    mov rdx, 0x0
    mov rcx, 0x0
    mov r8, 0xc8
    mov r9, 0x64
    mov rax, [black_colour]
    push rax
    push rax
    push 0x0
    call XCreateSimpleWindow
    mov [window], rax
    add rsp, 0x18

    mov rdi, [display]
    mov rsi, [window]
    mov rdx, 0x20000
    call XSelectInput

    mov rdi, [display]
    mov rsi, [window]
    call XMapWindow

    mov rdi, [display]
    mov rsi, [window]
    mov rdx, 0x0
    mov rcx, 0x0
    call XCreateGC
    mov [gc], rax
        
main_loop:

    mov rdi, [display]
    lea rsi, [event]
    call XNextEvent
    jmp main_loop

    lea rdi, [goodbye]
    call print

    mov rdi, 0x0
    call exit

section .data
    display: dq 0x0
    screen_number: dd 0x0
    black_colour: dd 0x0
    default_root_window: dq 0x0
    window: dq 0x0
    gc: dq 0x0
    event: resb 0xc0

section .rodata
    hello_world: db "hello world", 0xa, 0x0
    goodbye: db "goodbye", 0xa, 0x0
    x_open_display_failed: db "XOpenDisplay failed", 0xa, 0x0
    x_select_input_failed: db "XSelectInput failed", 0xa, 0x0
