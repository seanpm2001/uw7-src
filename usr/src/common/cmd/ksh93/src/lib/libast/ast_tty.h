#ident	"@(#)ksh93:src/lib/libast/ast_tty.h	1.1"

/* : : generated by proto : : */
/* : : generated from features/tty by iffe version 05/09/95 : : */
                  
#ifndef _def_tty_ast
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

#define _def_tty_ast	1

#if defined(__STDPP__directive) && defined(__STDPP__hide)
__STDPP__directive pragma pp:hide ioctl
#else
#define ioctl		______ioctl
#endif
#define _hdr_termios	1	/* #include <termios.h> ok */
#define _sys_termios	1	/* #include <sys/termios.h> ok */
#define _hdr_sgtty	1	/* #include <sgtty.h> ok */
#define _sys_ioctl	1	/* #include <sys/ioctl.h> ok */
#include <termio.h>

#if _sys_ioctl && !defined(_IOR)
#include <sys/ioctl.h>
#endif
#if defined(__STDPP__directive) && defined(__STDPP__hide)
__STDPP__directive pragma pp:nohide ioctl
#else
#undef	ioctl
#endif
extern __MANGLE__ int		ioctl __PROTO__((int, int, ...));
#endif
