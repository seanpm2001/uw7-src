#ident	"@(#)optim.c	11.1"
/*	Copyright (c) 1990, 1991, 1992, 1993 Novell, Inc. All Rights Reserved.	*/
/*	Copyright (c) 1984, 1985, 1986, 1987, 1988, 1989, 1990 Novell, Inc. All Rights Reserved.	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Novell Inc.	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *	Copyright (c) 1982, 1986, 1988
 *	The Regents of the University of California
 *	All Rights Reserved.
 *	Portions of this document are derived from
 *	software developed by the University of
 *	California, Berkeley, and its contributors.
 */

#ident "@(#)optim.c	1.13 'attmail mail(1) command'"
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 * Network name modification routines.
 */

#include "rcv.h"

static char		*lasthost ARGS((char *addr));
static char		*makeremote ARGS((char name[], char from[]));
static char		*tackon ARGS((char *sys, char *rest));
static char		*unuucp ARGS((char*));
static char		*conv_uucp ARGS((char*));

/*
 * Map a name into the correct network "view" of the
 * name.  This is done by prepending the name with the
 * network address of the sender, then optimizing away
 * nonsense.
 */

char *
netmap(username, fromname)
	char username[], fromname[];
{
	char nbuf[BUFSIZ], ret[BUFSIZ];
	register char *cp, *oname;

	if (debug) fprintf(stderr, "netmap(username '%s', fromname '%s')\n", username, fromname);
	if (strlen(fromname) == 0)
		return(username);
	if (any('@', username))
		return(username);
	if (any('@', fromname))
		return(conv_uucp(makeremote(username, fromname)));
	if (value("onehop") && ((cp = strchr(username, '!')) != 0) && cp > username)
		strcpy(nbuf, username);
	else {
		fromname = tackon(host, fromname);
		*strrchr(fromname, '!') = 0;
		username = tackon(lasthost(fromname), username);
		while (((cp = lasthost(fromname)) != 0) && ishost(cp, username)) {
			oname = username;
			username = strchr(username, '!') + 1;
			if (cp == fromname) {
				fromname[strlen(fromname)] = '!';
				if (value("mustbang") && !strchr(username, '!'))
					username = oname;
				return(conv_uucp(username));
			}
			*--cp = 0;
		}
		fromname[strlen(fromname)] = '!';
		fromname = strchr(fromname, '!') + 1;
		sprintf(nbuf, "%s!%s", fromname, username);
	}
	strcpy(ret, nbuf);
	cp = ret;
	if (debug) fprintf(stderr, "wind up with '%s'\n", username);
	if (!icequal(username, cp))
		return(conv_uucp(savestr(cp)));
	return(conv_uucp(username));
}

/*
 * Stick a host on the beginning of a uucp
 * address if it isn't there already.
 */
static char *
tackon(sys, rest)
	char *sys, *rest;
{
	while (*rest == '!')
		rest++;
	if (!ishost(sys, rest)) {
		char *r = salloc((unsigned)(strlen(sys) + strlen(rest)) + 2);
		sprintf(r, "%s!%s", sys, rest);
		rest = r;
	}
	return rest;
}

/*
 * Check equality of the first host in a uucp address.
 */
ishost(sys, rest)
	char *sys, *rest;
{
	while (*sys && *sys == *rest)
		sys++, rest++;
	return(*sys == 0 && *rest == '!');
}

/*
 * Return last host in a uucp address.
 */
static char *
lasthost(addr)
	char *addr;
{
	char *r = strrchr(addr, '!');
	return r ? ++r : addr;
}

/*
 * Optionally translate an old format uucp name into a new one.
 * Two types of translation are supported:
 *	 $conv == "internet" or -U specified
 *		"mach1!mach2!user" becomes "user@mach2.UUCP".
 *		This is optional because some information is
 *		necessarily lost (e.g. the route it got here
 *		via) and if we don't have the host in our
 *		routing tables, we lose.
 *
 *	$conv == "optimize"
 *		Remove loops in uucp(1C) address paths (typically
 *		generated by the reply command). No rerouting is
 *		performed; mail has no knowledge of UUCP routes or
 *		connections.
 *		"mach1!mach2!mach1!user" becomes "mach1!user" and
 *		"mach1!mach1!user" becomes "mach1!user".
 */
static char *
conv_uucp(uucpname)
char *uucpname;
{
	register char *cp;

	if (debug) fprintf(stderr, "conv_uucp(%s)\n", uucpname);
	if (UnUUCP)
		uucpname = unuucp(uucpname);

	else if ((cp = value("conv")) == NOSTR)
		/* EMPTY */;

	else if (strcmp(cp, "internet") == 0)
		uucpname = unuucp(uucpname);

	else if (strcmp(cp, "optimize") == 0)
		{
		if (!any('@', uucpname))
			bang_collapse(uucpname);
		uucpname = savestr(uucpname);
		}

	if (debug) fprintf(stderr, "conv_uucp returns %s\n", uucpname);
	return uucpname;
}

/*
 * Deal with conv="internet" conversion.
 */
static char *
unuucp(uucpname)
char *uucpname;
{
	register char *np, *hp, *cp;
	char result[100];
	char tname[300];
	
	if (debug) fprintf(stderr, "unuucp(%s)\n", uucpname);
	strcpy(tname, uucpname);
	np = strrchr(tname, '!');
	if (np == NOSTR)
		return uucpname;
	*np++ = 0;
	hp = strrchr(tname, '!');
	if (hp == NOSTR)
		hp = tname;
	else
		*hp++ = 0;
	cp = strchr(np, '@');
	if (cp == NOSTR)
		cp = strchr(np, '%');
	if (cp)
		*cp = 0;
	if (debug) fprintf(stderr, "host %s, uucpname %s\n", hp, np);
	sprintf(result, "%s@%s.UUCP", np, hp);
	if (debug) fprintf(stderr, "unuucp returns %s\n", result);
	return savestr(result);
}

/*
 * We have name with no @'s in it, and from with @'s.
 * Assume that name is meaningful only on the site in from.
 */
static char *
makeremote(username, fromname)
	char username[];
	char fromname[];
{
	register char *cp;
	char *rbuf;

	if (debug) fprintf(stderr, "makeremote(%s, %s) returns ", username, fromname);
	cp = strrchr(fromname, '@');
	rbuf = salloc(strlen(username) + strlen(cp) + 1);
	strcpy(rbuf, username);
	strcat(rbuf, cp);
	if (debug) fprintf(stderr, "%s\n", rbuf);
	return rbuf;
}
