#ident	"@(#)ld:i386/machrel.c	1.15"
/*
** Module machrel
** perform machine dependent relocations
*/

/****************************************
** Imports
****************************************/

#include	<unistd.h>
#include	<link.h>
#include	"sgs.h"
#include	"globals.h"

/****************************************
** Local Macros
****************************************/

#define IS_PC_RELATIVE(X) (((X) != R_386_BASEOFF) && pc_rel_type[(X)] == 1)

/*
** build an output relocation entry
** rloc is a pointer to the current position in the output
** relocation section
*/

#define BUILD_RELOC(osect, rtype, ndx, off, rloc)	{\
	Rel	*relbits = (Rel *)((char*) ((osect)->os_outrels->is_rawbits->d_buf)+*(rloc));\
	relbits->r_info = ELF32_R_INFO((ndx),(rtype));\
	relbits->r_offset = (off);\
	*(rloc) += sizeof(Rel); } 
	

/* NOTE: These macros will work reliably only on 32-bit 2's 
 * complement machines.  The type of P in all cases should
 * be unsigned char *
 */

#define	GET4(P)	((long)(((unsigned long)(P)[3] << 24) | \
			((unsigned long)(P)[2] << 16) | \
			((unsigned long)(P)[1] << 8) | \
			(unsigned long)(P)[0]))

#define	PUT4(V, P)	{ (P)[3] = ((V) >> 24); \
			  (P)[2] = ((V) >> 16); \
			  (P)[1] = ((V) >> 8); \
			  (P)[0] = (V); }


/* 386 instruction encodings - used for procedure linkage table entries */

/* opcodes: */

#define INST_JMP		0xe9
#define INST_PUSHL		0x68
#define	SPECIAL_INST		0xff
#define	PUSHL_DISP		0x35
#define	PUSHL_REG_DISP		0xb3
#define	JMP_DISP_IND		0x25
#define	JMP_REG_DISP_IND	0xa3

/****************************************
** Local Variable Definitions
****************************************/

/* static bit arrays used to determine whether a particular
 * relocation type is pc-relative or is a byte-swapped type
 */

static CONST unsigned char pc_rel_type[R_386_NUM] = { 	0,	/* R_386_NONE	*/
							0,	/* R_386_32	*/
							1,	/* R_386_PC32	*/
							0,	/* R_386_GOT32	*/
							1,	/* R_386_PLT32	*/
							0,	/* R_386_COPY	*/
							0,	/* R_386_GLOB_DAT		*/
							0,	/* R_386_JMP_SLOT	*/
							0,	/* R_386_RELATIVE	*/
							0,	/* R_386_GOTOFF	*/
							1 };	/* R_386_GOTPC	*/

/****************************************
** Local Function Declarations
****************************************/

LPROTO(void do_reloc, (Insect *, Word, Addr, Word));
LPROTO(void plt_entry, (Ldsym *, Word));
LPROTO(Bss_cpy *find_bss_cpy, (CONST char  *, Addr, Word));
LPROTO(Bss_cpy *find_by_name, (CONST char  *, CONST char *));

/****************************************
** Local Function Definitions
****************************************/


/* write a single relocated value to its reference location */

static void
do_reloc(isect, rtype, off, value)
	Insect	*isect;
	Word	rtype;
	Addr	off;
	Word	value;
{
	register unsigned char	*memloc;
	register long		uvalue;

DPRINTF(DBG_RELOC, (MSG_DEBUG, "In do_reloc"));
 
	/* find bits in input section */
	memloc = (unsigned char*) isect->is_rawbits->d_buf + off;

	switch (rtype) {
	case R_386_NONE:
		DPRINTF(DBG_RELOC, (MSG_DEBUG, "reloc case R_386_NONE"));
		break;
	case R_386_32:
	case R_386_PC32:
	case R_386_GOT32:
	case R_386_PLT32:
	case R_386_GOTPC:
	case R_386_GOTOFF:
	case R_386_BASEOFF:
#ifdef	DEBUG
		switch(rtype) {
		case R_386_32:
			DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc case R_386_32: value %#x, memloc %#x",
				    value, memloc));
			break;
		case R_386_PC32:
			DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc case R_386_PC32: value %#x, memloc %#x",
				    value, memloc));
			break;
		case R_386_GOT32:
			DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc case R_386_GOT32: value %#x, memloc %#x",
				    value, memloc));
			break;
		case R_386_PLT32:
			DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc case R_386_PLT32: value %#x, memloc %#x",
				    value, memloc));
			break;
		case R_386_GOTPC:
			DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc case R_386_GOTPC: value %#x, memloc %#x",
				    value, memloc));
			break;
		case R_386_GOTOFF:
			DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc case R_386_GOTOFF: value %#x, memloc %#x",
				    value, memloc));
			break;
		case R_386_BASEOFF:
			DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc case R_386_BASEOFF: value %#x, memloc %#x",
				    value, memloc));
			break;
		}
