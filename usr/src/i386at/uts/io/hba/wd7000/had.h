#ifndef _IO_HBA_WD7000_HAD_H	/* wrapper symbol for kernel use */
#define _IO_HBA_WD7000_HAD_H	/* subject to change without notice */

#ident	"@(#)kern-pdi:io/hba/wd7000/had.h	1.11"
#ident	"$Header$"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef _KERNEL_HEADERS

#include <fs/buf.h>		/* REQUIRED */
#include <io/target/sdi/sdi_edt.h>	/* REQUIRED */
#include <util/types.h>		/* REQUIRED */

#elif defined(_KERNEL)

#include <sys/buf.h>		/* REQUIRED */
#include <sys/sdi_edt.h>	/* REQUIRED */
#include <sys/types.h>		/* REQUIRED */

#endif /* _KERNEL_HEADERS */

#define	NUM_OGMB	16	/* Number of out MB/request Qs	*/
#define	NUM_ICMB	16	/* Number of in MB/completion Qs*/
#define	NUM_RBS		1	/* Number of non-SCSI_RB MB/request Qs*/

#ifdef PDI_SVR42
#define	TRUE	1
#define	FALSE	0
#endif


typedef struct		/* Mail box used both as a request and completion Q */
{
	unsigned int status:8;	/* MBOX status			     */
	unsigned int pdp:24;	/* Physical addr of SCSI_RB or HA_RB */
} MBOX;


typedef struct			/* Data block send to HA for SCSI cmds	*/
{
	unsigned char	opcode;
	unsigned char	dev;		/* TC LU command destination	*/
	unsigned char	cdb[12];	/* SCSI cdb			*/	
	unsigned char	ssb;		/* SCSI status byte		*/
	unsigned char	hasb;		/* HA status byte		*/
	unsigned char	xfer_size[3];	/* Size of data transfer 	*/
	unsigned char	pdp[3];		/* Physical ptr to data area	*/
	unsigned char	link[3];	/* Link cmds. ptr not suuported */
	unsigned int	res1:7;		/* Reserved for later use	*/
	unsigned int	rw:1;		/* Request type Read=1 Write=0	*/
	unsigned char	res2[6];	/* Reserved for later use	*/
} SCSI_RB;


union srb_buf			/* used when SRB is on free list	*/
{
	union srb_buf	*next;
	SCSI_RB		srb;
};


typedef struct			/* Data block send to HA for HA cmds	*/
{
	unsigned char	opcode;
	unsigned char	bytes[14];	/* Parameter bytes	*/
	unsigned char	status;		/* Status of request	*/
} HA_RB;


union init_blk			/* Init cmd definition	*/
{
	struct
	{
		unsigned char	opcode;
		unsigned char	ha_id;		/* HA SCSI id		*/
		unsigned char	on_time;	/* AT Bus on time	*/
		unsigned char	off_time;	/* AT Bus off time	*/
		unsigned char	resv;		/* reserved byte	*/
		unsigned char	mbox_pdp[3];	/* Start addr of Mboxes */
		unsigned char	num_ic_mb;	/* number of in Mboxes	*/
		unsigned char	num_og_mb;	/* number of out Mboxes */
	} init;
	unsigned char	data[10];
};


struct wd_addr	/* SCSI board I/O addresses	*/
{
	unsigned int	ha_status;	/* HA status register		*/
	unsigned int	cmd_reg;	/* HA command register		*/
	unsigned int	int_status;	/* HA int. status register	*/
	unsigned int	int_ack;	/* HA int. acknowledge register	*/
	unsigned int	control_reg;	/* HA control register		*/
	unsigned int	ha_vect;	/* HA interrupt vector		*/
};

/* Macros to get to above structure parameters	*/

