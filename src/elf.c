#include "woody.h"

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
