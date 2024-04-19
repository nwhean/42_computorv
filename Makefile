CFLAGS	=	-Wall -Wextra -Werror -pedantic -std=c90 $(INCLUDE) -fsanitize=address -g
INCLUDE	=	-Iinclude \
			-Iinclude/common \
			-Iinclude/container \
			-Iinclude/inter \
			-Iinclude/lexer \
			-Iinclude/parser \
			-Iliboop/include
LDFLAGS	=	-L. -Lliboop
LDLIBS	=	-lcomputorv -loop -lm
MAIN	=	main.c
NAME	=	a.out
LIBCOMP	=	libcomputorv.a

# source and object files
CONTAINER_DIR	=	container/
INTER_DIR		=	inter/
LEXER_DIR		=	lexer/
PARSER_DIR		=	parser/

SRCDIR	=	src
SRCS	=	$(addprefix $(CONTAINER_DIR), \
				Str.c \
				UnorderedMap.c \
			) \
			$(addprefix $(INTER_DIR), \
				Arith.c \
				Constant.c \
				Expr.c \
				Id.c \
				Node.c \
				Op.c \
				Unary.c \
			) \
			$(addprefix $(LEXER_DIR), \
				Complex.c \
				Lexer.c \
				Numeric.c \
				Rational.c \
				Token.c \
				Word.c \
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
