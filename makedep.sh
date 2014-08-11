#!/bin/sh
# $Ringlet: c/misc/fselect/makedep.sh,v 1.1 2014/01/07 13:41:51 roam Exp $
[ -z "$DEPENDFILE" ] && DEPENDFILE=.depend
gcc $CFLAGS -MM $* | egrep -ve '^# [0-9]+ ' > $DEPENDFILE
