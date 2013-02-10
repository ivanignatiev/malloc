##
## Makefile for malloc in /home/ignati_i/projects/malloc
##
## Made by Ivan Ignatiev
## Login   <ignati_i@epitech.net>
##
## Started on  Thu Jan 31 13:09:48 2013 Ivan Ignatiev
## Last update Sun Feb 10 18:47:32 2013 Kuznietsov Oleg
##

SRCS	= 	malloc.c

LNAME	= 	libmy_malloc_$(HOSTTYPE).so

CC	= 	cc

CFLAGS	+=	-Wall -Wextra -Werror -fPIC -lpthread -g

OBJS	=	$(SRCS:.c=.o)

RM	=	rm -f

all: 		$(LNAME)

$(LNAME): 	$(OBJS)
	  	$(CC) -fPIC -shared $(OBJS) -o $(LNAME)

clean:
		$(RM) $(OBJS)

fclean: 	clean
		$(RM) $(LNAME)

re: 		fclean all

.PHONY:		all