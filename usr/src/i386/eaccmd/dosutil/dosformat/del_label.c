/*		copyright	"%c%" 	*/

#ident	"@(#)eac:i386/eaccmd/dosutil/dosformat/del_label.c	1.1.1.2"
#ident  "$Header$"

/*
		del_label(handle)

	Deletes the volume label for the specified handle.
	Returns -1 on failure.
*/

#include	"MS-DOS.h"

#include	<stdio.h>

del_label(handle)
int	handle;
{
	long	disp;
	int	index;
	long	sector;

	if ((index = lookup_device(handle)) == -1) {
		(void) fprintf(stderr, "del_label(): Error - Handle %d not found in device table\n", handle);
		return(-1);
	}

	/*
		Read the root directory looking for the label
	*/
	for (sector = TABLE.root_base_sector; sector < TABLE.root_base_sector + TABLE.sectors_in_root; sector++) {
		/*
			Read this sector
		*/
		if (read_sector(handle, sector) == -1)
			return(-1);

		/*
			Look at each entry in this sector
		*/
		for (disp = 0 ; disp < TABLE.bytes_per_sector; disp += BYTES_PER_DIR) {
			if (sector_buffer[disp + FILE_ATTRIBUTE] == LABEL) {
				dir_sector = last_sector_read;

				if (rm_file(handle, disp) == -1) {
					(void) fprintf(stderr, "del_label(): Error - Failed to remove label located\n\tin sector %ld at displacement %ld\n", dir_sector, disp);
					return(-1);
				}

				return(0);
			}
		}
	}

	return(0);
}
