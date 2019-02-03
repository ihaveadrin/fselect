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
 */

#include <curses.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "fselect.h"
#include "fs_curs.h"

static char	cvs_id[] __UNUS =
"$Ringlet: c/misc/fselect/fs_curs.c,v 1.4 2014/01/07 13:30:52 roam Exp $";

static int	curs_initwin(WINDOW **pw);
static int	curs_closewin(WINDOW *w);

static int	curs_buildlist(char ***, unsigned, struct fsstat *, unsigned);
static int	curs_freelist(char **, unsigned);

static int	curs_dispone(WINDOW *, char **, unsigned, unsigned, unsigned);
static int	curs_displist(WINDOW *, char **, unsigned, int *,
		unsigned, unsigned);

int
fs_curs_sel(unsigned cnt, struct fsstat *afiles, int *aactive,
		unsigned viewf) {
	WINDOW *curswin;
	char **filestr;
	int yet, redisp, ch;
	unsigned start, cur;
	int my, mx;

	if (curs_buildlist(&filestr, cnt, afiles, viewf) < 0)
		return (-1);
	if (curs_initwin(&curswin) < 0)
		return (-1);

	getmaxyx(curswin, my, mx);
	cur = start = 0;
	yet = redisp = 1;
	while (yet) {
		if (redisp) {
			curs_displist(curswin, filestr, cnt, aactive,
			    start, cur);
			refresh();
		}

		switch (ch = getch(), ch) {
			case KEY_IC:
			case ' ':
				aactive[cur] = !aactive[cur];
				/* FALLTHROUGH */
			case KEY_DOWN:
			case 'j':
				if (cur < cnt-1) {
					cur++;
					redisp = 1;
					if (cur >= start + (unsigned) my)
						start = cur -  (unsigned) my + 1;
				}
				break;

			case KEY_UP:
			case 'k':
				if (cur) {
					cur--;
					redisp = 1;
					if (cur < start)
						start = cur;
				}
				break;

			case KEY_PPAGE:
			case 'K':
				/* keep relative page offset */
				cur -= start;
				if (start >=  (unsigned) my)
					start -=  (unsigned) my;
				else if (start)
					start = 0;
				cur += start;
				redisp = 1;
				break;

			case KEY_NPAGE:
			case 'J':
				cur -= start;
				if (start +  (unsigned) my < cnt)
					start +=  (unsigned) my;
				if (start + cur >= cnt)
					cur = cnt-1;
				else
					cur += start;
				redisp = 1;
				break;

			case KEY_HOME:
				start = cur = 0;
				redisp = 1;
				break;

			case KEY_END:
				start = ((cnt-1) /  (unsigned) my) *  (unsigned) my;
				cur = cnt-1;
				break;

			case '\033':
				memset(aactive, 0, cnt * sizeof(*aactive));
				yet = 0;
				break;

			case KEY_ENTER:
			case '\r':
			case '\n':
				yet = 0;
				break;
			case '.':
				aactive[cur] = !aactive[cur];
				if (cur < cnt-1) {
					cur++;
					redisp = 1;
					if (cur >= start +  (unsigned) my)
						start = cur -  (unsigned) my + 1;
				}
				yet = 0;
				break;

		}
	}

	if (curs_closewin(curswin) < 0)
		return (-1);
	if (curs_freelist(filestr, cnt) < 0)
		return (-1);
	return (0);
}

