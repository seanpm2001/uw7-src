#ident	"@(#)kern-i386at:util/symbols_p.c	1.39.9.1"
#ident	"$Header$"

/*
 * Generate symbols for use by assembly language files in kernel.
 *
 * This file is compiled using "-S"; "symbols.awk" walks over the assembly
 * file and extracts the symbols.
 */

#include <mem/vm_mdep.h>
#include <mem/vmparam.h>
#include <proc/disp_p.h>
#include <svc/bootinfo.h>
#include <svc/psm.h>
#ifdef MERGE386
#include <svc/mki.h>
#endif
#include <svc/intr.h>
#include <svc/pic.h>
#include <svc/trap.h>
#include <util/cglocal.h>
#include <util/ipl.h>
#include <util/param.h>
#include <util/plocal.h>

#define	offsetof(x, y)	((int)&((x *)0)->y)
#define	OFFSET(s, st, m) \
	size_t __SYMBOL___A_##s = offsetof(st, m)

#define	DEFINE(s, e) \
	size_t __SYMBOL___A_##s = (size_t)(e)

/*
 * Misc kernel virtual addresses and constants.
 */

DEFINE(KVPAGE0, KVPAGE0);

/*
 * Interrupt Priority Levels
 */

DEFINE(PLTTY, PLTTY);
DEFINE(INVPL, INVPL);
DEFINE(PLHI, PLHI);
DEFINE(PLBASE, PLBASE);
DEFINE(PLTIMEOUT, PLTIMEOUT);
DEFINE(PLDISK, PLDISK);
DEFINE(PLSTR, PLSTR);
DEFINE(PLMAX, PLMAX);
DEFINE(PLXCALL, PLXCALL);

/*
 * Processor local fields ("l.").
 */

OFFSET(L_IPL, struct plocal, prmpt_state.s_prmpt_state.s_ipl);
OFFSET(L_NOPRMPT, struct plocal, prmpt_state.s_prmpt_state.s_noprmpt);
OFFSET(L_PICIPL, struct plocal, picipl);
OFFSET(L_FPU_EXTERNAL, struct plocal, fpu_external);

/*
 * MSOP request numbers for calling PSM routines.
 */

DEFINE(MSOP_INIT_CPU, MSOP_INIT_CPU*4);
DEFINE(MSOP_INTR_ATTACH, MSOP_INTR_ATTACH*4);
DEFINE(MSOP_INTR_DETACH, MSOP_INTR_DETACH*4);
DEFINE(MSOP_INTR_MASK, MSOP_INTR_MASK*4);
DEFINE(MSOP_INTR_UNMASK, MSOP_INTR_UNMASK*4);
DEFINE(MSOP_INTR_COMPLETE, MSOP_INTR_COMPLETE*4);
DEFINE(MSOP_INTR_TASKPRI, MSOP_INTR_TASKPRI*4);
DEFINE(MSOP_XPOST, MSOP_XPOST*4);
DEFINE(MSOP_TICK_2, MSOP_TICK_2*4);
DEFINE(MSOP_TIME_GET, MSOP_TIME_GET*4);
DEFINE(MSOP_TIME_ADD, MSOP_TIME_ADD*4);
DEFINE(MSOP_TIME_SUB, MSOP_TIME_SUB*4);
DEFINE(MSOP_TIME_CVT, MSOP_TIME_CVT*4);
DEFINE(MSOP_TIME_SPIN, MSOP_TIME_SPIN*4);
DEFINE(MSOP_RTODC, MSOP_RTODC*4);
DEFINE(MSOP_WTODC, MSOP_WTODC*4);
DEFINE(MSOP_IDLE_SELF, MSOP_IDLE_SELF*4);
DEFINE(MSOP_IDLE_EXIT, MSOP_IDLE_EXIT*4);
DEFINE(MSOP_SHUTDOWN, MSOP_SHUTDOWN*4);
DEFINE(MSOP_OFFLINE_PREP, MSOP_OFFLINE_PREP*4);
DEFINE(MSOP_OFFLINE_SELF, MSOP_OFFLINE_SELF*4);
DEFINE(MSOP_START_CPU, MSOP_START_CPU*4);
DEFINE(MSOP_SHOW_STATE, MSOP_SHOW_STATE*4);
DEFINE(MSOP_FARCOPY, MSOP_FARCOPY*4);
DEFINE(MSOP_IO_READ_8, MSOP_IO_READ_8*4);
DEFINE(MSOP_IO_READ_16, MSOP_IO_READ_16*4);
DEFINE(MSOP_IO_READ_32, MSOP_IO_READ_32*4);
DEFINE(MSOP_IO_WRITE_8, MSOP_IO_WRITE_8*4);
DEFINE(MSOP_IO_WRITE_16, MSOP_IO_WRITE_16*4);
DEFINE(MSOP_IO_WRITE_32, MSOP_IO_WRITE_32*4);
DEFINE(MSOP_IO_REP_READ_8, MSOP_IO_REP_READ_8*4);
DEFINE(MSOP_IO_REP_READ_16, MSOP_IO_REP_READ_16*4);
DEFINE(MSOP_IO_REP_READ_32, MSOP_IO_REP_READ_32*4);
DEFINE(MSOP_IO_REP_WRITE_8, MSOP_IO_REP_WRITE_8*4);
DEFINE(MSOP_IO_REP_WRITE_16, MSOP_IO_REP_WRITE_16*4);
DEFINE(MSOP_IO_REP_WRITE_32, MSOP_IO_REP_WRITE_32*4);

#ifdef MERGE386
/*
 * Merge
 */
DEFINE(SWITCH_AWAY, SWITCH_AWAY*4);
DEFINE(SWITCH_TO, SWITCH_TO*4);
DEFINE(THREAD_EXIT, THREAD_EXIT*4);
DEFINE(RET_USER, RET_USER*4);
DEFINE(SIGNAL, SIGNAL*4);
DEFINE(QUERY, QUERY*4);
DEFINE(UW_PORT_ALLOC, UW_PORT_ALLOC*4);
DEFINE(UW_PORT_FREE, UW_PORT_FREE*4);
DEFINE(UW_COM_PPI_IOCTL, UW_COM_PPI_IOCTL*4);
DEFINE(UW_KD_PPI_IOCTL, UW_KD_PPI_IOCTL*4);
#endif

/*
 * Misc defines.
 */

DEFINE(EVT_SOFTINTMASK, EVT_SOFTINTMASK);
