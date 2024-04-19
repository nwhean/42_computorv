CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror $(INCLUDE) -fsanitize=address -g
INCLUDE	=	-Iunity \
			-I../include/common \
			-I../include/container \
			-I../include/inter \
			-I../include/lexer \
			-I../include/parser \
			-I../liboop/include
LDFLAGS	=	-L.. -L../liboop
LDLIBS	=	-lcomputorv -loop -lm

# source and object files
CONTAINER_DIR	=	container
INTER_DIR		=	inter
LEXER_DIR		=	lexer
PARSER_DIR		=	parser

CONTAINER_SRCS	=	$(shell find $(CONTAINER_DIR) -name '*.c')
INTER_SRCS		=	$(shell find $(INTER_DIR) -name '*.c')
LEXER_SRCS		=	$(shell find $(LEXER_DIR) -name '*.c')
PARSER_SRCS		=	$(shell find $(PARSER_DIR) -name '*.c')

SRCS	=	$(CONTAINER_SRCS) \
			$(INTER_SRCS) \
			$(LEXER_SRCS) \
			$(PARSER_SRCS)
MAINS	=	$(SRCS:%.c=%)

LIBCOMP	= libcomputorv.a

# $(info $$SRCS is [${SRCS}])
# $(info $$MAINS is [${MAINS}])

all:
	for file in $(MAINS); do \
		$(MAKE) $$file ; \
	done

%:
	$(MAKE) -C ..
	$(CC) $(CFLAGS) unity/unity.c $@.c $(LDFLAGS) $(LDLIBS) -o $@
	-./$@
	$(RM) $@


.PHONY: container inter lexer parser
.SILENT:
