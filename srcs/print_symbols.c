/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:14:54 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/02 10:19:41 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

// Determines the symbol type character for 32-bit ELF symbols following nm's
// convention: U=undefined, C=common, A/a=absolute, T/t=text (executable code),
// B/b=BSS (uninitialized data), D/d=data (initialized), R/r=read-only data,
// W/w=weak symbols, V/v=weak object symbols, N/n=debug symbols (sections),
// i=indirect function (GNU IFUNC). Uppercase indicates global binding or
// special cases (debug sections), lowercase is local binding.
static char	get_symbol_type_32(Elf32_Sym *symbol, Elf32_Shdr *sections)
{
	Elf32_Shdr	*section;

	if (ELF32_ST_TYPE(symbol->st_info) == STT_GNU_IFUNC)
		return ('i');
	if (ELF32_ST_BIND(symbol->st_info) == STB_WEAK)
	{
		if (ELF32_ST_TYPE(symbol->st_info) == STT_OBJECT)
			return (symbol->st_shndx == SHN_UNDEF) ? 'v' : 'V';
		if (symbol->st_shndx == SHN_UNDEF)
			return ('w');
		return ('W');
	}
	if (symbol->st_shndx == SHN_UNDEF)
		return ('U');
	if (symbol->st_shndx == SHN_COMMON)
		return ('C');
	if (symbol->st_shndx == SHN_ABS)
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'a' : 'A';
	if (symbol->st_shndx >= SHN_LORESERVE)
		return ('?');
	section = &sections[symbol->st_shndx];
	if (ELF32_ST_TYPE(symbol->st_info) == STT_SECTION && \
	(section->sh_type == SHT_PROGBITS || section->sh_type == SHT_STRTAB) && \
	!(section->sh_flags & SHF_ALLOC))
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'n' : 'N';
	if (section->sh_flags & SHF_EXECINSTR)
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL) ? 't' : 'T';
	else if (section->sh_type == SHT_NOBITS)
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'b' : 'B';
	else if (section->sh_flags & SHF_WRITE)
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'd' : 'D';
	else
		return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'r' : 'R';		
	return ('?');
}

// Prints 32-bit ELF symbols in nm format. Iterates through the symbol linked
// list, formatting each symbol's address as 8-digit hex (with leading zeros
// or spaces for undefined/weak symbols), followed by the symbol type character
// and name. For section symbols without names, displays the section name from
// the section header string table.
void	print_symbols_32(t_data *data)
{
	Elf32_Sym	*sym;
	t_list		*list;
	char		type;

	list = data->sym_list;
	while (list)
	{
		sym = (Elf32_Sym *)list->content;
		type = get_symbol_type_32(sym, data->elf32_sec_table);
		if (type == 'U' || type == 'w' || type == 'v')
			print_char_n_times(' ', HEX_DIGITS_32 + 1);
		else
		{
			print_char_n_times('0', HEX_DIGITS_32 - count_hex_digits(sym->st_value));
			ft_printf("%x ", sym->st_value);
		}
		ft_printf("%c ", type);
		if (sym->st_name == 0 && ELF32_ST_TYPE(sym->st_info) == STT_SECTION)
			ft_printf("%s\n", \
			&data->shstr_table[data->elf32_sec_table[sym->st_shndx].sh_name]);
		else
			ft_printf("%s\n", &data->str_table[sym->st_name]);
		list = list->next;
	}
}

// Determines the symbol type character for 64-bit ELF symbols following nm's
// convention: U=undefined, C=common, A/a=absolute, T/t=text (executable code),
// B/b=BSS (uninitialized data), D/d=data (initialized), R/r=read-only data,
// W/w=weak symbols, V/v=weak object symbols, N/n=debug symbols (sections),
// i=indirect function (GNU IFUNC). Uppercase indicates global binding or
// special cases (debug sections), lowercase is local binding.
static char	get_symbol_type_64(Elf64_Sym *symbol, Elf64_Shdr *sections)
{
	Elf64_Shdr	*section;

	if (ELF64_ST_TYPE(symbol->st_info) == STT_GNU_IFUNC)
		return ('i');
	if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
	{
		if (ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
			return (symbol->st_shndx == SHN_UNDEF) ? 'v' : 'V';
		if (symbol->st_shndx == SHN_UNDEF)
			return ('w');
		return ('W');
	}
	if (symbol->st_shndx == SHN_UNDEF)
		return ('U');
	if (symbol->st_shndx == SHN_COMMON)
		return ('C');
	if (symbol->st_shndx == SHN_ABS)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'a' : 'A';
	if (symbol->st_shndx >= SHN_LORESERVE)
		return ('?');
	section = &sections[symbol->st_shndx];
	if (ELF64_ST_TYPE(symbol->st_info) == STT_SECTION && \
	(section->sh_type == SHT_PROGBITS || section->sh_type == SHT_STRTAB) && \
	!(section->sh_flags & SHF_ALLOC))
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'n' : 'N';
	if (section->sh_flags & SHF_EXECINSTR)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL) ? 't' : 'T';
	else if (section->sh_type == SHT_NOBITS)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'b' : 'B';
	else if (section->sh_flags & SHF_WRITE)
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'd' : 'D';
	else
		return (ELF64_ST_BIND(symbol->st_info) == STB_LOCAL) ? 'r' : 'R';		
	return ('?');
}

// Prints 64-bit ELF symbols in nm format. Iterates through the symbol linked
// list, formatting each symbol's address as 16-digit hex (with leading zeros
// or spaces for undefined/weak symbols), followed by the symbol type character
// and name. For section symbols without names, displays the section name from
// the section header string table.
void	print_symbols_64(t_data *data)
{
	Elf64_Sym	*sym;
	t_list		*list;
	char		type;

	list = data->sym_list;
	while (list)
	{
		sym = (Elf64_Sym *)list->content;
		type = get_symbol_type_64(sym, data->elf64_sec_table);
		if (type == 'U' || type == 'w' || type == 'v')
			print_char_n_times(' ', HEX_DIGITS_64 + 1);
		else
		{
			print_char_n_times('0', HEX_DIGITS_64 - count_hex_digits(sym->st_value));
			ft_printf("%x ", sym->st_value);
		}
		ft_printf("%c ", type);
		if (sym->st_name == 0 && ELF64_ST_TYPE(sym->st_info) == STT_SECTION)
			ft_printf("%s\n", \
			&data->shstr_table[data->elf64_sec_table[sym->st_shndx].sh_name]);
		else
			ft_printf("%s\n", &data->str_table[sym->st_name]);
		list = list->next;
	}
}
