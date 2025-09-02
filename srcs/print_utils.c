/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:08:42 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/02 12:17:49 by jesuserr         ###   ########.fr       */
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
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	ft_putstr_fd(args->file_name, STDERR_FILENO);
	ft_putstr_fd(": no symbols\n", STDERR_FILENO);
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
