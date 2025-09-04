/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:08:42 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/04 12:04:43 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

uint8_t	print_file_format_not_recognized(t_args *args)
{
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	ft_putstr_fd(args->file_name, STDERR_FILENO);
	ft_putstr_fd(": file format not recognized\n", STDERR_FILENO);
	args->exit_status++;
	return (ELFCLASSNONE);
}

bool	print_no_symbols(t_args *args)
{
	if (ft_lstsize(args->cli_files_list) > 1 && !args->just_symbols)
		ft_printf("\n%s:\n", args->file_name);
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	ft_putstr_fd(args->file_name, STDERR_FILENO);
	ft_putstr_fd(": no symbols\n", STDERR_FILENO);
	return (false);
}

bool	print_warning_is_directory(t_args *args)
{
	ft_putstr_fd("ft_nm: Warning: '", STDERR_FILENO);
	ft_putstr_fd(args->file_name, STDERR_FILENO);
	ft_putstr_fd("' is a directory\n", STDERR_FILENO);
	return (false);
}

uint8_t	count_hex_digits(uint64_t number)
{
	uint8_t	count;

	if (number == 0)
		return (1);
	count = 0;
	while (number > 0)
	{
		number /= 16;
		count++;
	}
	return (count);
}

void	print_char_n_times(char character, uint16_t n)
{
	while (n--)
		ft_printf("%c", character);
}
