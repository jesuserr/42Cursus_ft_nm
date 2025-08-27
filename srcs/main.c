/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:12:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/27 14:28:29 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

// Frees memory allocated during program execution.
void	free_allocated_memory(t_args *args)
{
	if (args->cli_files_list)
		ft_lstclear(&args->cli_files_list, free);
	return ;
}

bool	print_file_format_not_recognized(t_args *args)
{
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	ft_putstr_fd(args->file_name, STDERR_FILENO);
	ft_putstr_fd(": file format not recognized\n", STDERR_FILENO);
	args->exit_status++;
	return (false);
}

// Extracts the ELF header from the mapped file content. It checks the magic
// number, class (32/64-bit), endianness, version and OS ABI from E_IDENT. If
// E_IDENT seems valid, it copies entire ELF header to the corresponding struct
// in t_args and returns true, otherwise returns false with an error message.
// The ELF header is 52 or 64 bytes long for 32-bit and 64-bit binaries.
// *** Only E_IDENT is checked for validity, other fields are not checked. ***
bool	extract_elf_header(t_args *args)
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
	ft_printf("So far ELF file detected!!\n", args->file_name);
	ft_hex_dump(&args->elf64_header, sizeof(Elf64_Ehdr), 16);
	ft_hex_dump(&args->elf32_header, sizeof(Elf64_Ehdr), 16);
	ft_printf("architecture: %d\n", ptr[EI_CLASS]);
	return (true);
}

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_args	args;
	t_list	*list;

	(void)argc;
	ft_bzero(&args, sizeof(t_args));
	parse_arguments(argv, &args);
	list = args.cli_files_list;
	while (list)
	{
		args.file_name = (char *)list->content;
		if (mmap_file_content(&args))
		{
			if (ft_lstsize(args.cli_files_list) > 1)
				ft_printf("%s:\n", args.file_name);
			extract_elf_header(&args);
			munmap(args.file_content, (size_t)args.file_size);
		}
		else
			args.exit_status++;
		ft_printf("\n");
		list = list->next;
	}
	free_allocated_memory(&args);
	return (args.exit_status);
}
