/*		copyright	"%c%" 	*/

# ident	"@(#)sac.c	1.4"
#ident  "$Header$"

/***************************************************************************
 * Command: sac
 *
 * Inheritable Privileges: P_MACWRITE,P_DACWRITE,P_OWNER
 *       Fixed Privileges: None
 *
 * Notes: sac(1m) mainly needs P_MACWRITE & P_DACWRITE to write to the
 *	  /var/adm/utmp file. 
 *	  In a file based privilege system (LPM - Enhanced Security):
 *	  sac(1m) does not pass privileges to the commands 
 *	  (found in _sactab) that it executes because it uses
 *	  /sbin/tfadmin to determine & give the necessary privileges to the
 *	  commands.  Thus, each command must have an entry under the
 *	  TFM database under the appropriate user.
 ***************************************************************************/

# include <stdio.h>
# include <fcntl.h>
# include <ctype.h>
# include <signal.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/stropts.h>
# include <unistd.h>
# include <utmp.h>
# include <memory.h>
# include <sac.h>
# include <priv.h>
# include <mac.h>
# include <sys/secsys.h>
# include <locale.h>
# include <pfmt.h>
# include "msgs.h"
# include "msg.h"
# include "extern.h"
# include "misc.h"
# include "structs.h"

# define NAMELEN	8	/* length in bytes of name field in utmp */
# define LINELEN	12	/* length in bytes of line field in utmp */

# define RESP	1		/* pollfail via no response to sanity poll */
# define DEATH	2		/* pollfail via child death */

/* signal whose dispositions will be changed */

static struct sigaction	Sigpoll;	/* SIGPOLL */
static struct sigaction	Sigcld;		/* SIGCLD */
static struct sigaction	Sigalrm;	/* SIGALRM */
static sigset_t Origmask;		/* original signal mask */

char    *msg_label = "UX:sac";


/*
 * Procedure:     main
 *
 * Restrictions:  getopt: None
 *
 * Notes:  scan args for sac, initialize everything, and wait
 *		     for commands from sacadm via the command pipe

*/

main(argc, argv)
int argc;
char *argv[];
{
	int c;				/* place to hold options */
	struct sigaction sigact;	/* for signal handling */

	(void)setlocale(LC_ALL, "");
	(void)setcat("uxsaf");
	(void)setlabel(msg_label);

	(void) sigprocmask(SIG_SETMASK, NULL, &Origmask);
	if (argc == 1)
		usage();
	(void) setpgrp();
	while ((c = getopt(argc, argv, "t:")) != -1) {
		switch (c) {
		case 't':
			if (Stime != 0)
				usage();
			Stime = atoi(optarg);
			if (Stime <= 0)
				usage();
			break;
		case '?':
			usage();
		}
	}
	if (optind < argc)
		usage();

	initialize();
	sigact.sa_flags = 0;
	sigact.sa_handler = pollpms;
	(void) sigemptyset(&sigact.sa_mask);
	(void) sigaddset(&sigact.sa_mask, SIGALRM);
	(void) sigaddset(&sigact.sa_mask, SIGCLD);
	(void) sigaddset(&sigact.sa_mask, SIGPOLL);
	(void) sigaction(SIGALRM, &sigact, &Sigalrm);

/*
 * minimize time spent in STARTING or UNKNOWN, pollpms() sets alarm
 */

	pollpms();
	for (;;)
		readpipe();
}


/*
 * Procedure:     usage
 *
 * Restrictions:
                 fopen: None
                 fprintf: None
 *
 * Notes - output a usage message on the console
 */

void
usage()
{
	FILE *fp;	/* scratch file pointer */

	fp = fopen("/dev/console", "w");
	if (fp)
		pfmt (fp, MM_ERROR, MSG157);
		(void) fprintf(fp, "SAC: Usage: sac -t sanity_interval\n");
	exit(1);
}

/*
 * Procedure:     initialize
 *
 * Restrictions:
                 doconfig: None
                 sprintf: None
                 unlink(2): None
                 mknod(2): None
                 access(2): None
                 chown(2): None!
                 open(2): None
*/



