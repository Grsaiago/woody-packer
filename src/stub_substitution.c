#include "woody.h"

int patch_stub_entry(ElfFile *elf, unsigned char stub[],
					 unsigned int stub_len) {
	Elf64_Phdr			  text_section;
	ProgramHeaderIterator phdr_it;
	uint64_t			  stub_vaddr;
	uint64_t			  jmp_end;
	int32_t				  rel;

	phdr_it = get_program_header_iterator(elf);
	if (phdr_it_find_next(&phdr_it, is_text_segment, &text_section) != 0)
		return (-1);

	stub_vaddr = text_section.p_vaddr + text_section.p_filesz;
	jmp_end = stub_vaddr + stub_len; // last byte of stub = last byte of jmp
	rel = (int32_t)(elf->header.e_entry - jmp_end);

	memcpy(stub + stub_len - 4, &rel, sizeof(int32_t));
	return (0);
}
