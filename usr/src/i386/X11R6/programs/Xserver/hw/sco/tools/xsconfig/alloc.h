/*
 *	@(#) alloc.h 11.1 97/10/22
 *
 *	Copyright (C) The Santa Cruz Operation, 1991.
 *	This Module contains Proprietary Information of
 *	The Santa Cruz Operation, and should be treated as Confidential.
 */
/*
 *	SCO MODIFICATION HISTORY
 *
 */

   
/*
   @(#) alloc.h 11.1 97/10/22
*/


/*
   (c) Copyright 1989 by Locus Computing Corporation.  ALL RIGHTS RESERVED.

   This material contains valuable proprietary products and trade secrets
   of Locus Computing Corporation, embodying substantial creative efforts
   and confidential information, ideas and expressions.  No part of this
   material may be used, reproduced or transmitted in any form or by any
   means, electronic, mechanical, or otherwise, including photocopying or
   recording, or in connection with any information storage or retrieval
   system without permission in writing from Locus Computing Corporation.
*/

/*
   alloc.h - Define memory allocation functions
*/

extern  char *AllocMem();
extern  char *ReallocMem();
extern  void FreeMem();
extern  char *StrDup();

/* <<< Function Prototypes >>> */

#if !defined(NOPROTO)

extern  void *AllocMem(unsigned int );
extern  void *ReallocMem(void *,unsigned int );
extern  void FreeMem(void *);
extern  char *StrDup(char *);

#endif
