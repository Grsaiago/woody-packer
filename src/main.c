#include "woody.h"
#include <elf.h>

int main(int argc, char *argv[]) {
	char				 *elf_data;
	ElfHeader			  elf;
	ProgramHeaderIterator phdr_it;

	if (argc != 2) {
		printf("usage: %s <elf_executable_file>\n", argv[0]);
		return (-1);
	}

	elf_data = map_file(argv[1]);
	if (elf_data == NULL) {
		return (-1);
	}

	if (new_elf_header(elf_data, &elf) != 0) {
		printf("%s: error loading the elf header: %s", argv[0],
			   strerror(errno));
		return (-1);
	}

	phdr_it = get_program_header_iterator(&elf);
	Elf64_Phdr phdr;
	do {
		if (phdr_next(&phdr_it, &phdr) == -1) {
			printf("algo de errado no next\n");
			break;
		}
		printf(
			"\nProgram header %d:\n"
			"  type: 0x%x  flags: 0x%x\n"
			"  offset: 0x%llx  vaddr: 0x%llx  paddr: 0x%llx\n"
			"  filesz: %llu  memsz: %llu  align: 0x%llx\n",
			phdr_it.idx, phdr.p_type, phdr.p_flags,
			(unsigned long long)phdr.p_offset, (unsigned long long)phdr.p_vaddr,
			(unsigned long long)phdr.p_paddr, (unsigned long long)phdr.p_filesz,
			(unsigned long long)phdr.p_memsz, (unsigned long long)phdr.p_align);

	} while (phdr_has_next(&phdr_it));
}
