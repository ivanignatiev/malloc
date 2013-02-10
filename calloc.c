/*
** calloc.c for calloc.c in /home/ignati_i//projects/malloc-2016-ignati_i
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Feb 10 19:39:33 2013 ivan ignatiev
** Last update Sun Feb 10 19:39:54 2013 ivan ignatiev
*/

#include		"static.h"

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
