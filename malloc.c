/*
** malloc.c for malloc in /home/ignati_i/projects/malloc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Thu Jan 31 13:15:48 2013 ivan ignatiev
** Last update Sun Feb 10 19:40:38 2013 ivan ignatiev
*/

#include		"static.h"

static void		malloc_fragment(t_malloc_head *start, size_t *size)
{
  ssize_t		size_next;
  t_malloc_head		*next;
  void			*addr;

  size_next = start->size_next - *size - _HEADER_SIZE;
  if (size_next > 0)
  {
    addr = start->addr + _HEADER_SIZE + *size;
    malloc_put_info(addr, 0, (size_t)size_next, *size);
    if ((next = malloc_goto_next( addr )) != NULL)
        next->size_prev = size_next;
  }
  else
    *size = start->size_next;
}

static void		*malloc_search_freespace(void *start, size_t *size,
						 size_t *prev_size)
{
  t_malloc_head		*next;
  t_malloc_head		*nav;

  next = start;
  *prev_size = 0;
  while ( (nav = malloc_get_info(next)) != NULL )
    {
      if (nav->alloc == 0 && nav->size_next >= *size)
	{
	  malloc_fragment(nav, size);
	  return (nav);
	}
      *prev_size = nav->size_next;
      next = malloc_goto_next(next);
    }
  return (next);
}

static t_malloc_head	*malloc_search_lastblock(void *start)
{
  t_malloc_head		*next;
  t_malloc_head		*nav;

  next = start;
  while (malloc_get_info(next) != NULL)
    {
      nav = next;
      next = malloc_goto_next(next);
    }
  return (malloc_get_info(nav));
}

void			*malloc(size_t size)
{
  size_t		size_prev;
  size_t		alloc_size;
  void			*addr;

  if (size == 0)
    return (NULL);
  if (malloc_init())
    {
      size = malloc_align(size, MALLOC_BOUNDARY);
      MALLOC_LOCK();
      addr = malloc_search_freespace(g_memory_begin, &size, &size_prev);
      alloc_size = size + _HEADER_SIZE;
      if (malloc_edges(addr, alloc_size))
	{
	  malloc_put_info(addr, 1, size, size_prev);
	  if (addr + alloc_size > g_memory_brk)
            g_memory_brk = addr + alloc_size;
	  MALLOC_UNLOCK();
	  return (addr + _HEADER_SIZE);
	}
      MALLOC_UNLOCK();
    }
  return (NULL);
}