#endif
		uvalue = GET4(memloc);
		uvalue += value;
		PUT4(uvalue, memloc);
		break;
	default:
		lderror(MSG_FATAL,
			gettxt(":1167","unknown relocation type %d in section %s of file %s"),
			rtype, isect->is_name, isect->is_file_ptr->fl_name);

	}
}


/* build a single plt entry - code is:
 *	if (building a.out)
 *		JMP	*got_off
 *	else
 *		JMP	*got_off@GOT(%ebx)
 *	PUSHL	&rel_off
 *	JMP	-n(%pc)		# -n is pcrel offset to first plt entry
 *
 *	The got_off@GOT entry gets filled with the address of the PUSHL,
 *	so the first pass through the plt jumps back here, jumping
 *	in turn to the first plt entry, which jumps to the dynamic
 *	linker.	 The dynamic linker then patches the GOT, rerouting
 *	future plt calls to the proper destination.
 */

static void
plt_entry(ldsym, rel_off)
	Ldsym	*ldsym;
	Word	rel_off;
{
	unsigned char *pent;
	Sword	plt_off;
	Word	got_off;
	unsigned char opnd;
	unsigned char *gent;

	got_off = ldsym->ls_PLTGOTndx * GOTENTSZ;
	plt_off = ldsym->ls_PLTndx * PLTENTSZ;
	pent = (unsigned char *)(plt_sect->is_rawbits->d_buf) + plt_off;
	gent = (unsigned char *)(got_sect->is_rawbits->d_buf) + got_off;

	/* fill in the field in the got with the address of the next instruction */
	PUT4(plt_sect->is_newVAddr + plt_off + PLT_INST_SZ, gent);

	if (!Gflag) {
		pent[0] = SPECIAL_INST;
		pent[1] = JMP_DISP_IND;
		pent += 2;
		PUT4((Word)(got_sect->is_newVAddr + got_off), pent);
	} else {
		pent[0] = SPECIAL_INST;
		pent[1] = JMP_REG_DISP_IND;
		pent += 2;
		PUT4((Word)(got_off), pent);
	}
	pent += 4;

	pent[0] = INST_PUSHL;
	pent++;
	PUT4(rel_off, pent);
	pent += 4;

	plt_off = -(plt_off + 16);	/* JMP, PUSHL, JMP take 16 bytes */
	pent[0] = INST_JMP;
	pent++;
	PUT4((long)plt_off, pent);
}

/****************************************
** Global Function Definitions
****************************************/

/* count relocation, got and plt entries for a single
 * relocation section
 * this code parallels the code for reloc_sect, below,
 * which is more heavily commented and actually
 * performs the relocations counted here
 */
