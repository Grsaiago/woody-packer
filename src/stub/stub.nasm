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
    db '...WOODY...',10
end_of_data_declaration:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel woody_str]
    mov rdx, 12
    syscall
    ; restore register values
    pop rdx
    pop rsi
    pop rdi
    pop rax
    ; jmp to original entrypoint (relative, works for PIE)
    jmp near 0x11223344
