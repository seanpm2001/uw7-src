/*		copyright	"%c%" 	*/

/*
 *	Copyright (c) 1982, 1986, 1988
 *	The Regents of the University of California
 *	All Rights Reserved.
 *	Portions of this document are derived from
 *	software developed by the University of
 *	California, Berkeley, and its contributors.
 */

#ident	"@(#)ufs.cmds:common/cmd/fs.d/ufs/repquota/repquota.c	1.8.9.3"
#ident "$Header$"
/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 * 
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 * 
 * 
 * 
 * 		Copyright Notice 
 * 
 * Notice of copyright on this source code product does not indicate 
 * publication.
 * 
 * 	(c) 1986,1987,1988,1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 * 	          All rights reserved.
 *  
 */

/*
 * Quota report
 */
#include <stdio.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mntent.h>
#include <sys/time.h>	
#include <sys/fs/sfs_quota.h>
#include <sys/stat.h>
#include <sys/mnttab.h>
#include <sys/vfstab.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>

#define LOGINNAMESIZE	8
struct username {
	struct username *u_next;
	u_short u_uid;
	char u_name[LOGINNAMESIZE + 1];
};
#define UHASH 997

struct username *uhead[UHASH];

struct username *adduid();
void	fmttime();
char *hasvfsopt();
struct username *lookup();
static	char	*mntopt();
void	prquota();
void	usage();

int highuid;

int	vflag;		/* verbose */
int	aflag;		/* all file systems */
char *listbuf[50];
char	*myname;	/* name of cmd; for error messages */

extern int	optind;
extern char	*optarg;

#define QFNAME "quotas"

main(argc, argv)
	int argc;
	char **argv;
{
	struct mnttab mntp;
	struct vfstab vfsbuf;
	register struct passwd *pwp;
	register struct username *up;
	char **listp;
	int listcnt;
	char quotafile[MAXPATHLEN + 1];
	FILE *mtab, *vfstab;
	int errs = 0;
	int	status;
	int	opt;

	myname = argv[0];
	while ((opt = getopt (argc, argv, "av")) != EOF) {
		switch (opt) {

		case 'v':
			vflag++;
			break;

		case 'a':
			aflag++;
			break;

		case '?':
			usage ();
		}
	}
	if (argc <= optind && !aflag) {
		usage ();
	}
	(void) setpwent();
	while ((pwp = getpwent()) != 0) {
		up = lookup((u_short)pwp->pw_uid);
		if (up == 0) {
			up = adduid((u_short)pwp->pw_uid);
			strncpy(up->u_name, pwp->pw_name, sizeof(up->u_name));
		}
	}
	(void) endpwent();
	if (quotactl(Q_ALLSYNC, NULL, 0, NULL) < 0 && errno == ENOTTY && vflag) {
		printf("Warning: Quotas are not compiled into this kernel\n");
	}
	/*
	 * If aflag go through vfstab and make a list of appropriate
	 * filesystems.
	 */
	if (aflag) {
		listp = listbuf;
		listcnt = 0;
		if ((vfstab = fopen(VFSTAB, "r")) == NULL) {
			fprintf(stderr, "%s: Can't open %s : %s\n",
				myname, VFSTAB, strerror(errno));
			exit(31+8);
		}
		while ((status = getvfsent(vfstab, &vfsbuf)) == NULL) {
			if (strcmp(vfsbuf.vfs_fstype, MNTTYPE_UFS) != 0 ||
			!hasvfsopt( &vfsbuf, MNTOPT_RQ))
				continue;
			*listp = malloc(strlen(vfsbuf.vfs_special) + 1);
			strcpy(*listp, vfsbuf.vfs_special);
			listp++;
			listcnt++;
		}
		(void) fclose(vfstab);
		*listp = (char *)0;
		listp = listbuf;
	} else {
		listp = &argv[optind];
		listcnt = argc - optind;
	}
	if ((mtab = fopen(MNTTAB, "r")) == NULL) {
		fprintf(stderr, "%s: Can't open %s : %s\n",
			myname, MNTTAB, strerror(errno));
		exit(31+8);
	}
	while ((status = getmntent(mtab, &mntp)) == NULL) {
		if (strcmp(mntp.mnt_fstype, MNTTYPE_UFS) == 0 &&
		    !hasmntopt(&mntp, MNTOPT_RO) &&
		    (oneof(mntp.mnt_special, listp, listcnt) ||
		     oneof(mntp.mnt_mountp, listp, listcnt)) ) {
			sprintf(quotafile, "%s/%s", mntp.mnt_mountp, QFNAME);
			errs +=
			    repquota(mntp.mnt_special,mntp.mnt_mountp,quotafile);
		}
	}
	(void) fclose(mtab);
	while (listcnt--) {
		if (*listp)
			fprintf(stderr, "Cannot report on %s\n", *listp);
	}
	if (errs > 0)
		errs += 31;
	exit(errs);
}

