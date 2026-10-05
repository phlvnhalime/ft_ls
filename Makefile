NAME		= ft_ls
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRC			= main.c src/parse.c src/run.c src/print.c src/sort.c src/utils.c
LIBFT_SRC	= $(wildcard $(LIBFT_DIR)/*.c) $(LIBFT_DIR)/libft.h $(LIBFT_DIR)/Makefile

all: $(NAME)

$(LIBFT): $(LIBFT_SRC)
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(SRC) lib/ft_ls.h
	$(CC) $(CFLAGS) -I lib -I $(LIBFT_DIR) $(SRC) $(LIBFT) -o $(NAME)

clean:
	make -C $(LIBFT_DIR) clean

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
