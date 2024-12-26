CFLAGS	=	-Wall -Wextra -Werror -pedantic -std=c90 $(INCLUDE) -fsanitize=address -g
INCLUDE	=	-Iinclude \
			-Iinclude/common \
			-Iinclude/container \
			-Iinclude/inter \
			-Iinclude/lexer \
			-Iinclude/other \
			-Iinclude/parser \
			-Iinclude/symbols \
			-Iliboop/include
LDFLAGS	=	-L. -Lliboop
LDLIBS	=	-lcomputorv -loop -lm -lgmp
MAIN	=	main.c
NAME	=	computorv2
LIBCOMP	=	libcomputorv.a

# source and object files
CONTAINER_DIR	=	container/
INTER_DIR		=	inter/
LEXER_DIR		=	lexer/
OTHER_DIR		=	other/
PARSER_DIR		=	parser/
SYMBOLS_DIR		=	symbols/

SRCDIR	=	src
SRCS	=	$(addprefix $(CONTAINER_DIR), \
				Str.c \
				UnorderedMap.c \
				Vec.c \
			) \
			$(addprefix $(INTER_DIR), \
				Arith.c \
				AssignStmt.c \
				BuiltIn.c \
				Constant.c \
				Expr.c \
				ExprStmt.c \
				FuncDef.c \
				Function.c \
				Id.c \
				MatExpr.c \
				Node.c \
				Op.c \
				Seq.c \
				Stmt.c \
				Unary.c \
				VecExpr.c \
			) \
			$(addprefix $(LEXER_DIR), \
				Complex.c \
				Lexer.c \
				Matrix.c \
				Numeric.c \
				Rational.c \
				Token.c \
				Vector.c \
				Word.c \
			) \
			$(addprefix $(OTHER_DIR), \
				mathematics.c \
				utility.c \
			) \
			$(addprefix $(PARSER_DIR), \
				Parser.c \
			) \
			$(addprefix $(SYMBOLS_DIR), \
				Env.c \
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
	$(MAKE) liboop.a -C $(LIBOOP_PATH) -s

gmp:
	tar -xvf gmp-6.3.0.tar.xz
	cd gmp-6.3.0/
	./configure
	make
	sudo make install

clean:
	$(MAKE) clean -C $(LIBOOP_PATH) -s
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE) fclean -C $(LIBOOP_PATH) -s
	$(RM) $(NAME) $(LIBCOMP)

re: fclean all

.PHONY: clean