void
initialize()
{
	int ret;			/* scratch return code */
	struct sigaction sigact;	/* for signal handling */

	openlog();
	log(gettxt(MSGID158,MSG158));
# ifdef DEBUG
	opendebug();
	log("Debugging turned on");
# endif
	
	if (chdir(HOME) < 0)
		error(E_CHDIR, EXIT);

/*
 * pass an invalid fd, shouldn't be doing pushes and pops in this per-system
 * configuration script (_sysconfig)
 */

	if ((ret = doconfig(-1, SYSCONFIG, 0)) != 0) {
		if (ret == -1)
			error(E_SYSCONF, EXIT);
		else {
			(void) sprintf(Scratch, gettxt(MSGID159,MSG159), ret);
			log(Scratch);
			error(E_BADSYSCONF, EXIT);
		}
	}

	sigact.sa_flags = 0;
	sigact.sa_handler = reap;
	(void) sigemptyset(&sigact.sa_mask);
	(void) sigaddset(&sigact.sa_mask, SIGALRM);
	(void) sigaddset(&sigact.sa_mask, SIGCLD);
	(void) sigaddset(&sigact.sa_mask, SIGPOLL);
	(void) sigaction(SIGCLD, &sigact, &Sigcld);

/*
 * establish pipe for PMS to communicate with sac
 */

	if (access("_sacpipe", 0) != 0) {
		/* not there, create one */
		(void) umask(0);
		if (mknod("_sacpipe", S_IFIFO | 0600, 0) < 0)
			error(E_NOPIPE, EXIT);
	}

	if (chown("_sacpipe", (uid_t) SAF_OWNER, (gid_t) SAF_GROUP) != 0) 
		error(E_CHOWN, EXIT);
		
	Sfd = open("_sacpipe", O_RDWR);
	if (Sfd < 0)
		error(E_NOPIPE, EXIT);

/*
 * establish pipe for sacadm to communicate with sac
 */

	Cfd = mk_cmd_pipe();

/*
 * read in _sactab, but don't start port monitors as a by-product
 * since we may be in recovery - start them explicitly instead
 */

	read_table(FALSE);
	startpoll();
	startpms();
}


/*
 * Procedure:     startpm
 *
 * Restrictions:
                 sprintf: None
                 access(2): None
                 mknod(2): None
                 chown(2): None!
                 open(2): P_MACWRITE

 * Notes: startpms - start initial set of port monitors
 *
 *
 */


void
startpms()
{
	register struct sactab *sp;	/* working pointer */
	int rflag;			/* recovery flag */
	pid_t checklock();

/*
 * check to see if we're really a recovering SAC (if any port monitors hold
 * locks, assume that we're in recovery), if so, start differently
 */

	rflag = 0;
	for (sp = Sactab; sp; sp = sp->sc_next) {
		if (checklock(sp)) {
			rflag = 1;
			sp->sc_sstate = sp->sc_pstate = UNKNOWN;
			sp->sc_ok = 1;
			sp->sc_exit = 0;
			(void) sprintf(Scratch, "%s/_pmpipe", sp->sc_tag);
			procprivl(CLRPRV, MACWRITE_W, 0);
			sp->sc_fd = open(Scratch, O_RDWR);
			procprivl(SETPRV, MACWRITE_W, 0);
			if (sp->sc_fd < 0) {

/*
 * if we get into here, we're in deep trouble.  PM seems to be running
 * and we're trying to recover, but we can't talk to it.  Unfortunately,
 * there's not much that can be done other than to try and restore a
 * sane state.  By setting sp->sc_ok to 0, this will look like a poll failure
 * and if sp->rs_rsmax > 0, PM will be restarted.
 */

				(void) sprintf(Scratch, gettxt(MSGID160,MSG160), sp->sc_tag);
				log(Scratch);
				(void) sendsig(sp, SIGTERM);
				sp->sc_ok = 0;
			}
		}
	}
	if (rflag) {
		readutmp();
		log(gettxt(MSGID161,MSG161));
		return;
	}

/*
 * normal startup
 */

	for (sp = Sactab; sp; sp = sp->sc_next) {
		if (sp->sc_flags & X_FLAG) {
			/* System Administator specified don't start */
			continue;
		}
		(void) startpm(sp);
	}
}

/*
 * Procedure:     readutmp
 *
 * Restrictions:
                 getutent: None
                 sprintf: None

 *
 * Notes - read the utmp file to find out the ids of
 *			 running port monitors (only called during a
 *			 recover start up).
 *
 *    After a sac failure, init will inherit all of the port
 *	  monitors and should get the SIGCLD's if they die (and
 *	  will clean up).  This is mainly for stuck processes,
 *	  although init would get the SIGCLD when the stuckie gets
 *	  killed, it doesn't hurt to have the sac check.  This is
 *	  only done once.
 *
 */


