NAME		= ft_ls
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRC			= main.c \
			  src/parse.c src/parse_flag.c src/parse_msg.c \
			  src/run.c src/run_file.c src/run_type.c \
			  src/run_operand.c src/run_read.c src/run_list.c \
			  src/print.c src/print_num.c src/print_dev.c \
			  src/print_names.c src/print_mode.c src/print_date.c \
			  src/print_measure.c src/print_long.c \
			  src/sort.c src/utils.c
LIBFT_SRC	= $(wildcard $(LIBFT_DIR)/*.c) $(LIBFT_DIR)/libft.h $(LIBFT_DIR)/Makefile

ifndef V
Q			= @
ECHO		= @printf
else
Q			=
ECHO		= @true
endif

all: $(NAME)

$(LIBFT): $(LIBFT_SRC)
	$(ECHO) "  libft compiling...\n"
	$(Q)make -C $(LIBFT_DIR) --no-print-directory

$(NAME): $(LIBFT) $(SRC) inc/ft_ls.h
	$(ECHO) "  ft_ls compiling...\n"
	$(Q)$(CC) $(CFLAGS) -I inc -I $(LIBFT_DIR) $(SRC) $(LIBFT) -o $(NAME)
	$(ECHO) "  done → $(NAME)\n"

clean:
	$(ECHO) "  cleaning objects...\n"
	$(Q)make -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	$(ECHO) "  removing $(NAME)...\n"
	$(Q)make -C $(LIBFT_DIR) fclean --no-print-directory
	$(Q)rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
