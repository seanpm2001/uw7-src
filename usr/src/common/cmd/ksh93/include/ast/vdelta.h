#ident	"@(#)ksh93:include/ast/vdelta.h	1.1"
#ifndef _VDELTA_H
#define _VDELTA_H	1

#ifndef __KPV__
#define __KPV__		1

#ifndef __STD_C
#ifdef __STDC__
#define	__STD_C		1
#else
#if __cplusplus
#define __STD_C		1
#else
#define __STD_C		0
#endif /*__cplusplus*/
#endif /*__STDC__*/
#endif /*__STD_C*/

#ifndef _BEGIN_EXTERNS_
#if __cplusplus
#define _BEGIN_EXTERNS_	extern "C" {
#define _END_EXTERNS_	}
#else
#define _BEGIN_EXTERNS_
#define _END_EXTERNS_
#endif
#endif /*_BEGIN_EXTERNS_*/

#ifndef _ARG_
#if __STD_C
#define _ARG_(x)	x
#else
#define _ARG_(x)	()
#endif
#endif /*_ARG_*/

#ifndef Void_t
#if __STD_C
#define Void_t		void
#else
#define Void_t		char
#endif
#endif /*Void_t*/

#ifndef NIL
#define NIL(type)	((type)0)
#endif /*NIL*/

#endif /*__KPV__*/

/* user-supplied functions to do io */
typedef struct _vddisc_s	Vddisc_t;
typedef int(*	Vdio_f)_ARG_((Void_t*, int, long, Vddisc_t*));
struct _vddisc_s
{	long	size;		/* total data size	*/
	Void_t*	data;		/* data array		*/
	Vdio_f	readf;		/* to read data		*/
	Vdio_f	writef;		/* to write data	*/
};

/* magic header for delta output */
#define VD_MAGIC	"vd01"

_BEGIN_EXTERNS_
extern long	vddelta _ARG_((Vddisc_t*,Vddisc_t*,Vddisc_t*,long));
extern long	vdupdate _ARG_((Vddisc_t*,Vddisc_t*,Vddisc_t*));
_END_EXTERNS_

#endif /*_VDELTA_H*/
