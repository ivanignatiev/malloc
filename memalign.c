/*
** memalign.c for malloc in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 19:35:40 2013 ivan ignatiev
** Last update Sun Feb 10 19:36:15 2013 ivan ignatiev
*/

#include		"static.h"

static void		memalign_fragment(void *addr, ssize_t *aligment,
					  size_t prev_size,
					  t_malloc_head *prev_block)
{
  if (*aligment > (ssize_t)(2 * _HEADER_SIZE)
      && malloc_edges(addr, (size_t) *aligment))
    malloc_put_info(addr, 0, (size_t) *aligment, prev_size);
  else
    {
      *aligment = prev_size;
      if (prev_block)
	prev_block->size_next = *aligment;
    }
}

static void		*memalign_address(ssize_t *aligment, size_t boundary)
{
  void			*addr;
  void			*aligned_addr;
  t_malloc_head		*prev_block;
  size_t		prev_size;

  prev_block = malloc_search_lastblock(g_memory_begin);
  if (prev_block)
    {
      prev_size = prev_block->size_next;
      addr = (void*)prev_block + prev_block->size_next + _HEADER_SIZE;
    }
  else
    {
      addr = g_memory_begin;
      prev_size = 0;
    }
  aligned_addr = (void*)malloc_align((size_t)addr, boundary) - _HEADER_SIZE;
  if (aligned_addr < g_memory_begin)
    aligned_addr += boundary;
  *aligment = aligned_addr - addr - _HEADER_SIZE;
  memalign_fragment(addr, aligment, prev_size, prev_block);
  return (aligned_addr);
}

void			*memalign(size_t boundary, size_t size)
{
  void			*aligned_addr;
  ssize_t		delta_size;

  if (!malloc_init() || size == 0)
    return (NULL);
  MALLOC_LOCK();
  aligned_addr = memalign_address(&delta_size, boundary);
  size = malloc_align(size + delta_size + 1, MALLOC_BOUNDARY);
  if (malloc_edges(aligned_addr, size))
    {
      malloc_put_info(aligned_addr, 1, size, delta_size);
      if (aligned_addr + size + _HEADER_SIZE > g_memory_brk)
	g_memory_brk = aligned_addr + size + _HEADER_SIZE;
      MALLOC_UNLOCK();
      return (aligned_addr + _HEADER_SIZE);
    }
  MALLOC_UNLOCK();
  return (NULL);
}

int			posix_memalign(void **memptr, size_t aligment, size_t size)
{
  if (aligment % sizeof(void*))
    return (EINVAL);
  if ((*memptr = memalign(aligment, size)) != NULL)
    {
      return (0);
    }
  return (ENOMEM);
}

void			*valloc(size_t size)
{
  return (memalign(getpagesize(), size));
}
