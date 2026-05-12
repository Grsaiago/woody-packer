#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int			fd;
	int			file_size;
	char	   *elf_data;
	Elf64_Ehdr	elf_hdr;
	Elf64_Phdr *program_header_table;

	if (argc != 2) {
		printf("usage: %s <elf_executable_file>\n", argv[0]);
		return (-1);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		printf("%s: error on opening file: %s", argv[0], strerror(errno));
		return (-1);
	}

	file_size = lseek(fd, 0, SEEK_END);
	if (file_size < 0) {
		printf("%s: error on lseek: %s", argv[0], strerror(errno));
		return (-1);
	}
	if (lseek(fd, 0, SEEK_SET) == -1) {
		printf("%s: error on lseek: %s", argv[0], strerror(errno));
		return (-1);
	}

	elf_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (elf_data == NULL) {
		printf("%s: error on mmap: %s", argv[0], strerror(errno));
		return (-1);
	}
	memcpy(&elf_hdr, elf_data, sizeof(Elf64_Ehdr));

	program_header_table = (Elf64_Phdr *)(elf_data + elf_hdr.e_phoff);
	for (int i = 0; i < elf_hdr.e_phnum; i++) {

		printf("\nProgram header %d:\n"
			   "  type: 0x%x  flags: 0x%x\n"
			   "  offset: 0x%llx  vaddr: 0x%llx  paddr: 0x%llx\n"
			   "  filesz: %llu  memsz: %llu  align: 0x%llx\n",
			   i, program_header_table[i].p_type,
			   program_header_table[i].p_flags,
			   (unsigned long long)program_header_table[i].p_offset,
			   (unsigned long long)program_header_table[i].p_vaddr,
			   (unsigned long long)program_header_table[i].p_paddr,
			   (unsigned long long)program_header_table[i].p_filesz,
			   (unsigned long long)program_header_table[i].p_memsz,
			   (unsigned long long)program_header_table[i].p_align);
	}
}
