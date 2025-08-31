/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:41:42 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/01 01:12:16 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

// Extracts the ELF header from the mapped file content. It checks the magic
// number, class (32/64-bit), endianness, version and OS ABI from E_IDENT. If
// E_IDENT seems valid, it copies entire ELF header to the corresponding struct,
// fills pointer to section header table and returns class (32/64-bit),
// otherwise returns ELFCLASSNONE (0) with an error message.
// The ELF header is 52 or 64 bytes long for 32-bit and 64-bit binaries.
// *** Only E_IDENT is checked for validity, other fields are not checked. ***
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
	ptr[EI_VERSION] != EV_CURRENT || ptr[EI_OSABI] != ELFOSABI_SYSV)
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
	return (ptr[EI_CLASS]);
}

bool	find_sym_table_32(t_args *args, t_data *data)
{
	char		*ptr;
	uint64_t	strtab_ix;
	uint64_t	symtab_ix;
	uint32_t	symtab_offset;

	ptr = (char *)args->file_content;
	for (symtab_ix = 0; symtab_ix < data->elf32_header.e_shnum; symtab_ix++)
	{
		if (data->elf32_sec_table[symtab_ix].sh_type == SHT_SYMTAB)
		{
			data->sym_table_ix = symtab_ix;
			symtab_offset = data->elf32_sec_table[symtab_ix].sh_offset;
			data->elf32_sym_table = (Elf32_Sym *)(ptr + symtab_offset);
			strtab_ix = data->elf32_sec_table[symtab_ix].sh_link;
			data->str_table = ptr + data->elf32_sec_table[strtab_ix].sh_offset;
			return (true);
		}
	}
	return (print_no_symbols(args));
}

void	extract_symbols_32(t_args *args, t_data *data)
{
	uint64_t	i;
	uint8_t		bind;
	uint8_t		type;
	uint64_t	sym_count;

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
		ft_printf("%s\n", &data->str_table[data->elf32_sym_table[i].st_name]);
	}
}

bool	find_sym_table_64(t_args *args, t_data *data)
{
	char		*ptr;
	uint64_t	strtab_ix;
	uint64_t	symtab_ix;
	uint64_t	symtab_offset;

	ptr = (char *)args->file_content;
	for (symtab_ix = 0; symtab_ix < data->elf64_header.e_shnum; symtab_ix++)
	{
		if (data->elf64_sec_table[symtab_ix].sh_type == SHT_SYMTAB)
		{
			data->sym_table_ix = symtab_ix;
			symtab_offset = data->elf64_sec_table[symtab_ix].sh_offset;
			data->elf64_sym_table = (Elf64_Sym *)(ptr + symtab_offset);
			strtab_ix = data->elf64_sec_table[symtab_ix].sh_link;
			data->str_table = ptr + data->elf64_sec_table[strtab_ix].sh_offset;
			return (true);
		}
	}
	return (print_no_symbols(args));
}

// the idea is for this function to extract the symbols from the symtab section
// and return them in a linked list of array of structs
//printf("value = '%016lx' ", data->elf64_sym_table[i].st_value);
//fflush(stdout);
//printf("type = '%d' ", ELF64_ST_TYPE(data->elf64_sym_table[i].st_info));
//ft_printf("value='%d'\n", data->elf64_sym_table[i].st_value);
void	extract_symbols_64(t_args *args, t_data *data)
{
	uint64_t	i;
	uint8_t		bind;
	uint8_t		type;
	uint64_t	sym_count;

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
		ft_printf("%s\n", &data->str_table[data->elf64_sym_table[i].st_name]);
	}
}
