/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:12:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/09 00:00:46 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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
