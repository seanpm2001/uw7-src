#ident	"@(#)OSRcmds:ksh/shlib/valup.c	1.1"
#pragma comment(exestr, "@(#) valup.c 25.3 93/01/20 ")
/*
 *	Copyright (C) The Santa Cruz Operation, 1990-1993.
 *		All Rights Reserved.
 *	This Module contains Proprietary Information of
 *	The Santa Cruz Operation, and should be treated as Confidential.
 */
/*									*/
/*		   Copyright (C) AT&T, 1984-1992			*/
/*			All Rights Reserved				*/
/*									*/
/*	  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.		*/
/*	    The copyright notice above does not evidence any		*/
/*	   actual or intended publication of such source code.		*/
/*									*/

/*
 *  Modification History
 *
 *	L000	scol!markhe	25 Nov 92
 *	- "name.h" needs the definition of 'MSG', so include "defs.h" (which
 *	  also happens to include "name.h")
 */

/*
 *   VALUP.C
 *
 *   Programmer:  D. G. Korn
 *
 *        Owner:  D. A. Lambeth
 *
 *         Date:  April 17, 1980
 *
 *
 *   NAM_STRVAL (NP)
 *        
 *        Return a pointer to the value of the node given by NP.
 *
 *
 *   See Also:  nam_putval(III), nam_longput(III), nam_free(III)
 */

#include	"defs.h"					/* L000 */
/* #include	"name.h" */					/* L000 */

extern char *utos();
extern char *ltos();
#ifdef FLOAT
extern char *ftos();
extern char *etos();
#endif /* FLOAT */


/*
 *   NAM_STRVAL (NP)
 *       
 *        struct namnod *NP;
 *
 *   Return a pointer to a character string that denotes the value
 *   of NP.  If NP refers to an array,  return a pointer to
 *   the value associated with the current index.
 *
 *   If NP is blocked, N_AVAIL, then the value of the node
 *   with the same name in the last tree is returned.
 *   Thus a node can become blocked after the lookup but
 *   before retrieving its value and still work correctly.
 *
 *   If the value of NP is an integer, the string returned will
 *   be overwritten by the next call to nam_strval.
 *
 *   If NP has no value, NULL is returned.
 */

char *nam_strval(np)
register struct namnod *np;
{
	register union Namval *up= &np->value.namval;
	register struct Nodval *nv;
#ifdef NAME_SCOPE
	if (nam_istype (np,N_AVAIL))	/* node blocked */
		/* use node with same name in last tree */
		np = nam_copy(np,0);
#endif /* NAME_SCOPE */
	if (!nam_istype(np,N_ARRAY|N_INDIRECT|N_INTGER))
		return(up->cp);
	else if (nam_istype (np, N_ARRAY))
	{
		if((nv = array_find(np,A_LOOKUP)) == NULL)
	        	return (NULL);
		up = &nv->namval;
	}
	if ((nam_istype (np, N_INDIRECT)) && up->cp)
		up = up->up;
	if (nam_istype (np, N_INTGER))
	{
#ifndef pdp11
		register
#endif /* pdp11 */
		long l;
        	if (nam_istype (np, N_CPOINTER))
			return(up->cp);
#ifdef FLOAT
		else if (nam_istype (np, N_DOUBLE))
		{
			if(nam_istype (np, N_EXPNOTE))
				return(etos(*up->dp,np->value.namsz));
			else
				return(ftos(*up->dp,np->value.namsz));
		}
#endif /* FLOAT */
		else if(nam_istype (np, (N_BLTNOD)))
			l = ((*up->fp->f_vp)());
        	else
		{
			if(up->lp == NULL)
				return(NULL);
			l = *(up->lp);
		}
		if (nam_istype (np, (N_BLTNOD|N_UNSIGN)))
#ifdef pdp11
			return(utos(l,np->value.namsz));
#else
			return(utos((unsigned long)l,np->value.namsz));
#endif /* pdp11 */
		return(ltos(l,np->value.namsz));
	}
	return (up->cp);
}

