CFLAGS	=	-Wall -Werror $(INCLUDE) -fsanitize=address -g
INCLUDE	=	-Iinclude -Iinclude/lexer -Iinclude/parser -Iliboop/include
LDFLAGS	=	-L. -Lliboop
LDLIBS	=	-lcomputorv -loop
MAIN	=	main.c
NAME	=	a.out
LIBCOMP	=	libcomputorv.a

# source and object files
LEXER_DIR	=	lexer/
PARSER_DIR	=	parser/

SRCDIR	=	src
SRCS	=	$(addprefix $(LEXER_DIR), \
				Lexer.c \
				Num.c \
				Real.c \
				Token.c \
			) \
			$(addprefix $(PARSER_DIR), \
				Parser.c \
			)
OBJDIR	=	obj
OBJS	=	$(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))

# library
LIBOOP_PATH	= ./liboop

all: $(NAME)

$(NAME): $(LIBOOP_PATH)/liboop.a $(LIBCOMP) $(MAIN)
	clear
	$(CC) $(CFLAGS) -o $(NAME) $(MAIN) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	printf "$(NEWLINE)Creating object file $@ from $<"

$(LIBCOMP):	$(OBJS)
	$(AR) crs $@ $?

$(LIBOOP_PATH)/liboop.a:
	$(MAKE) -C $(LIBOOP_PATH) -s

clean:
	$(MAKE) clean -C $(LIBOOP_PATH) -s
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE) fclean -C $(LIBOOP_PATH) -s
	$(RM) $(NAME) $(LIBCOMP)

re: fclean all

.PHONY: clean
