#include "../lib/ft_ls.h"

/*
 * Output: one name per line, or -l long format.
 *
 * -l: "total" line, aligned columns; extra operand widths when files+dirs.
 * Dates: HH:MM if within ~6 months and not in the future, else year.
 * Devices: major, minor from st_rdev (no <sys/sysmacros.h> — subject list).
 * Failed lstat: line like -????????? ? ? ? ?            ? name
 */

/*
 * Decodes st_rdev like glibc major(), without <sys/sysmacros.h>.
 */
static unsigned int	dev_major(dev_t dev)
{
	return (((dev >> 8) & 0xfff) | ((unsigned int)(dev >> 32) & ~0xfffu));
}

/*
 * Decodes st_rdev like glibc minor(), without <sys/sysmacros.h>.
 */
static unsigned int	dev_minor(dev_t dev)
{
	return ((dev & 0xff) | ((unsigned int)(dev >> 12) & ~0xffu));
}

/* Writes n in decimal into buf (needs at least 21 bytes). */
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

/* Number of decimal digits of n. */
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

/* Prints an unsigned number to stdout with no padding. */
static void	put_ull(unsigned long long n)
{
	char	buf[32];

	ull_to_str(n, buf);
	ft_putstr_fd(buf, 1);
}

/* Writes n spaces to stdout. */
static void	pad_spaces(int n)
{
	while (n > 0)
	{
		ft_putchar_fd(' ', 1);
		n--;
	}
}

/* Copies src into dst with a hard capacity limit (always NUL-terminated). */
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

/* Resolves owner name from uid, or writes the numeric id if unknown. */
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

/* Resolves group name from gid, or writes the numeric id if unknown. */
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

/*
 * Builds the 10-character mode string (e.g. "drwxr-xr-x").
 * setuid / setgid / sticky use s/S/t/T like ls.
 */
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

/*
 * Returns 1 if mtime should show "HH:MM" instead of the year.
 * Future timestamps always use the year (GNU ls behaviour).
 */
static int	is_recent(time_t mtime)
{
	time_t	now;

	now = time(NULL);
	if (mtime > now)
		return (0);
	return ((now - mtime) <= (time_t)15778476);
}

/*
 * Writes the 12-character date column: "Mmm dd HH:MM" or "Mmm dd  YYYY".
 */
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

/*
 * Size column text: byte size, or "major, minor" for device files.
 */
static void	fill_size(struct stat *st, char *buf)
{
	char	minor_buf[32];
	int		i;
	int		j;

	if (S_ISCHR(st->st_mode) || S_ISBLK(st->st_mode))
	{
		ull_to_str((unsigned long long)dev_major(st->st_rdev), buf);
		i = 0;
		while (buf[i])
			i++;
		buf[i++] = ',';
		buf[i++] = ' ';
		ull_to_str((unsigned long long)dev_minor(st->st_rdev), minor_buf);
		j = 0;
		while (minor_buf[j])
			buf[i++] = minor_buf[j++];
		buf[i] = '\0';
		return ;
	}
	ull_to_str((unsigned long long)st->st_size, buf);
}

/*
 * Disk usage units for the "total" line (GNU ls: 1024-byte blocks on Linux).
 */
static long long	block_units(struct stat *st)
{
	return ((long long)st->st_blocks / 2);
}

/* Raises *width to len when len is larger. */
static void	widen(int *width, int len)
{
	if (len > *width)
		*width = len;
}

/* Widens columns so every entry in list fits (including "?" placeholders). */
static void	measure_add(t_file *list, t_width *width)
{
	char	user[256];
	char	group[256];
	char	size[64];

	while (list)
	{
		if (!list->ok)
		{
			widen(&width->nlink, 1);
			widen(&width->user, 1);
			widen(&width->group, 1);
			widen(&width->size, 1);
			list = list->next;
			continue ;
		}
		widen(&width->nlink, ull_len((unsigned long long)list->st.st_nlink));
		owner_name(list->st.st_uid, user, sizeof(user));
		group_name(list->st.st_gid, group, sizeof(group));
		fill_size(&list->st, size);
		widen(&width->user, (int)ft_strlen(user));
		widen(&width->group, (int)ft_strlen(group));
		widen(&width->size, (int)ft_strlen(size));
		list = list->next;
	}
}