void
count_sect(isp, rsp, osp)
	Insect	*isp, *rsp;
	Os_desc	*osp;
{
	register Rel	*reloc, *rend;
	register Ldsym  *sym;
	Boolean		noload = FALSE;
	Word  		rstndx, rtype, rsize; 
	Bss_cpy		*tbss_cpy;
	Bss_cpy		*new_bss_cpy_ptr;
	
	/* is input section loadable? */
	if (!(isp->is_shdr->sh_flags & SHF_ALLOC))
		noload = TRUE;
				
	rsize = rsp->is_shdr->sh_size;
	reloc = (Rel *)rsp->is_rawbits->d_buf;

	/* read relocation records - there may be 2 types of relocation
	 * entries: one with an addend field and one without
	 * A relocation section will be all of one or the other kind.
	 * This code is only for one without an addend
	 */
				
	rend = (Rel*)((char *)reloc + rsize);
	for ( ; reloc < rend; ++reloc) {
		rtype = ELF32_R_TYPE(reloc->r_info);
		rstndx = ELF32_R_SYM(reloc->r_info);

		sym = rsp->is_file_ptr->fl_oldindex[rstndx];

		/* in the static case,
		 * just check for got entries
		 * we handled the ld -r case above 
		 */
		if (!dmode) {
			if (rtype == R_386_GOT32)
				if (!sym->ls_GOTndx)
					sym->ls_GOTndx = countGOT++;
			continue;
		}

		/* dynamic mode */
		if (ELF32_ST_BIND(sym->ls_syment->st_info) == STB_LOCAL) {
			if (!IS_PC_RELATIVE(rtype)) {
				if (rtype == R_386_GOT32) {
					if (!sym->ls_GOTndx) {
						sym->ls_GOTndx = countGOT++;
						if (Gflag)
							grels += sizeof(Rel);
					}
				} else if (Gflag && !noload &&
					rtype != R_386_GOTOFF &&
					rtype != R_386_BASEOFF)
					osp->os_szoutrel += sizeof(Rel);
			}
			continue;
		}

		/* if here, we have a global or weak symbol */
		
		if (rtype == R_386_GOT32) {
			if (!sym->ls_GOTndx) {
				sym->ls_GOTndx = countGOT++;
				if (Gflag || (sym->ls_deftag < REF_RELOBJ) 
				|| (sym->ls_syment->st_shndx == SHN_UNDEF))
					grels += sizeof(Rel);
			}
			continue;
		}
		if (rtype == R_386_PLT32) {
			if ((Gflag && !Bflag_symbolic && !sym->ls_Bsymbolic) 
				|| (sym->ls_deftag < REF_RELOBJ) ||
				(sym->ls_syment->st_shndx == SHN_UNDEF)) {
				if (!sym->ls_PLTndx) {
					sym->ls_PLTndx = countPLT++;
					sym->ls_PLTGOTndx = countGOT++;
					prels += sizeof(Rel);
				}
			}
			continue;
		}
		if (rtype == R_386_GOTOFF ) {
		lderror(MSG_FATAL,
			gettxt(":1517","illegal relocation type %d in section %s of file %s"),
			rtype, isp->is_name, isp->is_file_ptr->fl_name);
			
		}
		if ( rtype == R_386_GOTPC)
			continue;
		if ((sym->ls_deftag >= REF_RELOBJ) && 
			sym->ls_syment->st_shndx != SHN_UNDEF) {
			if (Gflag && !Bflag_symbolic && !sym->ls_Bsymbolic &&
				!noload && rtype != R_386_BASEOFF) 
				osp->os_szoutrel += sizeof(Rel); 
			else if (!IS_PC_RELATIVE(rtype) &&
				(rtype != R_386_BASEOFF)) {
				if (Gflag && !noload)
					osp->os_szoutrel += sizeof(Rel);
			}
			continue;
		}
		/* symbol defined in a .so
		 * or undefined 
		 */
		if ((sym->ls_deftag == REF_DEFN) || 
			(sym->ls_syment->st_shndx == SHN_UNDEF)) {

			Boolean	is_function = FALSE, is_object = FALSE;
			int	stype;

			stype = ELF32_ST_TYPE(sym->ls_syment->st_info);
			switch(stype)
			{
			case STT_FUNC:
				is_function = TRUE;
				break;
			case STT_OBJECT:
				is_object = TRUE;
				break;
			case STT_NOTYPE:
				if (rtype == R_386_PC32)
					is_function = TRUE;
				break;
			default:
				break;
			}
			if (Gflag || bflag || 
				(!is_function && !is_object) ||
				(!is_function &&
				(sym->ls_syment->st_shndx == SHN_UNDEF))) {
				osp->os_szoutrel += sizeof(Rel);
			} else {

			/* special relocation processing for dynamic a.outs
			 * referencing symbols defined in a .so w/o pic or got
			 * reloc types; if sym is a function, create plt entry
			 * for it; if data, then allocate space for this symbol in 
			 * common in this object and relocate reference to
			 * the new space; if sym  was initiallized, create
			 * relocation entry specifying that data must be copied
			 * at run-time
			 */

				if (is_function) {
					if (!sym->ls_PLTndx) {
						sym->ls_PLTndx = countPLT++;
						sym->ls_PLTGOTndx = countGOT++;
						prels += sizeof(Rel);
					}
				}
				else if (is_object &&
				(sym->ls_syment->st_shndx != SHN_ABS)) {
			
					   if (!find_by_name(sym->ls_flptr->fl_name,sym->ls_name)) {
						bss_cpy_ptr = find_bss_cpy(sym->ls_flptr->fl_name,sym->ls_syment->st_value,sym->ls_syment->st_size);
						tbss_cpy = (Bss_cpy *)mymalloc( sizeof(Bss_cpy));
						tbss_cpy->sh_lib_name = sym->ls_flptr->fl_name;
						tbss_cpy->data_seg_off = sym->ls_syment->st_value;
						tbss_cpy->sym_name = sym->ls_name;
						tbss_cpy->synonym_ptr = bss_cpy_ptr;
						tbss_cpy->binding = ELF_ST_BIND(sym->ls_def_binding);
						tbss_cpy->size = sym->ls_syment->st_size;
						tbss_cpy->ldsym = sym;
						(void) list_append(&bss_cpy_list,tbss_cpy);

						/* if there is an older one 
						(bss_cpy_ptr != NULL), then 
						figure out if the older one is
						contained within the newer one.
						If so, make sure bss is allocated
					        for the newer one (larger one),
					        and that the synonym pointers
					        get reversed. 
						*/

						if (bss_cpy_ptr){
				   		 if((bss_cpy_ptr->data_seg_off==
						       tbss_cpy->data_seg_off 
						     && 
							bss_cpy_ptr->size <
							tbss_cpy->size) 
						   ||
						    (tbss_cpy->data_seg_off <
						     bss_cpy_ptr->data_seg_off
						     &&
					             tbss_cpy->data_seg_off +
						   	tbss_cpy->size  >= 
						     bss_cpy_ptr->data_seg_off + 
							bss_cpy_ptr->size)
				    	       ){ /* then the new one is 
							the larger one */

						   DPRINTF(DBG_SYMS, (MSG_DEBUG,
						   "%s is the newer and larger one; %s is the smaller\n", 
						   sym->ls_name, bss_cpy_ptr->ldsym->ls_name));
						   sym->ls_not_alloc = FALSE;
						   bss_cpy_ptr->ldsym->ls_not_alloc = TRUE;
						   if (bss_cpy_ptr->ldsym->ls_COPYalloc)
							bss_cpy_ptr->ldsym->ls_COPYalloc = FALSE;
						   else
							copyrels += sizeof(Rel);

						   sym->ls_COPYalloc = TRUE;
						
						   /* now flip synonym_ptrs */

						   bss_cpy_ptr->synonym_ptr =
							tbss_cpy;
						   tbss_cpy->synonym_ptr = 0;
DPRINTF(DBG_SYMS, (MSG_DEBUG, "Updating smaller entry in list\n"));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsh_lib_name=%s", bss_cpy_ptr->sh_lib_name));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsym_name=%s", bss_cpy_ptr->sym_name));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tdata_seg_off=%x\n", bss_cpy_ptr->data_seg_off));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tbinding=%d", bss_cpy_ptr->binding));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsize=%x\n", bss_cpy_ptr->size));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsynonym_ptr=%x", bss_cpy_ptr->synonym_ptr));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tls_not_alloc=%d", bss_cpy_ptr->ldsym->ls_not_alloc));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tls_COPYalloc=%d\n", bss_cpy_ptr->ldsym->ls_COPYalloc));
				   		}
				   		else {
						     sym->ls_not_alloc = TRUE;
						     sym->ls_COPYalloc = FALSE;
				    		}
					     } else{  /* !bss_cpy_ptr */
						if (!sym->ls_COPYalloc){
							sym->ls_COPYalloc= TRUE;
							copyrels += sizeof(Rel);
						}
					       }
		
