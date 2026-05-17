#ifndef WOODY_H
#define WOODY_H

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	Elf64_Ehdr header;
	char	  *data;
} ElfHeader;

typedef struct {
	int			 idx;
	unsigned int size;
	Elf64_Phdr	*start;
} ProgramHeaderIterator;

// File manipulation
char *map_file(const char *filename);

// Elf header
int new_elf_header(char *data, ElfHeader *elf_header);

// ProgramHeaderIterator methods
ProgramHeaderIterator get_program_header_iterator(ElfHeader *elf);
int					  phdr_has_next(ProgramHeaderIterator *it);
int					  phdr_next(ProgramHeaderIterator *it, Elf64_Phdr *value);
int					  phdr_prev(ProgramHeaderIterator *it, Elf64_Phdr *value);
void				  phdr_first(ProgramHeaderIterator *it, Elf64_Phdr *value);
void				  phdr_last(ProgramHeaderIterator *it, Elf64_Phdr *value);
int phdr_nth(ProgramHeaderIterator *it, Elf64_Phdr *value, unsigned int nth);
int phdr_count(ProgramHeaderIterator *it);

#endif // WOODY_H
