#ident	"@(#)ksh93:src/lib/libast/confstr.c	1.1"

/* : : generated by proto : : */

#if !defined(__PROTO__)
#if defined(__STDC__) || defined(__cplusplus) || defined(_proto) || defined(c_plusplus)
#if defined(__cplusplus)
#define __MANGLE__	"C"
#else
#define __MANGLE__
#endif
#define __STDARG__
#define __PROTO__(x)	x
#define __OTORP__(x)
#define __PARAM__(n,o)	n
#if !defined(__STDC__) && !defined(__cplusplus)
#if !defined(c_plusplus)
#define const
#endif
#define signed
#define void		int
#define volatile
#define __V_		char
#else
#define __V_		void
#endif
#else
#define __PROTO__(x)	()
#define __OTORP__(x)	x
#define __PARAM__(n,o)	o
#define __MANGLE__
#define __V_		char
#define const
#define signed
#define void		int
#define volatile
#endif
#if defined(__cplusplus) || defined(c_plusplus)
#define __VARARG__	...
#else
#define __VARARG__
#endif
#if defined(__STDARG__)
#define __VA_START__(p,a)	va_start(p,a)
#else
#define __VA_START__(p,a)	va_start(p)
#endif
#endif
#include <ast.h>
#include <error.h>

/* : : generated by conf from conf.tab : : */

#include "confmap.h"
#include "conflib.h"

static char*
local_confstr __PARAM__((int op), (op)) __OTORP__(int op;){
	switch (op)
	{
#ifdef	_CS_PATH
	case _CS_PATH:
#ifdef	PATH
		return(PATH-0);
#else
#ifdef	_POSIX_PATH
		return(_POSIX_PATH-0);
#else
		return("/bin:/usr/bin"-0);
#endif
#endif
#endif
#ifdef	_CS_SHELL
	case _CS_SHELL:
#ifdef	SHELL
		return(SHELL-0);
#else
#ifdef	_AST_SHELL
		return(_AST_SHELL-0);
#else
		return("/bin/sh"-0);
#endif
#endif
#endif
#ifdef	_CS_TMP
	case _CS_TMP:
#ifdef	TMP
		return(TMP-0);
#else
#ifdef	_AST_TMP
		return(_AST_TMP-0);
#else
		return("/tmp"-0);
#endif
#endif
#endif
	default:
		break;
	}
	return(0);
}

size_t
confstr __PARAM__((int op, char* buf, size_t siz), (op, buf, siz)) __OTORP__(int op; char* buf; size_t siz;){
	char*	s;
	int	n;
#if _lib_confstr
#undef	confstr
	if (((n = op) >= 0 || -op <= confmap[0] && (n = confmap[-op]) >= 0) && (n = confstr(n, buf, siz)) > 0)
		return(n);
#endif
	if (s = local_confstr(op))
	{
		if ((n = strlen(s) + 1) >= siz)
		{
			if (siz == 0)
				return(n + 1);
			buf[n = siz - 1] = 0;
		}
		memcpy(buf, s, n);
		return(n);
	}
	errno = EINVAL;
	return(0);
}
