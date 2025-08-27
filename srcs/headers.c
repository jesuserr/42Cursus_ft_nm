/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:41:42 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/27 22:45:41 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static uint8_t	print_file_format_not_recognized(t_args *args)
{
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	ft_putstr_fd(args->file_name, STDERR_FILENO);
	ft_putstr_fd(": file format not recognized\n", STDERR_FILENO);
	args->exit_status++;
	return (ELFCLASSNONE);
}

// Extracts the ELF header from the mapped file content. It checks the magic
// number, class (32/64-bit), endianness, version and OS ABI from E_IDENT. If
// E_IDENT seems valid, it copies entire ELF header to the corresponding struct
// in t_args and returns class (32/64-bit), otherwise returns ELFCLASSNONE (0)
// with an error message.
// The ELF header is 52 or 64 bytes long for 32-bit and 64-bit binaries.
// *** Only E_IDENT is checked for validity, other fields are not checked. ***
uint8_t	extract_elf_header(t_args *args)
{
	uint32_t	magic_number;
	char		*ptr;

	ptr = (char *)args->file_content;
	ft_bzero(&args->elf32_header, sizeof(Elf32_Ehdr));
	ft_bzero(&args->elf64_header, sizeof(Elf64_Ehdr));
	if (args->file_size < sizeof(Elf32_Ehdr))
		return (print_file_format_not_recognized(args));
	magic_number = *(uint32_t *)args->file_content;
	if (magic_number != ELF_MAGIC_NUMBER || (ptr[EI_CLASS] != ELFCLASS64 && \
	ptr[EI_CLASS] != ELFCLASS32) || ptr[EI_DATA] != ELFDATA2LSB || \
	ptr[EI_VERSION] != EV_CURRENT || ptr[EI_OSABI] != ELFOSABI_SYSV)
		return (print_file_format_not_recognized(args));
	if (ptr[EI_CLASS] == ELFCLASS64)
		ft_memcpy(&args->elf64_header, args->file_content, sizeof(Elf64_Ehdr));
	else if (ptr[EI_CLASS] == ELFCLASS32)
		ft_memcpy(&args->elf32_header, args->file_content, sizeof(Elf32_Ehdr));
	return (ptr[EI_CLASS]);
}
