#ident	"@(#)ksh93:include/ast/stk.h	1.1"
/***************************************************************
*                                                              *
*                      AT&T - PROPRIETARY                      *
*                                                              *
*         THIS IS PROPRIETARY SOURCE CODE LICENSED BY          *
*                          AT&T CORP.                          *
*                                                              *
*                Copyright (c) 1995 AT&T Corp.                 *
*                     All Rights Reserved                      *
*                                                              *
*           This software is licensed by AT&T Corp.            *
*       under the terms and conditions of the license in       *
*       http://www.research.att.com/orgs/ssr/book/reuse        *
*                                                              *
*               This software was created by the               *
*           Software Engineering Research Department           *
*                    AT&T Bell Laboratories                    *
*                                                              *
*               For further information contact                *
*                     gsf@research.att.com                     *
*                                                              *
***************************************************************/

/* : : generated by proto : : */
                  
/*
 * David Korn
 * AT&T Bell Laboratories
 *
 * Interface definitions for a stack-like storage library
 *
 */

#ifndef _STK_H
#if !defined(__PROTO__)
#include <prototyped.h>
#endif

#define _STK_H

#include <sfio.h>

#if _DLL_INDIRECT_DATA && !_DLL
#define _Stk_data	(*_Stak_data)
#else
#define _Stk_data	_Stak_data
#endif

extern __MANGLE__ Sfio_t		_Stk_data;
#define stkstd		(&_Stk_data)

#define	Stk_t		Sfio_t

#define STK_SMALL	1		/* argument to stkopen		*/

#define	stkptr(sp,n)	((char*)((sp)->data)+(n))
#define	stktell(sp)	((sp)->next-(sp)->data)
#define stkseek(sp,n)	((n)==0?(char*)((sp)->next=(sp)->data):_stkseek(sp,n))

extern __MANGLE__ Stk_t*		stkopen __PROTO__((int));
extern __MANGLE__ Stk_t*		stkinstall __PROTO__((Stk_t*, char*(*)(int)));
extern __MANGLE__ int		stkclose __PROTO__((Stk_t*));
extern __MANGLE__ int		stklink __PROTO__((Stk_t*));
extern __MANGLE__ char*		stkalloc __PROTO__((Stk_t*, unsigned));
extern __MANGLE__ char*		stkcopy __PROTO__((Stk_t*,const char*));
extern __MANGLE__ char*		stkset __PROTO__((Stk_t*, char*, unsigned));
extern __MANGLE__ char*		_stkseek __PROTO__((Stk_t*, unsigned));
extern __MANGLE__ char*		stkfreeze __PROTO__((Stk_t*, unsigned));

#endif
