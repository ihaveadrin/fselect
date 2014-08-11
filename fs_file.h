#ifndef _INCLUDED_FS_FILE_H
#define _INCLUDED_FS_FILE_H

/*-
 *  Copyright (c) 2001-2014  Peter Pentchev, Teodoro Santoni
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
 *
 *  $Ringlet: c/misc/fselect/fs_file.h,v 1.3 2014/01/07 13:22:22 roam Exp $
 */

#include "fselect.h"

#include <sys/types.h>
#include <sys/stat.h>

/*
 * Definitions
 */

#define FS_FILEINC	8

struct fsstat {
	const char	*fname;
	struct stat	sb;
};

#define FS_SVIEW_MODE	0x0001
#define FS_SVIEW_UID	0x0002
#define FS_SVIEW_GID	0x0004
#define FS_SVIEW_SIZE	0x0008
#define FS_SVIEW_TIME	0x0010

#define FS_SVIEW_ALL	0xFFFF
#define FS_SVIEW_DEF	FS_SVIEW_ALL

/*
 * Variables
 */

extern unsigned		fs_filecnt, fs_fileall;
extern struct fsstat	*fs_stat;

/*
 * Functions
 */

int		fs_statfile(const char *);
int		fs_filetostr(char **, struct fsstat *, unsigned);
int		fs_strtoviewf(unsigned *, const char *);

#endif /* _INCLUDED */
