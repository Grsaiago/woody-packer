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
    push rcx
    ; start of routine
    ; write ...woody...
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel woody_str]
    mov rdx, 12
    syscall
    ; lea of where I'm at so i can jump to original text start
    lea rcx, [rel _start]
    mov rdx, rcx
    sub rcx, [rel original_text_size] ; rcx = text_start

    mov al, byte [rel encryption_key] ; key = valor_da_key
xor_loop:
    xor byte [rcx], al ; text[i] ^= key
    inc rcx
    cmp rcx, rdx
    jb xor_loop
xor_end:
    ; restore register values
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rax
    ; jmp to original entrypoint (relative, works for PIE)
    jmp near 0x11223344
