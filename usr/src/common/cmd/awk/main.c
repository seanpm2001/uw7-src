/*	copyright	"%c%"	*/

#ident	"@(#)awk:main.c	2.15.5.2"
#ident  "$Header$"

#define DEBUG
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <pfmt.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <sys/euc.h>
#include <getwidth.h>
eucwidth_t WW;

#define	CMDCLASS	"UX:"	/* Command classification */

/* you can safely delete this header file reference */
/* if you also delete the setlocale call below */

#ifdef __STDC__
#include <locale.h>
#else
#define setlocale(a,b)
#endif

#include "awk.h"
#include "y.tab.h"

char	*version;

int	dbg	= 0;
uchar	*cmdname;	/* gets argv[0] for error messages */
extern	FILE	*yyin;	/* lex input file */
uchar	*lexprog;	/* points to program argument if it exists */
uchar	**start_delayed; /* first name=val argument delayed for BEGIN code */
uchar	**after_delayed; /* first argument after the delayed name=val's */
extern	int errorflag;	/* non-zero if any syntax errors; set by yyerror */
int	compile_time = 2;	/* for error printing: */
				/* 2 = cmdline, 1 = compile, 0 = running */

#define MAXPFILE 100
uchar	*pfile[MAXPFILE]; /* program filenames from -f's */
int	npfile = 0;	/* number of filenames */
int	curpfile = 0;	/* current filename */

extern const char badopen[];

main(argc, argv, envp)
	int argc;
	uchar *argv[], *envp[];
{
	uchar *fs = NULL;
	char label[MAXLABEL+1];	/* Space for the catalogue label */
	extern void fpecatch();

	(void)setlocale(LC_ALL, "");
	{getwidth(&WW); WW._eucw2++; WW._eucw3++;}
	cmdname = ((cmdname = (uchar*) strrchr ((char*) argv[0], '/')) ?
		++cmdname : argv[0]);
	(void)strcpy(label, CMDCLASS);
	(void)strncat(label, (char*) cmdname, (MAXLABEL - sizeof(CMDCLASS) - 1));
	(void)setcat("uxawk");
	(void)setlabel(label);
	version = (char*) gettxt(":31", "version Oct 11, 1989");
 	if (argc == 1) {
		pfmt(stderr, MM_ERROR, ":32:Incorrect usage\n");
		pfmt(stderr, MM_ACTION,
			":107:Usage: %s [-Ffieldsep] [-v var=value] [-f programfile | 'program'] [files]\n",
			cmdname);
		exit(1);
	}
	signal(SIGFPE, fpecatch);
	yyin = NULL;
	syminit();
	while (argc > 1 && argv[1][0] == '-' && argv[1][1] != '\0') {
		if (strcmp((char*) argv[1], "--") == 0) {	/* explicit end of args */
			argc--;
			argv++;
			break;
		}
		switch (argv[1][1]) {
		case 'f':	/* next argument is program filename */
			if (npfile >= MAXPFILE)
				error(MM_ERROR, ":106:Too many program filenames");
			if (argv[1][2] != '\0') { /* arg is -fname */
				pfile[npfile++] = &argv[1][2];
			} else {
				argc--;
				argv++;
				if (argc <= 1)
					error(MM_ERROR, ":34:No program filename");
				pfile[npfile++] = argv[1];
			}
			break;
		case 'F':	/* set field separator */
			if (argv[1][2] != 0) {	/* arg is -Fsomething */
				if (argv[1][2] == 't' && argv[1][3] == 0)	/* wart: t=>\t */
					fs = (uchar *) "\t";
				else if (argv[1][2] != 0)
					fs = &argv[1][2];
			} else {		/* arg is -F something */
				argc--; argv++;
				if (argc > 1 && argv[1][0] == 't' && argv[1][1] == 0)	/* wart: t=>\t */
					fs = (uchar *) "\t";
				else if (argc > 1 && argv[1][0] != 0)
					fs = &argv[1][0];
			}
			if (fs == NULL || *fs == '\0')
				error(MM_WARNING, ":35:Field separator FS is empty");
			break;
		case 'v':	/* -v a=1 to be done NOW.  one -v for each */
			if (argv[1][2] != '\0') { /* arg is -va=1 */
				if (!isclvar(&argv[1][2]))
					error(MM_ERROR, ":105:malformed -v assignment");
				setclvar(&argv[1][2]);
			} else if (--argc > 1 && isclvar((++argv)[1])) {
				setclvar(argv[1]);
			} else {
				error(MM_ERROR, ":105:malformed -v assignment");
			}
			break;
		case 'd':
			dbg = atoi((char *)&argv[1][2]);
			if (dbg == 0)
				dbg = 1;
			pfmt(stdout, (MM_INFO | MM_NOGET), "%s %s\n",
				cmdname, version);
			break;
		default:
			pfmt(stderr, MM_WARNING,
				":36:Unknown option %s ignored\n", argv[1]);
			break;
		}
		argc--;
		argv++;
	}
	/* argv[1] is now the first argument */
	if (npfile == 0) {	/* no -f; first argument is program */
		if (argc <= 1)
			error(MM_ERROR, ":37:No program given");
		dprintf( ("program = |%s|\n", argv[1]) );
		lexprog = argv[1];
		argc--;
		argv++;
	}
	/* hold leading name=val arguments until just after BEGIN */
	if (argc > 1 && isclvar(argv[1])) {
		start_delayed = &argv[0];
		do {
			argv[0] = argv[1];
			argv++;
		} while (--argc > 1 && isclvar(argv[1]));
		after_delayed = &argv[0];
	}
	compile_time = 1;
	argv[0] = cmdname;	/* put prog name at front of arglist */
	dprintf( ("argc=%d, argv[0]=%s\n", argc, argv[0]) );
	arginit(argc, argv);
	envinit(envp);
	yyparse();
	if (fs)
		*FS = tostring(qstring(fs, '\0'));
	dprintf( ("errorflag=%d\n", errorflag) );
	if (errorflag == 0) {
		compile_time = 0;
		run(winner);
	} else
		bracecheck();
	exit(errorflag);
}

pgetc()		/* get program character */
{
	int c;

	for (;;) {
		if (yyin == NULL) {
			if (curpfile >= npfile)
				return EOF;
			if (!strcmp((char *)pfile[curpfile], "-"))
				yyin = stdin;
			else if ((yyin = fopen((char *) pfile[curpfile], "r")) == NULL)
				error(MM_ERROR, badopen,
					pfile[curpfile], strerror(errno));
		}
		if ((c = getc(yyin)) != EOF)
			return c;
		yyin = NULL;
		curpfile++;
	}
}
