/*		copyright	"%c%" 	*/

#ident	"@(#)_putmessage.c	1.2"
#ident	"$Header$"
/* LINTLIBRARY */

# include	<stdarg.h>
# include	<string.h>
# include	<errno.h>

# include	"msgs.h"

extern char	*_lp_msg_fmts[];
extern int	errno;

/* VARARGS */
#if	defined(__STDC__)
int _putmessage ( char * buf, short type, va_list arg )
#else
int _putmessage (buf, type, arg)
    char	*buf;
    short	type;
    va_list	arg;
#endif
{
    char	*fmt;
    char	*t_string;
    int		size = 0;
    long	t_long;
    short	t_short;

    if (type < 0 || type > LAST_MESSAGE)
    {
	errno =	EBADMSG;
	return(-1);
    }

    if (buf)
	(void) htos(buf + MESG_TYPE, type);

    size = MESG_LEN;

    fmt	= _lp_msg_fmts[type];

    while (*fmt	!= '\0')
	switch(*fmt++)
	{
	    case 'H':
		t_short = (short) va_arg(arg, int);
		if (buf)
		     (void) htos(buf + size, t_short);

		size +=	4;
		break;

	    case 'L':
		t_long = (long) va_arg(arg, int);
		if (buf)
		     (void) ltos(buf + size, t_long);

		size +=	8;
		break;

	    case 'S':
		t_string = (char *) va_arg(arg,	char *);
		t_short	= (t_string? strlen(t_string) : 0) + 1;

		if (buf)
		    (void) htos(buf + size, t_short);

		size +=	4;

		if (buf)
			if (t_string)
			    (void) memcpy(buf +	size, t_string,	t_short);
			else
			    (buf + size)[0] = 0;

		size +=	t_short;
		break;

	    case 'D':
		t_short	= (short) va_arg(arg, int) + 1;
		t_string = (char *) va_arg(arg,	char *);

		if (buf)
		    (void) htos(buf + size, t_short);

		size +=	4;

		if (buf)
		    if (t_string)
		    {
			(void) memcpy(buf + size, t_string, t_short);
			buf[size + t_short - 1] = '\0';
		    }
		    else
			*(buf + size) = '\0';
		
		size +=	t_short;
		break;
	}


    if (buf)
	*(buf + size) = '\0';

    size++;		/* Add a null, just on general principle */

    if (buf)
	(void) htos(buf + MESG_SIZE, size);

    return(size);
}
