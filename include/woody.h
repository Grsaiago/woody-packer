#ifndef WOODY_H
#define WOODY_H

#include <elf.h>
#include <errno.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	char		 *data;
	unsigned long size;
} MappedFile;

typedef struct {
	Elf64_Ehdr	  header;
	char		 *data;
	unsigned long size;
} ElfFile;

typedef struct {
	int			 idx;
	unsigned int size;
	Elf64_Phdr	*start;
} ProgramHeaderIterator;

// stub manipulation
int patch_stub_with_new_elf_info(ElfFile *elf, uint8_t encryption_key,
								 unsigned char stub[], unsigned int stub_len);

// File manipulation
int map_file(const char *filename, MappedFile *file);

// Elf file
int	 new_elf_file(MappedFile *file, ElfFile *elf_file);
void free_elf_file(ElfFile *elf_file);
int	 has_valid_code_cave(ElfFile *elf, size_t stub_size);
int	 new_elf_with_injected_stub(ElfFile *elf, const unsigned char stub[],
								const unsigned int stub_len,
								uint8_t encryption_key, ElfFile *new_elf);

// ProgramHeaderIterator methods
ProgramHeaderIterator get_program_header_iterator(ElfFile *elf);
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

// encryp/decrypt
extern void asm_encrypt(char *src, uint64_t size, uint8_t key);
void		asm_decrypt(char *src, uint64_t size, uint8_t key);

#endif // WOODY_H
