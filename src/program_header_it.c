#include "woody.h"

ProgramHeaderIterator get_program_header_iterator(ElfHeader *elf) {
	return (ProgramHeaderIterator){
		.idx = -1,
		.size = elf->header.e_phnum,
		.start = ((Elf64_Phdr *)(&elf->data[elf->header.e_phoff]))};
}

int phdr_has_next(ProgramHeaderIterator *it) {
	return (it->idx < (int)it->size);
}

int phdr_next(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	if (it->idx >= (int)it->size) {
		return (-1);
	}

	it->idx++;
	*value = it->start[it->idx];

	return (0);
}

int phdr_prev(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	if (it->idx <= 0) {
		return (-1);
	}

	it->idx--;
	*value = it->start[it->idx];

	return (0);
}

void phdr_first(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	*value = it->start[0];
}

void phdr_last(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	*value = it->start[it->size];
}

int phdr_nth(ProgramHeaderIterator *it, Elf64_Phdr *value, unsigned int nth) {
	if (nth >= it->size || nth <= 0) {
		return (-1);
	}

	*value = it->start[nth];

	return (0);
}

int phdr_count(ProgramHeaderIterator *it) { return (it->size); }
