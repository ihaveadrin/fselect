# Copyright (c) 2001-2019  Peter Pentchev, Teodoro Santoni, Dylan Göpel
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
# $Ringlet: c/misc/fselect/GNUmakefile,v 1.1 2014/01/07 13:41:51 roam Exp $

BDECFLAGS?=	-W -std=c18 -Wall -pedantic -Wbad-function-cast -Wcast-align \
		-Wcast-qual -Wchar-subscripts -Wconversion -Winline \
		-Wmissing-prototypes -Wnested-externs -Wpointer-arith \
		-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings \
		-Wno-unused-but-set-variable

CFLAGS_OS_VER=	$(shell uname -s)
CFLAGS_OS_REL=	$(shell uname -r)
CFLAGS_OS_HOST=	$(shell uname -n)

ifeq (${CFLAGS_OS_VER},FreeBSD)
CFLAGS_OS=	-DHAVE_FREEBSD
else
ifeq (${CFLAGS_OS_VER},Linux)
CFLAGS_OS=	-DHAVE_LINUX
else
CFLAGS_OS=
endif
endif

ifndef RELEASE
CFLAGS_OPT=
CFLAGS_DBG=	-g3 -DEBUG
else
CFLAGS_OPT=	-O -pipe
CFLAGS_DBG=
endif

include Makefile.inc

DEPENDFILE?=	.depend
INSTALL?=	/usr/bin/install

INST_COPY?=	-c
BINMODE?=	-m 555
MANMODE?=	-m 444
BINOWN?=	-o root
MANOWN?=	-o root

ifeq (${CFLAGS_OS_VER},FreeBSD)
BINGRP?=	-g wheel
MANGRP?=	-g wheel
else
BINGRP?=	-g root
MANGRP?=	-g root
endif

INSTALL_PROGRAM?=	${INSTALL} ${INST_COPY} ${BINMODE} ${BINOWN} ${BINGRP}
INSTALL_MAN?=	${INSTALL} ${INST_COPY} ${MANMODE} ${MANOWN} ${MANGRP}

MAN1DIR=	${MANDIR}1

OBJS?=		${SRCS:.c=.o}
LIBS?=		${LDADD}

# this NEEDS to be rewritten..
MAN1?=		${PROG}.1
MAN1Z?=		${MAN1:.1=.1.gz}

all:		${PROG} ${MAN1Z}

clean:
		${RM} ${PROG} ${OBJS} ${MAN1Z}

distclean:	clean

linkclean:
		${RM} ${PROG}

relink:		linkclean all

depend:
		${SETENV} CFLAGS="${CFLAGS}" DEPENDFILE="${DEPENDFILE}" \
		sh makedep.sh ${SRCS}

#install:	install-bin install-man
install:	install-bin

install-bin:	${PROG}
		-${MKDIR} ${DESTDIR}${BINDIR}
		${INSTALL_PROGRAM} ${PROG} ${DESTDIR}${BINDIR}

install-man:	${MAN1Z}
		-${MKDIR} ${DESTDIR}${MANDIR}1
		${INSTALL_MAN} ${MAN1Z} ${DESTDIR}${MAN1DIR}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} -o ${PROG} ${OBJS} ${LIBS}

# this NEEDS to be rewritten..
${MAN1Z}:	${MAN1}
		${COMPRESS_CMD} ${MAN1} > ${MAN1Z}

.c.o:
		${CC} -c ${CFLAGS} $<

ifneq (,$(wildcard ${DEPENDFILE}))
include ${DEPENDFILE}
endif
