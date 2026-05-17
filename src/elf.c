#include "woody.h"
#include <elf.h>
#include <stdint.h>

int new_elf_header(char *data, ElfHeader *elf_header) {
	if (memcmp(data, ELFMAG, SELFMAG) != 0) {
		return (-1);
	}
	memcpy(elf_header, data, sizeof(Elf64_Ehdr));
	if (elf_header->header.e_type != ET_EXEC &&
		elf_header->header.e_type != ET_DYN) {
		return (-1);
	}
	elf_header->data = data;
	return (0);
}

inline Elf64_Phdr *get_program_header_table(char	   *data,
											Elf64_Ehdr *elf_header) {
	return ((Elf64_Phdr *)(&data[elf_header->e_phoff]));
}

int has_valid_code_cave(ElfHeader *elf, size_t stub_size) {
	ProgramHeaderIterator it;
	Elf64_Phdr			  text;
	Elf64_Phdr			  next_to_text;
	uint64_t			  cave_size;

	it = get_program_header_iterator(elf);
	if (phdr_it_find_next(&it, is_text_segment, &text) != 0) {
		return (-1);
	}
	if (phdr_it_find_next(&it, is_pt_load, &next_to_text) != 0) {
		return (-1);
	}
	cave_size = (next_to_text.p_offset) - (text.p_offset + text.p_filesz);

	return (cave_size >= stub_size);
}
