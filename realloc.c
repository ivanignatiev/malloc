/*
** realloc.c for malloc in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 19:38:28 2013 ivan ignatiev
** Last update Sun Feb 10 19:38:57 2013 ivan ignatiev
*/

#include		"static.h"

static void		*realloc_same_place(t_malloc_head *p, size_t size)
{
  MALLOC_LOCK();
  malloc_fragment(p->addr, &size);
  p->size_next = size;
  MALLOC_UNLOCK();
  return (p->addr + _HEADER_SIZE);
}

static void		*realloc_edge(t_malloc_head *p, size_t size)
{
  MALLOC_LOCK();
  if (malloc_edges( p->addr,  size + _HEADER_SIZE ) )
    {
      p->size_next = size;
      if ( p->addr + size + _HEADER_SIZE > g_memory_brk)
	g_memory_brk = p->addr + size + _HEADER_SIZE;
      MALLOC_UNLOCK();
      return (p->addr + _HEADER_SIZE);
    }
  MALLOC_UNLOCK();
  return (NULL);
}

static void		*realloc_new(t_malloc_head *p, size_t size)
{
  void			*addr;
  void			*ptr;

  ptr = p->addr + _HEADER_SIZE;
  if ((addr = malloc(size)) != NULL)
    {
      malloc_memcpy(addr, ptr, p->size_next);
      if (ptr != addr)
	free(ptr);
      return (addr);
    }
  return (addr);
}

void			*realloc(void *ptr, size_t size)
{
  t_malloc_head		*p;

  size = malloc_align(size, MALLOC_BOUNDARY);
  if (ptr == NULL)
    return (malloc(size));
  if ((p = malloc_get_info( ptr - _HEADER_SIZE )) != NULL)
    {
      if (size == 0)
	{
	  free(ptr);
	  return (NULL);
	}
      size = malloc_align(size, MALLOC_BOUNDARY);
      if (p->size_next >= size)
	return (realloc_same_place(p, size));
      else if (malloc_get_info( malloc_goto_next(p) ) == NULL)
	return (realloc_edge(p, size));
      else
	return (realloc_new(p, size));
    }
  return (NULL);
}