void
readutmp()
{
	register struct sactab *sp;	/* working pointer */
	register struct sactab *savesp;	/* rembered port monitor match */
	register struct utmp *up;	/* working pointer */

	setutent();
	while (up = getutent()) {
		/* we're only interested in login processes */
		if (up->ut_type != LOGIN_PROCESS)
			continue;
		if (up->ut_user[NAMELEN - 1] == '\0') {

/*
 * possible port monitor and name is short enough to do a normal compare
 */

			sp = findpm(up->ut_user);
			if (sp && (sp->sc_sstate == UNKNOWN)) {
				/* found one */
				(void) memcpy(sp->sc_utid, up->ut_id, IDLEN);
				sp->sc_pid = (pid_t)up->ut_pid;
			}
		}
		else {

/*
 * possible port monitor name, but it could have been truncated.  If
 * a match is found on a unique prefix, then it should be the correct
 * entry.  If an ambiguity is found, ignore the entry, init will clean
 * up the entry if it dies.
 */

			savesp = (struct sactab *) NULL;
			for (sp = Sactab; sp; sp = sp->sc_next) {
				if (!strncmp(up->ut_name, sp->sc_tag, NAMELEN)) {
					if (savesp) {
						/* already found a match */
						savesp = (struct sactab *) NULL;
						(void) sprintf(Scratch, gettxt(MSGID162, MSG162), sp->sc_tag);
						log(Scratch);
						break;
					}
					else {
						savesp = sp;
					}
				}
			}
			if (savesp && (savesp->sc_sstate == UNKNOWN)) {
				/* found it */
				(void) memcpy(savesp->sc_utid, up->ut_id, IDLEN);
				savesp->sc_pid = (pid_t)up->ut_pid;
			}
		}
	}
	endutent();
}


/*
 * Procedure:     startpm
 *
 * Restrictions:
                 sprintf: None
                 access(2): None
                 mknod(2): None
                 chown(2): None!
                 open(2): P_MACWRITE
 * Notes:  start a particular PM
 *
 * Return codes: return code:	-1: _pid file locked
 * 			-2: any other reason
 *
 * Args: sp - pointer to sac's port monitor information
 *	      for designated port monitor
 *
 * Restrictions: mknod(2): <none>	open(2): <none>
 *		 chown(2): <none>
 */


startpm(sp)
register struct sactab *sp;
{
	sigset_t cset;		/* for signal handling */
	sigset_t tset;		/* for signal handling */
	pid_t pid;		/* pid of new port monitor */
	pid_t checklock();
	
# ifdef DEBUG
	debug("in startpm");
# endif
	if (checklock(sp)) {
		(void) sprintf(Scratch,gettxt(MSGID163, MSG163), sp->sc_tag);
		log(Scratch);
		return(-1);
	}

	(void) sprintf(Scratch, "%s/_pmpipe", sp->sc_tag);
	if (access(Scratch, 0) != 0) {
		/* not there, create one */
		(void) umask(0);
		if (mknod(Scratch, S_IFIFO | 0600, 0) < 0) {
			(void) sprintf(Scratch,  gettxt(MSGID165, MSG165), sp->sc_tag, errno);
			log(Scratch);
			return(-2);
		}
		if (chown(Scratch, (uid_t) SAF_OWNER, (gid_t) SAF_GROUP) != 0) {
			(void) sprintf(Scratch, gettxt(MSGID164, MSG164), sp->sc_tag, errno);
			log(Scratch);
			return(-2);
		}
        }
	procprivl(CLRPRV, MACWRITE_W, 0);
	sp->sc_fd = open(Scratch, O_RDWR);
	procprivl(SETPRV, MACWRITE_W, 0);
	if (sp->sc_fd < 0) {
		(void) sprintf(Scratch, gettxt(MSGID166, MSG166), sp->sc_tag, errno);
		log(Scratch);
		return(-2);
	}

	/* in case child dies too quickly */
	(void) sigprocmask(SIG_SETMASK, NULL, &cset);
	tset = cset;
	(void) sigaddset(&tset, SIGCLD);
	(void) sigprocmask(SIG_SETMASK, &tset, NULL);
	if ((pid = fork()) < 0) {
		(void) sprintf(Scratch, gettxt(MSGID167, MSG167), sp->sc_tag);
		log(Scratch);
		return(-2);
	}
	else if (!pid) {
		startit(sp);
		/* no return */
	}

/*
 * clean up old utmp if its there
 */

	cleanut(sp);

/*
 * create a utmp entry and set initial states
 */

	account(sp, pid);
	sp->sc_pstate = STARTING;
	if (sp->sc_lstate == NOTRUNNING)
		sp->sc_sstate = (sp->sc_flags & D_FLAG) ? DISABLED : ENABLED;
	else
		sp->sc_sstate = sp->sc_lstate;
	sp->sc_ok = 1;
	sp->sc_exit = 0;
	sp->sc_pid = pid;
	/* ok to take signals now that the table is up-to-table */
	(void) sigprocmask(SIG_SETMASK, &cset, NULL);
	return(0);
}


