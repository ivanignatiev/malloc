/*
** static.h for malloc in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 15:40:12 2013 ivan ignatiev
** Last update Sun Feb 10 15:51:31 2013 ivan ignatiev
*/

#ifndef STATIC_H_
# define STATIC_H_

# include <stdio.h>
# include <pthread.h>
# include <errno.h>
# include "malloc.h"

typedef struct	s_malloc_head
{
  void		*addr;
  size_t	size_next;
  size_t	size_prev;
  char		alloc;
}		t_malloc_head;

# define _HEADER_SIZE		sizeof(t_malloc_head)
# define _SBRK_FAIL		(void*)-1
# define MALLOC_LOCK()		pthread_mutex_lock(&g_malloc_mutex)
# define MALLOC_UNLOCK()	pthread_mutex_unlock(g_&malloc_mutex)
# define MALLOC_BOUNDARY	sizeof(void*)

static void		*g_memory_begin	= NULL;
static void		*g_memory_brk	= NULL;
static void		*g_memory_end	= NULL;
static pthread_mutex_t	g_malloc_mutex	= PTHREAD_MUTEX_INITIALIZER;

static void		malloc_memcpy(void *dest, void *src, size_t size);
static size_t		malloc_align(size_t size, size_t boundary);
static int		malloc_init();
static int		malloc_edges(void *addr, size_t alloc_size);
static malloc_head_t	*malloc_get_info(void *ptr);
static malloc_head_t	*malloc_goto_next(t_malloc_head *head);
static malloc_head_t	*malloc_goto_prev(t_malloc_head *head);
static void		malloc_put_info(void *ptr, char alloc,
					size_t size_next, size_t size_prev);
static void		malloc_fragment(malloc_head_t *start, size_t *size);
static void		*malloc_search_freespace(void *start, size_t *size,
						 size_t *prev_size);

#endif /* !STATIC_H_ */
