#===============================================================================
#
#	ident @(#) vacation.fp 11.1 97/10/30 
#
#******************************************************************************
#
#	Copyright (C) 1993-1997 The Santa Cruz Operation, Inc.
#		All Rights Reserved.
#
#	The information in this file is provided for the exclusive use of
#	the licensees of The Santa Cruz Operation, Inc.  Such users have the
#	right to use, modify, and incorporate this code into other products
#	for purposes authorized by the license agreement provided they include
#	this notice and the associated copyright notice with any such product.
#	The information in this file is provided "AS IS" without warranty.
#
#******************************************************************************
#
# This data file is used to register the vacation program with the CDE 
# desktop. It will be symlink'd at postinstall time to the /usr/dt/appconfig/
# type/C directory as a front panel type
#
#
# Modification History
#
# M000, 02-Jun-97, andrean
#	- Created.
#
#===============================================================================
CONTROL	Vacation
{
   TYPE			icon
   CONTAINER_NAME       Mail-Subpanel
   CONTAINER_TYPE       SUBPANEL
   POSITION_HINTS       1
   ICON			TEDsecSave
   LABEL		Vacation 
   PUSH_ACTION          vacation
}
