/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:14:54 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/01 12:42:29 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_symbols_32(t_data *data)
{
	Elf32_Sym	*sym;
	t_list		*list;

	list = data->sym_list;
	while (list)
	{
		sym = (Elf32_Sym *)list->content;
		if (sym->st_value == 0)
			print_char_n_times(' ', HEX_DIGITS_32 + 1);
		else
		{
			print_char_n_times('0', HEX_DIGITS_32 - count_hex_digits(sym->st_value));
			ft_printf("%x ", sym->st_value);
		}
		ft_printf("%s\n", &data->str_table[sym->st_name]);
		list = list->next;
	}
}

void	print_symbols_64(t_data *data)
{
	Elf64_Sym	*sym;
	t_list		*list;

	list = data->sym_list;
	while (list)
	{
		sym = (Elf64_Sym *)list->content;
		if (sym->st_value == 0)
			print_char_n_times(' ', HEX_DIGITS_64 + 1);
		else
		{
			print_char_n_times('0', HEX_DIGITS_64 - count_hex_digits(sym->st_value));
			ft_printf("%x ", sym->st_value);
		}
		ft_printf("%s\n", &data->str_table[sym->st_name]);
		list = list->next;
	}
}
