/*		copyright	"%c%" 	*/

#ident	"@(#)eac:i386/eaccmd/dosutil/dosformat/MS-DOS.c	1.1.1.2"
#ident  "$Header$"

/*
		MS-DOS.c

	Define all required globals, and #defines
*/

#include	"MS-DOS.h"

int	assignments_loaded = 0;
struct	table_struct	device_table[MAX_DEVICES];
long	dir_sector = -1;
char	drive;
struct	hardware_struct	hardware_table[MAX_DEVICES];
long	last_sector_read = -1;
char	device_pathname[MAX_FILENAME];
char	filename[MAX_FILENAME];
unsigned	char	sector_buffer[MAX_SECTOR_SIZE];
int	we_are_dosdir = 0;
int	we_are_dosinfo = 0;
int	we_are_dosrm = 0;
int	we_are_dosrmdir = 0;
