/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:09:50 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/27 21:08:31 by jesuserr         ###   ########.fr       */
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
# include <elf.h>					// for ELF structures

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define ELF_MAGIC_NUMBER		0x464C457F	// "\x7FELF" in little-endian

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
	char		*file_name;			// name of the current file
	uint64_t	file_size;			// size of the current file
	uint64_t	exit_status;		// exit status of the program
	Elf32_Ehdr	elf32_header;		// ELF 32-bit header
	Elf64_Ehdr	elf64_header;		// ELF 64-bit header
}	t_args;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/

/********************************** main.c ************************************/
void		free_allocated_memory(t_args *args);

/********************************** headers.c *********************************/
uint8_t		extract_elf_header(t_args *args);

/********************************** parser.c **********************************/
void		free_allocated_memory(t_args *args);
void		parse_arguments(char **argv, t_args *args);
bool		mmap_file_content(t_args *args);

#endif