#define	HA_STATUS(c)	wd_ad[c].ha_status
#define	CMD_REG(c)	wd_ad[c].cmd_reg
#define	INT_STATUS(c)	wd_ad[c].int_status
#define	INT_ACK(c)	wd_ad[c].int_ack
#define	CONTROL_REG(c)	wd_ad[c].control_reg


struct wd_ctrl 	/* SCSI control structure one for every ha card	*/
{
	unsigned short	state;			/* SCSI controller state    */
	unsigned short	max_jobs;		/* Maximum number of jobs   */
	unsigned short	jobs;			/* Total number of jobs     */
	unsigned short	maj;			/* External Major number    */
	unsigned short	tc_cnt;			/* Target controller count  */
	unsigned short	ha_id;			/* Host Adapter ID          */
	int		haver;			/* Host Adaptor version	    */
	int		resid;			/* timeout id for reset	    */
	unsigned char	waiting_jobs;
	unsigned char	active_jobs;
	unsigned char	next_tc;
	unsigned char	next_lu;
	buf_t		*ctr_bp;
	union srb_buf	*srb_list;		/* SRB free list pointer    */
	struct tc_ctrl 
	{
		unsigned short	lu_cnt;		/* Logical unit count       */
		unsigned short	waiting_jobs;
		struct lu_ctrl 
		{
			void	(*dv_func) ();	/* Target drvs event func */
			long	dv_param;	/* Target drvs parameter  */
			union 
			{
				unsigned long	all;
				struct 
				{
					unsigned char	suspended;
					unsigned char	normal;
					unsigned char	immediate;
					unsigned char	function;
				} t;
			} jobs;
			struct sb2	*jp;	/* Jobs list pointer        */
			struct sb2	*fp;	/* Function list pointer   */
			struct ident	*idp;	/* pointer to inqiry data */
			bcb_t		*bcbp;	/* Breakup control blk ptr*/
		} lu[MAX_LUS];
	} tc[MAX_TCS];
	MBOX	rq[NUM_OGMB];
	MBOX	cq[NUM_ICMB];
	daddr_t	pa[NUM_OGMB+NUM_RBS];	/* physical addresses of out/MB srb's */
	daddr_t	va[NUM_OGMB+NUM_RBS];	/* virtual addresses of out/MB srb's */
	union srb_buf srb[NUM_OGMB];
};





/* 	The HA Controller State Flags	*/

#define	C_OPERATIONAL 	0x0001	/* Able to do normal jobs (sysgen'ed) */
#define	C_RAM   	0x0002	/* Board is pumped                    */
#define C_FAULT		0x0004
#define C_TIMEOUT	0x0008
#define C_SYSGEN	0x0010	/* SYSGEN in progress                 */
#define C_DLM		0x0020	/* Down Load Memory in progress       */
#define C_FCF		0x0040	/* Force Call to Function in progress */
#define C_DSD		0x0080	/* Determine SubDevice in progress    */
#define	C_BUSY	   	0x0100	/* Normal job in progress             */
#define C_INIT_TIME	0x0200	/* Init time, (no sleep allowed)      */

#define C_REDT_REQ	0x1000	/* Request for function type jobs */

#define C_EXP_REQ	(C_PUMP_REQ|C_REDT_REQ)

#define C_RS_START	0x2000	/* Reset occcured no jobs are allowed */
#define C_RS_IMM	0x4000	/* Reset handling imm. jobs allowed   */
#define C_RS_BLK	0x6000  /* All jobs allowed if NOT true	*/
#define C_SANITY	0x8000	/* Sanity flag for firmware */
#define C_CK_STATE	(C_EXP_REQ|C_SANITY)


/*	LU structure suspended flags	*/

#define	SUSPEND		0x01	/* LU Q suspended on error check	*/
#define	PUMP_SUSPEND	0x02	/* LU Q suspended for pump or reset	*/
#define	PT_SUSPEND	0x04	/* LU Q suspended for pass-tru job	*/
#define	PT_ACTIVE	0x08	/* LU Q has pass-tru job queued		*/

