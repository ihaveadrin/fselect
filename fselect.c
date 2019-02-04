/*-
 *  Copyright (c) 2001-2019  Peter Pentchev, Teodoro Santoni, Dylan Göpel
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

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <getopt.h>

#include "fselect.h"
#include "fs_confirm.h"
#include "fs_curs.h"
#include "fs_file.h"
#include "fs_tty.h"

static char	cvs_id[] __UNUS =
"$Ringlet: c/misc/fselect/fselect.c,v 1.4 2014/01/07 13:22:22 roam Exp $";

typedef int	(*fs_selfun)(unsigned, struct fsstat *, int *, unsigned, int);

static fs_selfun 	fs_select[FS_STYPE_LAST] = {
	fs_confirm_sel,
	fs_curs_sel
};
static fs_seltype	stype = FS_STYPE_CURS;

static int	*fs_active;
static unsigned	viewf = FS_SVIEW_DEF;

static int max = 0;

int		parseopts(int, char **);
int		statfiles(int, char **);
int		outfiles(void);

void		usage(int);
void		version(int, int);

int
main(int argc, char **argv) {

	parseopts(argc, argv);
	argc -= optind;
	argv += optind;
	if (statfiles(argc, argv) < 0)
		usage(1);
	
	switchtotty();
	fs_select[stype](fs_filecnt, fs_stat, fs_active, viewf, max);
	switchtostdout();

	outfiles();
	return (EX_OK);
}

/*
 * Function:
 * 	usage			- display cmdline usage info
 * Inputs:
 * 	err			- error flag
 * Returns:
 * 	does not return
 * Modifies:
 * 	nothing; writes to stdout or stderr depending on the value of err
 */

void
usage(int ferr) {

	switchtostdout();
	fprintf(ferr? stderr: stdout,
	    "usage: fselect [-hV] [-n MAX_AMOUNT] [-f viewflags] [-y] file..\n"
	    "\t-f\tspecify format of files during selection;\n"
	    "\t-c\tuse curses/ncurses selection mode;\n"
	    "\t-y\tuse 'confirm one by one' selection mode;\n"
		"\t-n\tspecify a max number of files to be selected\n"
	    "\t-h\tprint this help message and exit;\n"
	    "\t-V\tprint version information and exit.\n"
	    "\t-n\tspecify maximal amount of selections\n"
	    );
	if (ferr)
		exit(EX_USAGE);
}

/*
 * Function:
 * 	version			- display version info
 * Inputs:
 * 	err			- error flag
 * 	blank			- leave a blank line after the version line
 * Returns:
 * 	nothing
 * Modifies:
 * 	nothing; writes to stdout or stderr depending on the value of err
 */

void
version(int ferr, int blank) {

	switchtostdout();
	fprintf(ferr? stderr: stdout,
	    "fselect v%d.%d, copyright (C) 2001-2014 Peter Pentchev, Teodoro Santoni.\n%s",
	    FS_VER_MAJ, FS_VER_MIN, blank? "\n": "");
}

/*
 * Function:
 * 	parseopts		- parse cmdline options
 * Inputs:
 * 	argc, argv		- main() args
 * Returns:
 * 	0 on success
 * 	errors out through usage() on cmdline syntax errors
 * Modifies:
 * 	nothing so far (FIXME)
 */

int
parseopts(int argc, char **argv) {
	int ch;
	int helpq, versq;

	helpq = versq = 0;
	opterr = 0;
	while (ch = getopt(argc, argv, FS_OPTSTR), ch != EOF)
		switch (ch) {
			case 'f':
				if (fs_strtoviewf(&viewf, optarg) < 0)
					usage(1);
				break;

			case 'N':
				fs_noswitch = 1;
				break;

			case 'c':
				stype = FS_STYPE_CURS;
				break;
			case 'y':
				stype = FS_STYPE_CONFIRM;
				break;

			case 'h':
				helpq = 1;
				break;
			case 'V':
				versq = 1;
				break;
			case 'n':
				max = atoi(optarg);
				if(max <= 0) {
					usage(0);
					exit(1);
				}
				break;
			default:
				usage(1);
				/* NOTREACHED */
		}

	if (versq)
		version(0, helpq);
	if (helpq)
		usage(0);
	if (helpq || versq)
		exit(EX_OK);
	return (0);
}

/*
 * Function:
 * 	statfiles		- stat all requested files
 * Inputs:
 * 	argc, argv		- main() args
 * Returns:
 * 	0 on success, -1 if no files found
 * Modifies:
 * 	fs_filecnt, fs_fileall, fs_stat, fs_active
 */

int
statfiles(int argc, char **argv) {
	int i, r;

	fs_filecnt = fs_fileall = 0;
	fs_stat = NULL;

	if (argc == 0)
		return (-1);
	for (i = 0; i < argc; i++)
		if (r = fs_statfile(argv[i]), r) {
			if (r > 0)
				warn("%s", argv[i]);
			else
				err(EX_OSERR, "%s", argv[i]);
		}
	
	if (fs_filecnt == 0)
		return (-1);
	if (fs_active = calloc(fs_filecnt, sizeof(*fs_active)),
	    fs_active == NULL) {
		errno = ENOMEM;
		err(EX_OSERR, "allocating files data");
	}

	return (0);
}

/*
 * Function:
 * 	outfiles		- output selected filenames
 * Inputs:
 * 	none
 * Returns:
 * 	0 on success, -1 with errno set on error
 * Modifies:
 * 	nothing; writes to stdout
 */

int
outfiles(void) {
	unsigned i;

	for (i = 0; i < fs_filecnt; i++)
		if (fs_active[i])
			printf("%s\n", fs_stat[i].fname);
	return (0);
}
