;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;         Distributed under the Boost Software License, Version 1.0.          ;;
;;            (See accompanying file LICENSE or copy at                        ;;
;;                 https://www.boost.org/LICENSE_1_0.txt)                      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .text

global _start

extern XBlackPixel
extern XClearWindow
extern XFillRectangle
extern XFlush
extern XLookupKeysym
extern XSync
extern XSetForeground
extern XWhitePixel

extern assert_not_null
extern assert_null
extern create_window
extern draw_rectangle
extern exit
extern get_time
extern linked_list_init
extern linked_list_iterator
extern linked_list_iterator_advance
extern linked_list_iterator_remove
extern linked_list_iterator_value
extern linked_list_push_back
extern memory_malloc
extern print
extern print_num
extern render_begin
extern render_end
extern sleep_ms
extern try_get_event

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Entry point to the program
;
_start:

    lea rdi, [hello_world]
    call print

    call create_entities
    call create_window

main_loop_start:
    ; get time at start of frame
    call get_time
    mov [frame_time], rax

    call try_get_event
    mov r12, rax

    ; no event so go to game logic
    cmp rax, 0x0
    je game_logic

    ; check if its a press event
    mov eax, [r12]
    cmp rax, 0x2
    je handle_key

    ; check if its a release event
    mov eax, [r12]
    cmp rax, 0x3
    jne game_logic ; if not go to game logic

handle_key:
    ; get the key code for the release event
    lea rdi, [r12]
    mov rsi, 0x0
    call XLookupKeysym

    ; see if its an XK_Escape
    cmp rax, 0xff1b
    jne handle_arrow_key

    ; if its key press then exit the game
    mov eax, [r12]
    cmp rax, 0x2
    je main_loop_end

handle_arrow_key:
    ; if it's not an XK_Right then check if its an XK_Left
    cmp rax, 0xff53
    jne left_check

    mov eax, [r12]
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

    mov eax, [r12]
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

    call ball_update
    call handle_collisions
    call render
    
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Render the entity list.
;
render:
    push rbp
    mov rbp, rsp

    call render_begin

    mov rdi, [entity_list]
    call linked_list_iterator
    push rax

render_loop_start:
    mov rax, [rsp]
    cmp rax, 0x0
    je render_loop_end

    mov rdi, [rsp]
    call linked_list_iterator_value

    mov rdi, [rax]
    mov rsi, [rax + 8]
    mov rdx, [rax + 16]
    mov rcx, [rax + 24]
    call draw_rectangle

    mov rdi, [rsp]
    call linked_list_iterator_advance
    mov [rsp], rax
    jmp render_loop_start

    call render_end
render_loop_end:

    add rsp, 0x8
    pop rbp
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Perform collision detections and resolutions.
;
handle_collisions:
    push rbp
    mov rbp, rsp
    sub rsp, 8

    mov rdi, [entity_list]
    call linked_list_iterator
    mov [rsp], rax

    mov rdi, [rsp]
    call linked_list_iterator_advance ; move past first entity so we don't check ball collides with itself
    mov [rsp], rax

    ; check collision with paddle
    mov rdi, [rsp]
    call linked_list_iterator_value

    lea rdi, [ball_x]
    mov rsi, rax
    call check_entity_collision

    cmp rax, 0x0
    je paddle_collision_end

    call handle_ball_paddle_collision

paddle_collision_end:

    mov rdi, [rsp]
    call linked_list_iterator_advance ; move passed paddle, onto the bricks
    mov [rsp], rax

brick_collision_loop_start:
    ; if we are at the end of the linked list then stop
    mov rax, [rsp]
    cmp rax, 0x0
    je brick_collision_loop_end

    ; get value from the iterator (which will be the address of an entity)
    mov rdi, [rsp]
    call linked_list_iterator_value

    ; check if ball is colliding with block
    lea rdi, [ball_x]
    mov rsi, rax
    call check_entity_collision

    cmp rax, 0x0
    jne handle_collision_found

    ; if no collision then move iterator to next block
    mov rdi, [rsp]
    call linked_list_iterator_advance
    mov [rsp], rax

    jmp brick_collision_loop_start

handle_collision_found:
    ; remove block from linked list
    mov rdi, [entity_list]
    mov rsi, [rsp]
    call linked_list_iterator_remove

    ; invert ball velocity
    mov rax, 0xa
    mov [ball_velocity_y], rax

brick_collision_loop_end:

    add rsp, 8
    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Handle ball paddle collision. This function assumes the collision has already been detected and needs resolving.
;
handle_ball_paddle_collision: 
    push rbp
    mov rbp, rsp

    ; we can simply just grab the data we need without having to go through the linked list
    mov rax, [ball_x]
    mov rbx, [paddle_x]

    ; check of collision happened on the left third of the paddle
    add rbx, 6
    cmp rax, rbx
    jge check_middle

    ; if it did then set the ball moving up and left
    mov rax, -7
    mov [ball_velocity_x], rax
    mov [ball_velocity_y], rax
    jmp handle_ball_paddle_collision_end

check_middle:
    ; check of collision happened on the middle third of the paddle
    add rbx, 8
    cmp rax, rbx
    jge check_end

    ; if it did then set the ball moving straight up
    mov rax, 0
    mov [ball_velocity_x], rax
    mov rax, -10
    mov [ball_velocity_y], rax
    jmp handle_ball_paddle_collision_end

check_end:
    ; the only other choice is the paddle hit the right third, so move it up and right
    mov rax, 7
    mov [ball_velocity_x], rax
    mov rax, -7
    mov [ball_velocity_y], rax
    jmp handle_ball_paddle_collision_end

handle_ball_paddle_collision_end:
    pop rbp
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Check collisions between two entities.
;
; @param rdi
;   Address of first entity.
;
; @param rsi
;   Address of second entity.
;
; @returns
;   0x1 if collision was detected, otherwise 0x0.
;
check_entity_collision:
    push rbp
    mov rbp, rsp

    ; rect1.x < rect2.x + rect2.w
    mov rax, [rdi]
    mov rbx, [rsi]
    mov rcx, [rsi + 16]
    add rbx, rcx
    cmp rax, rbx
    jge no_collision

    ; rect1.x + rect1.w > rect2.x
    mov rax, [rdi]
    mov rbx, [rdi + 16]
    add rax, rbx
    mov rcx, [rsi]
    cmp rax, rcx
    jle no_collision

    ; rect1.y < rect2.y + rect2.h
    mov rax, [rdi + 8]
    mov rbx, [rsi + 8]
    mov rcx, [rsi + 24]
    add rbx, rcx
    cmp rax, rbx
    jge no_collision

    ; rect1.h + rect1.y > rect1.y
    mov rax, [rdi + 24]
    mov rbx, [rdi + 8]
    add rax, rbx
    mov rcx, [rsi + 8]
    cmp rax, rcx
    jle no_collision

    mov rax, 0x1
    jmp check_collision_end
    
no_collision:
    mov rax, 0x0

check_collision_end:
    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Perform ball update logic
;
ball_update:
    push rbp
    mov rbp, rsp

    ; add y velocity to ball
    mov rax, [ball_y]
    add rax, [ball_velocity_y]
    mov [ball_y], rax

    ; add x velocity to ball
    mov rax, [ball_x]
    add rax, [ball_velocity_x]
    mov [ball_x], rax

    ; check if ball has gone off the top of the screen
    mov rax, [ball_y]
    cmp rax, 0x0
    jg check_ball_right_side

    ; invert y velocity
    mov rax, 0xa
    mov [ball_velocity_y], rax

check_ball_right_side:

    ; check if ball has gone of right of the screen
    mov rax, [ball_x]
    cmp rax, 800
    jl check_ball_left_side

    ; invert x velocity
    mov rax, [ball_velocity_x]
    neg rax
    mov [ball_velocity_x], rax
    jmp ball_update_end

check_ball_left_side:
    ; check if ball has gone of the left of the screen
    mov rax, [ball_x]
    cmp rax, 0
    jg ball_update_end

    ; invert x velocity
    mov rax, [ball_velocity_x]
    neg rax
    mov [ball_velocity_x], rax

ball_update_end:
    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Create all the entities for the game and insert them into a linked list.
;
create_entities:
    push rbp
    mov rbp, rsp

    call linked_list_init
    mov [entity_list], rax

    ; collision code assumes ball is first in the entity list, then the paddle then the bricks
    mov rdi, [entity_list]
    lea rsi, [ball_x] ; store address of ball in list
    call linked_list_push_back

    mov rdi, [entity_list]
    lea rsi, [paddle_x] ; store address of paddle data in list
    call linked_list_push_back

    mov rdi, 50
    call create_brick_row
    mov rdi, 80
    call create_brick_row
    mov rdi, 110
    call create_brick_row
    mov rdi, 140
    call create_brick_row
    mov rdi, 170
    call create_brick_row
    mov rdi, 200
    call create_brick_row

    pop rbp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Adds a row of 10 bricks to the entity list
;
; @param rdi
;   Y coord of row.
;
create_brick_row:
    push rbp
    mov rbp, rsp
    sub rsp, 24

    mov [rsp + 16], rdi

    mov rax, 0
    mov [rsp], rax ; store loop counter
    mov rax, 20
    mov [rsp + 8], rax ; store initial x coord

create_row_start:

    mov rax, [rsp]
    cmp rax, 10
    je create_row_end ; leave loop if we have added 10 bricks

    mov rdi, 32
    call memory_malloc

    ; fill out entity struct
    mov rbx, [rsp + 8]
    mov [rax], rbx
    mov rbx, [rsp + 16]
    mov [rax + 8], rbx
    mov rbx, 58
    mov [rax + 16], rbx
    mov rbx, 20
    mov [rax + 24], rbx

    mov rdi, [entity_list]
    mov rsi, rax
    call linked_list_push_back

    ; advance x coord for next iteration
    mov rax, [rsp + 8]
    add rax, 78
    mov [rsp + 8], rax

    ; increment iteration count
    mov rax, [rsp]
    inc rax
    mov [rsp], rax
    jmp create_row_start

create_row_end:

    add rsp, 24
    pop rbp
    ret

section .data
    entity_list: dq 0x0
    paddle_x: dq 0x12c
    paddle_y: dq 0x30c
    paddle_width: dq 0xc8
    paddle_height: dq 0x14
    ball_x: dq 0x1a4
    ball_y: dq 0x190
    ball_width: dq 0xa
    ball_height: dq 0xa
    ball_velocity_y: dq 0xa
    ball_velocity_x: dq 0x0
    left_arrow_status: dq 0x0
    right_arrow_status: dq 0x0
    frame_time: dq 0x0

section .rodata
    hello_world: db "hello world", 0xa, 0x0
    goodbye: db "goodbye", 0xa, 0x0
    sleep_for: db "sleep_for: ", 0x0
