/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:41:42 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/20 00:45:27 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

// Extracts the ELF header from the mapped file content. It checks the magic
// number, class (32/64-bit), endianness, version and OS ABI from E_IDENT. If
// E_IDENT seems valid, it copies entire ELF header to the corresponding struct,
// determines pointer to section header table and returns class (32 or 64-bit).
// If E_IDENT is not valid returns ELFCLASSNONE (0) with an error message.
// The ELF header is 52 or 64 bytes long for 32-bit and 64-bit binaries. Only
// E_IDENT is checked for file validity, more checks could be made, but this is
// my balance for this project.
uint8_t	extract_elf_header(t_args *args, t_data *data)
{
	uint32_t	magic_number;
	char		*ptr;

	ptr = (char *)args->file_content;
	if (args->file_size < sizeof(Elf32_Ehdr))
		return (print_file_format_not_recognized(args));
	magic_number = *(uint32_t *)args->file_content;
	if (magic_number != ELF_MAGIC_NUMBER || (ptr[EI_CLASS] != ELFCLASS64 && \
	ptr[EI_CLASS] != ELFCLASS32) || ptr[EI_DATA] != ELFDATA2LSB || \
	ptr[EI_VERSION] != EV_CURRENT || (ptr[EI_OSABI] != ELFOSABI_SYSV && \
	ptr[EI_OSABI] != ELFOSABI_LINUX))
		return (print_file_format_not_recognized(args));
	if (ptr[EI_CLASS] == ELFCLASS64)
	{
		ft_memcpy(&data->elf64_header, args->file_content, sizeof(Elf64_Ehdr));
		if (data->elf64_header.e_shoff + (data->elf64_header.e_shentsize * \
		data->elf64_header.e_shnum) > args->file_size)
			return (print_file_format_not_recognized(args));
		data->elf64_sec_table = (Elf64_Shdr *)(ptr + data->elf64_header.e_shoff);
	}
	else if (ptr[EI_CLASS] == ELFCLASS32)
	{
		ft_memcpy(&data->elf32_header, args->file_content, sizeof(Elf32_Ehdr));
		if (data->elf32_header.e_shoff + (data->elf32_header.e_shentsize * \
		data->elf32_header.e_shnum) > args->file_size)
			return (print_file_format_not_recognized(args));
		data->elf32_sec_table = (Elf32_Shdr *)(ptr + data->elf32_header.e_shoff);
	}
	data->elf_class = ptr[EI_CLASS];
	return (ptr[EI_CLASS]);
}

// Searches through the 32-bit ELF section header table to locate the symbol
// table section (SHT_SYMTAB). When found, it stores the symbol table index,
// calculates the symbol table pointer and finds the associated string table via
// sh_link. Also finds the section header string table. Returns true on success
// and false if no symbol table is found.
bool	find_sym_table_32(t_args *args, t_data *data)
{
	char		*ptr;
	uint64_t	i;
	uint64_t	strtab_ix;
	uint32_t	symtab_offset;
	uint16_t	shstrndx;

	ptr = (char *)args->file_content;
	for (i = 0; i < data->elf32_header.e_shnum; i++)
	{
		if (data->elf32_sec_table[i].sh_type == SHT_SYMTAB)
		{
			data->sym_table_ix = i;
			symtab_offset = data->elf32_sec_table[i].sh_offset;
			data->elf32_sym_table = (Elf32_Sym *)(ptr + symtab_offset);
			strtab_ix = data->elf32_sec_table[i].sh_link;
			data->str_table = ptr + data->elf32_sec_table[strtab_ix].sh_offset;
			shstrndx = data->elf32_header.e_shstrndx;
			data->shstr_table = ptr + data->elf32_sec_table[shstrndx].sh_offset;
			return (true);
		}
	}
	return (print_no_symbols(args));
}

