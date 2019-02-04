#ifndef _INCLUDED_FSELECT_H
#define _INCLUDED_FSELECT_H

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
 *
 *  $Ringlet: c/misc/fselect/fselect.h,v 1.4 2014/01/07 13:22:22 roam Exp $
 */

#ifdef __GNUC__
#define __UNUS		__attribute__((unused))

#ifndef __STRICT_ANSI__
#define __INLINE	inline
#else  /* __STRICT_ANSI__ */
#define __INLINE
#endif /* __STRICT_ANSI__ */

#else  /* __GNUC__ */
#define __UNUS
#define __INLINE
#endif /* __GNUC__ */

/*
 * Definitions
 */

typedef enum {
	FS_STYPE_CONFIRM, FS_STYPE_CURS, FS_STYPE_LAST
} fs_seltype;

#define FS_OPTSTR	"f:NcyhVn:"

/*
 * Variables
 */


/*
 * Functions
 */


#endif /* _INCLUDED */
