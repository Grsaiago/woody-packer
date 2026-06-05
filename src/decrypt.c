#include "woody.h"

void asm_decrypt(char *src, uint64_t size, uint8_t key) {
	for (unsigned int i = 0; i < size; i++) {
		src[i] ^= key;
	}
	return;
}

void encrypt_text_section(ElfFile *elf, uint64_t text_size, uint8_t key) {}
