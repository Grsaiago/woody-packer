#include "woody.h"
#include <elf.h>

int main(int argc, char *argv[]) {
	MappedFile			 mapped_file;
	ElfFile				 elf;
	ElfFile				 new_elf;
	int					 woody_fd;
	extern unsigned char stub_bin[];
	extern unsigned int	 stub_bin_len;
	uint8_t				 encryption_key = 'a';

	if (argc != 2) {
		printf("usage: %s <elf_executable_file>\n", argv[0]);
		return (-1);
	}

	if (map_file(argv[1], &mapped_file) != 0) {
		return (-1);
	}

	if (new_elf_file(&mapped_file, &elf) != 0) {
		printf("%s: error loading the elf header: %s\n", argv[0],
			   strerror(errno));
		return (-1);
	}

	if (has_valid_code_cave(&elf, stub_bin_len) != 0) {
		printf("%s: there is no valid code cave on this binary\n", argv[0]);
		return (-1);
	}

	if (patch_stub_with_new_elf_info(&elf, encryption_key, stub_bin,
									 stub_bin_len) != 0) {
		printf(
			"%s: failed to patch the stub with the original elf entrypoint\n",
			argv[0]);
		return (-1);
	}

	if (new_elf_with_injected_stub(&elf, stub_bin, stub_bin_len, encryption_key,
								   &new_elf) != 0) {
		printf("%s: failed to create new injected elf file\n", argv[0]);
		return (-1);
	}

	woody_fd = open("woody", O_CREAT | O_TRUNC | O_WRONLY,
					S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	if (woody_fd < 0) {
		printf("%s: failed to create woody file; %s\n", argv[0],
			   strerror(errno));
		return (-1);
	}
	if (write(woody_fd, new_elf.data, new_elf.size) < 0) {
		printf("%s: failed to write woody file; %s\n", argv[0],
			   strerror(errno));
		return (-1);
	}

	free_elf_file(&elf);
	return (0);
}
