/*-
 *  Copyright (c) 2001-2019  Peter Pentchev, Teodoro Santoni, dyveloper
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "fselect.h"
#include "fs_file.h"

static char	cvs_id[] __UNUS =
"$Ringlet: c/misc/fselect/fs_file.c,v 1.4 2014/01/07 13:22:22 roam Exp $";

unsigned	fs_filecnt, fs_fileall;
struct fsstat	*fs_stat;

static void	mode2str(char *, int, int, int, int);
static char *	date2str(time_t t);
static char *	uid2str(uid_t uid);
static char *	gid2str(gid_t gid);
char *		strdup(const char *s);


/*
 * Function:
 * 	fs_statfile		- read info about a file
 * Inputs:
 * 	fname			- file name to stat
 * Returns:
 * 	0 on success, 1 if file not found, -1 with errno set on error
 * Modifies:
 * 	nothing so far (FIXME)
 */

int
fs_statfile(const char *fname) {
	struct stat sb;

	if (stat(fname, &sb) < 0)
		return (errno == ENOENT? 1: -1);
	
	if (fs_filecnt == fs_fileall) {
		struct fsstat *nall;

		nall = realloc(fs_stat,
		    (fs_filecnt + FS_FILEINC) * sizeof(*fs_stat));
		if (nall == NULL) {
			errno = ENOMEM;
			return (-1);
		}
		fs_stat = nall;
		fs_fileall += FS_FILEINC;
	}
	
	fs_stat[fs_filecnt].fname = strdup(fname);
	memcpy(&fs_stat[fs_filecnt].sb, &sb, sizeof(fs_stat[fs_filecnt].sb));
	fs_filecnt++;
	return (0);
}

/*
 * Function:
 * 	fs_filetostr		- a string description of a file
 * Inputs:
 * 	pstr			- val/res description
 * 	pstat			- file to describe
 * Returns:
 * 	0 on success, -1 with errno set on error
 * Modifies:
 * 	allocates and stores desciption into *pstr
 */

#if 0
#define SZ_MCHAR	1
#define SZ_MODE		9
#define SZ_UNAME	8
#define SZ_GNAME	8
#define SZ_SIZE		10
#define SZ_DATE		6
#define SZ_TIME		7

#define OFF_MCHAR	0
#define OFF_MODE	(OFF_MCHAR + SZ_MCHAR + 0)
#define OFF_UNAME	(OFF_MODE + SZ_MODE + 1)
#define OFF_GNAME	(OFF_UNAME + SZ_UNAME + 1)
#define OFF_SIZE	(OFF_GNAME + SZ_GNAME + 1)
#define OFF_DATE	(OFF_SIZE + SZ_SIZE + 1)
#define OFF_TIME	(OFF_DATE + SZ_DATE + 1)
#define SZ_WHOLE	(OFF_TIME + SZ_TIME + 1)
#endif

#define SZ_WHOLE	57

int
fs_filetostr(char **pstr, struct fsstat *pstat, unsigned viewf) {
	char *s, mchar;
	unsigned len, sz, n;

	if (S_ISREG(pstat->sb.st_mode))
		mchar = '-';
	else if (S_ISDIR(pstat->sb.st_mode))
		mchar = 'd';
	else if (S_ISCHR(pstat->sb.st_mode))
		mchar = 'c';
	else
		mchar = '?';

	len = (unsigned) strlen(pstat->fname);
	sz = len + SZ_WHOLE;
	if (s = calloc(sz, 1), s == NULL) {
		errno = ENOMEM;
		return (-1);
	}
	
	if (viewf & FS_SVIEW_MODE) {
		s[0] = mchar;
		mode2str(s+1, (int) pstat->sb.st_mode, S_IRUSR, S_IWUSR, S_IXUSR);
		mode2str(s+4, (int) pstat->sb.st_mode, S_IRGRP, S_IWGRP, S_IXGRP);
		mode2str(s+7, (int) pstat->sb.st_mode, S_IROTH, S_IWOTH, S_IXOTH);
	}

	n = (unsigned) strlen(s);
	if (viewf & FS_SVIEW_UID)
		n += (unsigned) snprintf(s + n, sz - n, " %-8s",
		    uid2str(pstat->sb.st_uid));
	if (viewf & FS_SVIEW_GID)
		n += (unsigned) snprintf(s + n, sz - n, " %-8s",
		    gid2str(pstat->sb.st_gid));
	if (viewf & FS_SVIEW_SIZE)
		n += (unsigned) snprintf(s + n, sz - n, " %10lu",
		    (unsigned long) pstat->sb.st_size);
	if (viewf & FS_SVIEW_TIME)
		n += (unsigned) snprintf(s + n, sz - n, " %s",
		    date2str(pstat->sb.st_ctime));
	snprintf(s + n, sz - n, " %s", pstat->fname);

	*pstr = s;
	return (0);
}

static void
mode2str(char *s, int mode, int r, int w, int x) {

	s[0] = (mode & r)? 'r': '-';
	s[1] = (mode & w)? 'w': '-';
	s[2] = (mode & x)? 'x': '-';
}

static char *
date2str(time_t t) {
	static char s[20] = "";

	strftime(s, sizeof(s), "%b %e %T", localtime(&t));
	return (s);
}

static char *
uid2str(uid_t uid) {
	struct passwd *pwd;
	static char buf[10];

	if (pwd = getpwuid(uid), pwd == NULL) {
		snprintf(buf, sizeof(buf), "%u", uid);
		return (buf);
	}
	return (pwd->pw_name);
}

static char *
gid2str(gid_t gid) {
	struct group *grp;
	static char buf[10];

	if (grp = getgrgid(gid), grp == NULL) {
		snprintf(buf, sizeof(buf), "%u", gid);
		return (buf);
	}
	return (grp->gr_name);
}

/*
 * Function:
 * 	fs_strtoview	- obtain view flags from a string representation
 * Inputs:
 * 	pviewf		- val/res view flags
 * 	s		- string arg
 * Returns:
 * 	0 on success, -1 on incorrect string syntax
 * Modifies:
 * 	*pviewf
 */

static struct {
	char		l;
	unsigned	f;
} let2view[] = {
	{'M',	FS_SVIEW_MODE},
	{'U',	FS_SVIEW_UID},
	{'G',	FS_SVIEW_GID},
	{'Z',	FS_SVIEW_SIZE},
	{'T',	FS_SVIEW_TIME}
};

#define LET2VIEWCNT	(sizeof(let2view)/sizeof(let2view[0]))

int
fs_strtoviewf(unsigned *pviewf, const char *s) {
	unsigned i, viewf;

	for (viewf = *pviewf; *s; s++)
		switch (*s) {
			case '0':
			case 'N':
			case 'n':
			case '-':
				viewf = 0;
				break;
			case 'A':
			case 'a':
			case '+':
				viewf = FS_SVIEW_ALL;
				break;
			case '*':
				viewf = FS_SVIEW_ALL ^ viewf;
				break;
			default:
				for (i = 0; i < LET2VIEWCNT; i++) {
					if (let2view[i].l == *s) {
						viewf |= let2view[i].f;
						break;
					}
					if (let2view[i].l == toupper(*s)) {
						viewf &= ~let2view[i].f;
						break;
					}
				}
				if (i == LET2VIEWCNT)
					return (-1);
		}

	*pviewf = viewf;
	return (0);
}
