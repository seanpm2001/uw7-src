#ident	"@(#)acpp:common/cpp.h	1.67"
/* cpp.h - ANSI cpp global header */


#include "interface.h"
#include "../../intemu/common/intemu.h"

#	if (defined CPLUSPLUS && defined DBG) ||\
 	 (! defined CPLUSPLUS && !defined NODBG )
#	ifndef NODBG
#		define DEBUG
#	endif
#endif

#ifdef DEBUG
#	include <assert.h>
#endif

#ifdef __STDC__
#	include <stdlib.h>
#	include <stddef.h>
#else
#	define ptrdiff_t long
#	define const
#endif

#ifdef DEBUG
#	define	COMMENT(arg)	assert(arg)
#	define	DBGASSERT(arg)	assert(arg)
#	define	DBGCODE(x)	x
#	undef	DEBUG
#	define	DEBUG(ch)	(debuglevels[(ch) - 'a'])
#else
#	define	COMMENT(arg)
#	define	DBGASSERT(arg)
#	define	DBGCODE(x)
#endif
#ifdef PERF
#	define	PERFCODE(x)	x
#	undef	PERF
#	define	PERF(ch)	(perflevels[(ch) - 'a'])
#else
#	define	PERFCODE(x)
#endif

/* command line option bit values */
#define F_KEEP_COMMENTS	0x0001	/* preserve comments */
#define F_NO_DIRECTIVES	0x0002	/* don't output # directives */
#define F_INCLUDE_FILES	0x0004	/* print included files on stderr */
#define F_TWO_COMMENTS	0x0008	/* silently recognize C++ // comments */
#define F_2CMNTS_WARNED	0x0010	/* have already warned (-Xc only) */
#define F_Xt		0x0020	/* transistion option */
#define F_Xa		0x0040	/* ansi option */
#define F_Xc		0x0080	/* conforming option */
#define F_XARGMASK	0x00e0	/* mask of -X flag arguments */
#ifdef CXREF
#define F_CXREF		0x0100	/* C cross reference */
#endif
#define F_DOLLAR_WARNED 0x0200	/* have warned about '$' in id (-Xc only) */

#define UERROR		fl_uerror
#define FATAL		fl_fatal
#define TKERROR		fl_tkerror
#define TKWARN		fl_tkwarn
#define WARN		fl_warn

/* Often in these source files, there are places where
** "switch"-es are done on token codes. Due
** to the way compilers generate code for switch statements,
** it is important to have relatively close "case" expressions,
** so a jump table will be produced. 
** if the "case" expressions are too divergent, a
** search tree will be produced - which would have
** a deleterious effect on many oft-used routines.
** So, switches should be done on C_ENUMNO, not the
** entire Token.code.
*/
#define	CASE(code)	case ((code) & C_ENUMNO):

extern char	tk_chtypes[];
extern unsigned int	pp_flags;	/* holds F_* bit values */
extern char lx_dotifile();
extern int	dollar_allowed_in_id;	/* boolean: '$' allowed in an identifier */

#ifdef MERGED_CPP
extern void	(* pp_interface)();	/* interface routine to merged process */
#endif
#ifdef DEBUG
extern int	debuglevels[];
#endif
#ifdef PERF
extern int	perflevels[];
#endif
void free();

extern	Token * ex_condexpr(	/* Token *	*/ );
extern	Token * ex_directive(	/* Token *	*/ );
extern	void	ex_init(	/* void		*/ );
extern	Token *	ex_input( /* Token *, Macro *	*/ );

extern	void	pp_internal(	/* const char * */	); 
extern	void	pp_nodefaults();
#ifdef __STDC__
extern	void *	pp_malloc( /*	unsigned int	*/ );
extern	void *	pp_realloc( /*	char *, unsigned int	*/ );
#else
extern	char *	pp_malloc( /*	unsigned int	*/ );
extern	char *	pp_realloc( /*	char *, unsigned int	*/ );
#endif
#ifdef CXREF
extern	void	pp_xref( /* Macro*, long, char */);
#endif
extern	char *	ch_alloc( /*	unsigned int	*/ );
extern	char *	ch_saven( /*	char *, unsigned int	*/ );
extern	void	ch_init( /*	void	*/ );
extern	void	pp_printmem( /* const char *, int */ );
extern  void    lx_init( /*	void	*/   );

extern	Token * tk_bool( /* int		*/ );
extern	Token * tk_cp(	 /* Token *	*/ );
extern	Token * tk_cpl(	 /* Token * 	*/ );
extern	Token * tk_eol(	 /* Token * 	*/ );
extern	void	tk_extra(/* Token *	*/ );
extern	void	tk_hide( /* Token *	*/ );
#ifndef MERGED_CPP
extern	void	tk_allow_dollar_ident( /* void	*/ );
#endif
extern	int	tk_chk_if_dollar_allowed_in_id( /* void */ );
extern	void	tk_init( /* void	*/ );
extern	unsigned int	tk_lenl( /* Token *	*/ );
extern	void	tk_merge(/* Token *, Token *, int */ );
extern	Token *	tk_new(	 /* void	*/ );
extern	Token *	tk_paste(/* void	*/ );
extern	Token * tk_rm(	 /* Token *	*/ );
extern	void	tk_rml(	 /* Token *	*/ );
extern	void	tk_rmto( /* Token *, Token *	*/ );
extern	Token *	tk_rmws( /* Token *	*/ );
#ifdef DEBUG
extern	char *	tk_saycode( /* int	*/ );
extern  void    tk_pr(   /* int   */ );
extern  void    tk_prl(  /* int  */ );
#endif
extern	Token * tk_tokenize( /* char *	*/ );

extern	int	xp_value(	/* Token* */ );
extern int cplusplus;	/* Flag to select c/c++ lexing, -B option controls */
extern chars_signed; /* "Signedness" of "plain" chars, determined in acomp */
extern int preprocess_doti(); /* If true, force preprocessing of .i */
