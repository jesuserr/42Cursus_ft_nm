/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:12:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/27 22:51:25 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	list_symbols(t_args *args)
{
	uint8_t	elf_class;

	if (mmap_file_content(args))
	{
		if (ft_lstsize(args->cli_files_list) > 1)
			ft_printf("%s:\n", args->file_name);
		elf_class = extract_elf_header(args);
		if (elf_class == ELFCLASS32)
		{
			ft_printf("32 bits ELF header extracted successfully!!\n");
			ft_hex_dump(&args->elf32_header, sizeof(Elf32_Ehdr), 8);
		}
		else if (elf_class == ELFCLASS64)
		{
			ft_printf("64 bits ELF header extracted successfully!!\n");
			ft_hex_dump(&args->elf64_header, sizeof(Elf64_Ehdr), 16);
		}
		munmap(args->file_content, (size_t)args->file_size);
	}
	else
		args->exit_status++;
	ft_printf("\n");
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
		list_symbols(&args);
		list = list->next;
	}
	free_allocated_memory(&args);
	return (args.exit_status);
}
