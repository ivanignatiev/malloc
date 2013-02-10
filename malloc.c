/*
** malloc.c for malloc in /home/ignati_i/projects/malloc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Thu Jan 31 13:15:48 2013 ivan ignatiev
** Last update Sun Feb 10 16:12:40 2013 ivan ignatiev
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
  if (memory_begin == NULL)
    {
      if ((memory_begin = sbrk(0)) == _SBRK_FAIL)
	return (0);
      memory_brk = memory_begin;
      memory_end = memory_begin;
    }
  return (1);
}

static int		malloc_edges(void *addr, size_t alloc_size)
{
  int			page_size;
  int			pages_count;
  size_t		delta_size;

  if (addr + alloc_size < memory_brk)
    return (1);
  delta_size = addr + alloc_size - memory_brk;
  if (memory_end >  memory_brk + delta_size)
    {
      memory_brk += delta_size;
      return (1);
    }
  pages_count = (memory_brk + delta_size - memory_end);
  if (pages_count > 0)
    {
      page_size = getpagesize();
      pages_count = pages_count / page_size + 1;
      if (sbrk(page_size * pages_count) != _SBRK_FAIL)
	memory_end += page_size * pages_count;
      else
	return (0);
    }
  return (1);
}

static malloc_head_t	*malloc_get_info(void *ptr)
{
  malloc_head_t         *head;

  if (ptr != NULL && ptr >= memory_begin
      && ptr + _HEADER_SIZE < memory_brk)
  {
    head = (malloc_head_t*)ptr;
    if (head->addr == ptr)
      return (head);
  }
  return (NULL);
}

static malloc_head_t	*malloc_goto_next(malloc_head_t *head)
{
  if (head != NULL && ((void*)head + _HEADER_SIZE) < memory_brk
      && head->addr == head)
    return (head->addr + _HEADER_SIZE + head->size_next);
  else
    return (NULL);
}

static malloc_head_t	*malloc_goto_prev(malloc_head_t *head)
{
  if (head != NULL && ((void*)head) > memory_begin
      && head->addr == (void*)head)
    return (head->addr - _HEADER_SIZE - head->size_prev);
  else
    return (NULL);
}

static void		malloc_put_info(void *ptr, char alloc,
					size_t size_next, size_t size_prev)
{
  malloc_head_t		head;

  head.addr = ptr;
  head.size_next = size_next;
  head.size_prev = size_prev;
  head.alloc = alloc;
  malloc_memcpy(head.addr, &head, _HEADER_SIZE);
}

static void		malloc_fragment(malloc_head_t *start, size_t *size)
{
  ssize_t		size_next;
  malloc_head_t		*next;
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
  malloc_head_t		*next;
  malloc_head_t		*nav;

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

static malloc_head_t	*malloc_search_lastblock(void *start)
{
  malloc_head_t		*next;
  malloc_head_t		*nav;

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
      addr = malloc_search_freespace(memory_begin, &size, &size_prev);
      alloc_size = size + _HEADER_SIZE;
      if (malloc_edges(addr, alloc_size))
	{
	  malloc_put_info(addr, 1, size, size_prev);
	  if (addr + alloc_size > memory_brk)
            memory_brk = addr + alloc_size;
	  MALLOC_UNLOCK();
	  return (addr + _HEADER_SIZE);
	}
      MALLOC_UNLOCK();
    }
  return (NULL);
}

void			*calloc(size_t num, size_t size)
{
  char			*addr;
  size_t		i;

  size = num * size;
  if ((addr = (char*)malloc(size)) != NULL)
    {
      i = 0;
      while (i < size)
	{
	  addr[i] = 0;
	  ++i;
	}
    }
  return (addr);
}

void			*realloc(void *ptr, size_t size)
{
  malloc_head_t		*p;
  void			*addr;

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
      if (p->size_next >= size)
	{
	  MALLOC_LOCK();
	  malloc_fragment(p->addr, &size);
	  p->size_next = size;
	  MALLOC_UNLOCK();
	  return (ptr);
	}
      else if (malloc_get_info( malloc_goto_next(p) ) == NULL)
	{
	  MALLOC_LOCK();
	  if (malloc_edges( p->addr,  size + _HEADER_SIZE ) )
	    {
	      p->size_next = size;
	      if ( p->addr + size + _HEADER_SIZE > memory_brk)
		memory_brk = p->addr + size + _HEADER_SIZE;
	      MALLOC_UNLOCK();
	      return (ptr);
	    }
	  MALLOC_UNLOCK();
	}
      else if ((addr = malloc(size)) != NULL)
	{
	  malloc_memcpy(addr, ptr, p->size_next);
	  if (ptr != addr)
	    free(ptr);
	  return (addr);
	}
    }
  return (NULL);
}

void			*memalign(size_t boundary, size_t size)
{
  void			*addr;
  void			*aligned_addr;
  ssize_t		delta_size;
  malloc_head_t		*prev_block;
  size_t		prev_size;

  if (!malloc_init() || size == 0)
    return (NULL);
  prev_block = malloc_search_lastblock(memory_begin);
  if (prev_block)
    {
      prev_size = prev_block->size_next;
      addr = (void*)prev_block + prev_block->size_next + _HEADER_SIZE;
    }
  else
    {
      addr = memory_begin;
      prev_size = 0;
    }
  aligned_addr = (void*)malloc_align((size_t)addr, boundary) - _HEADER_SIZE;
  if (aligned_addr < memory_begin)
    aligned_addr += boundary;
  delta_size = aligned_addr - addr - _HEADER_SIZE;
  MALLOC_LOCK();
  if (delta_size > (ssize_t)(2 * _HEADER_SIZE))
    {
      if (malloc_edges(addr, (size_t) delta_size))
	{
	  malloc_put_info(addr, 0, (size_t) delta_size, prev_size);
	}
      else
	{
	  MALLOC_UNLOCK();
	  return (NULL);
	}
    }
  else
    {
      delta_size = prev_size;
      if (prev_block)
	prev_block->size_next = delta_size;
    }
  size = malloc_align(size + delta_size + 1, MALLOC_BOUNDARY);
  if (malloc_edges(aligned_addr, size))
    {
      malloc_put_info(aligned_addr, 1, size, delta_size);
      if (aligned_addr + size + _HEADER_SIZE > memory_brk)
	memory_brk = aligned_addr + size + _HEADER_SIZE;
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

static void		*merge_freespace(malloc_head_t *head)
{
  size_t		value;
  malloc_head_t		*merged;

  merged = head;
  value = head->size_next;
  while ((head = malloc_goto_prev(head)) != NULL
	 && (head = malloc_get_info(head)) != NULL && head->alloc != 1)
    {
      head->size_next += value + _HEADER_SIZE;
      merged = head;
      value = head->size_next;
    }
  head = merged;
  value = 0;
  while ((head = malloc_goto_next(head)) != NULL
	 && (head = malloc_get_info(head)) != NULL && head->alloc != 1)
    value += head->size_next + _HEADER_SIZE;
  merged->size_next += value;
  return (merged);
}

void			free(void *ptr)
{
  malloc_head_t		*head;
  malloc_head_t		*next;
  unsigned long		edge;
  int			page_size;

  MALLOC_LOCK();
  if (ptr && (head = malloc_get_info(ptr - _HEADER_SIZE)) != NULL
      && head->alloc)
    {
      head->alloc = 0;
    head = merge_freespace(head);
    if ( (next = malloc_get_info( malloc_goto_next(head) ) ) == NULL)
      {
	memory_brk = head;
	page_size = getpagesize();
	edge = (size_t) memory_brk / page_size;
	if ( (size_t) memory_brk % page_size )
	  ++edge;
	memory_end = ((void*)(page_size * edge));
	brk(memory_end);
      }
    else
      next->size_prev = head->size_next;
    }
  MALLOC_UNLOCK();
}

void			show_alloc_mem_detail()
{
  malloc_head_t		*next;

  printf(stdout, "break : %p\n", memory_end);
  next = memory_begin;
  while ((next = malloc_get_info(next)) != NULL)
    {
      printf("%p - %p : %lu bytes\n",
	     next->addr,
	     next->addr + next->size_next + _HEADER_SIZE,
	     next->size_next);
      next = malloc_goto_next(next);
    }
  printf("end : %p (sysbrk: %p) (%lu)\n", memory_brk, memory_end,
	 (memory_end - memory_begin) / getpagesize());
}

void			show_alloc_mem_detail()
{
  malloc_head_t		*next;

  printf(stdout, "begin : %p\n", memory_begin);
  next = memory_begin;
  while ((next = malloc_get_info(next)) != NULL)
    {
      printf("A(%d, %p) SIZE : %lu, PREV : %lu, MEM: %p - %p (%lu)\n",
	     next->alloc,
	     next->addr + _HEADER_SIZE,
	     next->size_next,
	     next->size_prev,
	     next->addr,
	     next->addr + next->size_next + _HEADER_SIZE,
	     (next->size_next + _HEADER_SIZE));
      next = malloc_goto_next(next);
    }
  printf("end : %p (sysbrk: %p) (%lu)\n", memory_brk, memory_end,
	 (memory_end - memory_begin) / getpagesize());
}