// Iterates through the 32-bit ELF symbol table and extracts symbols based on
// command-line flags. Filters out file/section symbols unless debugger mode is
// enabled, shows only undefined symbols if -u flag is set, or only external
// symbols if -g flag is set. Selected symbols are copied into a linked list.
void	extract_symbols_32(t_args *args, t_data *data)
{
	uint64_t	i;
	uint8_t		bind, type;
	uint64_t	sym_count;
	Elf32_Sym	*new_node;
	t_list		*last_node, *new_list_node;

	sym_count = data->elf32_sec_table[data->sym_table_ix].sh_size / sizeof(Elf32_Sym);
	for (i = 1; i < sym_count; i++)
	{
		bind = ELF32_ST_BIND(data->elf32_sym_table[i].st_info);
		type = ELF32_ST_TYPE(data->elf32_sym_table[i].st_info);
		if (!args->debugger_only && (type == STT_FILE || \
		type == STT_SECTION || data->elf32_sym_table[i].st_name == 0))
			continue ;
		if (args->undefined_only && data->elf32_sym_table[i].st_shndx != SHN_UNDEF)
			continue ;
		if (args->external_only && bind != STB_GLOBAL && bind != STB_WEAK)
			continue ;
		new_node = malloc(sizeof(Elf32_Sym));
		ft_memcpy(new_node, &data->elf32_sym_table[i], sizeof(Elf32_Sym));
		new_list_node = ft_lstnew(new_node);
		if (!data->sym_list)
			data->sym_list = new_list_node;
		else
			last_node->next = new_list_node;
		last_node = new_list_node;
	}
}

// Searches through the 64-bit ELF section header table to locate the symbol
// table section (SHT_SYMTAB). When found, it stores the symbol table index,
// calculates the symbol table pointer and finds the associated string table via
// sh_link. Also finds the section header string table. Returns true on success
// and false if no symbol table is found.
bool	find_sym_table_64(t_args *args, t_data *data)
{
	char		*ptr;
	uint64_t	i;
	uint64_t	strtab_ix;
	uint64_t	symtab_offset;
	uint16_t	shstrndx;

	ptr = (char *)args->file_content;
	for (i = 0; i < data->elf64_header.e_shnum; i++)
	{
		if (data->elf64_sec_table[i].sh_type == SHT_SYMTAB)
		{
			data->sym_table_ix = i;
			symtab_offset = data->elf64_sec_table[i].sh_offset;
			data->elf64_sym_table = (Elf64_Sym *)(ptr + symtab_offset);
			strtab_ix = data->elf64_sec_table[i].sh_link;
			data->str_table = ptr + data->elf64_sec_table[strtab_ix].sh_offset;
			shstrndx = data->elf64_header.e_shstrndx;
			data->shstr_table = ptr + data->elf64_sec_table[shstrndx].sh_offset;
			return (true);
		}
	}
	return (print_no_symbols(args));
}

// Iterates through the 64-bit ELF symbol table and extracts symbols based on 
// command-line flags. Filters out file/section symbols unless debugger mode is
// enabled, shows only undefined symbols if -u flag is set, or only external   
// symbols if -g flag is set. Selected symbols are copied into a linked list.
void	extract_symbols_64(t_args *args, t_data *data)
{
	uint64_t	i;
	uint8_t		bind, type;
	uint64_t	sym_count;
	Elf64_Sym	*new_node;
	t_list		*last_node, *new_list_node;

	sym_count = data->elf64_sec_table[data->sym_table_ix].sh_size / sizeof(Elf64_Sym);
	for (i = 1; i < sym_count; i++)
	{
		bind = ELF64_ST_BIND(data->elf64_sym_table[i].st_info);
		type = ELF64_ST_TYPE(data->elf64_sym_table[i].st_info);
		if (!args->debugger_only && (type == STT_FILE || \
		type == STT_SECTION || data->elf64_sym_table[i].st_name == 0))
			continue ;
		if (args->undefined_only && data->elf64_sym_table[i].st_shndx != SHN_UNDEF)
			continue ;
		if (args->external_only && bind != STB_GLOBAL && bind != STB_WEAK)
			continue ;
		new_node = malloc(sizeof(Elf64_Sym));
		ft_memcpy(new_node, &data->elf64_sym_table[i], sizeof(Elf64_Sym));
		new_list_node = ft_lstnew(new_node);
		if (!data->sym_list)
			data->sym_list = new_list_node;
		else
			last_node->next = new_list_node;
		last_node = new_list_node;
	}
}

// Note on 'extract_symbols_64' and 'extract_symbols_32' functions:
// Project was evaluated using ft_lstadd_back() to add new nodes at the
// end of the list, resulting in O(n²) complexity, since the whole list had to
// be traversed to find the last node on each insertion, resulting in very long
// execution times for large symbol tables (i.e. liblldb.so with ~150K symbols).
// For optimization, a pointer to the last node is maintained and new nodes are
// appended directly, achieving O(n).
