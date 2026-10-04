#include "../lib/ft_ls.h"

static long long	mtime_sec(struct stat *st)
{
	return ((long long)st->st_mtime);
}

static long long	mtime_nsec(struct stat *st)
{
#if defined(__APPLE__)
	return ((long long)st->st_mtimespec.tv_nsec);
#else
	return ((long long)st->st_mtim.tv_nsec);
#endif
}

static int	cmp_files(t_file *a, t_file *b, t_flags *flags)
{
	int			cmp;
	long long	a_sec;
	long long	b_sec;
	long long	a_nsec;
	long long	b_nsec;

	cmp = 0;
	if (flags->t && a->ok && b->ok)
	{
		a_sec = mtime_sec(&a->st);
		b_sec = mtime_sec(&b->st);
		if (a_sec != b_sec)
			cmp = (a_sec > b_sec) - (a_sec < b_sec);
		else
		{
			a_nsec = mtime_nsec(&a->st);
			b_nsec = mtime_nsec(&b->st);
			if (a_nsec != b_nsec)
				cmp = (a_nsec > b_nsec) - (a_nsec < b_nsec);
		}
		cmp = -cmp;
	}
	if (cmp == 0)
		cmp = ft_strcmp(a->name, b->name);
	if (flags->r)
		cmp = -cmp;
	return (cmp);
}

static t_file	*merge_files(t_file *a, t_file *b, t_flags *flags)
{
	t_file	dummy;
	t_file	*tail;

	tail = &dummy;
	dummy.next = NULL;
	while (a && b)
	{
		if (cmp_files(a, b, flags) <= 0)
		{
			tail->next = a;
			a = a->next;
		}
		else
		{
			tail->next = b;
			b = b->next;
		}
		tail = tail->next;
	}
	if (a)
		tail->next = a;
	else
		tail->next = b;
	return (dummy.next);
}

static void	split_files(t_file *src, t_file **left, t_file **right)
{
	t_file	*slow;
	t_file	*fast;

	slow = src;
	fast = src->next;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	*left = src;
	*right = slow->next;
	slow->next = NULL;
}

t_file	*sort_files(t_file *list, t_flags *flags)
{
	t_file	*left;
	t_file	*right;

	if (!list || !list->next)
		return (list);
	split_files(list, &left, &right);
	left = sort_files(left, flags);
	right = sort_files(right, flags);
	return (merge_files(left, right, flags));
}
