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


#endif