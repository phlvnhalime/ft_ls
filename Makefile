name = ft_ls
src = main.c src/parse.c

all : $(name)

$(name) : $(src) lib/ft_ls.h
	gcc -Wall -Wextra -Werror -I lib -o $(name) $(src)

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
