BITS 64

_start:
jmp short end_of_data_declaration
original_text_size:
db 'grsaiago' ; placeholder for text segment size
encryption_key:
db '0' ; placeholder for key to be used for obfuscation
woody_str:
db '...WOODY...',10
end_of_data_declaration:
    ; save register values
    push rax
    push rdi
    push rsi
    push rdx
    ; start of routine
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
