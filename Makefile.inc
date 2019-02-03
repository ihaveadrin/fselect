# Copyright (c) 2001-2019  Peter Pentchev, Teodoro Santoni, dyveloper
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $Ringlet: c/misc/fselect/Makefile.inc,v 1.1 2014/01/07 13:41:51 roam Exp $

PROG=		fselect
SRCS=		fselect.c fs_confirm.c fs_curs.c fs_file.c fs_tty.c
MAN1=

FS_VER_MAJ=	1
FS_VER_MIN=	0

CFLAGS_OPT?=	-O
CFLAGS_DBG?=	-g
CFLAGS_WARN?=	-Wall -W ${BDECFLAGS}

CFLAGS_VER=	-DFS_VER_MAJ=${FS_VER_MAJ} -DFS_VER_MIN=${FS_VER_MIN}

CFLAGS?=	${CFLAGS_OPT} ${CFLAGS_DBG} ${CFLAGS_WARN}
CFLAGS+=	${CFLAGS_VER} ${CFLAGS_OS}

WARNS?=		9

DPADD+=		${LIBNCURSES}
LDADD+=		-lncurses

LOCALBASE?=	/usr/local
PREFIX?=	${LOCALBASE}
BINDIR?=	${PREFIX}/bin
MANDIR?=	${PREFIX}/man/man

RM?=		rm -f
MKDIR?=		mkdir -p
SETENV?=	env
LN?=		ln -s
