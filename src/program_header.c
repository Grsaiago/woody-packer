#include "woody.h"
#include <elf.h>

inline const char *phdr_get_type_str(Elf64_Phdr *hdr) {
	switch (hdr->p_type) {
	case (PT_NULL):
		return "PT_NULL";
	case (PT_LOAD):
		return "PT_LOAD";
	case (PT_DYNAMIC):
		return "PT_DYNAMIC";
	case (PT_INTERP):
		return "PR_INTERP";
	case (PT_NOTE):
		return "PT_NOTE";
	case (PT_SHLIB):
		return "PT_SHLIB";
	case (PT_PHDR):
		return "PT_PHDR";
	case (PT_LOPROC):
		return "PT_LOPROC";
	case (PT_HIPROC):
		return "PT_HIPROC";
	case (PT_GNU_STACK):
		return "PT_GNU_STACK";
	default:
		return "unknown";
	}
}

inline int is_text_segment(const Elf64_Phdr *hdr) {
	return (hdr->p_flags & PF_X && hdr->p_flags & PF_R);
}

inline int is_data_segment(const Elf64_Phdr *hdr) {
	return (hdr->p_flags & PF_W && hdr->p_flags & PF_R);
}

inline int is_pt_load(const Elf64_Phdr *hdr) {
	return (hdr->p_type == PT_LOAD);
}
