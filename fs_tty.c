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

#include <fcntl.h>
#include <unistd.h>

#include "fselect.h"
#include "fs_tty.h"

static char	cvs_id[] __UNUS =
"$Ringlet: c/misc/fselect/fs_tty.c,v 1.3 2014/01/07 13:22:22 roam Exp $";

int		fs_noswitch;

static int	ttyactive, fs_ttyfd, fs_outfd;

void
switchtotty(void) {
	int outfd, ttyfd;

	if (ttyactive || fs_noswitch)
		return;
	outfd = dup(1);
	ttyfd = open("/dev/tty", O_RDWR);
	dup2(ttyfd, 1);

	fs_outfd = outfd;
	fs_ttyfd = ttyfd;
	ttyactive = 1;
}

void
switchtostdout(void) {

	if (!ttyactive || fs_noswitch)
		return;

	close(1);
	close(fs_ttyfd);
	dup2(fs_outfd, 1);
	ttyactive = 0;
}

