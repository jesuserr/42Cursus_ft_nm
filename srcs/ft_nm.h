/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:09:50 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/08 23:52:55 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h" 
# include <sys/stat.h>				// for fstat
# include <stdbool.h>				// for booleans
# include <stdint.h>				// for fixed-width integer types
# include <stdio.h>					// for perror

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/

# define COLOR_RESET			"\033[0m"		// reset color to default
# define COLOR_DIRECTORY		"\033[01;34m"	// bold blue
# define COLOR_SYMLINK			"\033[01;36m"	// bold cyan
# define COLOR_EXECUTABLE		"\033[01;32m"	// bold green
# define COLOR_SOCKET			"\033[01;35m"	// bold magenta
# define COLOR_NAMED_PIPE		"\033[40;33m"	// yellow on black
# define COLOR_DEVICE			"\033[01;37m"	// bold white
# define COLOR_STICKY_DIR		"\033[30;42m"	// black on green
# define COLOR_SETGID_FILE		"\033[30;43m"	// black on yellow
# define COLOR_SETUID_FILE		"\033[30;41m"	// black on red

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/

typedef struct s_args
{
	bool		debugger_only;		// option -a
	bool		external_only;		// option -g
	bool		no_sort;			// option -p
	bool		reverse_sort;		// option -r
	bool		undefined_only;		// option -u
	t_list		*cli_files_list;	// linked list of files to 'nm'
}	t_args;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/

/********************************** main.c ************************************/
void		free_allocated_memory(t_args *args);

/********************************** parser.c **********************************/
void		parse_arguments(char **argv, t_args *args);

#endif
