/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:12:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/04 11:14:28 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

// Processes and displays symbols from an ELF file. Determines ELF class
// (32/64-bit), locates symbol table, extracts symbols, sorts them (unless -p
// flag is set), and prints in nm format. Handles both 32-bit and 64-bit ELF
// files with proper cleanup of allocated memory and file mapping.
static void	list_symbols(t_args *args, t_data *data)
{
	uint8_t	elf_class;

	elf_class = extract_elf_header(args, data);
	if (elf_class == ELFCLASS32 && find_sym_table_32(args, data))
	{
		if (ft_lstsize(args->cli_files_list) > 1 && !args->just_symbols)
			ft_printf("\n%s:\n", args->file_name);
		extract_symbols_32(args, data);
		if (!args->no_sort)
			sort_list(&data->sym_list, data, args->reverse_sort);
		print_symbols_32(args, data);
	}
	else if (elf_class == ELFCLASS64 && find_sym_table_64(args, data))
	{
		if (ft_lstsize(args->cli_files_list) > 1 && !args->just_symbols)
			ft_printf("\n%s:\n", args->file_name);
		extract_symbols_64(args, data);
		if (!args->no_sort)
			sort_list(&data->sym_list, data, args->reverse_sort);
		print_symbols_64(args, data);
	}
	if (data->sym_list)
		ft_lstclear(&data->sym_list, free);
	munmap(args->file_content, (size_t)args->file_size);
}

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_args	args;
	t_data	data;
	t_list	*list;

	(void)argc;
	ft_bzero(&args, sizeof(t_args));
	parse_arguments(argv, &args);
	list = args.cli_files_list;
	while (list)
	{
		args.file_name = (char *)list->content;
		ft_bzero(&data, sizeof(t_data));
		if (mmap_file_content(&args))
			list_symbols(&args, &data);
		else
			args.exit_status++;
		list = list->next;
	}
	free_allocated_memory(&args);
	return (args.exit_status);
}
