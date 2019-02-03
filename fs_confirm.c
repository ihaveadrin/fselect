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

#include <stdio.h>
#include <stdlib.h>

#include "fselect.h"
#include "fs_confirm.h"

static char	cvs_id[] __UNUS =
"$Ringlet: c/misc/fselect/fs_confirm.c,v 1.3 2014/01/07 13:22:22 roam Exp $";

int
fs_confirm_sel(unsigned cnt, struct fsstat *afiles, int *aactive,
		unsigned viewf, int max) {
	unsigned i;
	int ch;
	char *s;
	int select_amount = 0;

	for (i = 0; i < cnt; i++) {
		if (fs_filetostr(&s, afiles+i, viewf) < 0)
			return (-1);
		printf("%s (%s) ", s,
		    (aactive[i]? "Y/n": "y/N"));
		switch (ch = getchar()) {
			case 'Y':
			case 'y':
				select_amount++;
				aactive[i] = 1;
				if(select_amount == max) {
					free(s);
					return 0;
				}
				break;
			case 'N':
			case 'n':
				aactive[i] = 0;
				break;
		}

		/* skip to EOL */
		while ((ch != '\n') && (ch != EOF))
			ch = getchar();
		free(s);
	}
	
	return (0);
}