DPRINTF(DBG_SYMS, (MSG_DEBUG, "Made entry to list\n"));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsh_lib_name=%s", tbss_cpy->sh_lib_name));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsym_name=%s", tbss_cpy->sym_name));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tdata_seg_off=%x\n", tbss_cpy->data_seg_off));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tbinding=%d", tbss_cpy->binding));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\toriginal binding=%d", 
					ELF_ST_BIND(sym->ls_def_binding)));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsize=%x\n", tbss_cpy->size));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsynonym_ptr=%x", tbss_cpy->synonym_ptr));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tls_not_alloc=%d", sym->ls_not_alloc));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tls_COPYalloc=%d\n", sym->ls_COPYalloc));
					
					} /* if !find_by_name */

					sym->ls_syment->st_shndx = SHN_COMMON;
					sym->ls_syment->st_value = WORD_ALIGN;
					if (bss_align < sym->ls_syment->st_value)
						bss_align = sym->ls_syment->st_value;
				}
			}
		} /* end not defined */
	} /* end read reloc entries */
}


/* void fillin_gotplt
** machine dependent part of relocate() that
** initializes the first got entry
** and fills in the reserved slot of the plt
*/
void
fillin_gotplt()
{

	/* initialize first got entry with address of dynamic section */
	if (dmode && got_sect) {
		Ldsym *sym;
		if((sym = sym_find("_DYNAMIC", NOHASH)) == NULL)
			lderror(MSG_SYSTEM,gettxt(":1168","internal error:  `_DYNAMIC` not found in ld symbol table"));
		else if (!sym->ls_GOTdone) 
			PUT4(sym->ls_syment->st_value,
				((unsigned char *)got_sect->is_rawbits->d_buf));
	}


        /* fill in the reserved slot in the procedure linkage table
	 * the first entry is:
	 *  if (building a.out) {
	 *	PUSHL	got[1]		# the address of the link map entry
	 *	JMP	*got[2]		# the address of rtbinder
	 *  } else {
	 *	PUSHL	got[1]@GOT(%ebx)	# the address of the link map entry
	 *	JMP	*got[2]@GOT(%ebx)	# the address of rtbinder
	 *  }
	 */
	if (dmode && plt_sect) {
		unsigned char *pent;
		unsigned long plt_addr; /* distance between the plt entry and the GOT */

                pent = (unsigned char *)plt_sect->is_rawbits->d_buf + PLT_XRTLD * PLTENTSZ;
		if (!Gflag) {
			pent[0] = SPECIAL_INST;
			pent[1] = PUSHL_DISP;
			pent += 2;
			PUT4((Word)(got_sect->is_newVAddr + GOT_XLINKMAP * GOTENTSZ), pent);
			pent += 4;
			pent[0] = SPECIAL_INST;
			pent[1] = JMP_DISP_IND;
			pent += 2;
			PUT4((Word)(got_sect->is_newVAddr + GOT_XRTLD * GOTENTSZ), pent);
		} else {
			pent[0] = SPECIAL_INST;
			pent[1] = PUSHL_REG_DISP;
			pent += 2;
			PUT4((Word)(GOT_XLINKMAP * GOTENTSZ), pent);
			pent += 4;
			pent[0] = SPECIAL_INST;
			pent[1] = JMP_REG_DISP_IND;
			pent += 2;
			PUT4((Word)(GOT_XRTLD * GOTENTSZ), pent);
		}
	}

}



