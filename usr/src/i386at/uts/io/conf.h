#ifndef _IO_CONF_H	/* wrapper symbol for kernel use */
#define _IO_CONF_H	/* subject to change without notice */

#ident	"@(#)kern-i386at:io/conf.h	1.29.9.1"
#ident	"$Header$"

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Configuration specific defines and structures.
 */

#ifdef _KERNEL_HEADERS

#include <util/types.h> /* REQUIRED */

#elif defined(_KERNEL) || defined(_KMEMUSER)

#include <sys/types.h> /* REQUIRED */

#endif /* _KERNEL_HEADERS */

/*
 * Device flags.
 *
 * Bit 0 to bit 15 are reserved for kernel.
 * Bit 16 to bit 31 are reserved for different machines.
 */
#define D_NEW		0x00	/* new-style driver */
#define	D_OLD		0x01	/* old-style driver; no longer supported */
#define D_DMA		0x02	/* driver does DMA */
#define D_BLKOFF	0x400	/* driver understands b_blkoff; */
				/* i.e. is byte addressable */
/*
 * Temporary flag to allow creating large file system on these
 * devices before they are fully converted to DDI 8. This flag
 * and all its references should be removed when DDI 8 is complete.
 */
#define D_LFS		0x8000

#define	D_STR		0x0800		/* NOT USED by DDI 8+  drivers */
#define	D_MOD		0x1000
#define	D_PSEUDO	0x2000
#define	D_RANDOM	0x4000
#define	D_HOT		0x10000

/*
 * Added for UFS.
 */
#define D_SEEKNEG       0x04    /* negative seek offsets are OK */
#define D_TAPE          0x08    /* magtape device (no bdwrite when cooked) */
/*
 * Added for ease of transition from pre-DDI drivers.  By not setting this
 * flag, the driver only sees pre-SVR4-style buffers.
 */
#define D_NOBRKUP	0x10	/* no breakup needed for new drivers */
/*
 * Security additions, for drivers requiring special MAC access policies.
 */
#define D_INITPUB	0x20	/* device is public in system setting */
#define D_NOSPECMACDATA	0x40	/* no MAC access check for data transfer */ 
				/* and no inode access time change */ 
#define D_RDWEQ		0x80	/* destructive reads, read equal, write eq */
#define SECMASK		(D_INITPUB|D_NOSPECMACDATA|D_RDWEQ)
				/* mask of all security flags */
/*
 * d_auxflag values
 */
#define DAF_REQDMA	0x1	/* driver is ddi version 5 or less */
#define DAF_PHYSREQ	0x2	/* driver is ddi version 4 or less */
#define DAF_PRE8	0x4	/* driver is ddi version 7 or less */
#define DAF_STATIC	0x8	/* driver is statically linked */
#define DAF_STR		0x10	/* streams driver; used only for registered,
				 * unloaded driver, for benefit of /dev/sad */

/*
 * MP-related flags.
 */
#define D_MP		0x100	/* driver/module is MP */
#define D_UPF		0x200	/* mux is UP-friendly */
/*
 * End of devflag definitions.
 */

#define ROOTFS_NAMESZ	7	/* Maximum length of root fstype name */

#define	FMNAMESZ	8	/* max length of streams module name */

#if defined _KERNEL || defined _KMEMUSER

/*
 * Declaration of block device switch. Each entry (row) is
 * the only link between the main unix code and the driver.
 * The initialization of the device switches is in the file conf.c.
 */
struct bdevsw {
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_strategy)();
	int	(*d_size)();
	int	(*d_devinfo)();
	char	*d_name;
	struct iobuf	*d_tab;
	int	*d_flag;
	int	d_cpu;
	struct module	*d_modp;
};

/*
 * Character device switch.
 *
 * WARNING:  struct cdevsw d_cpu and struct __cfg_info bind_cpu *MUST* be
 * at the same offset.
 */
struct cdevsw {
	int	(*d_open)();
	int	(*d_close)();
	int	(*d_read)();
	int	(*d_write)();
	int	(*d_ioctl)();
	int	(*d_mmap)();
	int	(*d_segmap)();
	int	(*d_poll)();
	int	(*d_msgio)();
	int	(*d_devinfo)();
	int	d_auxflag;
	struct streamtab *d_str;
	char	*d_name;
	int	*d_flag;
	int	d_cpu;
	struct module *d_modp;
};

/*
 * STREAMS module information
 */

struct fmodsw {
	char	f_name[FMNAMESZ+1];
	struct streamtab *f_str;
	int	*f_flag;		/* same as device flag */
	struct module	*f_modp;
};

/*
 * Console-capable device table
 */

struct constab {
	char	*cn_name;
	struct conssw *cn_consswp;
	int	cn_cpu;
};

/*
 * Statically-linked (non-autoconfig) modules which need interrupts
 */
struct intr_list {
	char	*il_name;	/* module name */
	int	*il_devflagp;	/* pointer to module's devflag */
	void	(*il_handler)();
};

/*
 * Configuration data for loadable modules.  Generated by the configuration
 * tools and pointed to by the module wrapper.
 */
struct mod_conf_data {
	uint_t	mcd_version;	/* version number of this structure */
	time_t	mcd_unload_delay;
};

#define MCD_VERSION	1	/* current version of mod_conf_data */

/*
 * Interface definitions.  Used by DLM to verify interface conformance
 * and to do symbol remapping for compatibility interfaces.
 */

struct intfc_sym {
	char	*ifs_name;
	char	*ifs_newname;
};

struct interface {
	char	*if_name;
	char	*if_version;
	struct intfc_sym *if_symbols;
	char	*if_depends;
	struct interface *if_next_intfc;
	struct interface *if_next_ver;
	struct interface *if_rep_intfc;
	int	if_order;
};

/* static entrytype 1 module struct */
struct _DLKM {
	const char *_DLKM;
	int (*_load) (void);
	int (*_unload) (void);
};

#define SYM_DROPPED	((char *)-1)

#ifndef _DI_PARM_T
#define _DI_PARM_T
typedef int di_parm_t;
#endif

#endif /* _KERNEL || _KMEMUSER */

#if defined _KERNEL

extern struct bdevsw bdevsw[];
extern struct cdevsw cdevsw[];
extern struct fmodsw fmodsw[];
extern struct constab constab[];
extern struct intr_list static_intr_list[];
extern struct interface interfaces[];
extern struct _DLKM static_DLKM[];

extern int	bdevcnt;
extern int	cdevcnt;
extern int	fmodcnt;
extern int	conscnt;

extern int bdevswsz;
extern int cdevswsz;
extern int fmodswsz;

#if !defined(DI_MEDIA)
#if defined(_DDI_C) || _DDI < 8
#define DI_BCBP		0
#else
#undef DI_BCBP
#endif
#define DI_MEDIA	1
#endif

#endif /* _KERNEL */

#if defined(__cplusplus)
        }
#endif
#endif /* _IO_CONF_H */
