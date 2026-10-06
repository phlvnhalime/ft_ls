/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpehliva <hpehliva@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 18:40:00 by hpehliva          #+#    #+#             */
/*   Updated: 2026/10/06 18:40:00 by hpehliva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <string.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/types.h>
# include "../libft/libft.h"

typedef struct s_flags
{
	int	l;
	int	rec;
	int	a;
	int	r;
	int	t;
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
	char			type;
	struct s_file	*next;
}	t_file;

typedef struct s_width
{
	int	nlink;
	int	user;
	int	group;
	int	size;
}	t_width;

typedef struct s_lfields
{
	char	mode[11];
	char	nlink[32];
	char	user[256];
	char	group[256];
	char	size[64];
	char	date[13];
}	t_lfields;

typedef struct s_read
{
	char	*path;
	t_flags	*flags;
	char	*prog;
	t_file	**out;
	int		*status;
	int		open_fail;
}	t_read;

typedef struct s_listdir
{
	char	*path;
	t_flags	*flags;
	char	*prog;
	int		show_header;
	int		need_blank;
	int		open_fail;
	int		*status;
}	t_listdir;

int			parse_args(int ac, char **av, t_args *args);
int			read_flags(t_flags *flags, char *prog, char *arg);
void		illegal_option(char *prog, char option);
void		unrecognized_option(char *prog, char *arg);
int			same_word(char *a, char *b);
int			is_flag_word(char *arg);
int			set_flag(t_flags *flags, char option);

int			run_ls(t_args *args, char *prog);
void		free_files(t_file *list);
t_file		*new_file(char *name, char *path);
int			append_file(t_file **head, t_file **tail, t_file *node);
char		type_from_dirent(unsigned char d_type);
char		type_from_mode(mode_t mode);
void		fill_entry(t_file *node, unsigned char d_type);
void		mark_operand(t_file *file, t_flags *flags);
t_file		*make_operand(char *path, t_flags *flags);
void		raise_status(int *status, int code);
int			read_directory(t_read *ctx);
int			is_dot(char *name);
int			list_directory(t_listdir *ctx);

int			ft_strcmp(char *a, char *b);
char		*join_path(char *dir, char *name);
void		print_error(char *prog, char *phrase, char *path, int err);
t_file		*sort_files(t_file *list, t_flags *flags);

void		ull_to_str(unsigned long long n, char *buf);
int			ull_len(unsigned long long n);
void		put_ull(unsigned long long n);
void		pad_spaces(int n);
void		widen(int *width, int len);
void		fill_size(struct stat *st, char *buf);
long long	block_units(struct stat *st);
void		owner_name(uid_t uid, char *buf, size_t cap);
void		group_name(gid_t gid, char *buf, size_t cap);
void		fill_mode(mode_t mode, char *buf);
void		format_date(time_t mtime, char *out);
void		measure(t_file *list, t_width *width);
void		measure_add(t_file *list, t_width *width);
void		print_link_target(t_file *file);
void		put_left(char *s, int width);
void		put_right(char *s, int width);
void		fill_lfields(t_file *file, t_lfields *f);
void		print_long_line(t_file *file, t_width *width);
void		print_total(t_file *list);
void		print_files(t_file *list, t_flags *flags, int as_dir,
				t_file *extra);

#endif
