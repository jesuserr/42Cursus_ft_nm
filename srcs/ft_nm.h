/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:09:50 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/25 15:50:11 by jesuserr         ###   ########.fr       */
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
# include <sys/mman.h>				// for mmap/munmap
# include <fcntl.h>					// for open

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/

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
	void		*file_content;		// pointer to the mapped file content
	uint64_t	file_size;			// size of the current file
	uint64_t	exit_status;		// exit status of the program
}	t_args;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/

/********************************** main.c ************************************/
void		free_allocated_memory(t_args *args);

/********************************** parser.c **********************************/
void		parse_arguments(char **argv, t_args *args);
bool		mmap_file_content(t_args *args, char *file_name);

#endif