/*
 * Procedure:     cleanut
 *
 * Restrictions:
                 getutid: None
                 sprintf: None
                 modut: None
 *
 * Notes:	- clean out a utmp record for a port monitor
 *
 * Args:  sp - pointer to sac's port monitor information for
 * 	       designated port monitor
 *
 */


void
cleanut(sp)
register struct sactab *sp;
{
	struct utmp utmp;			/* prototype utmp entry */
	register struct utmp *up = &utmp;	/* and a pointer to it */
	register int i;				/* scratch variable */
	int zerocheck;				/* scratch variable */
	char buf[SIZE];				/* scratch buffer */

/*
 * check to see if there is a utmp entry to clean up (indicated by a non
 * zero utmp id
 */

	zerocheck = 0;
	for (i = 0; i < IDLEN; ++i) {
		zerocheck += sp->sc_utid[i];
	}
	if (zerocheck == 0)
		return;

	setutent();
	(void) memcpy(up->ut_id, sp->sc_utid, IDLEN);
	up->ut_type = LOGIN_PROCESS;
	if ((up = getutid(up)) == NULL) {
		(void) sprintf(buf, gettxt(MSGID168,MSG168), sp->sc_tag);
		log(buf);
	}
	else {
		if (up->ut_type == DEAD_PROCESS) {
			/* init cleaned it up */
			endutent();
			return;
		}
		up->ut_type = DEAD_PROCESS;
		up->ut_exit.e_termination = (sp->sc_exit & 0xff);
		up->ut_exit.e_exit = ((sp->sc_exit >> 8) & 0xff);
		(void) time(&up->ut_time);
		if (modut(up) == NULL) {
			(void) sprintf(buf, gettxt(MSGID169,MSG169), sp->sc_tag);
			log(buf);
		}
	}
	endutent();
}


/*
 * Procedure:     account
 *
 * Restrictions:
                 makeut: None

 * Notes - create a utmp record for a port monitor
 *
 * Args:  pid - process id of port monitor
 *
 */


void
account(sp, pid)
register struct sactab *sp;
pid_t pid;
{
	struct utmp utmp;			/* prototype utmp entry */
	register struct utmp *up = &utmp;	/* and a pointer to it */
	struct utmp *err_up;		/* return structure of makeut() */

	(void) memset(up, '\0', sizeof(utmp));
	(void) strncpy(up->ut_user, sp->sc_tag, NAMELEN);
	up->ut_pid = (o_pid_t)pid;
	up->ut_type = LOGIN_PROCESS;
	up->ut_id[0] = 'P';
	up->ut_id[1] = 'M';
	up->ut_id[2] = SC_WILDC;
	up->ut_id[3] = SC_WILDC;
	(void) time(&up->ut_time);
	err_up = makeut(up);
	if (err_up == NULL) {
		log("Could not create utmp entry");
		(void) memset(sp->sc_utid, '\0', IDLEN);
	}
	else {
		(void) memcpy(sp->sc_utid, up->ut_id, IDLEN);
	}
}


/*
 * Procedure:     startit
 *
 * Restrictions:
                 sprintf: none
                 doconfig: none
                 fcntl(2): none
 *
 * Nontes:  finish starting a particular port monitor,
 *			establish environment, etc. 
 *			(Note: this is the child at this point)
 *
 * Args: sp - pointer to sac's port monitor information for
 *		     designated port monitor
 * 
 */


