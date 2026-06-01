#include "woody.h"

int new_elf_file(MappedFile *file, ElfFile *elf_header) {
	if (memcmp(file->data, ELFMAG, SELFMAG) != 0) {
		return (-1);
	}
	memcpy(elf_header, file->data, sizeof(Elf64_Ehdr));
	if (elf_header->header.e_type != ET_EXEC &&
		elf_header->header.e_type != ET_DYN) {
		return (-1);
	}
	elf_header->data = file->data;
	elf_header->size = file->size;
	return (0);
}

void free_elf_file(ElfFile *elf) {
	if (elf->data != NULL) {
		munmap(elf->data, elf->size);
		elf->data = NULL;
	}
}

inline Elf64_Phdr *get_program_header_table(char	   *data,
											Elf64_Ehdr *elf_header) {
	return ((Elf64_Phdr *)(&data[elf_header->e_phoff]));
}

int has_valid_code_cave(ElfFile *elf, size_t stub_size) {
	ProgramHeaderIterator it;
	Elf64_Phdr			  text;
	Elf64_Phdr			  next_to_text;
	uint64_t			  cave_size;

	it = get_program_header_iterator(elf);
	if (phdr_it_find_next(&it, is_text_segment, &text) != 0) {
		return (-1);
	}
	if (phdr_it_find_next(&it, is_pt_load, &next_to_text) != 0) {
		return (-1);
	}
	cave_size = (next_to_text.p_offset) - (text.p_offset + text.p_filesz);

	return (!(cave_size >= stub_size));
}

int new_elf_with_injected_stub(ElfFile *elf, const unsigned char stub[],
							   const unsigned int stub_len, ElfFile *new_elf) {
	char				 *new_elf_data;
	Elf64_Phdr			  text_section;
	ProgramHeaderIterator phdr_it;
	unsigned long		  text_section_end;
	Elf64_Ehdr			 *ehdr;
	Elf64_Phdr			 *phdrs;

	phdr_it = get_program_header_iterator(elf);
	if (phdr_it_find_next(&phdr_it, is_text_segment, &text_section) != 0) {
		return (-1);
	}

	new_elf_data = malloc(elf->size);
	if (new_elf_data == NULL) {
		return (-1);
	}

	// copy the original elf
	memcpy(new_elf_data, elf->data, elf->size);

	// overwrite the cave with the stub
	text_section_end = text_section.p_offset + text_section.p_filesz;
	memcpy(new_elf_data + text_section_end, stub, stub_len);

	// extend the text segment to cover the stub
	ehdr = (Elf64_Ehdr *)new_elf_data;
	phdrs = (Elf64_Phdr *)(new_elf_data + ehdr->e_phoff);
	for (int i = 0; i < ehdr->e_phnum; i++) {
		if (phdrs[i].p_offset == text_section.p_offset &&
			phdrs[i].p_filesz == text_section.p_filesz) {
			phdrs[i].p_filesz += stub_len;
			phdrs[i].p_memsz += stub_len;
			break;
		}
	}

	// redirect entrypoint to stub
	ehdr->e_entry = text_section.p_vaddr + text_section.p_filesz;

	*new_elf = (ElfFile){.data = new_elf_data, .size = elf->size};
	memcpy(&new_elf->header, new_elf_data, sizeof(Elf64_Ehdr));

	return (0);
}
