;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;         Distributed under the Boost Software License, Version 1.0.         ;;
;;            (See accompanying file LICENSE or copy at                       ;;
;;                 https://www.boost.org/LICENSE_1_0.txt)                     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .text

global _start

extern XBlackPixel
extern XCheckWindowEvent
extern XClearWindow
extern XCreateGC
extern XCreateSimpleWindow
extern XDefaultRootWindow
extern XDefaultScreen
extern XFillRectangle
extern XFlush
extern XLookupKeysym
extern XMapWindow
extern XNextEvent
extern XOpenDisplay
extern XSelectInput
extern XSetForeground
extern XWhitePixel

extern assert_not_null
extern assert_null
extern exit
extern print
extern print_num
extern sleep_ms

_start:

    lea rdi, [hello_world]
    call print

    ; X11 window setup

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
    call XWhitePixel
    mov [white_colour], rax

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
    mov r8, 0x320
    mov r9, 0x320
    mov rax, [black_colour]
    push rax
    push rax
    push 0x0
    call XCreateSimpleWindow
    mov [window], rax
    add rsp, 0x18

    mov rdi, [display]
    mov rsi, [window]
    mov rdx, 0x20002
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

    ; wait until window has appeared
wait_loop_start:
    mov rdi, [display]
    lea rsi, [event]
    call XNextEvent

    mov eax, [event]
    cmp rax, 0x13
    je wait_loop_end

    jmp wait_loop_start
wait_loop_end:


main_loop_start:
    ; see if we want have events
    mov rdi, [display]
    mov rsi, [window]
    mov rdx, 0x02
    lea rcx, [event]
    call XCheckWindowEvent

    ; no event so go to game logic
    cmp rax, 0x0
    je game_logic

    ; check if its a release event
    mov eax, [event]
    cmp rax, 0x3
    jne game_logic ; if not go to game logic

    ; get the key code for the release event
    lea rdi, [event]
    mov rsi, 0x0
    call XLookupKeysym

    ; if it's a XK_q then quit
    cmp rax, 0x71
    je main_loop_end

    ; if it's not an XK_Right then go to game logic
    cmp rax, 0xff53
    jne game_logic

    ; right arrow was releases so move paddle to the right
    mov rax, [paddle_x]
    add rax, 10
    mov [paddle_x], rax

game_logic:

    ; clear window
    mov rdi, [display]
    mov rsi, [window]
    call XClearWindow

    ; set foreground color for drawing
    mov rdi, [display]
    mov rsi, [gc]
    mov rdx, [white_colour]
    call XSetForeground

    ; draw paddle
    mov rdi, [display]
    mov rsi, [window]
    mov rdx, [gc]
    mov rcx, [paddle_x]
    mov r8, [paddle_y]
    mov r9, [paddle_width]
    mov rax, [paddle_height]
    push rax
    call XFillRectangle
    add rsp, 0x8

    ; ensure all draw commands are flushed
    mov rdi, [display]
    call XFlush

    ; sleep for short time to prevent screen tearing
    mov rdi, 33
    call sleep_ms
        
    jmp main_loop_start
main_loop_end:

    lea rdi, [goodbye]
    call print

    mov rdi, 0x0
    call exit

section .data
    display: dq 0x0
    screen_number: dq 0x0
    black_colour: dq 0x0
    white_colour: dq 0x0
    default_root_window: dq 0x0
    window: dq 0x0
    gc: dq 0x0
    event: resb 0xc0
    paddle_width: dq 0xc8
    paddle_height: dq 0x14
    paddle_x: dq 0x12c
    paddle_y: dq 0x30c

section .rodata
    hello_world: db "hello world", 0xa, 0x0
    goodbye: db "goodbye", 0xa, 0x0
    x_open_display_failed: db "XOpenDisplay failed", 0xa, 0x0
    x_select_input_failed: db "XSelectInput failed", 0xa, 0x0
    x_set_foreground_failed: db "XSetForeground failed", 0xa, 0x0
