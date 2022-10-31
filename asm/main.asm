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
extern XSync
extern XOpenDisplay
extern XSelectInput
extern XSetForeground
extern XWhitePixel

extern assert_not_null
extern assert_null
extern exit
extern get_time
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
    mov rdx, 0x20003
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
    ; get time at start of frame
    call get_time
    mov [frame_time], rax

    ; see if we want have events
    mov rdi, [display]
    mov rsi, [window]
    mov rdx, 0x03
    lea rcx, [event]
    call XCheckWindowEvent

    ; no event so go to game logic
    cmp rax, 0x0
    je game_logic

    ; check if its a press event
    mov eax, [event]
    cmp rax, 0x2
    je handle_key

    ; check if its a release event
    mov eax, [event]
    cmp rax, 0x3
    jne game_logic ; if not go to game logic

handle_key:
    ; get the key code for the release event
    lea rdi, [event]
    mov rsi, 0x0
    call XLookupKeysym

    ; see if its an XK_Escape
    cmp rax, 0xff1b
    jne handle_arrow_key

    ; if its key press then exit the game
    mov eax, [event]
    cmp rax, 0x2
    je main_loop_end

handle_arrow_key:
    ; if it's not an XK_Right then check if its an XK_Left
    cmp rax, 0xff53
    jne left_check

    mov eax, [event]
    cmp rax, 0x2
    jne right_release

    mov rax, 0x1
    mov [right_arrow_status], rax
    jmp game_logic

right_release:
    mov rax, 0x0
    mov [right_arrow_status], rax
    jmp game_logic

left_check:
    ; if its not an XK_Left then go to game logic
    cmp rax, 0xff51
    jne game_logic

    mov eax, [event]
    cmp rax, 0x2
    jne left_release

    mov rax, 0x1
    mov [left_arrow_status], rax
    jmp game_logic

left_release:
    mov rax, 0x0
    mov [left_arrow_status], rax

game_logic:
    mov rax, [right_arrow_status]
    cmp rax, 0x0
    je right_arrow_update_finish

    mov rax, [paddle_x]
    add rax, 10
    mov [paddle_x], rax
right_arrow_update_finish:

    mov rax, [left_arrow_status]
    cmp rax, 0x0
    je left_arrow_update_finish

    mov rax, [paddle_x]
    sub rax, 10
    mov [paddle_x], rax
left_arrow_update_finish:

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

    ; sync commands with server
    mov rdi, [display]
    mov rsi, 0
    call XSync

    ; get end frame time
    call get_time
    
    ; see if we have spent less than 30ms in this frame
    mov rbx, [frame_time]
    sub rax, rbx
    cmp rax, 30

    jg main_loop_start

    ; sleep for remainder of 30ms
    mov rbx, 30
    sub rbx, rax
    mov rdi, rbx
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
    left_arrow_status: dq 0x0
    right_arrow_status: dq 0x0
    frame_time: dq 0x0

section .rodata
    hello_world: db "hello world", 0xa, 0x0
    goodbye: db "goodbye", 0xa, 0x0
    sleep_for: db "sleep_for: ", 0x0
    x_open_display_failed: db "XOpenDisplay failed", 0xa, 0x0
    x_select_input_failed: db "XSelectInput failed", 0xa, 0x0
    x_set_foreground_failed: db "XSetForeground failed", 0xa, 0x0
