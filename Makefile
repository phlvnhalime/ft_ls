name = ft_ls

all : $(name)

$(name) : $(name).c
	gcc -Wall -Wextra -Werror -o $(name) $(name).c

clean :
	rm -f $(name)

fclean : clean
	rm -f $(name)

re : fclean all

test: --valgrind
	./$(name)

valgrind :
	valgrind --leak-check=full --show-leak-kinds=all ./$(name)

.PHONY : all clean fclean re test valgrind
