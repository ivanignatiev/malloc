/*
** free.c for malloc in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 19:33:32 2013 ivan ignatiev
** Last update Sun Feb 10 19:34:23 2013 ivan ignatiev
*/

#include		"static.h"

static void		*merge_freespace(t_malloc_head *head)
{
  size_t		value;
  t_malloc_head		*merged;

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

static void		free_cuttail(void *head)
{
  unsigned long		edge;
  int			page_size;

  g_memory_brk = head;
  page_size = getpagesize();
  edge = (size_t) g_memory_brk / page_size;
  if ( (size_t) g_memory_brk % page_size )
    ++edge;
  g_memory_end = ((void*)(page_size * edge));
  brk(g_memory_end);
}

void			free(void *ptr)
{
  t_malloc_head		*head;
  t_malloc_head		*next;

  MALLOC_LOCK();
  if (ptr && (head = malloc_get_info(ptr - _HEADER_SIZE)) != NULL
      && head->alloc)
    {
      head->alloc = 0;
      head = merge_freespace(head);
      if ( (next = malloc_get_info( malloc_goto_next(head) ) ) == NULL)
	free_cuttail(head);
      else
	next->size_prev = head->size_next;
    }
  MALLOC_UNLOCK();
}
