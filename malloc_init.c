/*
** malloc_init.c for malloc in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 19:40:18 2013 ivan ignatiev
** Last update Sun Feb 10 19:40:48 2013 ivan ignatiev
*/

#include		"static.h"

static void		malloc_memcpy(void *dest, void *src, size_t size)
{
  size_t		i;

  i = 0;
  while (i < size)
    {
      *((char*)(dest) + i) = *((char*)(src) + i);
      ++i;
    }
}

static size_t		malloc_align(size_t size, size_t boundary)
{
  size_t		units_count;

  units_count = size / boundary + 1;
  if (size % boundary)
    ++units_count;
  return (units_count * boundary);
}

static int		malloc_init()
{
  if (g_memory_begin == NULL)
    {
      if ((g_memory_begin = sbrk(0)) == _SBRK_FAIL)
	return (0);
      g_memory_brk = g_memory_begin;
      g_memory_end = g_memory_begin;
    }
  return (1);
}

static int		malloc_edges(void *addr, size_t alloc_size)
{
  int			page_size;
  int			pages_count;
  size_t		delta_size;

  if (addr + alloc_size < g_memory_brk)
    return (1);
  delta_size = addr + alloc_size - g_memory_brk;
  if (g_memory_end >  g_memory_brk + delta_size)
    {
      g_memory_brk += delta_size;
      return (1);
    }
  pages_count = (g_memory_brk + delta_size - g_memory_end);
  if (pages_count > 0)
    {
      page_size = getpagesize();
      pages_count = pages_count / page_size + 1;
      if (sbrk(page_size * pages_count) != _SBRK_FAIL)
	g_memory_end += page_size * pages_count;
      else
	return (0);
    }
  return (1);
}