void
startit(sp)
register struct sactab *sp;
{
	static char istate[SIZE];	/* place to put ISTATE env var. */
	static char pmtag[SIZE];	/* place to put PMTAG env var. */
	char **argvp;			/* arglist for PM */
	register int i;			/* loop control variable */
	register long ndesc;		/* # of file descriptors configured */
	int ret;			/* scratch return value */
	sigset_t cset;			/* for signal handling */
	sigset_t tset;			/* for signal handling */

/*
 * establish the home directory
 */
	if (chdir(sp->sc_tag) < 0) {
		(void) sprintf(Scratch, gettxt(MSGID170,MSG170), HOME, sp->sc_tag);
		log(Scratch);
		exit(1);
	}

/*
 * interpret the configuration script, pass an invalid fd, shouldn't be
 * doing pushes and pops in this script
 */

	(void) sigprocmask(SIG_SETMASK, NULL, &cset);
	tset = cset;
	(void) sigaddset(&tset, SIGCLD);
	(void) sigprocmask(SIG_SETMASK, &tset, NULL);
	if ((ret = doconfig(-1, "_config", 0)) != 0) {
		if (ret == -1) {
			(void) sprintf(Scratch, gettxt(MSGID171,MSG171), sp->sc_tag);
			log(Scratch);
			exit(1);
		}
		else {
			(void) sprintf(Scratch, gettxt(MSGID172,MSG172), sp->sc_tag, ret);
			log(Scratch);
			exit(1);
		}
	}

/*
 * add the promised environment variables
 */

	if (sp->sc_lstate == NOTRUNNING)
		(void) sprintf(istate, "ISTATE=%s", (sp->sc_flags & D_FLAG) ? "disabled" : "enabled");
	else
		(void) sprintf(istate, "ISTATE=%s", (sp->sc_lstate == DISABLED) ? "disabled" : "enabled");
	if (putenv(istate)) {
		(void) sprintf(Scratch, gettxt(MSGID173,MSG173), sp->sc_tag);
		log(Scratch);
		exit(1);
	}
	(void) sprintf(pmtag, "PMTAG=%s", sp->sc_tag);
	if (putenv(pmtag)) {
		(void) sprintf(Scratch, gettxt(MSGID173,MSG173), sp->sc_tag);
		log(Scratch);
		exit(1);
	}

/*
 * build an argv
 */

	argvp = mkargv(sp);
	
	(void) sprintf(Scratch, gettxt(MSGID174,MSG174), sp->sc_tag);
	log(Scratch);
	ndesc = ulimit(4, 0L);
	for (i = 0; i < ndesc; i++)
		(void) fcntl(i, F_SETFD, 1);
	/* restore orignal handlers and mask */
	(void) sigaction(SIGPOLL, &Sigpoll, NULL);
	(void) sigaction(SIGCLD, &Sigcld, NULL);
	(void) sigaction(SIGALRM, &Sigalrm, NULL);
	(void) sigprocmask(SIG_SETMASK, &Origmask, NULL);
	(void) execve(argvp[0], argvp, environ);
	(void) sprintf(Scratch, gettxt(MSGID175,MSG175), sp->sc_tag);
	log(Scratch);
	exit(1);
}


/*
 * Procedure: mkargv - Given a pointer to a struct sactab, construct
 *		       argv for an exec system call.
 *
 * Args: sp - pointer to sac's port monitor information for
 *	      designated port montior
 */

# define NARGS 50		/* max # of args */

static char *newargv[NARGS];	/* place for argv list */
static char *delim = " \t'\"";	/* delimiter list */

char **
mkargv(sp)
register struct sactab *sp;
{
	register char **argvp = newargv;	/* scratch pointer */
	register char *p = sp->sc_cmd;		/* working pointer */
	char delch;				/* delimiter seen */
	register char *savep;			/* scratch pointer */
	register char *tp;			/* scratch pointer */

	/*
	 * Check what privilege mechanism the system currently has.
	 * If the system has a file based privilege mechanism
	 * then execute the command through the /sbin/tfadmin command
	 * and the TFM database.  Otherwise, execute the command
	 * directly.  
	 */
	
	if (secsys(ES_PRVID,0) == -1)
		*argvp++ = "/sbin/tfadmin";
	else
		*argvp = 0;

	savep = p;
	while (p && *p) {
		if (p = strpbrk(p, delim)) {
			switch (*p) {
			case ' ':
			case '\t':
				/* "normal" cases */
				*p++ = '\0';
				*argvp++ = savep;
				/* zap trailing white space */
				while (isspace(*p))
					p++;
				savep = p;
				break;
			case '"':
			case '\'':
				/* found a string */
				delch = *p; /* remember the delimiter */
				savep = ++p;

/*
 * We work the string in place, embedded instances of the string delimiter,
 * i.e. \" must have the '\' removed.  Since we'd have to do a compare to
 * decide if a copy were needed, it's less work to just do the copy, even
 * though it is most likely unnecessary.
 */

				tp = p;
				for (;;) {
					if (*p == '\0') {
						(void) sprintf(Scratch, gettxt(MSGID176,MSG176),
							sp->sc_tag);
						log(Scratch);
						exit(1);
					}
					if (*p == delch) {
						if (*(tp - 1) == '\\') { /* \delim */
							*(tp - 1) = *p;
							p++;
						}
						else { /* end of string */
							*tp = 0;
							*argvp++ = savep;
							p++;
							/* zap trailing white space */
							while (isspace(*p))
								p++;
							savep = p;
							break;
						}
					}
					else {
						*tp++ = *p++;
					}
				}
				break;
			default:
				log(gettxt(MSGID176,MSG176));
				exit(1);
			}
		}
		else
			*argvp++ = savep;
	}
	*argvp = 0;
	return(newargv);
}


