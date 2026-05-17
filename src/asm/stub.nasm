BITS 64

_start:
    ; save register values
    push rax
    push rdi
    push rsi
    push rdx
    ; start of routine
    jmp short end_of_data_declaration
woody_str:
    db '...WOODY...',10 ; declare it here so the declaration is jmp'ed
end_of_data_declaration:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel woody_str]
    mov rdx, 12
    syscall
    ; end of routine
    ; restore register values
    pop rdx
    pop rsi
    pop rdi
    pop rax

    ; stub replacement + jmp to original entrypoint
    mov rax, 0x1234567890
    jmp rax