repquota(fsdev, fsfile, qffile)
	char *fsdev;
	char *fsfile;
	char *qffile;
{
	FILE *qf;
	u_short uid;
	struct dqblk dqbuf;
	struct stat statb;

	qf = fopen(qffile, "r");
	if (qf == NULL) {
		fprintf(stderr, "%s: Can't open %s : %s\n",
			myname, qffile, strerror(errno));
		return (1);
	}
	if (vflag || aflag)
		printf("%s (%s):\n", fsdev, fsfile);
	if (fstat(fileno(qf), &statb) < 0) {
		fprintf(stderr, "%s: Can't stat %s : %s\n",
			myname, qffile, strerror(errno));
		fclose(qf);
		return (1);
	}
	header();
	if ((uid = geteuid()) == 0) {
		for (uid = 0; ; uid++) {
			(void) fread(&dqbuf, sizeof(struct dqblk), 1, qf);
			if (feof(qf))
				break;
			if (!vflag &&
		    	dqbuf.dqb_curfiles == 0 && dqbuf.dqb_curblocks == 0)
				continue;
		
			prquota(uid, &dqbuf);
		}
	} else { 
		(void) fseek(qf, (long)dqoff(uid), 0);
		if (fread(&dqbuf, sizeof(struct dqblk), 1, qf) == 1) {
			if (!vflag ||
		    	dqbuf.dqb_curfiles != 0 || dqbuf.dqb_curblocks != 0)
				prquota(uid, &dqbuf);
		}
	}
	fclose(qf);
	return (0);
}

header()
{

	printf(
"                      Block limits                      File limits\n"
	);
	printf(
"User           used   soft   hard    timeleft    used   soft   hard    timeleft\n"
	);
}

void	prquota(uid, dqp)
	u_short uid;
	struct dqblk *dqp;
{
	struct timeval tv;
	register struct username *up;
	char ftimeleft[80], btimeleft[80];

	if (dqp->dqb_bsoftlimit == 0 && dqp->dqb_bhardlimit == 0 &&
	    dqp->dqb_fsoftlimit == 0 && dqp->dqb_fhardlimit == 0)
		return;
	(void) time(&(tv.tv_sec));
	tv.tv_usec = 0;
	up = lookup(uid);
	if (up)
		printf("%-10s", up->u_name);
	else
		printf("#%-9d", uid);
	if (dqp->dqb_bsoftlimit && dqp->dqb_curblocks>=dqp->dqb_bsoftlimit) {
		if (dqp->dqb_btimelimit == 0) {
			strcpy(btimeleft, "NOT STARTED");
		} else if (dqp->dqb_btimelimit > tv.tv_sec) {
			fmttime(btimeleft,
			    (long)(dqp->dqb_btimelimit - tv.tv_sec));
		} else {
			strcpy(btimeleft, "EXPIRED");
		}
	} else {
		btimeleft[0] = '\0';
	}
	if (dqp->dqb_fsoftlimit && dqp->dqb_curfiles>=dqp->dqb_fsoftlimit) {
		if (dqp->dqb_ftimelimit == 0) {
			strcpy(ftimeleft, "NOT STARTED");
		} else if (dqp->dqb_ftimelimit > tv.tv_sec) {
			fmttime(ftimeleft,
			    (long)(dqp->dqb_ftimelimit - tv.tv_sec));
		} else {
			strcpy(ftimeleft, "EXPIRED");
		}
	} else {
		ftimeleft[0] = '\0';
	}
	printf("%c%c%7d%7d%7d%12s %7d%7d%7d%12s\n",
		dqp->dqb_bsoftlimit && 
		    dqp->dqb_curblocks >= 
		    dqp->dqb_bsoftlimit ? '+' : '-',
		dqp->dqb_fsoftlimit &&
		    dqp->dqb_curfiles >=
		    dqp->dqb_fsoftlimit ? '+' : '-',
		dbtob(dqp->dqb_curblocks) / 1024,
		dbtob(dqp->dqb_bsoftlimit) / 1024,
		dbtob(dqp->dqb_bhardlimit) / 1024,
		btimeleft,
		dqp->dqb_curfiles,
		dqp->dqb_fsoftlimit,
		dqp->dqb_fhardlimit,
		ftimeleft
	);
}

void	fmttime(buf, time)
	char *buf;
	register long time;
{
	int i;
	static struct {
		int c_secs;		/* conversion units in secs */
		char * c_str;		/* unit string */
	} cunits [] = {
		{60*60*24*28, "months"},
		{60*60*24*7, "weeks"},
		{60*60*24, "days"},
		{60*60, "hours"},
		{60, "mins"},
		{1, "secs"}
	};

	if (time <= 0) {
		strcpy(buf, "EXPIRED");
		return;
	}
	for (i = 0; i < sizeof(cunits)/sizeof(cunits[0]); i++) {
		if (time >= cunits[i].c_secs)
			break;
	}
	sprintf(buf, "%.1f %s", (double)time/cunits[i].c_secs, cunits[i].c_str);
}

