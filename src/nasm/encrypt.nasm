global asm_encrypt

section .text
asm_encrypt:
    push    rbp ; function prologue
	mov rbp, rsp; function prologue

	xor rax, rax
loop_start:
    cmp rax, rsi;
    jge loop_end; while (i >= x)
    xor byte [rdi + rax], dl
    inc rax
    jmp loop_start;
loop_end:
	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue
	ret
