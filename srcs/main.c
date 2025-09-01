/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:12:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/01 12:26:23 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	list_symbols(t_args *args, t_data *data)
{
	uint8_t	elf_class;

	if (ft_lstsize(args->cli_files_list) > 1)
		ft_printf("%s:\n", args->file_name);
	elf_class = extract_elf_header(args, data);
	if (elf_class == ELFCLASS32 && find_sym_table_32(args, data))
	{
		extract_symbols_32(args, data);
		print_symbols_32(data);
	}
	else if (elf_class == ELFCLASS64 && find_sym_table_64(args, data))
	{
		extract_symbols_64(args, data);
		print_symbols_64(data);
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
		ft_printf("\n");
		list = list->next;
	}
	free_allocated_memory(&args);
	return (args.exit_status);
}
