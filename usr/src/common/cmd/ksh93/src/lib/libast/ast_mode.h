#ident	"@(#)ksh93:src/lib/libast/ast_mode.h	1.1"
/* : : generated from features/mode.c by iffe version 05/09/95 : : */
#ifndef _def_mode_ast
#define _def_mode_ast	1
#define S_ITYPE(m)	((m)&S_IFMT)
#define S_ISLNK(m)	(S_ITYPE(m)==S_IFLNK)
#define S_ISSOCK(m)	(S_ITYPE(m)==S_IFSOCK)

#define S_IPERM		(S_ISUID|S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG|S_IRWXO)

#define _S_IDPERM	1
#define _S_IDTYPE	1

#define BUFFERSIZE	8192

#endif
