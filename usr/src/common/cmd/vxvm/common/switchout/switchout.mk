# @(#)src/cmd/vxvm/common/switchout/switchout.mk	1.1 10/16/96 02:21:58 - 
#ident	"@(#)cmd.vxvm:common/switchout/switchout.mk	1.1"

# Copyright(C)1996 VERITAS Software Corporation.  ALL RIGHTS RESERVED.
# UNPUBLISHED -- RIGHTS RESERVED UNDER THE COPYRIGHT
# LAWS OF THE UNITED STATES.  USE OF A COPYRIGHT NOTICE
# IS PRECAUTIONARY ONLY AND DOES NOT IMPLY PUBLICATION
# OR DISCLOSURE.
# 
# THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND
# TRADE SECRETS OF VERITAS SOFTWARE.  USE, DISCLOSURE,
# OR REPRODUCTION IS PROHIBITED WITHOUT THE PRIOR
# EXPRESS WRITTEN PERMISSION OF VERITAS SOFTWARE.
# 
#               RESTRICTED RIGHTS LEGEND
# USE, DUPLICATION, OR DISCLOSURE BY THE GOVERNMENT IS
# SUBJECT TO RESTRICTIONS AS SET FORTH IN SUBPARAGRAPH
# (C) (1) (ii) OF THE RIGHTS IN TECHNICAL DATA AND
# COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013.
#               VERITAS SOFTWARE
# 1600 PLYMOUTH STREET, MOUNTAIN VIEW, CA 94043

COMMON_DIR = ../../common/switchout

COM_TARGETS = vxinfo \
	vxmake \
	vxmend \
	vxplex \
	vxsd \
	vxvol

COM_VINFO_OBJS = $(COMMON_DIR)/volinfo.o
COM_VINFO_LINT_OBJS = volinfo.ln
COM_VINFO_SRCS = $(COMMON_DIR)/volinfo.c

COM_VMAKE_OBJS = $(COMMON_DIR)/volmake.o
COM_VMAKE_LINT_OBJS = volmake.ln
COM_VMAKE_SRCS = $(COMMON_DIR)/volmake.c

COM_VMEND_OBJS = $(COMMON_DIR)/volmend.o
COM_VMEND_LINT_OBJS = volmend.ln
COM_VMEND_SRCS = $(COMMON_DIR)/volmend.c

COM_VPLEX_OBJS = $(COMMON_DIR)/volplex.o
COM_VPLEX_LINT_OBJS = volplex.ln
COM_VPLEX_SRCS = $(COMMON_DIR)/volplex.c

COM_VSD_OBJS = $(COMMON_DIR)/volsd.o
COM_VSD_LINT_OBJS = volsd.ln
COM_VSD_SRCS = $(COMMON_DIR)/volsd.c

COM_VVOL_OBJS = $(COMMON_DIR)/volume.o
COM_VVOL_LINT_OBJS = volume.ln
COM_VVOL_SRCS = $(COMMON_DIR)/volume.c

COM_LOCALINC = -I../../common/libcmd

COM_OBJS = $(COM_VINFO_OBJS) \
	$(COM_VMAKE_OBJS) \
	$(COM_VMEND_OBJS) \
	$(COM_VPLEX_OBJS) \
	$(COM_VSD_OBJS) \
	$(COM_VVOL_OBJS)

COM_LINT_OBJS = $(COM_VINFO_LINT_OBJS) \
	$(COM_VMAKE_LINT_OBJS) \
	$(COM_VMEND_LINT_OBJS) \
	$(COM_VPLEX_LINT_OBJS) \
	$(COM_VSD_LINT_OBJS) \
	$(COM_VVOL_LINT_OBJS)

COM_SRCS = $(COM_VINFO_SRCS) \
	$(COM_VMAKE_SRCS) \
	$(COM_VMEND_SRCS) \
	$(COM_VSD_SRCS) \
	$(COM_VVOL_SRCS)

COM_HDRS =
