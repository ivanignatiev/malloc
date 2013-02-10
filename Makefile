##
## Makefile for malloc in /home/ignati_i/projects/malloc
##
## Made by Ivan Ignatiev
## Login   <ignati_i@epitech.net>
##
## Started on  Thu Jan 31 13:09:48 2013 Ivan Ignatiev
## Last update Sun Feb 10 20:05:29 2013 ivan ignatiev
##

SRCS	= 	malloc.c \
		malloc_init.c \
		malloc_nav.c \
		memalign.c \
		realloc.c \
		calloc.c \
		show_mem.c \
		free.c

LNAME	= 	libmy_malloc_$(HOSTTYPE).so
LINKN	=	libmy_malloc.so

CC	= 	cc

TMPNAME	=	malloc.tmp.c

RM	=	rm -f

all: 		$(LNAME)

$(LNAME): 	$(SRCS)
		cat $(SRCS) > $(TMPNAME)
	  	$(CC) -fPIC -shared -Wall -Wextra -Werror -fPIC -lpthread $(TMPNAME) -o $(LNAME)
		ln $(LNAME) $(LINKN)

clean:
		$(RM) $(TMPNAME)
fclean: 	clean
		$(RM) $(LINKN)
		$(RM) $(LNAME)

re: 		fclean all

.PHONY:		all