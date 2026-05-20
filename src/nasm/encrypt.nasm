global asm_encrypt

section .text
asm_encrypt:
    push    rbp ; function prologue
	mov	rbp, rsp; function prologue

loop_start:

    jmp loop_start
loop_end:

	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue
	ret
