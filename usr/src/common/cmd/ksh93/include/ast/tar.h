#ident	"@(#)ksh93:include/ast/tar.h	1.1"
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
 * Standard Archive Format
 * USTAR - Uniform Standard Tape ARchive
 */

#ifndef _TAR_H
#if !defined(__PROTO__)
#include <prototyped.h>
#endif

#define _TAR_H

#define TBLOCK		512
#define NAMSIZ		100
#define PFXSIZ		155

#define TMODLEN		8
#define TUIDLEN		8
#define TGIDLEN		8
#define TSIZLEN		12
#define TMTMLEN		12
#define TCKSLEN		8

#define TMAGIC		"ustar"		/* ustar and a null		*/
#define TMAGLEN		6
#define TVERSION	"00"		/* 00 and no null		*/
#define TVERSLEN	2
#define TUNMLEN		32
#define TGNMLEN		32
#define TDEVLEN		8

/*
 * values used in typeflag field
 */

#define REGTYPE		'0'		/* regular file			*/
#define AREGTYPE	0		/* alternate REGTYPE		*/
#define LNKTYPE		'1'		/* hard link			*/
#define SYMTYPE		'2'		/* soft link			*/
#define CHRTYPE		'3'		/* character special		*/
#define BLKTYPE		'4'		/* block special		*/
#define DIRTYPE		'5'		/* directory			*/
#define FIFOTYPE	'6'		/* FIFO special			*/
#define CONTYPE		'7'		/* reserved			*/
#define SOKTYPE		'8'		/* socket -- reserved		*/
#define VERTYPE		'V'		/* version -- reserved		*/
#define EXTTYPE		'x'		/* extended header -- reserved	*/

/*
 * bits used in mode field
 */

#define TSUID		04000		/* set uid on exec		*/
#define TSGID		02000		/* set gid on exec		*/
#define TSVTX		01000		/* sticky bit -- reserved	*/

/*
 * file permissions
 */

#define TUREAD		00400		/* read by owner		*/
#define TUWRITE		00200		/* write by owner		*/
#define TUEXEC		00100		/* execute by owner		*/
#define TGREAD		00040		/* read by group		*/
#define TGWRITE		00020		/* execute by group		*/
#define TGEXEC		00010		/* write by group		*/
#define TOREAD		00004		/* read by other		*/
#define TOWRITE		00002		/* write by other		*/
#define TOEXEC		00001		/* execute by other		*/

struct header
{
	char		name[NAMSIZ];
	char		mode[TMODLEN];
	char		uid[TUIDLEN];
	char		gid[TGIDLEN];
	char		size[TSIZLEN];
	char		mtime[TMTMLEN];
	char		chksum[TCKSLEN];
	char		typeflag;
	char		linkname[NAMSIZ];
	char		magic[TMAGLEN];
	char		version[TVERSLEN];
	char		uname[TUNMLEN];
	char		gname[TGNMLEN];
	char		devmajor[TDEVLEN];
	char		devminor[TDEVLEN];
	char		prefix[PFXSIZ];
};

union hblock
{
	char		dummy[TBLOCK];
	struct header	dbuf;
};

#endif
