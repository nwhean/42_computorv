LIBOOP_PATH	= ./liboop

all: $(LIBOOP_PATH)/liboop.a

$(LIBOOP_PATH)/liboop.a:
	$(MAKE) -C $(LIBOOP_PATH) -s

clean:
	$(MAKE) clean -C $(LIBOOP_PATH) -s

fclean: clean
	$(MAKE) fclean -C $(LIBOOP_PATH) -s

re: fclean all

.PHONY: clean
