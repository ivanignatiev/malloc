/*
** show_mem.c for malloc in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 19:34:37 2013 ivan ignatiev
** Last update Sun Feb 10 19:35:12 2013 ivan ignatiev
*/

#include		"static.h"

void			show_alloc_mem()
{
  t_malloc_head		*next;

  printf("break : %p\n", g_memory_end);
  next = g_memory_begin;
  while ((next = malloc_get_info(next)) != NULL)
    {
      printf("%p - %p : %lu bytes\n",
	     next->addr,
	     next->addr + next->size_next + _HEADER_SIZE,
	     next->size_next);
      next = malloc_goto_next(next);
    }
  printf("end : %p (sysbrk: %p) (%lu)\n", g_memory_brk, g_memory_end,
	 (g_memory_end - g_memory_begin) / getpagesize());
}

void			show_alloc_mem_detail()
{
  t_malloc_head		*next;

  printf("begin : %p\n", g_memory_begin);
  next = g_memory_begin;
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
  printf("end : %p (sysbrk: %p) (%lu)\n", g_memory_brk, g_memory_end,
	 (g_memory_end - g_memory_begin) / getpagesize());
}