/* void reloc_sect()
** process a single relocation section
** There are 2 types of relocation: globals/non-section locals, section locals
** for globals and non-section locals, the reference location or relocation
** entry addend field contains a signed constant.
** The relocated value is the value (vaddr) of the
** symbol referenced plus that constant.  If the relocation
** type is pc-relative, the address of the reference point
** is subtracted from the value.
** For section locals, the reference point contains a signed constant
** plus the offset of the symbol definition into the section
** in which it is defined. The relocated value is the constant
** plus offset plus virtual address of the section containing
** the symbol definition. Again, if the relocation
** type is pc-relative, the address of the reference point
** is subtracted from the value.
*/

void
reloc_sect(isect, rsect, osect)
	Insect	*isect, *rsect;
	Os_desc	*osect;
{
	register Word	value; 
	Word		sav_val;
	Word		roff, rstndx, rsize, refaddr;
	Word		R1addr, ovalue;
	unsigned char	rtype, R1type;
	register Rel	*reloc, *rend;
	register Ldsym	*rldsym;
	Boolean		noload = FALSE;


	DPRINTF(DBG_RELOC, (MSG_DEBUG,
			    "relocating section %s from file %s",
			    isect->is_name, isect->is_file_ptr->fl_name));
	/* is input section loadable ? */
	if (!(isect->is_shdr->sh_flags & SHF_ALLOC))
		noload = TRUE;

	rsize = rsect->is_shdr->sh_size;
	reloc = (Rel *)rsect->is_rawbits->d_buf;

	/* read relocation records - there may be 2 types of relocation
	 * entries: one with an addend field and one without
	 * A relocation section will be all of one or the other kind.
	 * This code is only for one without an addend
	 */

	rend = (Rel *)((char *)reloc + rsize);
	for ( ; reloc < rend; ++reloc) {
		rtype = ELF32_R_TYPE(reloc->r_info);
		roff = reloc->r_offset;
		rstndx = ELF32_R_SYM(reloc->r_info);
		DPRINTF(DBG_RELOC, (MSG_DEBUG,
				    "reloc: type %d, offset %d, rstndx %d",
				    rtype, roff, rstndx));

		/* get the ld symbol table version of the relocation symbol,
		 * and its new updated value 
		 */
		rldsym = rsect->is_file_ptr->fl_oldindex[rstndx];
		if (ELF32_ST_BIND(rldsym->ls_syment->st_info) == STB_LOCAL &&
			rldsym->ls_syment->st_shndx == SHN_UNDEF &&
			rldsym->ls_syment->st_name != 0) {
			lderror(MSG_FATAL,
				gettxt(":1169","relocation against undefined local symbol %s from file %s"),
				rldsym->ls_name, rldsym->ls_flptr->fl_name);
		}
		value = rldsym->ls_syment->st_value;

		/* calculate virtual address of reference point;
		 * equals offset into section + vaddr of section
		 * for loadable sections, or offset plus section
		 * displacement for nonloadable sections
		 */
		if (!noload && (aflag || dmode))
			refaddr = roff + isect->is_newVAddr;
		else
			refaddr = roff + isect->is_displ;

		/* static case - preserving relocations  (ld -r or ld -a -r) */
		if (rflag) {
			if( (ELF32_ST_BIND(rldsym->ls_syment->st_info) == STB_LOCAL)
			&& (ELF32_ST_TYPE(rldsym->ls_syment->st_info) == STT_SECTION) ) {
			/* relocation off section symbol - update
			 * reference location to account for displacement
			 * of input section containing symbol definition
			 */
				if (!aflag)
					do_reloc(isect, rtype, roff, rldsym->ls_scnptr->is_displ);
				BUILD_RELOC(osect, rtype, rldsym->ls_scnptr->is_outsect_ptr->os_ndxsectsym, refaddr, &orels);
			}
			else 
				BUILD_RELOC(osect, rtype, rldsym->ls_outndx, refaddr, &orels);
			if (!aflag)
				continue;
		}

		/* building an absolute file or shared object */
		if (ELF32_ST_BIND(rldsym->ls_syment->st_info) == STB_LOCAL) {
			if (ELF32_ST_TYPE(rldsym->ls_syment->st_info) == STT_SECTION) {
				/* for section symbols, value is vaddr of
				 * the section for loadable sections,
				 * displacement of the input section
				 * within its output section for non-loadable
				 */
				if (rldsym->ls_scnptr->is_shdr->sh_flags & SHF_ALLOC)
					value = rldsym->ls_scnptr->is_newVAddr;
				else
					value = rldsym->ls_scnptr->is_displ;
			}
			else if (rtype == R_386_GOTPC)
				value = (Word) (got_sect->is_newVAddr) - refaddr;
			else if (rtype == R_386_GOT32) {
				/* global offset table relocation -
				 * relocate reference to got entry
				 * and initialize got entry if not
				 * already done; if a shared object,
				 * create an output reloc entry for got
				 * since address we calculate is relative
				 * to 0
				 */
				ovalue = value;
				value = (Word) (rldsym->ls_GOTndx * GOTENTSZ);
				if (!rldsym->ls_GOTdone) {
					rldsym->ls_GOTdone = TRUE;
					R1addr = (Word)((char *)(got_sect->
						is_rawbits->d_buf) +
						(rldsym->ls_GOTndx * GOTENTSZ));
					PUT4(ovalue, (char *)R1addr);
					if (Gflag) {
						BUILD_RELOC(got_sect->
							is_outsect_ptr,
							R_386_RELATIVE,
							STN_UNDEF,
							value +
							((Word) got_sect->
							is_newVAddr), &grels);
					}
				}
			}
			if (rtype == R_386_GOTOFF) {
				/* static offset from the GOT */
				ovalue = value;
				value -= (Word) (got_sect->is_newVAddr);
			}
			if (IS_PC_RELATIVE(rtype))
				value -= refaddr;
			else if (Gflag && !noload) {
				if (rtype != R_386_GOT32 && rtype != R_386_GOTOFF && rtype != R_386_BASEOFF) {
					R1type = R_386_RELATIVE;
					BUILD_RELOC(osect, R1type, STN_UNDEF,
					refaddr, &orels);
				}
			}
			do_reloc(isect, rtype, roff, value);
			continue;
		} 


		/* if here, we have a global or weak symbol */

		if (rtype == R_386_GOT32) {
			/* relocate reference to got entry for this
			 * symbol; if got entry has not already
			 * been initialized, do so and create
			 * an output relocation record for it if necessary
			 */
			ovalue = value;
			value = (Word) (rldsym->ls_GOTndx * GOTENTSZ);
			if (!rldsym->ls_GOTdone) {
				rldsym->ls_GOTdone = TRUE;
				R1addr = (Word)((char *)(got_sect->is_rawbits->
					d_buf) + (rldsym->ls_GOTndx * GOTENTSZ));

				/* if building a .so (w/o Bsymbolic) or
				 * the symbol is undefined or defined in a
				 * .so, we initialize the got entry with 0
				 * and create a reloc entry; otherwise
				 * we place the symbol value in the got
				 * entry - if building a .so w/ Bsymbolic
				 * we still need a reloc entry, since the
				 * value calculated is 0-based
				 */
				if ((rldsym->ls_deftag < REF_RELOBJ) ||
					(rldsym->ls_syment->st_shndx 
						== SHN_UNDEF) ||
					(Gflag && !Bflag_symbolic &&
						!rldsym->ls_Bsymbolic)) {
						PUT4(0, (char *)R1addr);
						BUILD_RELOC(got_sect->is_outsect_ptr, R_386_GLOB_DAT, rldsym->ls_outndx, value + ((Word) got_sect->is_newVAddr), &grels);
					}
				else {
					PUT4(ovalue, (char *)R1addr);
					if (Gflag && (Bflag_symbolic ||
						rldsym->ls_Bsymbolic)) {
						BUILD_RELOC(got_sect->
							is_outsect_ptr,
							R_386_RELATIVE,
							STN_UNDEF,
							value + ((Word) got_sect->is_newVAddr), &grels);
					}
				}
			}
			do_reloc(isect, rtype, roff, value);
			continue;
		} 
		if (rtype == R_386_GOTPC) {
			value = (Word) (got_sect->is_newVAddr) - refaddr;
			do_reloc(isect, rtype, roff, value);
			continue;
		}
		if (rtype == R_386_PLT32) {
			/* procedure linkage table reloc -
			 * if we don't have a definition or are
			 * building a shared object (w/o Bsymbolic)
			 * we create a plt entry, and relocate this
			 * reference to that entry - else we
			 * relocate reference directly to symbol
			 */
			if ((Gflag && !Bflag_symbolic && !rldsym->ls_Bsymbolic) ||
				(rldsym->ls_deftag < REF_RELOBJ) ||
				(rldsym->ls_syment->st_shndx == SHN_UNDEF)) {
				value = (Word)(plt_sect->is_newVAddr) +
					(rldsym->ls_PLTndx * PLTENTSZ);
				if (!rldsym->ls_PLTdone) {
					rldsym->ls_PLTdone = TRUE;
					plt_entry(rldsym, prels);
					BUILD_RELOC(plt_sect->is_outsect_ptr,
						R_386_JMP_SLOT,
						rldsym->ls_outndx,
						(Word)(got_sect->is_newVAddr) +
						rldsym->ls_PLTGOTndx * GOTENTSZ,
						&prels);
				}
			}
			value -= refaddr; /* plt references are pc-relative */
			do_reloc(isect, rtype, roff, value);
			continue;
		} 
		if ((rldsym->ls_deftag >= REF_RELOBJ) && 
			(rldsym->ls_syment->st_shndx != SHN_UNDEF)) {
			/* symbol defined in a dot-o - 
			 * non-GOT, non-PLT relocation 
			 * if building a .so (w/o Bsymbolic) just create 
			 * a reloc entry since we don't know if definition 
			 * we have is one that will be used at run-time; 
			 * else do relocation if building a .so w/ Bsymbolic 
			 * and the reloc type is not pc-relative, 
			 * create a reloc entry, since vaddr is 0-based
			 */
			if (Gflag && !Bflag_symbolic && !rldsym->ls_Bsymbolic
				&& !noload && rtype != R_386_BASEOFF) {
				BUILD_RELOC(osect, rtype, rldsym->ls_outndx, refaddr, &orels);
			} else {
				if (IS_PC_RELATIVE(rtype))
					value -= refaddr;
				else if (Gflag && (Bflag_symbolic ||
					rldsym->ls_Bsymbolic)
					&& !noload && 
					rtype != R_386_BASEOFF) {
					R1type = R_386_RELATIVE;
					BUILD_RELOC(osect, R1type, STN_UNDEF, refaddr, &orels);
				}
				do_reloc(isect, rtype, roff, value);
			}
			continue;
		} 
		if (rtype == R_386_BASEOFF)
			lderror(MSG_FATAL,
				gettxt(":1611","R_386_BASEOFF relocation against undefined symbol %s from file %s"),
				rldsym->ls_name, isect->is_file_ptr->fl_name);
		if ((rldsym->ls_deftag == REF_DEFN) || 
			(rldsym->ls_syment->st_shndx == SHN_UNDEF)) {
			/* undefined symbol or defined in a shared object 
			 * if symbol undefined or building a shared object 
			 * or bflag is set
			 * just create an output reloc entry; if building
			 * an a.out and bflag is not set:
			 *  1) if symbol is a function, create a plt entry
			 *        for it and relocate reference to that entry
			 *  2) if symbol is an object we have already
			 *	  allocated space for it in bss; we relocate
			 *	  the reference to that space; if original
			 *	  symbol was initialized data we also create
			 *	  a special reloc entry that tells the 
			 *	  dynamic linker to copy the data at run-time;
			 */	  

			Boolean	is_function = FALSE, is_object = FALSE;
			int	stype;

			stype = ELF32_ST_TYPE(rldsym->ls_syment->st_info);
			switch(stype)
			{
			case STT_FUNC:
				is_function = TRUE;
				break;
			case STT_OBJECT:
				is_object = TRUE;
				break;
			case STT_NOTYPE:
				if (rtype == R_386_PC32)
					is_function = TRUE;
				break;
			default:
				break;
			}
			if (Gflag || bflag || 
				(!is_function && !is_object) ||
				(!is_function &&
				(rldsym->ls_syment->st_shndx == SHN_UNDEF))) {
				BUILD_RELOC(osect, rtype, rldsym->ls_outndx, refaddr, &orels);
			}
			else {
				if (is_function) {
					value = (Word)(plt_sect->is_newVAddr) +
						(rldsym->ls_PLTndx * PLTENTSZ);
					if (!rldsym->ls_PLTdone) {
						rldsym->ls_PLTdone = TRUE;
						plt_entry(rldsym, prels);
						BUILD_RELOC(plt_sect->
							is_outsect_ptr,
							R_386_JMP_SLOT,
							rldsym->ls_outndx,
							(Word)(got_sect->
							is_newVAddr) +
							rldsym->ls_PLTGOTndx *
							GOTENTSZ, &prels);
					}
				}
				else if (is_object &&
				(rldsym->ls_syment->st_shndx != SHN_ABS)) {

						bss_cpy_ptr = find_by_name(rldsym->ls_flptr->fl_name,rldsym->ls_name);
						if (rldsym->ls_COPYalloc) {
							bss_cpy_ptr->ldsym->ls_syment->st_value = value;

DPRINTF(DBG_SYMS, (MSG_DEBUG, "\nUpdating value\n"));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "sh_lib_name=%s", bss_cpy_ptr->sh_lib_name));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsym_name=%s", bss_cpy_ptr->sym_name));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tdata_seg_off=%x\n", bss_cpy_ptr->data_seg_off));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tvalue = %x", bss_cpy_ptr->ldsym->ls_syment->st_value));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tsynonym_ptr=%x", bss_cpy_ptr->synonym_ptr));
DPRINTF(DBG_SYMS, (MSG_DEBUG, "\tbinding=%d\n", bss_cpy_ptr->binding));

							if (!rldsym->ls_COPYdone){
							rldsym->ls_COPYdone = TRUE;
							BUILD_RELOC(bss_sect->is_outsect_ptr, R_386_COPY, rldsym->ls_outndx, value, &copyrels);
							}
						}
				}
				/* are other types errors ? */
				if (IS_PC_RELATIVE(rtype))
					value -= refaddr;
				do_reloc(isect, rtype, roff, value);
			}
		}
	} /* end of while reloc loop */
	return;
}

