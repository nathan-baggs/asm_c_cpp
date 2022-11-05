;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;         Distributed under the Boost Software License, Version 1.0.          ;;
;;            (See accompanying file LICENSE or copy at                        ;;
;;                 https://www.boost.org/LICENSE_1_0.txt)                      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global create_window
global try_get_event

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
extern render_begin
extern draw_rectangle
extern render_end
extern print
extern print_num

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This file contains functions for creating a window and rendering to it.


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Create and display an X Window.
;
create_window:
    push rbp
    mov rbp, rsp

    ; X11 window setup

    mov rdi, 0x0
    call XOpenDisplay
    mov [display], rax
    mov rdi, rax
    lea rsi, [x_open_display_failed]
    call assert_not_null

    call XDefaultScreen
    mov [screen_number], rax

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

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Try and get an event
;
; @returns
;   Address of an event, or 0x0 if no event was available.
;
try_get_event:
    push rbp
    mov rbp, rsp

    ; see if we want have events
    mov rdi, [display]
    mov rsi, [window]
    mov rdx, 0x03
    lea rcx, [event]
    call XCheckWindowEvent

    cmp rax, 0x0
    je try_get_event_end

    ; we got an event so return pointer to event object
    lea rax, [event]

try_get_event_end:
    pop rbp
    ret

; Perform pre-render tasks
render_begin:
    push rbp
    mov rbp, rsp

    ; clear window
    mov rdi, [display]
    mov rsi, [window]
    call XClearWindow

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Draw a rectangle.
;
; @param rdi
;   X coord of rectangle
;
; @param rsi
;   Y coord of rectangle
;
; @param rdx
;   Width of rectangle
;
; @param rcx
;   Height of rectangle
;
draw_rectangle:
    push rbp
    mov rbp, rsp

    ; push args to stack so we can easily pop them into the correct registers
    push rcx
    push rdx
    push rsi
    push rdi

    ; set foreground color for drawing
    mov rdi, [display]
    mov rsi, [gc]
    mov rdx, [white_colour]
    call XSetForeground

    ; draw paddle
    mov rdi, [display]
    mov rsi, [window]
    mov rdx, [gc]
    pop rcx
    pop r8
    pop r9 ; note that the last arg is now at at the top of the stack
    call XFillRectangle
    add rsp, 0x8

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Perform post-render tasks
;
render_end:
    push rbp
    mov rbp, rsp

    ; ensure all draw commands are flushed
    mov rdi, [display]
    call XFlush

    pop rbp
    ret

section .data
    display: dq 0x0
    screen_number: dq 0x0
    black_colour: dq 0x0
    white_colour: dq 0x0
    default_root_window: dq 0x0
    window: dq 0x0
    gc: dq 0x0
    event: resb 0xc0

section .rodata
    hello_world: db "hello world", 0xa, 0x0
    goodbye: db "goodbye", 0xa, 0x0
    sleep_for: db "sleep_for: ", 0x0
    x_open_display_failed: db "XOpenDisplay failed", 0xa, 0x0
    x_select_input_failed: db "XSelectInput failed", 0xa, 0x0
    x_set_foreground_failed: db "XSetForeground failed", 0xa, 0x0
