#include "woody.h"

char *map_file(const char *filename) {
	int		 fd;
	long int file_size;
	char	*elf_data;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("error on opening file: %s", strerror(errno));
		return (NULL);
	}

	file_size = lseek(fd, 0, SEEK_END);
	if (file_size < 0) {
		printf("error on lseek: %s", strerror(errno));
		return (NULL);
	}
	if (lseek(fd, 0, SEEK_SET) == -1) {
		printf("error on lseek: %s", strerror(errno));
		return (NULL);
	}

	elf_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (elf_data == NULL) {
		printf("error on mmap: %s", strerror(errno));
		return (NULL);
	}

	close(fd);
	return elf_data;
}
