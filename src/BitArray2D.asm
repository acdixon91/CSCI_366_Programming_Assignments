
        global  _set_bit_elem
        global  _get_bit_elem
        section .text

_set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col


        ; r8 contains index
        ; r10 contains which byte
        ; r11 contains byte_offset
        ; rcx contains mask



        mov [rbp - 8], r13
        mov [rbp - 16], r14

        mov rax, rsi
        mul rdx         ; row*row_width
        add rax, rcx    ; + column
        mov r8, rax     ; index

        mov r9, 8
        div r9          ; index / 8
        mov r10, rax

        mul r9          ; bit_offset * 8
        mov r11, r8
        sub r11, rax    ; index - (byte_offset * 8)

        sub r9, r11     ; 8 - bit_offset
        sub r9, 1       ; - 1 = mask
        mov rcx, r9     ; move mask to rcx

        mov r13, 1      ; moving 1 into r8
        shl r13, cl     ; shift r12 by mask value

        mov rsi, r10
        mov r9, 8
        mov rax, rsi    ; move chosen byte num to rax
;        mul r9          ; multiply byte num * 8 bits


        mov r14, [rdi + rax]    ; get the byte at that address
        or r14, r13             ; or mask + byte
        mov [rdi + rax], r14    ; move result back into memory


        mov r13, [rbp - 8]
        mov r14, [rbp - 16]

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




_get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here - for now returning 0

        ; rdi contains array pointer
        ; rsi contains row width
                ; rdx contains row
                ; rcx contains col


                ; r8 contains index
                ; r10 contains which byte
                ; r11 contains byte_offset
                ; rcx contains mask

                mov [rbp - 8], r13
                mov [rbp - 16], r14


                mov rax, rsi
                mul rdx         ; row*row_width
                add rax, rcx    ; + column
                mov r8, rax     ; index

                mov r9, 8
                div r9          ; index / 8
                mov r10, rax

                mul r9          ; bit_offset * 8
                mov r11, r8
                sub r11, rax    ; index - (byte_offset * 8)

                sub r9, r11     ; 8 - bit_offset
                sub r9, 1       ; - 1 = mask
                mov rcx, r9     ; move mask to rcx

                mov r13, 1      ; moving 1 into r8
                shl r13, cl     ; shift r12 by mask value

                mov rax, r10    ; move chosen byte num to rax
                mov r9, 8
;                mul r9          ; multiply byte num * 8 bits

                mov r14, [rdi + rax]    ; get the byte at that address
                and r14, r13            ; and mask + byte
                cmp r14, 0
                setg r14b

                movsx rax, r14b

                mov r13, [rbp - 8]
                mov r14, [rbp - 16]


        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
