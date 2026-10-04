#ifndef FT_LS_H
# define FT_LS_H

# include <unistd.h> // write, readlink
# include <dirent.h> // opendir, readdir, closedir
# include <sys/stat.h> // stat, lstat
# include <sys/xattr.h> // getxattr, setxattr (get and set extended attributes)
# include <pwd.h> // getpwuid (get user name from user id)
# include <grp.h> // getgrgid (get group name from group id)
# include <time.h> // time
# include <string.h> // strcmp, strcpy, strlen
# include <stdlib.h> // malloc, free
# include <stdio.h> // printf
# include <stdbool.h> // true, false
# include <errno.h>
# include <sys/types.h>

typedef struct s_flags
{
	bool	l;
	bool	R;
	bool	a;
	bool	r;
	bool	t;
}	t_flags;

typedef struct s_args
{
	t_flags	flags;
	char	**paths;
	int		path_count;
}	t_args;

typedef struct s_file
{
	char			*name;
	char			*path;
	struct stat		st;
	int				ok;
	int				is_dir;
	int				err_no;
	struct s_file	*next;
}	t_file;

int		parse_args(int ac, char **av, t_args *args);
int		run_ls(t_args *args, char *prog);
size_t	ft_strlen(char *str);
int		ft_strcmp(char *a, char *b);
char	*ft_strdup(char *str);
char	*join_path(char *dir, char *name);
void	ft_putchar_fd(int fd, char c);
void	ft_putstr_fd(int fd, char *str);
void	print_error(char *prog, char *path, int err);
t_file	*sort_files(t_file *list, t_flags *flags);
void	print_files(t_file *list, t_flags *flags, int as_dir);

#endif