/*
 * Procedure: pollpms
 *
 * Notes:  send out sanity polls, if sc_sstate and sc_pstate are
 *	   the same (everyone agrees on the state) or if SAC thinks PM
 *	   should be stopping, send out a status message;
 *	   otherwise, send out a message indicating the state the SAC
 *	   thinks the PM should be entering
 */

void
pollpms()
{
	register struct sactab *sp;	/* working pointer */
	struct sacmsg sacmsg;		/* message to send to PM */

# ifdef DEBUG
	debug("alarm went off");
# endif
	for (sp = Sactab; sp; sp = sp->sc_next) {
		if (sp->sc_pstate == NOTRUNNING || sp->sc_pstate == FAILED) {
			/* don't bother if no one is home */
			continue;
		}
		if (sp->sc_ok == 0) {
			/* PM has stopped responding */
			pollfail(sp, RESP);
			continue;
		}

/*
 * note - if we're in recovery, a SC_STATUS message is sent
 * (sc_sstate = UNKNOWN and sc_pstate = UNKNOWN)
 */

		if (sp->sc_sstate == sp->sc_pstate) {
			sacmsg.sc_type = SC_STATUS;
			sacmsg.sc_size = 0;
		}
		else {
			switch (sp->sc_sstate) {
			case ENABLED:
				sacmsg.sc_type = SC_ENABLE;
				sacmsg.sc_size = 0;
				break;
			case DISABLED:
				sacmsg.sc_type = SC_DISABLE;
				sacmsg.sc_size = 0;
				break;
			case STARTING:
			case STOPPING:
			case NOTRUNNING:
			case FAILED:
			case UNKNOWN:
				/* if NOTRUNNING or FAILED, PM will probably
				   not respond to poll, that's how we detect
				   that it's gone */
				sacmsg.sc_type = SC_STATUS;
				sacmsg.sc_size = 0;
				break;
			default:
				error(E_BADSTATE, EXIT);
			}
		}

		/* send the message */
		sendpmmsg(sp, &sacmsg);
		sp->sc_ok = 0;
	}
	(void) alarm(Stime);
}


/*
 * Procedure: reap - clean up dead children, equivalent to a "fast" poll
 *		     failure 
 *
 * Args:	signo - signal #
 */

void
reap(signo)
int signo;	/* not used - but it keeps the compiler happy */
{
	register struct sactab *sp;	/* working pointer */
	pid_t pid;			/* returned pid from wait */
	int status;			/* returned status from wait */

	pid = wait(&status);
	for (sp = Sactab; sp; sp = sp->sc_next) {
		if (sp->sc_pid == pid)
			break;
	}
	if (sp == NULL) {
		/* not from a port monitor we know about */
		return;
	}
	sp->sc_exit = status;
	/* only call pollfail for "stuck" and stopping processes */
	if (sp->sc_pstate != NOTRUNNING && sp->sc_pstate != FAILED)
		pollfail(sp, DEATH);
	return;
}


/*
 * Procedure: pollfail - handle the case where a PM stops responding to
 *			 a sanity poll 
 *
 * Args: sp - pointer to sac's port monitor information for
 *	      designated port monitor
 *	 reason - RESP or DEATH (indicates why pollfail called)
 */