#define	MAX_CMDSZ	12
#define	MAX_ROM_JOBS	1
#define	MAX_RAM_JOBS	2

#define	PASS		1
#define	FAIL		0
#define	SUCCESS		0

#define	LU(c,t,l)	wd_ctrl[c].tc[t].lu[l]
#define	TC(c,t)		wd_ctrl[c].tc[t]
#define	SC(c)		wd_ctrl[c]

struct sb2 				/* SCSI driver SCB structure	*/
{
	struct sb	*sbp;		/* target driver SCB definition	   */
	struct sb2	*sp_forw;	/* lu queue forward pointer	   */
	struct sb2	*sp_back;	/* lu queue backward poiter	   */
	long		sp_sent;	/* TRUE if job has been sent to FW */
	long		sp_time;	/* job time out variable	   */
	long		sp_pdb;		/* Physical data pointer           */
	long		sp_read;	/* set on reads from HA board	   */
	long		sp_isz;
	long		sp_vip;
};

union sb_item 		/* SCB structure as allocated in master file	*/
{
	union sb_item	*next;
	struct sb2	sb2;
};

#define N_SCBS		37	/* number of SCBs allocated per LU	*/

struct cdb_item		/* HA driver CDB as allocated in master file	*/
{
	buf_t		*cdb_bp;
	char		cdb[MAX_CMDSZ];
	struct cdb_item		*next;
};


/*============================================================================*/
/* Bit Structure of the External Device Number
/*      
/*           MAJOR           MINOR      
/*      -------------------------------
/*      |  mmmmmmmm  |  ccc  ttt ll   |
/*      -------------------------------
/*      
/*         m = major number assigned by idinstall
/*	   c = Host Adapter Card number (0-7)
/*         t = target controller ID (0-7)
/*         l = logical unit number (0-3)
/*
/*============================================================================*/


#define	SC_CONTROL(x)	wd_ctoi[((x>>5) & 0x7)]/* C# from minor number */
#define	SC_TARGET(x)	((x >> 2) & 0x7)	/* TC# from minor number */

#define	SC_ILLEGAL(c,t)	(c >= wd_hacnt)
				

#define	SDI_TARGET(x)	(x & 0x07)		/* TC# from SCB	*/

#define	MAKE_DEV(x,y)		((x << 8) | y)
#define SUBDEV(t,l)		((t << 5) | l)

/* This define is used to extract the host adapter version */
#define RD_HA_VER	0X0089		
		
/* Time-out values */

#define	T_RESET		10			/* Reset delay time  -- 10ms  */
#define	ONE_MSEC	1			/* One 100th of a second      */
#define	ONE_SECOND	100
#define	ONE_MINUTE	(60 * ONE_SECOND)	/* Timout delay time -- 60sec */
#define	ONE_HOUR	(60 * ONE_MINUTE)
#define	ONE_DAY		(24 * ONE_HOUR)
#define	ONE_WEEK	(7 * ONE_DAY)		/* Used as the max timeout    */
/* used for timeout when polling for interrupts */

#define ONE_MILL_U_SEC		1000000		/* 1 second */		
#define THREE_MILL_U_SEC	3000000		/* 3 seconds */		
#define TEN_MILL_U_SEC		10000000	/* 10 seconds */
#define	MAX_JOB_SIZE		(128 * 1024)
#define WD_24BMASK		0xFFFFFF

/*
 * This value was arbitrarily chosen. We should probably
 * base it on  the number of host adapters in the system.
 */
#define	NUM_DMA_LISTS		16


typedef	struct
{
	char	count[3];
	char	physaddr[3];
} DMA_PAIR;


typedef	struct	dma_list
{
	DMA_PAIR *dma_pair;
	struct	dma_list *dma_next;
} DMA_LIST;

#if defined(__cplusplus)
	}
#endif

#endif /* _IO_HBA_WD7000_HAD_H */