oneof(target, olistp, on)
	char *target;
	register char **olistp;
	register int on;
{
	char **listp = olistp;
	int n = on;

	while (n--) {
		if (*listp && strcmp(target, *listp) == 0) {
			*listp = (char *)0;
			return (1);
		}
		listp++;
	}
	return (0);
}

struct username *
lookup(uid)
	u_short uid;
{
	register struct username *up;

	for (up = uhead[uid % UHASH]; up != 0; up = up->u_next)
		if (up->u_uid == uid)
			return (up);
	return ((struct username *)0);
}

struct username *
adduid(uid)
	u_short uid;
{
	struct username *up, **uhp;

	up = lookup(uid);
	if (up != 0)
		return (up);
	up = (struct username *)calloc(1, sizeof(struct username));
	if (up == 0) {
		fprintf(stderr, 
			"%s: out of memory for username structures\n",
			myname);
		exit(31+1);
	}
	uhp = &uhead[uid % UHASH];
	up->u_next = *uhp;
	*uhp = up;
	up->u_uid = uid;
	if ((int)uid > (int)highuid)
		highuid = uid;
	return (up);
}

void	usage ()
{
	fprintf(stderr, "ufs usage:\n");
	fprintf(stderr, "repquota [-v] -a\n");
	fprintf(stderr, "repquota [-v] filesys ...\n");
	exit(31+1);
}

quotactl(cmd, special, uid, addr)
	int		cmd;
	char		*special;
	int		uid;
	caddr_t		addr;
{
	int		save_errno;
	int		fd;
	int		status;
	struct quotctl	quota;
	char		mountpoint[256];
	FILE		*fstab;
	struct mnttab	mntp;


	if ((special == NULL) && (cmd == Q_ALLSYNC)) {
		/*
		 * Find the mount point of the special device.   This is
		 * because the ioctl that implements the quotactl call has
		 * to go to a real file, and not to the block device.
		 */
		if ((fstab = fopen(MNTTAB, "r")) == NULL) {
			fprintf(stderr, "%s: Can't open %s : %s\n",
				myname, MNTTAB, strerror(errno));
			exit (31+1);
		}
		fd = (-1);
		while ((status = getmntent(fstab, &mntp)) == NULL) {
			if (strcmp(mntp.mnt_fstype, MNTTYPE_UFS) != 0 ||
				hasmntopt(&mntp, MNTOPT_RO))
				continue;
			(void) sprintf(mountpoint, "%s/%s", 
						mntp.mnt_mountp, QFNAME);
			if ((fd = open(mountpoint, O_RDONLY)) >= 0)
				break;
		}
		fclose(fstab);
		if (fd < 0) {
			/* No quotas file on any mounted file system. 
			   This is legal. */
			errno = ENOENT;
			return(-1);
		}
	}
	quota.op = cmd;
	quota.uid = uid;
	quota.addr = addr;
	status = ioctl (fd, Q_QUOTACTL, &quota);
	save_errno = errno;
	close (fd);
	errno = save_errno;
	return (status);
}

char *
hasmntopt(mnt, opt)
        register struct mnttab *mnt;
        register char *opt;
{
        char *f, *opts;
        static char *tmpopts;
	char	*mntopt();

        if (tmpopts == 0) {
                tmpopts = (char *)calloc(256, sizeof (char));
                if (tmpopts == 0)
                        return (0);
        }
        strcpy(tmpopts, mnt->mnt_mntopts);
        opts = tmpopts;
        f = mntopt(&opts);
        for (; *f; f = mntopt(&opts)) {
                if (strncmp(opt, f, strlen(opt)) == 0)
                        return (f - tmpopts + mnt->mnt_mntopts);
        }
        return (NULL);
}

char *
hasvfsopt(vfs, opt)
        register struct vfstab *vfs;
        register char *opt;
{
        char *f, *opts;
        static char *tmpopts;

        if (tmpopts == 0) {
                tmpopts = (char *)calloc(256, sizeof (char));
                if (tmpopts == 0)
                        return (0);
        }
        strcpy(tmpopts, vfs->vfs_mntopts);
        opts = tmpopts;
        f = mntopt(&opts);
        for (; *f; f = mntopt(&opts)) {
                if (strncmp(opt, f, strlen(opt)) == 0)
                        return (f - tmpopts + vfs->vfs_mntopts);
        }
        return (NULL);
}

static char *
mntopt(p)
        char **p;
{
        char *cp = *p;
        char *retstr;

        while (*cp && isspace(*cp))
                cp++;
        retstr = cp;
        while (*cp && *cp != ',')
                cp++;
        if (*cp) {
                *cp = '\0';
                cp++;
        }
        *p = cp;
        return (retstr);
}