static Bss_cpy *
find_bss_cpy(lib_name,offset,size)
	CONST char *lib_name;
	Addr offset;
{
	register Listnode *ip;
	Bss_cpy *dp;
	for (LIST_TRAVERSE(&bss_cpy_list,ip,dp)) {
		if (strcmp(lib_name,dp->sh_lib_name) == SAME &&
			((offset >= dp->data_seg_off &&
			 offset + size <= dp->data_seg_off + dp->size) ||
			 (dp->data_seg_off >= offset &&
		          dp->data_seg_off + dp->size <= offset + size )))
				return dp;
	}
	return NULL;
}

static Bss_cpy *
find_by_name(lib_name,sym_name)
	CONST char *lib_name;
	CONST char *sym_name;
{
	register Listnode *ip;
	Bss_cpy *dp;

	for (LIST_TRAVERSE(&bss_cpy_list,ip,dp)) {
	if (strcmp(lib_name,dp->sh_lib_name) == SAME &&
		strcmp(sym_name,dp->sym_name) == SAME)
			return dp;
	}
	return NULL;
}

static Insect	*fake_link_map;

/* create fake r_debug and link_map structures for static
 * a.outs for use by C++ runtime in exception handling
 * r_debug structure we know about has following members:
 *	int	r_version
 *	struct link_map	*r_map
 *	unsigned long	r_brk
 *	enum		r_state
 *	unsigned long	r_ldbase
 *
 * link_map has following members:
 *	unsigned long	l_addr
 *	char	*l_name
 *	Elf32_Dyn	*l_ld
 *	struct link_map	*l_next
 *	struct link_map	*l_prev
 *	unsigned long	l_tstart
 *	unsigned long	l_tsize
 *	void	*l_eh_ranges
 *	unsigned long	l_eh_ranges_sz
 */
