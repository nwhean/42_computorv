# Implicit variables
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -pedantic -std=c90 $(INCLUDE) -g3 -fsanitize=address
INCLUDE	=	-Iinclude
FDFLAGS	=	-L.
FDLIBS	=	-loop

# source and object files

SRCDIR	=	src
SRCS	=	Object.c \
			Circle.c \
			Point.c

OBJDIR	=	obj
OBJS	=	$(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))
OBJS_ROOT	=

# targets
MAIN	=	main.c
NAME	=	main.out
LIBOBJ	=	liboop.a

all: $(NAME)

$(NAME):  $(LIBOBJ) $(MAIN)
	$(CC) $(CFLAGS) -o $(NAME) $(MAIN) $(FDFLAGS) $(FDLIBS)

$(LIBOBJ): $(OBJS)
	$(AR) -rcs $(LIBOBJ) $?

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

test: all
	./$(NAME) p c

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME) $(LIBOBJ)

re: fclean all

.PHONY: clean test
