#ifndef WOODY_H
#define WOODY_H

#include "stub.h"

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
int has_valid_code_cave(ElfHeader *elf, size_t stub_size);

// ProgramHeaderIterator methods
ProgramHeaderIterator get_program_header_iterator(ElfHeader *elf);
void				  phdr_it_restart(ProgramHeaderIterator *it);
int					  phdr_it_has_next(ProgramHeaderIterator *it);
int phdr_it_next(ProgramHeaderIterator *it, Elf64_Phdr *value);
int phdr_it_prev(ProgramHeaderIterator *it, Elf64_Phdr *value);
int phdr_it_nth(ProgramHeaderIterator *it, Elf64_Phdr *value, unsigned int nth);
int phdr_it_count(ProgramHeaderIterator *it);
void phdr_it_first(ProgramHeaderIterator *it, Elf64_Phdr *value);
void phdr_it_last(ProgramHeaderIterator *it, Elf64_Phdr *value);
int	 phdr_it_find_next(ProgramHeaderIterator *it,
					   int (*finder_func)(const Elf64_Phdr *),
					   Elf64_Phdr *const value);

// ProgramHeader methods
const char *phdr_get_type_str(Elf64_Phdr *hdr);
int			is_text_segment(const Elf64_Phdr *hdr);
int			is_data_segment(const Elf64_Phdr *hdr);
int			is_pt_load(const Elf64_Phdr *hdr);

#endif // WOODY_H