void
build_link_map_for_eh()
{
	Insect		*isect;
	Ldsym		*lsym;

	if (((lsym = sym_find("_r_debug", NOHASH)) == 0) ||
		(lsym->ls_syment->st_shndx != SHN_UNDEF))
		return;
	lsym->ls_syment->st_info = ELF_ST_INFO(STB_GLOBAL, STT_OBJECT);
	lsym->ls_syment->st_size = 5 * 4; /* sizeof struct r_debug */
	lsym->ls_syment->st_shndx = 1; /* make up a number that
					* is not one of the special
					* reserved values */

	/* BUMP_GLOB("_r_debug") */
	count_outglobs++;
	count_strsize += sizeof("_r_debug");

	lsym->ls_deftag = REF_RELOBJ;
	lsym->ls_flptr = NULL;

	isect = NEWZERO(Insect);
	isect->is_rawbits = mymalloc(sizeof(Elf_Data));
	isect->is_rawbits->d_type = ELF_T_BYTE;
	isect->is_rawbits->d_off = 0;
	isect->is_rawbits->d_size = 14 * 4; /* sizeof r_debug + sizeof
					     * link_map
					     */
	isect->is_rawbits->d_align = 4;
	isect->is_rawbits->d_version = EV_CURRENT;
	isect->is_rawbits->d_buf = mycalloc(isect->is_rawbits->d_size);
	isect->is_shdr = (Shdr *)mycalloc(sizeof(Shdr));
	isect->is_shdr->sh_type = SHT_PROGBITS;
	isect->is_shdr->sh_flags = SHF_ALLOC | SHF_WRITE;
	isect->is_shdr->sh_size = isect->is_rawbits->d_size;
	isect->is_shdr->sh_addralign = 4;
	isect->is_name = mymalloc(sizeof(".data"));
	strcpy(isect->is_name, ".data");
	place_section(isect);
	lsym->ls_scnptr = isect;
	fake_link_map = isect;
}

