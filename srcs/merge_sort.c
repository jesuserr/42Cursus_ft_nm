/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:22:47 by jesuserr          #+#    #+#             */
/*   Updated: 2025/09/02 10:07:33 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
// ****** File imported from ft_ls project, modified to fit ft_nm needs ***** //

// Comparison function to sort the list by symbol name. Handles both 32-bit and
// 64-bit ELF symbols, extracting symbol names from either the string table or
// section header string table (for section symbols without names). Supports
// both normal and reverse sorting order based on the reverse flag.
static int	compare(const void *a, const void *b, t_data *data, bool reverse)
{
	Elf64_Sym	*sym_a_64, *sym_b_64;
	Elf32_Sym	*sym_a_32, *sym_b_32;
	char		*name_a, *name_b;

	if (data->elf_class == ELFCLASS32)
	{
		sym_a_32 = (Elf32_Sym *)a;
		sym_b_32 = (Elf32_Sym *)b;
		if (sym_a_32->st_name == 0 && ELF32_ST_TYPE(sym_a_32->st_info) == STT_SECTION)
			name_a = &data->shstr_table[data->elf32_sec_table[sym_a_32->st_shndx].sh_name];
		else
			name_a = &data->str_table[sym_a_32->st_name];
		if (sym_b_32->st_name == 0 && ELF32_ST_TYPE(sym_b_32->st_info) == STT_SECTION)
			name_b = &data->shstr_table[data->elf32_sec_table[sym_b_32->st_shndx].sh_name];
		else
			name_b = &data->str_table[sym_b_32->st_name];
	}
	else if (data->elf_class == ELFCLASS64)
	{
		sym_a_64 = (Elf64_Sym *)a;
		sym_b_64 = (Elf64_Sym *)b;
		if (sym_a_64->st_name == 0 && ELF64_ST_TYPE(sym_a_64->st_info) == STT_SECTION)
			name_a = &data->shstr_table[data->elf64_sec_table[sym_a_64->st_shndx].sh_name];
		else
			name_a = &data->str_table[sym_a_64->st_name];
		if (sym_b_64->st_name == 0 && ELF64_ST_TYPE(sym_b_64->st_info) == STT_SECTION)
			name_b = &data->shstr_table[data->elf64_sec_table[sym_b_64->st_shndx].sh_name];
		else
			name_b = &data->str_table[sym_b_64->st_name];
	}
	if (reverse)
		return (ft_strcmp(name_b, name_a));
	else
		return (ft_strcmp(name_a, name_b));
}

// Uses the fast and slow pointer technique to split the list into two halves.
// First of all checks again if the list is empty or has only one element (it
// has already been checked in sort_list() but is good practice to check it).
// Then using the fast and slow pointers, it finds the middle of the list and
// splits it into two halves by setting the next pointer of the slow pointer to
// NULL.
static void	split_list(t_list *head, t_list **middle)
{
	t_list	*fast;
	t_list	*slow;

	if (!head || !head->next)
	{
		*middle = NULL;
		return ;
	}
	slow = head;
	fast = head->next;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	*middle = slow->next;
	slow->next = NULL;
}

// Function could be recursive but it is implemented as iterative to avoid
// potential stack overflow for very large lists.
// Uses dummy node technique which is a common pattern in linked list
// manipulation to simplify code by eliminating special cases for the head node.
// Tail is used to keep track of the last node in the merged list.
static t_list	*merge_lists(t_list *a, t_list *b, t_data *data, bool reverse)
{
	t_list	dummy;
	t_list	*tail;

	if (!a)
		return (b);
	if (!b)
		return (a);
	dummy.next = NULL;
	tail = &dummy;
	while (a && b)
	{
		if (compare(a->content, b->content, data, reverse) <= 0)
		{
			tail->next = a;
			a = a->next;
		}
		else
		{
			tail->next = b;
			b = b->next;
		}
		tail = tail->next;
	}
	if (a)
		tail->next = a;
	else
		tail->next = b;
	return (dummy.next);
}

// Sorts a linked list using the merge sort algorithm.
// https://www.youtube.com/watch?v=TGveA1oFhrc
// If the list is empty or has only one element, it returns since it is sorted.
// The sort_list() function takes a double pointer (t_list **list) instead of a
// single pointer (t_list *list) because it needs to modify the original pointer
// that points to the head of the list.
void	sort_list(t_list **list, t_data *data, bool reverse)
{
	t_list	*head;
	t_list	*middle;

	if (!list || !*list || !(*list)->next)
		return ;
	head = *list;
	split_list(head, &middle);
	sort_list(&head, data, reverse);
	sort_list(&middle, data, reverse);
	*list = merge_lists(head, middle, data, reverse);
}
