/*		copyright	"%c%" 	*/

#ident	"@(#)hslconv.c	1.2"
#ident	"$Header$"
/* LINTLIBRARY */

# include	<string.h>

static char		_lp_hextable[17] = "0123456789ABCDEF";

#if	defined(__STDC__)
char * ltos ( char * s, unsigned long l)
#else
char * ltos (s, l)
    char	*s;
    unsigned long	l;
#endif
{
    int	i = 7;

    while (i >=	0)
    {
	s[i--] = _lp_hextable[l % 16];
	l /= 16;
    }
    s += 8;
    return(s);
}

#if	defined(__STDC__)
char * htos ( char * s, unsigned short h)
#else
char * htos (s, h)
    char	*s;
    unsigned short	h;
#endif
{
    int	i = 3;

    while (i >= 0)
    {
	s[i--] = _lp_hextable[(long)h % 16];
	h = (long) h / 16;
    }
    s += 4;
    return(s);
}

#if	defined(__STDC__)
unsigned long stol ( char * s )
#else
unsigned long stol (s)
    char	*s;
#endif
{
    int			i = 0;
    unsigned long	l = 0;

    while (i < 8)
    {
	l <<= 4;
	l += strchr(_lp_hextable, s[i++]) - _lp_hextable;
    }
    return(l);
}

#if	defined(__STDC__)
unsigned short stoh ( char * s )
#else
unsigned short stoh (s)
    char	*s;
#endif
{
    int			i = 0;
    unsigned short	h = 0;

    while (i < 4)
    {
	h <<= 4;
	h += strchr(_lp_hextable, s[i++]) - _lp_hextable;
    }
    return(h);
}
