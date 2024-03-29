/*
** malloc.h for malloc in /home/ignati_i/projects/malloc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Thu Jan 31 17:13:28 2013 ivan ignatiev
** Last update Sun Feb 10 19:53:29 2013 oleg kuznietsov
*/

#ifndef MALLOC_H_
# define MALLOC_H_

# include <unistd.h>

void	*malloc(size_t size);
void	*calloc(size_t num, size_t size);
void	*realloc(void *ptr, size_t size);
void	free(void *ptr);
void	show_alloc_mem(void);
void	show_alloc_mem_detail(void);
void	*valloc(size_t size);
void	*memalign(size_t boundary, size_t size);
int	posix_memalign(void **memptr, size_t aligment, size_t size);

#endif /* !MALLOC_H_ */
