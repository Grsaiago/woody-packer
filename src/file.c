#include "woody.h"

int map_file(const char *filename, MappedFile *file) {
	int		 fd;
	long int file_size;
	char	*elf_data;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("error on opening file: %s", strerror(errno));
		return (-1);
	}

	file_size = lseek(fd, 0, SEEK_END);
	if (file_size < 0) {
		printf("error on lseek: %s", strerror(errno));
		return (-1);
	}
	if (lseek(fd, 0, SEEK_SET) != 0) {
		printf("error on lseek: %s", strerror(errno));
		return (-1);
	}

	elf_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (elf_data == NULL) {
		printf("error on mmap: %s", strerror(errno));
		return (-1);
	}

	*file = (MappedFile){.data = elf_data, .size = file_size};
	close(fd);
	return (0);
}