/* Resets widths then measures list. */
static void	measure(t_file *list, t_width *width)
{
	width->nlink = 1;
	width->user = 0;
	width->group = 0;
	width->size = 1;
	measure_add(list, width);
}

/* Prints " -> target" after a symbolic link's name. */
static void	print_link_target(t_file *file)
{
	char	target[4096];
	ssize_t	n;

	if (!file->ok || !S_ISLNK(file->st.st_mode))
		return ;
	n = readlink(file->path, target, sizeof(target) - 1);
	if (n < 0)
		return ;
	target[n] = '\0';
	ft_putstr_fd(" -> ", 1);
	ft_putstr_fd(target, 1);
}

/* Prints s left-aligned in a column of width. */
static void	put_left(char *s, int width)
{
	ft_putstr_fd(s, 1);
	pad_spaces(width - (int)ft_strlen(s));
}

/* Prints s right-aligned in a column of width. */
static void	put_right(char *s, int width)
{
	pad_spaces(width - (int)ft_strlen(s));
	ft_putstr_fd(s, 1);
}

/*
 * Fills the -l fields for one entry. When lstat failed, every field is "?".
 */
static void	fill_lfields(t_file *file, t_lfields *f)
{
	if (!file->ok)
	{
		f->mode[0] = file->type;
		ft_memset(f->mode + 1, '?', 9);
		f->mode[10] = '\0';
		ft_strlcpy(f->nlink, "?", sizeof(f->nlink));
		ft_strlcpy(f->user, "?", sizeof(f->user));
		ft_strlcpy(f->group, "?", sizeof(f->group));
		ft_strlcpy(f->size, "?", sizeof(f->size));
		ft_strlcpy(f->date, "?", sizeof(f->date));
		return ;
	}
	fill_mode(file->st.st_mode, f->mode);
	ull_to_str((unsigned long long)file->st.st_nlink, f->nlink);
	owner_name(file->st.st_uid, f->user, sizeof(f->user));
	group_name(file->st.st_gid, f->group, sizeof(f->group));
	fill_size(&file->st, f->size);
	format_date(file->st.st_mtime, f->date);
}

/*
 * Prints one -l line:
 * mode nlink user group size date name [-> target]
 */
static void	print_long_line(t_file *file, t_width *width)
{
	t_lfields	f;

	fill_lfields(file, &f);
	ft_putstr_fd(f.mode, 1);
	ft_putchar_fd(' ', 1);
	put_right(f.nlink, width->nlink);
	ft_putchar_fd(' ', 1);
	put_left(f.user, width->user);
	ft_putchar_fd(' ', 1);
	put_left(f.group, width->group);
	ft_putchar_fd(' ', 1);
	put_right(f.size, width->size);
	ft_putchar_fd(' ', 1);
	put_right(f.date, 12);
	ft_putchar_fd(' ', 1);
	ft_putstr_fd(file->name, 1);
	print_link_target(file);
	ft_putchar_fd('\n', 1);
}

/* Prints "total N" for a directory listing in -l mode. */
static void	print_total(t_file *list)
{
	long long	total;

	total = 0;
	while (list)
	{
		if (list->ok)
			total += block_units(&list->st);
		list = list->next;
	}
	ft_putstr_fd("total ", 1);
	put_ull((unsigned long long)total);
	ft_putchar_fd('\n', 1);
}

/*
 * Prints a sorted block of entries.
 * With -l: long format (as_dir also prints "total").
 * Without -l: one name per line.
 * extra takes part in column widths when file and directory operands share -l.
 */
void	print_files(t_file *list, t_flags *flags, int as_dir, t_file *extra)
{
	t_width	width;
	t_file	*it;

	if (flags->l)
	{
		if (as_dir)
			print_total(list);
		measure(list, &width);
		measure_add(extra, &width);
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
		ft_putstr_fd(list->name, 1);
		ft_putchar_fd('\n', 1);
		list = list->next;
	}
}
