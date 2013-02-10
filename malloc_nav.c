/*
** malloc_nav.c for  in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 19:37:25 2013 ivan ignatiev
** Last update Sun Feb 10 19:38:20 2013 ivan ignatiev
*/

#include		"static.h"

static t_malloc_head	*malloc_get_info(void *ptr)
{
  t_malloc_head         *head;

  if (ptr != NULL && ptr >= g_memory_begin
      && ptr + _HEADER_SIZE < g_memory_brk)
  {
    head = (t_malloc_head*)ptr;
    if (head->addr == ptr)
      return (head);
  }
  return (NULL);
}

static t_malloc_head	*malloc_goto_next(t_malloc_head *head)
{
  if (head != NULL && ((void*)head + _HEADER_SIZE) < g_memory_brk
      && head->addr == head)
    return (head->addr + _HEADER_SIZE + head->size_next);
  else
    return (NULL);
}

static t_malloc_head	*malloc_goto_prev(t_malloc_head *head)
{
  if (head != NULL && ((void*)head) > g_memory_begin
      && head->addr == (void*)head)
    return (head->addr - _HEADER_SIZE - head->size_prev);
  else
    return (NULL);
}

static void		malloc_put_info(void *ptr, char alloc,
					size_t size_next, size_t size_prev)
{
  t_malloc_head		head;

  head.addr = ptr;
  head.size_next = size_next;
  head.size_prev = size_prev;
  head.alloc = alloc;
  malloc_memcpy(head.addr, &head, _HEADER_SIZE);
}
