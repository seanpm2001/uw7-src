/*		copyright	"%c%" 	*/

#ident	"@(#)pkdefs.c	1.2"
#ident "$Header$"

#include "uucp.h"

#define USER 1
#include "pk.h"
char next[8]	={ 1,2,3,4,5,6,7,0};	/* packet sequence numbers */
unsigned char mask[8]	={ 1,2,4,010,020,040,0100,0200 };
int	pkactive;
int pkdebug;
int pksizes[] = {
	1, 32, 64, 128, 256, 512, 1024, 2048, 4096, 1
};
struct pack *pklines[NPLINES];
