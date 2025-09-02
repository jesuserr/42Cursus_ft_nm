/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:09:50 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/02 13:29:30 by jesuserr         ###   ########.fr       */
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
# define HEX_DIGITS_32			8			// Number of hex digits in 32-bit
# define HEX_DIGITS_64			16			// Number of hex digits in 64-bit

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/

typedef struct s_args
{
	bool		debugger_only;		// option -a
	bool		external_only;		// option -g
	bool		just_symbols;		// option -j (extra bonus)
	bool		no_sort;			// option -p
	bool		reverse_sort;		// option -r
	bool		print_size;			// option -S (extra bonus)
	bool		undefined_only;		// option -u
	t_list		*cli_files_list;	// linked list of files to 'nm'
	void		*file_content;		// pointer to the mapped file content
	char		*file_name;			// name of the current file
	uint64_t	file_size;			// size of the current file
	uint64_t	exit_status;		// exit status of the program
}	t_args;

typedef struct s_data
{
	Elf32_Ehdr	elf32_header;		// ELF 32-bit header
	Elf32_Shdr	*elf32_sec_table;	// Pointer to 32-bit section header table
	Elf32_Sym	*elf32_sym_table;	// Pointer to 32-bit symbol table
	Elf64_Ehdr	elf64_header;		// ELF 64-bit header	
	Elf64_Shdr	*elf64_sec_table;	// Pointer to 64-bit section header table
	Elf64_Sym	*elf64_sym_table;	// Pointer to 64-bit symbol table
	char		*str_table;			// Pointer to string table
	char		*shstr_table;		// Pointer to section header string table
	uint64_t	sym_table_ix;		// Index of symtab header in sect hdr table
	t_list		*sym_list;			// Linked list of symbols
	uint8_t		elf_class;			// ELF class (ELFCLASS32 or ELFCLASS64)
}	t_data;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/

/********************************** headers.c *********************************/
uint8_t		extract_elf_header(t_args *args, t_data *data);
bool		find_sym_table_32(t_args *args, t_data *data);
void		extract_symbols_32(t_args *args, t_data *data);
bool		find_sym_table_64(t_args *args, t_data *data);
void		extract_symbols_64(t_args *args, t_data *data);

/********************************** merge_sort.c ******************************/
void		sort_list(t_list **list, t_data *data, bool reverse);

/********************************** parser.c **********************************/
void		free_allocated_memory(t_args *args);
void		parse_arguments(char **argv, t_args *args);
bool		mmap_file_content(t_args *args);

/********************************** print_symbols.c ***************************/
void		print_symbols_32(t_args *args, t_data *data);
void		print_symbols_64(t_args *args, t_data *data);

/********************************** print_utils.c *****************************/
uint8_t		print_file_format_not_recognized(t_args *args);
bool		print_no_symbols(t_args *args);
uint8_t		count_hex_digits(uint64_t number);
void		print_char_n_times(char character, uint16_t n);

#endif
