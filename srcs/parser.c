/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:12:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/02 13:00:48 by jesuserr         ###   ########.fr       */
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

static void	print_invalid_option_and_exit(const char option, t_args *args)
{
	ft_putstr_fd("ft_nm: invalid option -- '", STDERR_FILENO);
	ft_putchar_fd(option, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_putstr_fd("Try 'ft_nm --help' for more information.\n", STDERR_FILENO);
	free_allocated_memory(args);
	exit(EXIT_FAILURE);
}

static void	print_usage(void)
{
	ft_printf("Usage:\n"
		"  ft_nm [option(s)] [file(s)]\n\n"
		"Options:\n"
		"  --help                  Display this help and exit\n"
		"  -a                      Display debugger-only symbols\n"
		"  -g                      Display only external symbols\n"
		"  -p                      Do not sort the symbols\n"
		"  -r                      Reverse the sense of the sort\n"
		"  -S                      Print size of defined symbols\n"
		"  -u                      Display only undefined symbols\n\n"
		"List symbols in [file(s)] (a.out by default).\n"
		"ft_nm: supported targets: 32-bit (x86_32), 64-bit (x64), .o and .so\n"
		"\nGitHub repository: <https://github.com/jesuserr/42Cursus_ft_nm>\n");
	exit(EXIT_SUCCESS);
}

// First of all verifies if '--help' is present along all arguments to not
// allocate anything and just print the help message and exit.
// All arguments that are not options (starting with '-') are added to the
// linked list of files to process. If no files are provided, 'a.out' is added
// to the list by default.
void	parse_arguments(char **argv, t_args *args)
{
	for (uint8_t i = 1; argv[i]; i++)
		if (!ft_strncmp(argv[i], "--help", 6) && ft_strlen(argv[i]) == 6)
			print_usage();
	for (uint8_t i = 1; argv[i]; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			for (uint8_t j = 1; argv[i][j]; j++)
			{
				if (argv[i][j] == 'a')
					args->debugger_only = true;
				else if (argv[i][j] == 'g')
					args->external_only = true;
				else if (argv[i][j] == 'p')
					args->no_sort = true;
				else if (argv[i][j] == 'r')
					args->reverse_sort = true;
				else if (argv[i][j] == 'S')
					args->print_size = true;
				else if (argv[i][j] == 'u')
					args->undefined_only = true;
				else
					print_invalid_option_and_exit(argv[i][j], args);
			}
		}
		else
			ft_lstadd_back(&args->cli_files_list, ft_lstnew(ft_strdup(argv[i])));
	}
	if (!args->cli_files_list)
		ft_lstadd_back(&args->cli_files_list, ft_lstnew(ft_strdup("a.out")));
}

// Uses 'mmap' to map the entire file into memory in one shot. Way more 
// efficient than reading the file multiple times. File size is kept for the
// 'munmap' function to know how many bytes to unmap when the program finishes
// (among other purposes).
// Returns true if mapping was successful, false otherwise.
bool	mmap_file_content(t_args *args)
{
	struct stat	file_stat;
	int			fd;

	fd = open(args->file_name, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("ft_nm: ", STDERR_FILENO);
		ft_putstr_fd(args->file_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (false);
	}
	if (fstat(fd, &file_stat) < 0)
	{
		close(fd);
		perror("fstat");
		return (false);
	}
	args->file_content = NULL;
	args->file_size = 0;
	if (file_stat.st_size > 0)
	{
		args->file_content = mmap(NULL, (size_t)file_stat.st_size, PROT_READ, \
		MAP_PRIVATE, fd, 0);
		if (args->file_content == MAP_FAILED)
		{
			close(fd);
			perror("mmap");
			return (false);
		}
		args->file_size = (uint64_t)file_stat.st_size;
	}
	close(fd);
	if (file_stat.st_size == 0)
		return (false);
	return (true);
}