int fs_curs_cho(unsigned cnt, struct fsstat *afiles, int *aactive,
		unsigned viewf) {
	WINDOW *curswin;
	char **filestr;
	int yet, redisp, ch;
	int my, mx;
	unsigned start, cur;

	if (curs_buildlist(&filestr, cnt, afiles, viewf) < 0)
		return (-1);
	if (curs_initwin(&curswin) < 0)
		return (-1);

	getmaxyx(curswin, my, mx);
	cur = start = 0;
	yet = redisp = 1;
	while (yet) {
		if (redisp) {
			curs_displist(curswin, filestr, cnt, aactive,
			    start, cur);
			refresh();
		}

		switch (ch = getch(), ch) {
			case KEY_DOWN:
			case 'j':
				if (cur < cnt-1) {
					cur++;
					redisp = 1;
					if (cur >=  start + (unsigned) my)
						start = cur - (unsigned) my + 1;
				}
				break;

			case KEY_UP:
			case 'k':
				if (cur) {
					cur--;
					redisp = 1;
					if (cur < start)
						start = cur;
				}
				break;

			case KEY_PPAGE:
			case 'K':
				/* keep relative page offset */
				cur -= start;
				if (start >= (unsigned) my)
					start -= (unsigned) my;
				else if (start)
					start = 0;
				cur += start;
				redisp = 1;
				break;

			case KEY_NPAGE:
			case 'J':
				cur -= start;
				if (start + (unsigned) my < cnt)
					start += (unsigned) my;
				if (start + cur >= cnt)
					cur = cnt-1;
				else
					cur += start;
				redisp = 1;
				break;

			case KEY_HOME:
				start = cur = 0;
				redisp = 1;
				break;

			case KEY_END:
				start = (unsigned int) (((cnt-1) / (unsigned int) my) * (unsigned) my);
				cur = cnt-1;
				break;

			case '\033':
				yet = 0;
				break;

			case KEY_ENTER:
			case '\r':
			case '\n':
				yet = 0;
				aactive[cur] = !aactive[cur];
				break;	


		}

	}
	if (curs_closewin(curswin) < 0)
		return (-1);
	if (curs_freelist(filestr, cnt) < 0)
		return (-1);
	return (0);
}

static int
curs_initwin(WINDOW **pw) {
	WINDOW *w;

	if (w = initscr(), w == NULL)
		return (-1);
	meta(w, TRUE);
	keypad(w, TRUE);

	*pw = w;
	return (0);
}

static int
curs_closewin(WINDOW *w) {

	if (w) {
		werase(w);
		refresh();
		delwin(w);
	}
	endwin();
	return (0);
}

static int
curs_buildlist(char ***strs, unsigned cnt, struct fsstat *afiles,
		unsigned viewf) {
	char **pstrs;
	unsigned i;

	if (pstrs = calloc(cnt, sizeof(*pstrs)), pstrs == NULL) {
		errno = ENOMEM;
		err(EX_OSERR, "allocating file display data");
	}
	
	for (i = 0; i < cnt; i++)
		if (fs_filetostr(pstrs+i, afiles+i, viewf) < 0)
			err(EX_OSERR, "building file display data");

	*strs = pstrs;
	return (0);
}

static int
curs_freelist(char **strs, unsigned cnt) {
	unsigned i;

	for (i = 0; i < cnt; i++)
		if (strs[i])
			free(strs[i]);
	memset(strs, 0, cnt * sizeof(*strs));
	free(strs);
	return (0);
}

static int
curs_dispone(WINDOW *w, char **strs, unsigned idx, unsigned row,
		unsigned attr) {
	int mx, my;

	getmaxyx(w, my, mx);
	wmove(w, (int) row, 0);
	wclrtoeol(w);
	if (attr)
		wattron(w, attr);
	/* Urrrrgh... wprintw() should take a const, dammit! */
	wprintw(w, (char *)(unsigned long long)" %-*.*s ", mx-3, mx-3,
	    strs[idx]);
	if (attr)
		wattroff(w, attr);
	return (0);
}

static int
curs_displist(WINDOW *w, char **strs, unsigned cnt, int *aactive,
		unsigned start, unsigned cur) {
	unsigned i;
	int mx, my;

	getmaxyx(w, my, mx);
	for (i = start; (i < start + (unsigned) my) && (i < cnt); i++)
		curs_dispone(w, strs, i, i - start,
		    (i == cur? A_REVERSE: 0) | (aactive[i]? A_BOLD: 0));
	for (; i < start + (unsigned) my; i++) {
		wmove(w, (int) (i - start), 0);
		wclrtoeol(w);
	}
	return (0);
}
