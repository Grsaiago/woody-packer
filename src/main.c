#include "woody.h"

int main(int argc, char *argv[]) {
	char	 *elf_data;
	ElfHeader elf;
	// extern unsigned char stub_bin[];
	extern unsigned int stub_bin_len;

	// size_t	  pagesz = (size_t)sysconf(_SC_PAGESIZE);
	// uintptr_t start = (uintptr_t)stub_bin & ~(uintptr_t)(pagesz - 1);
	// size_t	  len = (size_t)((uintptr_t)stub_bin - start) + stub_bin_len;
	// len = (len + pagesz - 1) & ~(pagesz - 1);
	// if (mprotect((void *)start, len, PROT_READ | PROT_EXEC) != 0) {
	// 	perror("mprotect");
	// 	return (-1);
	// }
	// void (*func)(void) = (void (*)(void))stub_bin;
	// func();

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

	if (has_valid_code_cave(&elf, stub_bin_len) != 0) {
		printf("%s: there is no valid code cave on this binary\n", argv[0]);
	}
}
