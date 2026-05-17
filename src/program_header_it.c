#include "woody.h"
#include <elf.h>

inline ProgramHeaderIterator get_program_header_iterator(ElfHeader *elf) {
	return (ProgramHeaderIterator){
		.idx = -1,
		.size = elf->header.e_phnum,
		.start = ((Elf64_Phdr *)(&elf->data[elf->header.e_phoff]))};
}

inline void phdr_it_restart(ProgramHeaderIterator *it) {
	it->idx = 0;
	return;
}

inline int phdr_it_has_next(ProgramHeaderIterator *it) {
	return (it->idx < (int)it->size);
}

int phdr_it_next(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	if (it->idx >= (int)it->size) {
		return (-1);
	}

	it->idx++;
	*value = it->start[it->idx];

	return (0);
}

int phdr_it_prev(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	if (it->idx <= 0) {
		return (-1);
	}

	it->idx--;
	*value = it->start[it->idx];

	return (0);
}

inline void phdr_it_first(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	*value = it->start[0];
}

inline void phdr_it_last(ProgramHeaderIterator *it, Elf64_Phdr *value) {
	*value = it->start[it->size];
}

int phdr_it_nth(ProgramHeaderIterator *it, Elf64_Phdr *value,
				unsigned int nth) {
	if (nth >= it->size || nth <= 0) {
		return (-1);
	}

	*value = it->start[nth];

	return (0);
}

inline int phdr_it_count(ProgramHeaderIterator *it) { return (it->size); }

/**
 * find_first - Find the first program header matching a predicate.
 * @it: Iterator over the program header table (will be restarted).
 * @finder_func: Predicate returning non-zero when a header matches.
 * @value: Output pointer to receive the matching header.
 *
 * Restarts the iterator and scans forward until @finder_func returns
 * non-zero. When a match is found, writes it to @value and returns 0.
 * Returns -1 if no match is found.
 */
int find_first(ProgramHeaderIterator *it,
			   int (*finder_func)(const Elf64_Phdr *),
			   Elf64_Phdr *const value) {
	phdr_it_restart(it);
	while (phdr_it_has_next(it)) {
		phdr_it_next(it, value);
		if (finder_func(value)) {
			return (0);
		}
	}
	return (-1);
}
