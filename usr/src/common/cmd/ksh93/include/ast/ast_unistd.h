#ident	"@(#)ksh93:include/ast/ast_unistd.h	1.1"
/***************************************************************
*                                                              *
*                      AT&T - PROPRIETARY                      *
*                                                              *
*         THIS IS PROPRIETARY SOURCE CODE LICENSED BY          *
*                          AT&T CORP.                          *
*                                                              *
*                Copyright (c) 1995 AT&T Corp.                 *
*                     All Rights Reserved                      *
*                                                              *
*           This software is licensed by AT&T Corp.            *
*       under the terms and conditions of the license in       *
*       http://www.research.att.com/orgs/ssr/book/reuse        *
*                                                              *
*               This software was created by the               *
*           Software Engineering Research Department           *
*                    AT&T Bell Laboratories                    *
*                                                              *
*               For further information contact                *
*                     gsf@research.att.com                     *
*                                                              *
***************************************************************/
/* : : generated from features/unistd.c by iffe version 05/09/95 : : */
#ifndef _def_unistd_ast
#define _def_unistd_ast	1
#undef	_SC_AIO_LISTIO_MAX
#define _SC_AIO_LISTIO_MAX	(-1)
#undef	_SC_AIO_MAX
#define _SC_AIO_MAX	(-2)
#undef	_SC_AIO_PRIO_DELTA_MAX
#define _SC_AIO_PRIO_DELTA_MAX	(-3)
#undef	_SC_ARG_MAX
#define _SC_ARG_MAX	(-4)
#undef	_SC_ASYNCHRONOUS_IO
#define _SC_ASYNCHRONOUS_IO	(-5)
#undef	_PC_ASYNC_IO
#define _PC_ASYNC_IO	(-6)
#undef	_SC_ATEXIT_MAX
#define _SC_ATEXIT_MAX	(-82)
#undef	_SC_AVPHYS_PAGES
#define _SC_AVPHYS_PAGES	(-121)
#undef	_SC_BC_BASE_MAX
#define _SC_BC_BASE_MAX	(-7)
#undef	_SC_BC_DIM_MAX
#define _SC_BC_DIM_MAX	(-8)
#undef	_SC_BC_SCALE_MAX
#define _SC_BC_SCALE_MAX	(-9)
#undef	_SC_BC_STRING_MAX
#define _SC_BC_STRING_MAX	(-10)
#undef	_SC_2_CHAR_TERM
#define _SC_2_CHAR_TERM	(-16)
#undef	_SC_CHILD_MAX
#define _SC_CHILD_MAX	(-17)
#undef	_PC_CHOWN_RESTRICTED
#define _PC_CHOWN_RESTRICTED	(-18)
#undef	_SC_CKPT
#define _SC_CKPT	(-97)
#undef	_SC_CLK_TCK
#define _SC_CLK_TCK	(-19)
#undef	_SC_CLOCKRES_MIN
#define _SC_CLOCKRES_MIN	(-20)
#undef	_SC_COLL_WEIGHTS_MAX
#define _SC_COLL_WEIGHTS_MAX	(-21)
#undef	_SC_XOPEN_CRYPT
#define _SC_XOPEN_CRYPT	(-116)
#undef	_SC_2_C_BIND
#define _SC_2_C_BIND	(-11)
#undef	_SC_2_C_DEV
#define _SC_2_C_DEV	(-12)
#undef	_SC_2_C_VERSION
#define _SC_2_C_VERSION	(-98)
#define _POSIX2_C_VERSION	-1
#undef	_SC_DELAYTIMER_MAX
#define _SC_DELAYTIMER_MAX	(-22)
#undef	_SC_XOPEN_ENH_I18N
#define _SC_XOPEN_ENH_I18N	(-117)
#define _XOPEN_ENH_I18N	-1
#undef	_SC_EXPR_NEST_MAX
#define _SC_EXPR_NEST_MAX	(-23)
#undef	_SC_FCHR_MAX
#define _SC_FCHR_MAX	(-24)
#undef	_SC_2_FORT_DEV
#define _SC_2_FORT_DEV	(-25)
#undef	_SC_2_FORT_RUN
#define _SC_2_FORT_RUN	(-26)
#undef	_SC_FSYNC
#define _SC_FSYNC	(-27)
#undef	_SC_IOV_MAX
#define _SC_IOV_MAX	(-100)
#undef	_SC_JOB_CONTROL
#define _SC_JOB_CONTROL	(-30)
#define _POSIX_JOB_CONTROL	1
#undef	_SC_LINE_MAX
#define _SC_LINE_MAX	(-31)
#undef	_PC_LINK_MAX
#define _PC_LINK_MAX	(-32)
#undef	_SC_LOCALEDEF
#define _SC_LOCALEDEF	(-99)
#undef	_SC_2_LOCALEDEF
#define _SC_2_LOCALEDEF	(-33)
#undef	_SC_LOGNAME_MAX
#define _SC_LOGNAME_MAX	(-129)
#undef	_SC_MAPPED_FILES
#define _SC_MAPPED_FILES	(-36)
#undef	_PC_MAX_CANON
#define _PC_MAX_CANON	(-37)
#undef	_PC_MAX_INPUT
#define _PC_MAX_INPUT	(-38)
#undef	_SC_MEMLOCK
#define _SC_MEMLOCK	(-40)
#undef	_SC_MEMLOCK_RANGE
#define _SC_MEMLOCK_RANGE	(-41)
#undef	_SC_MEMORY_PROTECTION
#define _SC_MEMORY_PROTECTION	(-42)
#undef	_SC_MESSAGE_PASSING
#define _SC_MESSAGE_PASSING	(-43)
#undef	_SC_MQ_OPEN_MAX
#define _SC_MQ_OPEN_MAX	(-44)
#undef	_SC_MQ_PRIO_MAX
#define _SC_MQ_PRIO_MAX	(-45)
#undef	_SC_NACLS_MAX
#define _SC_NACLS_MAX	(-122)
#undef	_PC_NAME_MAX
#define _PC_NAME_MAX	(-46)
#undef	_SC_NGROUPS_MAX
#define _SC_NGROUPS_MAX	(-47)
#undef	_PC_NO_TRUNC
#define _PC_NO_TRUNC	(-48)
#undef	_SC_NPROCESSORS_CONF
#define _SC_NPROCESSORS_CONF	(-123)
#undef	_SC_NPROCESSORS_ONLN
#define _SC_NPROCESSORS_ONLN	(-124)
#undef	_SC_OPEN_MAX
#define _SC_OPEN_MAX	(-49)
#undef	_SC_AES_OS_VERSION
#define _SC_AES_OS_VERSION	(-119)
#undef	_SC_PAGESIZE
#define _SC_PAGESIZE	(-51)
#undef	_SC_PAGE_SIZE
#define _SC_PAGE_SIZE	(-101)
#undef	_SC_PASS_MAX
#define _SC_PASS_MAX	(-130)
#undef	_CS_PATH
#define _CS_PATH	(-52)
#undef	_PC_PATH_MAX
#define _PC_PATH_MAX	(-53)
#undef	_SC_PHYS_PAGES
#define _SC_PHYS_PAGES	(-125)
#undef	_SC_PID_MAX
#define _SC_PID_MAX	(-54)
#undef	_PC_PIPE_BUF
#define _PC_PIPE_BUF	(-55)
#undef	_SC_PRIORITIZED_IO
#define _SC_PRIORITIZED_IO	(-57)
#undef	_SC_PRIORITY_SCHEDULING
#define _SC_PRIORITY_SCHEDULING	(-58)
#undef	_PC_PRIO_IO
#define _PC_PRIO_IO	(-59)
#undef	_SC_REALTIME_SIGNALS
#define _SC_REALTIME_SIGNALS	(-60)
#undef	_SC_REGEXP
#define _SC_REGEXP	(-95)
#undef	_SC_RESOURCE_LIMITS
#define _SC_RESOURCE_LIMITS	(-96)
#undef	_SC_RE_DUP_MAX
#define _SC_RE_DUP_MAX	(-61)
#undef	_SC_RTSIG_MAX
#define _SC_RTSIG_MAX	(-62)
#undef	_SC_SAVED_IDS
#define _SC_SAVED_IDS	(-63)
#define _POSIX_SAVED_IDS	1
#undef	_SC_SEMAPHORES
#define _SC_SEMAPHORES	(-66)
#undef	_SC_SEM_NSEMS_MAX
#define _SC_SEM_NSEMS_MAX	(-67)
#undef	_SC_SEM_VALUE_MAX
#define _SC_SEM_VALUE_MAX	(-68)
#undef	_SC_SHARED_MEMORY_OBJECTS
#define _SC_SHARED_MEMORY_OBJECTS	(-69)
#undef	_CS_SHELL
#define _CS_SHELL	(-120)
#undef	_SC_XOPEN_SHM
#define _SC_XOPEN_SHM	(-118)
#undef	_SC_SIGQUEUE_MAX
#define _SC_SIGQUEUE_MAX	(-72)
#undef	_SC_SIGRT_MAX
#define _SC_SIGRT_MAX	(-126)
#undef	_SC_SIGRT_MIN
#define _SC_SIGRT_MIN	(-127)
#undef	_SC_STD_BLK
#define _SC_STD_BLK	(-74)
#undef	_SC_STREAM_MAX
#define _SC_STREAM_MAX	(-75)
#undef	_SC_2_SW_DEV
#define _SC_2_SW_DEV	(-76)
#undef	_PC_SYMLINK_MAX
#define _PC_SYMLINK_MAX	(-93)
#undef	_PC_SYMLOOP_MAX
#define _PC_SYMLOOP_MAX	(-94)
#undef	_SC_SYNCHRONIZED_IO
#define _SC_SYNCHRONIZED_IO	(-77)
#undef	_PC_SYNC_IO
#define _PC_SYNC_IO	(-78)
#undef	_SC_SYSPID_MAX
#define _SC_SYSPID_MAX	(-79)
#undef	_SC_TIMERS
#define _SC_TIMERS	(-80)
#undef	_SC_TIMER_MAX
#define _SC_TIMER_MAX	(-81)
#undef	_CS_TMP
#define _CS_TMP	(-131)
#undef	_SC_TMP_MAX
#define _SC_TMP_MAX	(-128)
#undef	_SC_TZNAME_MAX
#define _SC_TZNAME_MAX	(-83)
#undef	_SC_UID_MAX
#define _SC_UID_MAX	(-85)
#undef	_SC_XOPEN_UNIX
#define _SC_XOPEN_UNIX	(-115)
#undef	_SC_2_UPE
#define _SC_2_UPE	(-89)
#undef	_PC_VDISABLE
#define _PC_VDISABLE	(-90)
#define _POSIX_VDISABLE	0
#undef	_SC_VERSION
#define _SC_VERSION	(-91)
#define _POSIX_VERSION	199009
#undef	_SC_2_VERSION
#define _SC_2_VERSION	(-92)
#define _POSIX2_VERSION	-1
#undef	_SC_XOPEN_VERSION
#define _SC_XOPEN_VERSION	(-110)
#define _XOPEN_VERSION	4
#undef	_SC_XOPEN_XCU_VERSION
#define _SC_XOPEN_XCU_VERSION	(-111)
#define _XOPEN_XCU_VERSION	3
#define _XOPEN_XPG4	1
#endif
