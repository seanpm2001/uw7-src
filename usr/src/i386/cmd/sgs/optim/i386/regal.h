#ident	"@(#)optim:i386/regal.h	1.4.1.8"
#ifndef REGAL_H
#define REGAL_H

#include "optim.h"

/* variable storage classes .. see storclass.h */
#define SCLNULL 0
#define AUTO 1
#define REGISTER 4
#define PARAM 9
#define RPARAM 17
/* make up a new storage class just for variables that "look
   like globals in the .s */
#define GLOBAL 200
#define NO_PARAM  400

#define is_intrinsic(p) ((p)->usage == 1)

enum valid_types {
	ONLY_BYTE,
	ALL_TYPES,
	NO_TYPES
} ;


struct regal 
{
  char	*rglname;	/* name of quantity - auto, param or global */
  int 	rglestim;		/* estimator of cycle payoff */
  int 	ebp_refs;		/* estimator of ebp references */
  int	rglscl;			/* scl of quant to put in reg */
  int	rgllen;			/* length in bytes of quantity */
  enum 	scratch_status {
      unk_scratch, /* haven't done ld analysis for this yet */
      no_scratch,  /* can't put this regal into scratch */
      ok_scratch
  } rgl_scratch_use;
  struct regal *rgl_hash_next;	/* next regal in hash chain */
  struct regal *rglnext;	/* next regal in linked list */
  enum valid_types rgl_instr_type;
};

extern struct regals *first_regals, *first_label;
extern boolean lookup_regals();
extern void kill_label();
extern void add_label_to_align();

struct assign {
	char *asrname[3];	/* register name */
	int asrfld;	/* bit location of the register */
	unsigned asrregn;	/* register number */
	int asrtype;	/* register type */
	struct assign *	h_reg;	/* pointer to overlapping "companion" reg */
	struct assign * l_reg;	/* pointer to second overlapping 
				   "companion" reg */
	int asavail;	/* AVAIL, NOTAVAIL */
	struct regal * assigned_regal; /* points to a regal assigned
			  by the register allocator, null if AVAIL or
			  assigned by compiler or a companion register
			  of an assigned register */
};

#define reg_eax 0
#define reg_ecx 1
#define reg_edx 2
#define reg_ebx 3
#define reg_esp 4
#define reg_ebp 5
#define reg_esi 6
#define reg_edi 7
#define reg_ebi 8

#define LO (0)
#define HI (0)

#define reg_al reg_eax+LO
#define reg_bl reg_ebx+LO
#define reg_cl reg_ecx+LO
#define reg_dl reg_edx+LO

#define reg_ah reg_eax+HI
#define reg_bh reg_ebx+HI
#define reg_ch reg_ecx+HI
#define reg_dh reg_edx+HI
/* This should prolly be in some sgs header, since the compiler,
   debugger, and optimizer have to agree on it. */

#endif
