#include "../lib/ft_ls.h"

#if defined(__linux__)
# include <sys/sysmacros.h>
#endif

typedef struct s_width
{
	int	nlink;
	int	user;
	int	group;
	int	size;
}	t_width;

static void	ull_to_str(unsigned long long n, char *buf)
{
	char	tmp[32];
	int		i;
	int		j;

	i = 0;
	if (n == 0)
		tmp[i++] = '0';
	while (n > 0)
	{
		tmp[i++] = (char)('0' + (n % 10));
		n /= 10;
	}
	j = 0;
	while (i > 0)
		buf[j++] = tmp[--i];
	buf[j] = '\0';
}

static int	ull_len(unsigned long long n)
{
	int	len;

	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	put_ull(unsigned long long n)
{
	char	buf[32];

	ull_to_str(n, buf);
	ft_putstr_fd(1, buf);
}

static void	pad_spaces(int n)
{
	while (n > 0)
	{
		ft_putchar_fd(1, ' ');
		n--;
	}
}

static void	copy_str(char *dst, char *src, size_t cap)
{
	size_t	i;

	i = 0;
	if (!src)
		src = "";
	while (src[i] && i + 1 < cap)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

static void	owner_name(uid_t uid, char *buf, size_t cap)
{
	struct passwd	*pw;

	pw = getpwuid(uid);
	if (pw)
		copy_str(buf, pw->pw_name, cap);
	else
		ull_to_str((unsigned long long)uid, buf);
	(void)cap;
}

static void	group_name(gid_t gid, char *buf, size_t cap)
{
	struct group	*gr;

	gr = getgrgid(gid);
	if (gr)
		copy_str(buf, gr->gr_name, cap);
	else
		ull_to_str((unsigned long long)gid, buf);
	(void)cap;
}

static void	fill_mode(mode_t mode, char *buf)
{
	const char	*rwx = "rwxrwxrwx";
	int			i;

	if (S_ISREG(mode))
		buf[0] = '-';
	else if (S_ISDIR(mode))
		buf[0] = 'd';
	else if (S_ISLNK(mode))
		buf[0] = 'l';
	else if (S_ISCHR(mode))
		buf[0] = 'c';
	else if (S_ISBLK(mode))
		buf[0] = 'b';
	else if (S_ISFIFO(mode))
		buf[0] = 'p';
	else if (S_ISSOCK(mode))
		buf[0] = 's';
	else
		buf[0] = '?';
	i = 0;
	while (i < 9)
	{
		if (mode & (1 << (8 - i)))
			buf[i + 1] = rwx[i];
		else
			buf[i + 1] = '-';
		i++;
	}
	if (mode & S_ISUID)
		buf[3] = (mode & S_IXUSR) ? 's' : 'S';
	if (mode & S_ISGID)
		buf[6] = (mode & S_IXGRP) ? 's' : 'S';
	if (mode & S_ISVTX)
		buf[9] = (mode & S_IXOTH) ? 't' : 'T';
	buf[10] = '\0';
}

static int	is_recent(time_t mtime)
{
	time_t	now;
	time_t	six;

	now = time(NULL);
	six = (time_t)15778476;
	if (mtime > now)
		return ((mtime - now) <= six);
	return ((now - mtime) <= six);
}

static void	format_date(time_t mtime, char *out)
{
	char	*ct;

	ct = ctime(&mtime);
	if (!ct)
	{
		out[0] = '\0';
		return ;
	}
	out[0] = ct[4];
	out[1] = ct[5];
	out[2] = ct[6];
	out[3] = ' ';
	out[4] = ct[8];
	out[5] = ct[9];
	out[6] = ' ';
	if (is_recent(mtime))
	{
		out[7] = ct[11];
		out[8] = ct[12];
		out[9] = ct[13];
		out[10] = ct[14];
		out[11] = ct[15];
	}
	else
	{
		out[7] = ' ';
		out[8] = ct[20];
		out[9] = ct[21];
		out[10] = ct[22];
		out[11] = ct[23];
	}
	out[12] = '\0';
}

static void	fill_size(struct stat *st, char *buf)
{
	char	minor_buf[32];
	int		i;
	int		j;

	if (S_ISCHR(st->st_mode) || S_ISBLK(st->st_mode))
	{
		ull_to_str((unsigned long long)major(st->st_rdev), buf);
		i = 0;
		while (buf[i])
			i++;
		buf[i++] = ',';
		buf[i++] = ' ';
		ull_to_str((unsigned long long)minor(st->st_rdev), minor_buf);
		j = 0;
		while (minor_buf[j])
			buf[i++] = minor_buf[j++];
		buf[i] = '\0';
		return ;
	}
	ull_to_str((unsigned long long)st->st_size, buf);
}

static long long	block_units(struct stat *st)
{
#if defined(__APPLE__)
	return ((long long)st->st_blocks);
#else
	return ((long long)st->st_blocks / 2);
#endif
}

static void	measure(t_file *list, t_width *width)
{
	char	user[256];
	char	group[256];
	char	size[64];
	int		len;

	width->nlink = 1;
	width->user = 0;
	width->group = 0;
	width->size = 1;
	while (list)
	{
		len = ull_len((unsigned long long)list->st.st_nlink);
		if (len > width->nlink)
			width->nlink = len;
		owner_name(list->st.st_uid, user, sizeof(user));
		group_name(list->st.st_gid, group, sizeof(group));
		fill_size(&list->st, size);
		len = (int)ft_strlen(user);
		if (len > width->user)
			width->user = len;
		len = (int)ft_strlen(group);
		if (len > width->group)
			width->group = len;
		len = (int)ft_strlen(size);
		if (len > width->size)
			width->size = len;
		list = list->next;
	}
}

static void	print_link_target(t_file *file)
{
	char	target[4096];
	ssize_t	n;

	if (!S_ISLNK(file->st.st_mode))
		return ;
	n = readlink(file->path, target, sizeof(target) - 1);
	if (n < 0)
		return ;
	target[n] = '\0';
	ft_putstr_fd(1, " -> ");
	ft_putstr_fd(1, target);
}

static void	print_long_line(t_file *file, t_width *width)
{
	char	mode[11];
	char	date[13];
	char	user[256];
	char	group[256];
	char	size[64];

	fill_mode(file->st.st_mode, mode);
	format_date(file->st.st_mtime, date);
	owner_name(file->st.st_uid, user, sizeof(user));
	group_name(file->st.st_gid, group, sizeof(group));
	fill_size(&file->st, size);
	ft_putstr_fd(1, mode);
	ft_putstr_fd(1, "  ");
	pad_spaces(width->nlink - ull_len((unsigned long long)file->st.st_nlink));
	put_ull((unsigned long long)file->st.st_nlink);
	ft_putchar_fd(1, ' ');
	ft_putstr_fd(1, user);
	pad_spaces(width->user - (int)ft_strlen(user));
	ft_putstr_fd(1, "  ");
	ft_putstr_fd(1, group);
	pad_spaces(width->group - (int)ft_strlen(group));
	ft_putstr_fd(1, "  ");
	pad_spaces(width->size - (int)ft_strlen(size));
	ft_putstr_fd(1, size);
	ft_putchar_fd(1, ' ');
	ft_putstr_fd(1, date);
	ft_putchar_fd(1, ' ');
	ft_putstr_fd(1, file->name);
	print_link_target(file);
	ft_putchar_fd(1, '\n');
}

static void	print_total(t_file *list)
{
	long long	total;

	total = 0;
	while (list)
	{
		total += block_units(&list->st);
		list = list->next;
	}
	ft_putstr_fd(1, "total ");
	put_ull((unsigned long long)total);
	ft_putchar_fd(1, '\n');
}

void	print_files(t_file *list, t_flags *flags, int as_dir)
{
	t_width	width;
	t_file	*it;

	if (flags->l)
	{
		if (as_dir)
			print_total(list);
		measure(list, &width);
		it = list;
		while (it)
		{
			print_long_line(it, &width);
			it = it->next;
		}
		return ;
	}
	while (list)
	{
		ft_putstr_fd(1, list->name);
		ft_putchar_fd(1, '\n');
		list = list->next;
	}
}