void
pollfail(sp, reason)
register struct sactab *sp;
int reason;
{
	char buf[SIZE];			/* scratch buffer */
	sigset_t cset;			/* for signal handling */
	sigset_t tset;			/* for signal handling */

# ifdef DEBUG
	debug("in pollfail");
# endif

/*
 * first, remove the utmp entry and clean up any links
 */

	cleanut(sp);

	if (sp->sc_pstate == STOPPING) {
		(void) sprintf(buf, gettxt(MSGID178,MSG178), sp->sc_tag);
		log(buf);
		sp->sc_pstate = NOTRUNNING;
		sp->sc_lstate = NOTRUNNING;
		(void) close(sp->sc_fd);
	}
	else {

/*
 * PM in trouble - if it's still there, try to put it out of its misery
 * We play with SIGCLD here to that after SIGKILL is sent, the catcher
 * routine reap() is not called until we're ready (note: when a catcher
 * is established for SIGCLD and any zombies are present, the signal is
 * immediately received)
 */

		(void) sigprocmask(SIG_SETMASK, NULL, &cset);
		tset = cset;
		(void) sigaddset(&tset, SIGCLD);
		(void) sigprocmask(SIG_SETMASK, &tset, NULL);
		(void) sendsig(sp, SIGKILL);
		if (sp->sc_rscnt < sp->sc_rsmax) {
			/* try to restart it */
			if (reason == RESP)
				(void) sprintf(buf, gettxt(MSGID179,MSG179), sp->sc_tag);
			else
				(void) sprintf(buf, gettxt(MSGID180,MSG180), sp->sc_tag);
			log(buf);
			sp->sc_rscnt++;
			(void) close(sp->sc_fd);
			(void) startpm(sp);
		}
		else {
			sp->sc_sstate = sp->sc_pstate = FAILED;
			(void) close(sp->sc_fd);
			(void) sprintf(buf, gettxt(MSGID181,MSG181), sp->sc_tag);
			log(buf);
		}
	}
	(void) sigprocmask(SIG_SETMASK, &cset, NULL);
}


/*
 * Procedure: readpipe - read messages from _sacpipe
 */


void
readpipe()
{
	struct pmmsg pmmsg;			/* incoming message */
	register struct pmmsg *pp = &pmmsg;	/* and a pointer to it */
	register struct sactab *sp;		/* working pointer */
	int ret;				/* return value from read */

/*
 * This routine's main purpose is to maintain the state associated with
 * each of the known port monitors.  Because it may be confusing, following
 * is a brief discussion of what is happening.  Three different views of
 * a port monitor's state exist: sc_sstate, sc_pstate, and sc_lstate.
 * sc_sstate is the state in which the sac has been instructed to place
 * a port monitor.  sc_lstate is essentially a shadow of this field, however,
 * it will only take on the values ENABLED, DISABLED, and NOTRUNNING.
 * sc_lstate is used if a port monitor dies to restart it in the state in
 * which it was last running.  sc_pstate is the last state that the port
 * monitor reported itself in.  Note that if the administrator specifies
 * a state change, there is a window where sc_sstate and sc_pstate will
 * be different (until the port monitor enacts and acknowledges the change).
 *
 * These states interact with the polling loop to determine which message
 * should be sent to a port monitor.  If the states agree, an SC_STATUS
 * is sent.  If they disagree, the appropriate message to put the port
 * monitor in the correct state is sent (SC_ENABLE or SC_DISABLE).  sc_pstate
 * is the state that is reported back to an AC_STATUS request.  Finally,
 * when in recovery (sc_sstate and sc_pstate both = UNKNOWN), the sac will
 * take the port monitor's reported state as the true state.  This is the
 * only instance in which a port monitor can cause sc_sstate to change.
 */

	for (;;) {
		if (read(Sfd, pp, sizeof(pmmsg)) < 0) {
			if (errno != EINTR)
				error(E_BADREAD, EXIT);
			continue;
		}

		while (pp->pm_size) {

/*
 * there's data after the header, unfortunately, we don't understand
 * any of it because only class 1 (no data) messages are defined.  Just
 * flush it
 */

			ret = read(Sfd, Scratch, (pp->pm_size > SIZE) ? (unsigned) SIZE : (unsigned) pp->pm_size);
			if (ret < 0) {
				if (errno != EINTR)
					error(E_BADREAD, EXIT);
				continue;
			}
			else
				pp->pm_size -= ret;
		}

		sp = findpm(pp->pm_tag);
		if (sp == NULL) {
			log("message from unknown process");
			continue;
		}
		switch (pp->pm_type) {
		case PM_UNKNOWN:
			(void) sprintf(Scratch,gettxt(MSGID182,MSG182) , sp->sc_tag);
			log(Scratch);
			/* fall through */
		case PM_STATUS:
			/*
			 * paranoia check, if port monitor reports garbage
			 * state, pretend it said UNKNOWN
			 */
			if (!validstate(pp->pm_state)) {
				pp->pm_state = UNKNOWN;
				(void) sprintf(Scratch, gettxt(MSGID183,MSG183), sp->sc_tag);
				log(Scratch);
			}
			if (sp->sc_sstate == sp->sc_pstate) {
				/* everyone seems to agree on the current state */
				if (sp->sc_sstate == UNKNOWN) {
					/* special case for recovery */
					sp->sc_sstate = pp->pm_state;
					sp->sc_pstate = pp->pm_state;
					if (pp->pm_state == ENABLED || pp->pm_state == DISABLED)
						/* note: sc_lstate is NOTRUNNING by default */
						sp->sc_lstate = pp->pm_state;
				}
				if (pp->pm_state != sp->sc_pstate) {
					/* something isn't right here, PM
					   changed state without orders, try
					   to restore to correct state */
					sp->sc_pstate = pp->pm_state;
				}
			}
			else if (sp->sc_sstate == pp->pm_state) {
				/* PM changed to state requested */
				(void) sprintf(Scratch, "port monitor <%s> changed state from %s to %s",
						sp->sc_tag, pstate(sp->sc_pstate), pstate(pp->pm_state));
				log(Scratch);
				sp->sc_pstate = pp->pm_state;
			}
			else if (sp->sc_pstate != pp->pm_state) {
				/* something isn't right here, PM isn't
				   in the state it was, nor is it in the
				   state we just tried to put it in, try
				   to restore to correct state if we should */
				if (sp->sc_pstate != STOPPING)
					sp->sc_pstate = pp->pm_state;
			}
			break;
		default:
			(void) sprintf(Scratch, gettxt(MSGID184,MSG184), sp->sc_tag);
			log(Scratch);
			break;
		}
		/* no matter what, PM did answer the poll */
		sp->sc_ok = 1;
		/* Note the messages it understands */
		sp->sc_maxclass = pp->pm_maxclass;
	}
}


