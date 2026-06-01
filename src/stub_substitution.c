#include "woody.h"

static int patch_stub_with_text_size(uint64_t text_size, unsigned char stub[],
									 unsigned int stub_len);

static int patch_stub_with_encryption_key(uint8_t key, unsigned char stub[],
										  unsigned int stub_len);

static int patch_stub_with_entrypoint(uint64_t		original_entrypoint,
									  Elf64_Phdr   *text_section,
									  unsigned char stub[],
									  unsigned int	stub_len);

int patch_stub_with_new_elf_info(ElfFile *elf, uint8_t encryption_key,
								 unsigned char stub[], unsigned int stub_len) {

	Elf64_Phdr			  text_section;
	ProgramHeaderIterator phdr_it;

	phdr_it = get_program_header_iterator(elf);
	if (phdr_it_find_next(&phdr_it, is_text_segment, &text_section) != 0) {
		return (-1);
	}

	// this one should come before the text_size one because it looks for the
	// 'text size placeholder id'
	if (patch_stub_with_encryption_key(encryption_key, stub, stub_len) != 0) {
		return (-1);
	}
	if (patch_stub_with_text_size(text_section.p_filesz, stub, stub_len) != 0) {
		return (-1);
	}
	if (patch_stub_with_entrypoint(elf->header.e_entry, &text_section, stub,
								   stub_len) != 0) {
		return (-1);
	}

	return (0);
}

static int patch_stub_with_encryption_key(uint8_t		encryption_key,
										  unsigned char stub[],
										  unsigned int	stub_len) {
	static const unsigned char marker[8] = {0x67, 0x72, 0x73, 0x61,
											0x69, 0x61, 0x67, 0x6f};
	const unsigned int		   marker_len = sizeof(marker);
	const unsigned int last_valid_compare_position = stub_len - marker_len;

	for (unsigned int i = 0; i <= last_valid_compare_position; i++) {
		if (memcmp(&stub[i], marker, marker_len) == 0) {
			stub[i + sizeof(marker)] =
				encryption_key; // the encryption key should always be
								// right after the text size placeholder
			return (0);
		}
	}
	return (-1);
}

static int patch_stub_with_entrypoint(uint64_t		original_entrypoint,
									  Elf64_Phdr   *text_section,
									  unsigned char stub[],
									  unsigned int	stub_len) {
	uint64_t stub_start_vaddr;
	uint64_t jmp_placeholder_offset;
	int32_t	 placeholder_addr;

	stub_start_vaddr = text_section->p_vaddr + text_section->p_filesz;
	jmp_placeholder_offset =
		stub_start_vaddr + stub_len; // last byte of stub = last byte of jmp
	placeholder_addr = (int32_t)(original_entrypoint - jmp_placeholder_offset);

	memcpy(stub + stub_len - 4, &placeholder_addr, sizeof(int32_t));
	return (0);
}

static int patch_stub_with_text_size(uint64_t text_size, unsigned char stub[],
									 unsigned int stub_len) {
	static const unsigned char marker[8] = {0x67, 0x72, 0x73, 0x61,
											0x69, 0x61, 0x67, 0x6f};
	const unsigned int		   marker_len = sizeof(marker);
	const unsigned int last_valid_compare_position = stub_len - marker_len;

	for (unsigned int i = 0; i <= last_valid_compare_position; i++) {
		if (memcmp(&stub[i], marker, marker_len) == 0) {
			memcpy(&stub[i], &text_size, marker_len);
			return (0);
		}
	}
	return (-1);
}
