#ident	"@(#)ksh93:src/lib/libast/pathconf.c	1.1"

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

static long
statconf __PARAM__((struct stat* st, int op), (st, op)) __OTORP__(struct stat* st; int op;){
	switch (op)
	{
#ifdef	_PC_ASYNC_IO
	case _PC_ASYNC_IO:
#ifdef	ASYNC_IO
		return(ASYNC_IO-0);
#else
#ifdef	_POSIX_ASYNC_IO
		return(_POSIX_ASYNC_IO-0);
#else
		break;
#endif
#endif
#endif
#ifdef	_PC_CHOWN_RESTRICTED
	case _PC_CHOWN_RESTRICTED:
#ifdef	CHOWN_RESTRICTED
		return(CHOWN_RESTRICTED-0);
#else
#ifdef	_POSIX_CHOWN_RESTRICTED
		return(_POSIX_CHOWN_RESTRICTED-0);
#else
		break;
#endif
#endif
#endif
#ifdef	_PC_LINK_MAX
	case _PC_LINK_MAX:
#ifdef	LINK_MAX
		return(LINK_MAX-0);
#else
#ifdef	MAXLINK
		return(MAXLINK-0);
#else
#ifdef	SHRT_MAX
		return(SHRT_MAX-0);
#else
#ifdef	_POSIX_LINK_MAX
		return(_POSIX_LINK_MAX-0);
#else
		return(8-0);
#endif
#endif
#endif
#endif
#endif
#ifdef	_PC_MAX_CANON
	case _PC_MAX_CANON:
#ifdef	MAX_CANON
		return(MAX_CANON-0);
#else
#ifdef	CANBSIZ
		return(CANBSIZ-0);
#else
#ifdef	_POSIX_MAX_CANON
		return(_POSIX_MAX_CANON-0);
#else
		return(255-0);
#endif
#endif
#endif
#endif
#ifdef	_PC_MAX_INPUT
	case _PC_MAX_INPUT:
#ifdef	MAX_INPUT
		return(MAX_INPUT-0);
#else
#ifdef	MAX_CANON
		return(MAX_CANON-0);
#else
#ifdef	_POSIX_MAX_INPUT
		return(_POSIX_MAX_INPUT-0);
#else
		return(255-0);
#endif
#endif
#endif
#endif
#ifdef	_PC_NAME_MAX
	case _PC_NAME_MAX:
#ifdef	NAME_MAX
		return(NAME_MAX-0);
#else
#ifdef	_LOCAL_NAME_MAX
		return(_LOCAL_NAME_MAX-0);
#else
#ifdef	_POSIX_NAME_MAX
		return(_POSIX_NAME_MAX-0);
#else
		return(14-0);
#endif
#endif
#endif
#endif
#ifdef	_PC_NO_TRUNC
	case _PC_NO_TRUNC:
#ifdef	NO_TRUNC
		return(NO_TRUNC-0);
#else
#ifdef	_POSIX_NO_TRUNC
		return(_POSIX_NO_TRUNC-0);
#else
		break;
#endif
#endif
#endif
#ifdef	_PC_PATH_MAX
	case _PC_PATH_MAX:
#ifdef	PATH_MAX
		return(PATH_MAX-0);
#else
#ifdef	MAXPATHLEN
		return(MAXPATHLEN-0);
#else
#ifdef	_POSIX_PATH_MAX
		return(_POSIX_PATH_MAX-0);
#else
		return(256-0);
#endif
#endif
#endif
#endif
#ifdef	_PC_PIPE_BUF
	case _PC_PIPE_BUF:
#ifdef	PIPE_BUF
		return(PIPE_BUF-0);
#else
#ifdef	_POSIX_PIPE_BUF
		return(_POSIX_PIPE_BUF-0);
#else
		return(512-0);
#endif
#endif
#endif
#ifdef	_PC_PRIO_IO
	case _PC_PRIO_IO:
#ifdef	PRIO_IO
		return(PRIO_IO-0);
#else
#ifdef	_POSIX_PRIO_IO
		return(_POSIX_PRIO_IO-0);
#else
		break;
#endif
#endif
#endif
#ifdef	_PC_SYMLINK_MAX
	case _PC_SYMLINK_MAX:
#ifdef	SYMLINK_MAX
		return(SYMLINK_MAX-0);
#else
#ifdef	_LOCAL_SYMLINK_MAX
		return(_LOCAL_SYMLINK_MAX-0);
#else
#ifdef	_POSIX_SYMLINK_MAX
		return(_POSIX_SYMLINK_MAX-0);
#else
		return(255-0);
#endif
#endif
#endif
#endif
#ifdef	_PC_SYMLOOP_MAX
	case _PC_SYMLOOP_MAX:
#ifdef	SYMLOOP_MAX
		return(SYMLOOP_MAX-0);
#else
#ifdef	_POSIX_SYMLOOP_MAX
		return(_POSIX_SYMLOOP_MAX-0);
#else
		return(8-0);
#endif
#endif
#endif
#ifdef	_PC_SYNC_IO
	case _PC_SYNC_IO:
#ifdef	SYNC_IO
		return(SYNC_IO-0);
#else
#ifdef	_POSIX_SYNC_IO
		return(_POSIX_SYNC_IO-0);
#else
		break;
#endif
#endif
#endif
#ifdef	_PC_VDISABLE
	case _PC_VDISABLE:
#ifdef	VDISABLE
		return(VDISABLE-0);
#else
#ifdef	_POSIX_VDISABLE
		return(_POSIX_VDISABLE-0);
#else
		break;
#endif
#endif
#endif
	default:
		break;
	}
	errno = EINVAL;
	return(-1);
}

long
fpathconf __PARAM__((int fd, int op), (fd, op)) __OTORP__(int fd; int op;){
	int		n;
	struct stat	st;
#if _lib_fpathconf
#undef	fpathconf
	if ((n = op) >= 0 || -op <= confmap[0] && (n = confmap[-op]) >= 0)
	{
		long	val;
		int	olderrno;
		int	syserrno;

		olderrno = errno;
		errno = 0;
		val = fpathconf(fd, n);
		syserrno = errno;
		errno = olderrno;
		if (val != -1L || syserrno == 0)
			return(val);
	}
#endif
	return((n = fstat(fd, &st)) ? n : statconf(&st, op));
}

long
pathconf __PARAM__((const char* path, int op), (path, op)) __OTORP__(const char* path; int op;){
	int		n;
	struct stat	st;
#if _lib_pathconf
#undef	pathconf
	if ((n = op) >= 0 || -op <= confmap[0] && (n = confmap[-op]) >= 0)
	{
		long	val;
		int	olderrno;
		int	syserrno;

		olderrno = errno;
		errno = 0;
		val = pathconf(path, n);
		syserrno = errno;
		errno = olderrno;
		if (val != -1L || syserrno == 0)
			return(val);
	}
#endif
	return((n = stat(path, &st)) ? n : statconf(&st, op));
}