/*
 * Procedure: validstate - determine if arg s a valid return state from
 *			   a port monitor 
 * Notes: return 1 if ok, 0 otherwise
 *
 * Args: state - state to be verified
 */

validstate(state)
unchar state;
{
	switch (state) {
	case PM_ENABLED:
	case PM_DISABLED:
	case PM_STARTING:
	case PM_STOPPING:
		return(1);
	default:
		return(0);
	}
}


/*
 * Procedure:     mk_cmd_pipe
 *
 * Restrictions:
                 unlink(2): None
                 open(2): None
                 fattach: None
                 chown(2): None

 * Notes - create the command pipe used by sacadm
 *
 * Restrictions: unlink(2): <none>	open(2): <none>
 *		 fattach: <none>	chown(2): <none>
 *
 *       Since the process is running at SYS_PRIVATE level,
 *	  CMDPIPE and the pipe created with pipe(2) will also
 *	  be at SYS_PRIVATE level. 
 */


mk_cmd_pipe()
{
	int fds[2];			/* pipe endpoints */
	int fd;				/* scratch file descriptor */

	(void) unlink(CMDPIPE);
	fd = open(CMDPIPE, O_RDWR | O_CREAT, 0600);
	if (fd < 0)
		error(E_CMDPIPE, EXIT);
	close(fd);
	
	if (pipe(fds) < 0)
		error(E_PIPE, EXIT);
	if (fattach(fds[0], CMDPIPE) < 0)
		error(E_FATTACH, EXIT);
		
	if (chown(CMDPIPE, (uid_t) SAF_OWNER, (gid_t) SAF_GROUP) != 0) 
		error(E_CHOWN, EXIT);

	return(fds[1]);
}


/*
 * Procedure: startpoll - enable polling on command pipe by setting up
 *			  to catch SIGPOLL 
 *
 * Restrictions: ioctl(2):  <none>
 */


void
startpoll()
{
	struct sigaction sigact;	/* for signal handling */

	if (ioctl(Cfd, I_SETSIG, S_INPUT) < 0)
		error(E_SETSIG, EXIT);
	sigact.sa_flags = 0;
	sigact.sa_handler = sigpoll;
	(void) sigemptyset(&sigact.sa_mask);
	(void) sigaddset(&sigact.sa_mask, SIGALRM);
	(void) sigaddset(&sigact.sa_mask, SIGCLD);
	(void) sigaddset(&sigact.sa_mask, SIGPOLL);
	(void) sigaction(SIGPOLL, &sigact, &Sigpoll);
}
