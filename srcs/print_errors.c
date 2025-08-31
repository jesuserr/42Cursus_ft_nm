/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 11:06:07 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/31 11:09:30 by jesuserr         ###   ########.fr       */
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

void	print_no_symbols(t_args *args)
{
	ft_putstr_fd("ft_nm: ", STDERR_FILENO);
	ft_putstr_fd(args->file_name, STDERR_FILENO);
	ft_putstr_fd(": no symbols\n", STDERR_FILENO);
}
