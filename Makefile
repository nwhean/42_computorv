CFLAGS	=	-Wall -Werror $(INCLUDE) -fsanitize=address -g
INCLUDE	=	-Iinclude \
			-Iinclude/inter \
			-Iinclude/lexer \
			-Iinclude/parser \
			-Iinclude/symbols \
			-Iliboop/include
LDFLAGS	=	-L. -Lliboop
LDLIBS	=	-lcomputorv -loop
MAIN	=	main.c
NAME	=	a.out
LIBCOMP	=	libcomputorv.a

# source and object files
INTER_DIR	=	inter/
LEXER_DIR	=	lexer/
PARSER_DIR	=	parser/
SYMBOLS_DIR	=	symbols/

SRCDIR	=	src
SRCS	=	$(addprefix $(INTER_DIR), \
				Arith.c \
				Expr.c \
				Node.c \
				Op.c \
			) \
			$(addprefix $(LEXER_DIR), \
				Lexer.c \
				Num.c \
				Real.c \
				Token.c \
				Word.c \
			) \
			$(addprefix $(PARSER_DIR), \
				Parser.c \
			) \
			$(addprefix $(SYMBOLS_DIR), \
				Type.c \
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