void
update_link_map_for_eh()
{
	char		*p;
	unsigned long	laddr;
	Ldsym		*lsym;
	Listnode	*np1;
	Sg_desc		*sgp;

	if (!fake_link_map ||
		((lsym = sym_find("_r_debug", NOHASH)) == 0))
		return;
	laddr = lsym->ls_syment->st_value + (5 * 4); /* sizeof (struct r_debug) */
	for (LIST_TRAVERSE(&seg_list, np1, sgp)) {
		/* find text segment */
		if ((sgp->sg_phdr.p_type == PT_LOAD) &&
			((sgp->sg_phdr.p_flags & (PF_R|PF_W|PF_X)) ==
				(PF_R|PF_X)))
				break;
	}
	p = fake_link_map->is_outdata->d_buf;
	/* write out r_debug structure - fields not set were
	 * initialized to 0 by calloc
	 */
	PUT4(1, p); /* r_version */
	p += 4;
	PUT4(laddr, p);  /* r_map */
	p += 4*4;
	/* r_brk */
	/* r_state */
	/* r_ldbase */

	/* write out link_map  - fields not set were initialized
	 * to 0 by calloc
	 */
	p += 5*4;
	/*  l_addr */
	/*  l_name */
	/*  l_ld */
	/*  l_next */
	/*  l_prev */
	if (sgp) {
		PUT4(sgp->sg_phdr.p_vaddr, p); /* l_tstart */
		p += 4;
		PUT4(sgp->sg_phdr.p_memsz, p); /* l_tsize */
		p += 4;
	}
	else {
		p += 2*4;
		/*  l_tstart */
		/*  l_tsize */
	}
	PUT4(eh_ranges_sect->os_shdr->sh_addr, p); /* l_eh_ranges */
	p += 4;
	PUT4(eh_ranges_sect->os_shdr->sh_size, p); /* l_eh_ranges_sz */
}
