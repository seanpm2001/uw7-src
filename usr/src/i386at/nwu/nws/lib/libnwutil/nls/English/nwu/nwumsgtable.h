 


/* Copyright 1991, 1992 Unpublished Work of Novell, Inc.
** All Rights Reserved.
**
** This work is an unpublished work and contains confidential,
** proprietary and trade secret information of Novell, Inc. Access
** to this work is restricted to (I) Novell employees who have a
** need to know to perform tasks within the scope of their
** assignments and (II) entities other than Novell who have
** entered into appropriate agreements.
**
** No part of this work may be used, practiced, performed,
** copied, distributed, revised, modified, translated, abridged,
** condensed, expanded, collected, compiled, linked, recast,
** transformed or adapted without the prior written consent
** of Novell.  Any use or exploitation of this work without
** authorization could subject the perpetrator to criminal and
** civil liability.
**
*/



#if !defined(__nwumsgtable_h__)
#define __nwumsgtable_h__

/*  */
/*  DEVELOPERS:  Do NOT add messages or constants to this file */
/*  */
/*  You must go to file "nls/English/nwumsg.m4" and make additions there. */
/*  */
#define MSG_NW_REV_SET 1

#define MSG_NW_REV 1 
/* Do NOT touch the following REV string. (MUST match nwumsgs.msg)*/
#define	MSG_NW_REV_STR "@(#)$Id$" 
/*  */
/* Defines for MSG_NW Domain */
#define MSG_NW_SET 2



/* This revision string is not to be translated */
#define NWU_REVISION 1
/* This copyright string is not to be translated */
#define NWU_COPYRIGHT 2
#define NWU_CONFIG_FILE 3
#define NWU_INVALID_CONSOLE 4
#define NWU_LOG_FILE_ERROR 5
#define NWU_RUN_AS_ROOT 6

#define NWU_DRIVER_LINK_FAILURE 7

#define NWU_MAX_CONNECTIONS 8
#define NWU_BAD_SYS_VOLUME 9
#define NWU_NCPIPX_INITIALIZATION_FAILURE 10
#define NWU_SHM_SIZE 11
#define NWU_SHM_SIZE_TOO_SMALL 12
#define NWU_DEFAULT_LICENSED_CONNS 13
#define NWU_CREATE_SHM_FAILURE 14
#define NWU_SHARED_MEMORY_INIT_FAILURE 15
#define NWU_SHM_NOT_INITIALIZED 16
#define NWU_SHARED_MEMORY_ATTACH_FAILURE 17
#define NWU_NO_MORE_LICENSED_CONNECTIONS 18
#define NWU_ABORT 19
#define NWU_ADDRESS_FAILURE 20
#define NWU_SERVER_ADDRESS 21
#define NWU_INVALID_SERVER_NAME 22
#define NWU_NO_SYS_VOLUME 23
#define NWU_INIT_FILE_SYSTEM_FAILURE 24
#define NWU_SHM_ALLOC_FAIL 25
#define NWU_LOC_ALLOC_FAIL 26
#define NWU_VOL0_WRONG_NAME 27
#define NWU_VOLUME_MOUNT_ERROR 28
#define NWU_VOLUME_MOUNT_SUCCESS 29
#define NWU_VOLUME_MOUNT_NPFS 30
#define NWU_NWUSERS_NOT_DEFINED 31
#define NWU_NWUSERS_NOT_ALLOWED 32
#define NWU_NWUSERS_ERROR 33
#define NWU_HOST_USER_ERROR 34

#define NWU_XFS_INVALID_ACCESS_CONTROL_OPTION 35

#define NWU_INIT_NDS 36
#define NWU_DS_INSTALL_FAIL 37
#define NWU_DS_LOAD_FAIL 38
#define NWU_DS_ERR_ADD_REPLICA 39
#define NWU_DS_ADDING_REPLICA 40
#define NWU_VOLUME_MOUNT_IN_PROGRESS 41
#define NWU_ERR_DS_CAN_UNLOAD 42
#define NWU_IOCTL_NEMUX_FAILURE 43
#define NWU_DAEMON_FAILURE 44
#define NWU_PUT_MESSAGE_FAILED 45
#define NWU_INIT_TRUSTEES 46
#define NWU_STREAM_BUFFER_READ_ERROR 47
#define NWU_COMM_SETUP 48
#define NWU_NO_BIN_DIRECTORY 50
#define NWU_SERVER_IS_UP 51
#define NWU_CLEAN_SHUTDOWN 52
#define NWU_ENGINES_TO_START_ERROR 53
#define NWU_NEMUX_CLONE_OPEN_FAIL 54
#define NWU_SPAWN_PROCESS_ERROR 55
#define NWU_IOCTL_GET_IDENTITY 56
#define NWU_IOCTL_SHUTDOWN 57
#define NWU_SETSID_FAILURE 58
#define NWU_VPID_FAILURE 59
#define NWU_SERVER_DOWN 60
#define NWU_STOP 61
#define NWU_SERVER_IS_DOWN 62
#define NWU_SERVER_INIT_FAILED 63
#define NWU_FILE_SYSTEM_I18N_FAILED 64
#define NWU_FORK_FAILURE 65
#define NWU_DAEMON_SPAWN_FAILURE 66
#define NWU_VPID_TABLE_ERROR 67
#define NWU_VGL_TREE_ERROR 68
#define NWU_SERVER_NEEDS_DSINSTALL 69
#define NWU_SERVER_NEEDS_DSREPAIR 70
#define NWU_HEAP_FAILURE 71
#define NWU_INFORM_LOGFILE_WRITE 72
#define NWU_INFORM_LOGFILE_SIZE 73
#define NWU_INFORM_LOGFILE_CREATE 74
#define NWU_INFORM_CONSOLE_NOT_OPEN 75
#define NWU_SERVER_CORE_DUMPED 76
#define NWU_SERVER_ID_FAILURE 77
#define NWU_NCP_ENGINE_SPAWN_FAILURE 78
#define NWU_NWU_DAEMON_EXIT 79
#define NWU_SHUTDOWN_REQUEST 80
#define NWU_UNKNOWN_REQUEST 81
#define NWU_NEMUX_GETMSG_FAILURE 82
#define NWU_WATCHDOG_LOGOUT 83
#define NWU_WATCHDOG_DETACH 84
#define NWU_LOGOUT_CONNECTION 85
#define NWU_FIVE_MIN_TERMINATED 86
#define NWU_WATCHDOG_LOCK_BUSY 87
#define NWU_CONTROL_DIRECTORY_MISSING 88
#define NWU_SERVER_IS_COMMING_UP 89
#define NWU_SERVER_IS_GOING_DOWN 90
#define NWU_NOTIFY_STATIONS_SERVER_DOWN 91
#define NWU_CM_ERROR 95
#define NWU_CM_SYNTAX_ERROR 96
#define NWU_CM_NOT_INITIALIZED 97
#define NWU_CM_INVALID_TYPE 99
#define NWU_CM_PARAM_NOT_FOUND 100
#define NWU_TOO_MANY_ENGINES_TO_START 103
#define NWU_SERVER_NAME_CHANGED 110
#define NWU_NET_NUM_CHANGED 111
#define NWU_SHM_SIZE_CHANGED 112
#define NWU_ENGINES_TO_START_CHANGED 113
#define NWU_MAX_ENGINES_CHANGED 114
#define NWU_MAX_CONNECTIONS_CHANGED 115
#define NWU_MAX_VOLUMES_CHANGED 116
#define NWU_INIT_QMS_ERROR 117
#define NWU_INIT_ACCOUNT_ERROR 118
#define NWU_INIT_SPOOL_ERROR 119
#define NWU_MAX_CONNECTIONS_TOO_LOW 120

#define NWU_RUN_UTILITY_AS_ROOT 122
#define NWU_NWSHUT_DOWN_IN_1_HOUR 124
#define NWU_NWSHUT_DOWN_IN_X_HOURS 125
#define NWU_NWSHUT_SERVER_DOWN_IN_X_HOURS 126
#define NWU_NWSHUT_DOWN_IN_X_MINUTES 127
#define NWU_NWSHUT_SERVER_DOWN_IN_X_MINUTES 128
#define NWU_NWSHUT_COMPLETED 129
#define NWU_NWSHUT_STARTED 130
#define NWU_NWSHUT_TRY_FORCE 131
#define NWU_NWSHUT_DOWN_IN_X_SECONDS 132
#define NWU_NWSHUT_SERVER_DOWN_IN_X_SECONDS 133
#define NWU_NWSHUT_ABORT_QUESTION 134
#define NWU_NWSHUT_SERVER_SHUTDOWN_ABORTED 135
#define NWU_NWSHUT_USAGE 136
#define NWU_NWSHUT_UNKNOWN_FLAG 137
#define NWU_NWSHUT_UNKNOWN_ARGUMENT 138
#define NWU_GET_CHANGED_SERVERS_ERROR 139
#define NWU_CANT_ADVERTIZE_SERVER 140
#define NWU_SAP_INET 142
#define NWU_SAP_NODE 143
#define NWU_SAP_SOCKET 144
#define NWU_GET_ALL_SERVERS_ERROR 150
#define NWU_TRUSTEE_INIT_FAILURE 151
#define NWU_TRUSTEE_OPEN_FAILURE 152
#define NWU_LOG_TRUSTEE_CHANGE_FAILURE 153
#define NWU_TRUSTEE_CHANGE_FAILURE 154
#define NWU_TRUSTEE_NOT_VALID 155
#define NWU_TRUSTEE_NEW_FILE 156
#define NWU_UNKNOWN_NCP_SERVICE_CATEGORY 160
#define NWU_UNKNOWN_NCP 161
#define NWU_BAD_SEQUENCE_NUMBER 162
#define NWU_CLEARTEXT_PASSWORD_USED 163
#define NWU_SERVER_DOWN_REMOTE 164
#define NWU_REATTACH_CLIENT 166
#define NWU_CONNECTION_SLOT_FAILURE 167
#define NWU_CONNECTION_FAILURE 168
#define NWU_SHUTDOWN_SIGNAL_IGNORED 174
#define NWU_NEMUX_INIT_ERROR 176
#define NWU_SIGNAL_SHUTDOWN 177
#define NWU_UNEXPECTED_SIGNAL 181
#define NWU_MAX_CONNECTS_REACHED 182
#define NWU_NO_MORE_CONNECTIONS 183
#define NWU_RECEIVED_HANGUP_SIGNAL 198
#define NWU_RECEIVED_HANGUP_MESSAGE 199
#define NWU_GET_NEG_PACKET_SIZE_FAILURE 200
#define NWU_BURST_MODE_INVALID_FUNCTION 203
#define NWU_BURST_MODE_FOR_CLIENT_NOT_ENABLED 204
#define NWU_BURST_CONNECTION_DENIED 207
#define NWU_NO_MORE_BURST_CONNECTIONS 209
#define NWU_CREATE_CACHE_FAILURE 210
#define NWU_INVALID_SIGNATURE_WITH_NAME 211
#define NWU_INVALID_SIGNATURE 212
#define NWU_BURST_FRAGMENT_ERROR 213
#define NWU_RESTART_BURST_WRITE 214
#define NWU_PASSWORD_FAILURE_LOGOUT 215
#define NWU_SIGNATURES_REQUIRED 216
#define NWU_INVALID_BLAST_PACKET 217
#define NWU_CONNECTION_ENTRY_INVALID 220
#define NWU_INVALID_CLIENT 222
#define NWU_UNKNOWN_PROTOCOL 223
#define NWU_TIMER_DAEMON_INIT_ERROR 225
#define NWU_LOGICAL_LOCK_DAEMON_INIT_ERROR 226
#define NWU_OPEN_NETACCT_FAILED 227
#define NWU_ACCT_BAL_EXCEEDED 228
#define NWU_ACCT_DELETED 229
#define NWU_CONN_TERMINATED 230
#define NWU_MESSAGE_LIST_TRANSMIT_FAILURE 231
#define NWU_WATCHDOG_LIST_TRANSMIT_FAILURE 232
#define NWU_SRVR_CONN_TERMINATED 233
#define NWU_SRVR_CONNS_TERMINATED 234
#define NWU_MWMSGQ_DS_JANITOR_CREATE_ERROR 240
#define NWU_RUN_DS_JANITOR_ERROR 241
#define NWU_SPAWN_JANITOR_FAILED 242
#define NWU_DS_JANITOR_WRONG_ARG_COUNT 243
#define NWU_MWMSGQ_DS_BACKGRD_CREATE_ERROR 245
#define NWU_RUN_DS_BACKGRD_ERROR 246
#define NWU_SPAWN_BACKGRD_FAILED 247
#define NWU_DS_BACKGRD_WRONG_ARG_COUNT 248
#define NWU_MWMSGQ_DS_SKULKER_CREATE_ERROR 250
#define NWU_RUN_DS_SKULKER_ERROR 251
#define NWU_SPAWN_SKULKER_FAILED 252
#define NWU_DS_SKULKER_WRONG_ARG_COUNT 253
#define NWU_SFI_ENFILE 256
#define NWU_SFI_EMFILE 257
#define NWU_SERVICE_CONNECTION_REJECTED 258
#define NWU_LOGIN_REJECTED 259
#define NWU_SYSCFG_SAVE_FAIL 260
#define NWU_SFD_FAILURE 261
#define NWU_LOGIN_SUCCESS 262
#define NWU_LOGOUT_SUCCESS 263
#define NWU_SERVER_LOGIN_DISABLED 264
#define NWU_SERVER_LOGIN_ENABLED 265
#define NWU_SERVER_LOGIN_DISABLED_BY_CONSOLE 266
#define NWU_SERVER_LOGIN_ENABLED_BY_CONSOLE 267
#define NWU_FCNTL_FAILED 268
#define NWU_SET_PRIORITY_ERROR 270
#define NWU_SERVER_LOCK_INIT_ERROR 273
#define NWU_IPC_INIT_ERROR 274
#define NWU_IPC_LOST_ERROR 275
#define NWU_LOAD_BALANCE_ERROR 276
#define NWU_IPX_OPEN_FAILED 277
#define NWU_NCPIPX_OPEN_FAILED 278
#define NWU_PKBIPX_OPEN_FAILED 279
#define NWU_SET_ITIMER_FAILED 280
#define NWU_TIMER_PID_FILE_OPEN_ERROR 281
#define NWU_IPX_INTERNAL_NET_NOT_SET 282
#define NWU_NO_LOCK_NO_SHUT 283
#define NWU_SHM_VPID_POOL_CREATE_ERROR 300
#define NWU_SHM_VGLMISC_POOL_CREATE_ERROR 301
#define NWU_SHM_IO_POOL_CREATE_ERROR 302
#define NWU_SHM_MAN_POOL_CREATE_ERROR 303
#define NWU_SHM_CONNTABLE_POOL_CREATE_ERROR 304
#define NWU_SHM_MISC_POOL_CREATE_ERROR 305
#define NWU_SHM_FILELOCK_POOL_CREATE_ERROR 306
#define NWU_SHM_FILESYS_POOL_CREATE_ERROR 307
#define NWU_SHM_LOGLOCK_POOL_CREATE_ERROR 308
#define NWU_SHM_PHYSLOCK_POOL_CREATE_ERROR 309
#define NWU_SHM_SEMA_POOL_CREATE_ERROR 310
#define NWU_SHM_NDS_POOL_CREATE_ERROR 311
#define NWU_SHM_TS_POOL_CREATE_ERROR 312
#define NWU_SPAWN_NTSD_FAILED 313
#define NWU_NTSD_WRONG_ARG_COUNT 314

#define NWU_NPFS_DATA_FORK_RECOVERY_FAILED 400

#define NWU_NPFS_DATA_FORK_DELETE_FAILED_DURING_RENAME 401

#define NWU_NPFS_SUB_FORK_DELETE_FAILED_DURING_RENAME 402

#define NWU_NPFS_GNODE_REDIRECT_UPDATE_FAILURE 403

#define NWU_NPFS_VOLUME_MOUNT_DISCOVERED_INCONSISTENCIES 404

#define NWU_NPFS_VOLUME_MOUNT_LOG_FILE_OPEN_FAILURE 405

#define NWU_NPFS_VOLUME_MOUNT_LOG_FILE_CLOSE_FAILURE 407

#define NWU_NPFS_VOLUME_REVISION_CHECK_FAILED 408

#define NWU_NPFS_VOLUME_MOUNT_POINT_CHANGE 409

#define NWU_NPFS_VOLUME_NAME_SPACE_CHANGE 410

#define NWU_NPFS_MAC_RECORD_NOT_FOUND 411

#define NWU_NPFS_MAC_OPERATION_REQUIRES_MAC_NAMESPACE 412

#define NWU_NPFS_DIR_RECORD_NOT_FOUND 413

#define NWU_NPFS_INFORM_VOLUME_FULL_NO_INODES 414

#define NWU_NPFS_CANNOT_SYNC_DIRECTORY 415

#define NWU_NPFS_CANNOT_SYNC_DIRECTORY_OPEN 416

#define NWU_NPFS_FILE_STAT_FAILED 417

#define NWU_NPFS_ADD_FILE_FAILED 418

#define NWU_NPFS_CANNOT_SYNC_DIRECTORY_CLOSE 419

#define NWU_NPFS_VOLUME_PURGE_INPROGRESS 420

#define NWU_NPFS_ERROR_NO_EXTENDED_NAMES_FILE 421

#define NWU_NPFS_USINODES_EXISTS_CONTROL_DIRECTORY_CORRUPT 422

#define NWU_NPFS_ERROR_TRUSTEE_FILE_EXISTS 423

#define NWU_NPFS_ERROR_MAC_DESKTOP_DATABASE_FILE_EXISTS 424

#define NWU_NPFS_USINODES_FILE_MISSING_CONTROL_DIRECTORY_CORRUPT 425

#define NWU_NPFS_ERROR_TRUSTEE_FILE_MISSING 426

#define NWU_NPFS_ERROR_EXTENDED_NAMES_FILE_EXISTS 427

#define NWU_NPFS_INFORM_VOLUME_FULL_NO_INODES_EXTRA 428

#define NWU_NPFS_VOLUME_EXPAND_ERROR 429

#define NWU_NPFS_ERROR_REMOUNT_FAILED_ERASE_FILE 430

#define NWU_NPFS_CDROM_REMOUNT_SUCCESS 431


#define NWU_NPFS_LOG_MAC_FORK_REATTACHED 500
#define NWU_NPFS_LOG_MAC_FORK_NOT_FOUND 501

#define NWU_NPFS_LOG_OS2_FORK_REATTACHED 502
#define NWU_NPFS_LOG_OS2_FORK_NOT_FOUND 503

#define NWU_NPFS_LOG_DATA_FORK_NOT_FOUND 504

#define NWU_VOL_CDROM_NAME 590
#define NWU_VOL_STANDARD_NAME 591
#define NWU_VOL_EXTENDED_NAME 592
#define NWU_VOL_READ_ONLY_NAME 593
#define NWU_VOL_READ_WRITE_NAME 594
#define NWU_VOL_NETWARE_NAME 595
#define NWU_VOL_UNIX_NAME 596
#define NWU_VOL_BOTH_NAME 597
#define NWU_VOL_NONE_NAME 598

#define NWU_INVALID_LICENSE_FILE_SIZE 601
#define NWU_INVALID_LICENSE_FILE_PRODUCT_ID 602
#define NWU_INVALID_LICENSE_FILE_MAJOR_VERSION 603
#define NWU_INVALID_LICENSE_FILE_MINOR_VERSION 604
#define NWU_VALID_LICENSE 605
#define NWU_INVALID_LICENSE 606
#define NWU_NO_MORE_LICENSE_FILES 607
#define NWU_LICENSE_FILE_PROCESSING_COMPLETE 608
#define NWU_DUPLICATE_LICENSE_FILE 609
#define NWU_LICENSE_NOT_NWS 610
#define NWU_SERVER_LICENSE_VIOLATION 611
#define NWU_LICENSE_PACKET_GETMSG_FAILURE 612
#define NWU_LICENSE_PACKET_PUTMSG_FAILURE 613
#define NWU_SERVER_DUPLICATE_LICENSE 614
#define NWU_INVALID_LICENSE_FILE_SITE_ID 615

#define NWU_NCPX_CANNOT_GET_SHM_KEY 701
#define NWU_NCPX_CANNOT_GET_SHM_ACCESS 702
#define NWU_NCPX_CANNOT_ATTACH_SHM 703
#define NWU_NCPX_POOL_CREATE_ERROR 705
#define NWU_NCPX_CHILD_CAUGHT_HANGUP 706
#define NWU_NCPX_CHILD_CAUGHT_ERROR 707
#define NWU_NCPX_HANDLER_OVERWROTE_BUFFER 708
#define NWU_NCPX_EVENT_REGISTRATION_CLEAR_CONN_FAILURE 709
#define NWU_NCPX_EVENT_REGISTRATION_LOGOUT_CONN_FAILURE 710
#define NWU_NCPX_EVENT_REGISTRATION_SERVER_DOWN_FAILURE 711
#define NWU_NCPX_ALLOC_FAILURE_ON_CONN_EVENT_NOTIFICATION 712
#define NWU_NCPX_MSGQ_SEND_FAILURE_ON_CONN_EVENT_NOTIFICATION 713
#define NWU_NCPX_ALLOC_FAILURE_ON_SERVER_DOWN_EVENT_NOTIFICATION 714
#define NWU_NCPX_MSGQ_SEND_FAILURE_ON_SERVER_DOWN_EVENT_NOTIFICATION 715
#define NWU_NCPX_MSGQ_SEND_FAILURE_ON_NCP_DISPATCH 716
#define NWU_NCPX_SERVER_DOWN_WITH_EXTENSIONS_STILL_REGISTERED 717
#define NWU_NCPX_NWUSERS_NOT_DEFINED 718
#define NWU_NCPX_PROCPRIV_SETPRV_FAILURE 719
#define NWU_NCPX_PROCPRIV_CNTPRV_FAILURE 720
#define NWU_NCPX_MALLOC_FAILURE 721
#define NWU_NCPX_PROCPRIV_GETPRV_FAILURE 722
#define NWU_NCPX_NEED_PRIVS 723
#define NWU_NCPX_FORK_FAILURE 724

#define LOGIN_910 910
#define LOGIN_911 911
#define LOGIN_916 916
#define LOGIN_928 928
#define LOGIN_931 931
#define LOGIN_933 933
#define LOGIN_1373 1373
#define NOSTIME_2664 2664
#define NOSTIME_2665 2665
#define NWU_NOT_LOGGED_IN 3156
#define LOGIN_3157 3157
#define LOGIN_3158 3158
#define LOGIN_3162 3162
#define LOGIN_3163 3163
#define LOGIN_3192 3192
#define LOGIN_3735 3735
#define LOGIN_3736 3736

#define NWU_RLIMIT_FAILED 4250
#define DTDB_SHARED_STRUCT_ALLOC_FAILED 5001
#define DTDB_DB_DIR_ACCESS_ERROR 5002
#define DTDB_APPLDB_TRUNC_ERROR 5003
#define DTDB_ICONDB_TRUNC_ERROR 5004
#define DTDB_APPLDB_ERROR 5005
#define DTDB_ICONDB_ERROR 5006
#define DTDB_COMMDB_ERROR 5007
#define DTDB_ICON_ALLOC_ERROR 5008
#define DTDB_PSEUDONYM_UNLINK_ERROR 5009
#define DTDB_PSEUDONYM_ACCESS_ERROR 5010
#define DTDB_PSEUDONYM_SET_ERROR 5011

#define MSG_NWVM_SET 3


#define NWVM_CMERR_VOLTAB 1
#define NWVM_OPENFAIL 2
#define NWVM_CMERR_MAX_VOLUMES 3
#define NWVM_OUT_OF_LOCAL_MEMORY 4
#define NWVM_VOLTAB_CORRUPT 5
#define NWVM_NO_MORE_ENTRIES 6
#define NWVM_ENTRY_NOT_FOUND 7
#define NWVM_VOLTAB_READ_ONLY 8
#define NWVM_ILLEGAL_SYSVOL_NAME 9
#define NWVM_DUPLICATE_VOLUME_NAME 10
#define NWVM_VOLUME_LIMIT_EXCEEDED 11
#define NWVM_DUPLICATE_HOST_PATH 12
#define NWVM_DUPLICATE_CONTROL_PATH 13
#define NWVM_MOD_NS_FAILED 15
#define NWVM_MOUNT_PATH_NOT_EXIST 16
#define NWVM_CONTROL_PATH_NOT_EXIST 17
#define NWVM_INVALID_NAME_SPACES 18
#define NWVM_ANCESTOR_CONTROL_PATH 19
#define NWVM_DESCENDANT_CONTROL_PATH 20
#define NWVM_ANCESTOR_MOUNT_POINT 21
#define NWVM_DESCENDANT_MOUNT_POINT 22
#define NWVM_ANCESTOR_CONTROL_MOUNT 23
#define NWVM_DESCENDANT_CONTROL_MOUNT 24
#define NWVM_CONTROL_MOUNT_SAME 25
#define NWVM_INVALID_VOL_NAME 26
#define NWVM_REPAIR_ERROR 27
#define NWVM_OPTIMIZE_ERROR 28
#define NWVM_VOL_NAME_TOO_SHORT 29
#define NWVM_VOL_NAME_TOO_LONG 30
#define NWVM_MOD_MOUNT_POINT_FAILED 31
#define NWVM_CMD_DUP_TYPE_OPT 50
#define NWVM_CMD_DUP_PATH_OPT 51
#define NWVM_CMD_DUP_NAMESPACE_OPT 52
#define NWVM_CMD_DUP_NAME_OPT 53
#define NWVM_CMD_DUP_CONTROL_OPT 54
#define NWVM_CMD_DUP_OPTIONS_OPT 55
#define NWVM_CMD_INVAL_TYPE_OPT 56
#define NWVM_CMD_COMMAND_MUTL_EXCL 57
#define NWVM_CMD_USAGE_1 101
#define NWVM_CMD_USAGE_2 102
#define NWVM_CMD_USAGE_3 103
#define NWVM_CMD_USAGE_4 104
#define NWVM_CMD_USAGE_5 105
#define NWVM_CMD_USAGE_6 106
#define NWVM_CMD_USAGE_7 107
#define NWVM_CMD_USAGE_8 108
#define NWVM_CMD_USAGE_9 109
#define NWVM_CMD_USAGE_10 110
#define NWVM_CMD_USAGE_11 111
#define NWVM_CMD_USAGE_12 112
#define NWVM_CMD_USAGE_13 113
#define NWVM_CMD_USAGE_14 114
#define NWVM_CMD_MISSING_VOLSPEC 200
#define NWVM_CMD_TOO_MANY_ARGS 201
#define NWVM_CMD_VOLSPEC_ON_CREATE 202
#define NWVM_CMD_CREAT_REQ_NAME 203
#define NWVM_CMD_CREAT_REQ_PATH 204
#define NWVM_CMD_CREAT_NO_TYPE 205
#define NWVM_CMD_CREAT_REQ_CONTROL 206
#define NWVM_CMD_CREAT_EXTRA_CONTROL 207
#define NWVM_CMD_COULDNT_GET_VOL 208
#define NWVM_CMD_CREATE_FAILED 209
#define NWVM_CMD_UPDATE_FAILED 210
#define NWVM_CMD_COULDNT_DELETE_VOL 211
#define NWVM_CMD_NEED_ONE_CMD 231
#define NWVM_CMD_ADD_NEEDS_PATHS 232
#define NWVM_CMD_DELETE_TAKES_NONE 233
#define NWVM_CMD_MODIFY_NEEDS_ONE 234
#define NWVM_CMD_OPTIMIZE_TAKES_NONE 235
#define NWVM_CMD_REPAIR_TAKES_NONE 236
#define NWVM_CMD_ASSUME_NPFS 237
#define NWVM_CMD_ASSUME_DOS_UNIX 238
#define NWVM_CMD_ASSUME_DEFAULT_OPTIONS 239
#define NWVM_CMD_COULDNT_ADD_VOL 240
#define NWVM_CMD_COULDNT_MODIFY_VOL 241
#define NWVM_CMD_COULDNT_REPAIR_VOL 242
#define NWVM_CMD_COULDNT_OPTIMIZE_VOL 243
#define NWVM_CMD_NUMBER_LABEL 300
#define NWVM_CMD_NAME_LABEL 301
#define NWVM_CMD_TYPE_LABEL 302
#define NWVM_CMD_PATH_LABEL 303
#define NWVM_CMD_NAMESPACE_LABEL 304
#define NWVM_CMD_CONTROL_LABEL 305
#define NWVM_CMD_OPTIONS_LABEL 306
#define NWVM_CMD_UPDATE_REQ_CONTROL 307
#define NWVM_CMD_UPDATE_EXTRA_CONTROL 308
#define NWVM_CMD_VOLUME_MOUNTED 309
#define NWVM_CMD_NOT_ROOT 310
#define NWVM_CMD_EXIT 311
#define NWVM_CMD_SERVER_STRUCT_FAILURE 312
#define NWVM_CMD_I18N__FAILURE 313


#define NWVM_OLD_INODES_SAVED 513
#define NWVM_OLD_EXTNAMES_SAVED 514
#define NWVM_UNSUPPORTED_OPTION 515
#define NWVM_ADD_DIR_ENTRY_VERBOSE 516
#define NWVM_PROCESS_EMPTY_DIRECTORY_VERBOSE 517
#define NWVM_PROCESS_DIRECTORY_VERBOSE 518
#define NWVM_DIR_ENTRY_ASSIGNED_BLOCK_VERBOSE 519
#define NWVM_DONE_PROCESSING_DIR_VERBOSE 520
#define NWVM_NOTHING_ENTERED 521
#define NWVM_REMOVING_OLD_INODES_FILE_VERBOSE 522
#define NWVM_CHANGE_MOUNT_POINT_QUESTION 523
#define NWVM_USAGE_1 524
#define NWVM_USAGE_2 525
#define NWVM_USAGE_3 526
#define NWVM_USAGE_4 527
#define NWVM_ENTER_MOUNT_POINT 529
#define NWVM_MOUNT_POINT_TOO_LONG 530
#define NWVM_MOUNT_POINT_ALREADY_USED 531
#define NWVM_SELECT_DIFFERENT_MOUNT_POINT 532
#define NWVM_MOUNT_POINT_DOES_NOT_EXIST 533
#define NWVM_ADJUST_HIGHEST_ALLOC_BLOCK 534
#define NWVM_MANUAL_SET_MOUNT_POINT 535
#define NWVM_ADJUST_FIRST_FREE_BLOCK 536
#define NWVM_ADJUST_FIRST_DIR_BLOCK 537
#define NWVM_ADJUST_NEXT_DIR_BLOCK 538
#define NWVM_ADD_BLOCK_TO_FREE_LIST 539
#define NWVM_BAD_FREE_DIR_BLOCK_ORDER 540
#define NWVM_VERIFY_MORE_THAN_ONE_ERROR 541

#define NWVM_NO_MEMORY 551
#define NWVM_SFD_INIT_FAILURE 552
#define NWVM_NO_VOLUMES 553
#define NWVM_VOLUME_NOT_VALID 554
#define NWVM_VOLTAB_UPDATE_ERROR 555
#define NWVM_INODES_FILE_MODIFY_ERROR 556
#define NWVM_INODES_EXTEND_FAILURE 558
#define NWVM_LONG_MOUNT_POINT_RETRIEVE_ERROR 559
#define NWVM_LONG_MOUNT_POINT_ADD_ERROR 560
#define NWVM_LONG_UNIX_NAME_RETRIEVE_ERROR 561
#define NWVM_LONG_DIR_NAME_RETRIEVE_ERROR 562
#define NWVM_LONG_DIR_NAME_ADD_ERROR 563
#define NWVM_LONG_OS2_NAME_RETRIEVE_ERROR 564
#define NWVM_LONG_OS2_NAME_ADD_ERROR 565
#define NWVM_DIR_ENTRY_NOT_FOUND 566
#define NWVM_CANT_ADD_DELETE_UNIX_NS 567
#define NWVM_CANT_ADD_DELETE_DOS_NS 568
#define NWVM_NS_INVALID 569
#define NWVM_INVALID_ACTION 570
#define NWVM_MAC_NS_ALREADY_ACTIVE 571
#define NWVM_MAC_NS_NOT_ACTIVE 572
#define NWVM_OS2_NS_ALREADY_ACTIVE 573
#define NWVM_OS2_NS_NOT_ACTIVE 574
#define NWVM_STAT_FAILED 575
#define NWVM_INODES_OPEN_FAILED 576
#define NWVM_FILE_ALREADY_EXISTS 577
#define NWVM_CANT_CREATE_INODES_FILE 578
#define NWVM_CANT_OPEN_EXT_NAMES_FILE 579
#define NWVM_CANT_CREATE_EXT_NAMES_FILE 580
#define NWVM_CANT_READ_TRUSTEE_INFO 584
#define NWVM_VERIFY_ZERO_ERRORS 585
#define NWVM_VERIFY_ONE_ERROR 586
#define NWVM_VOLTAB_BACKUP_SAVED 587
#define NWVM_CANT_CREATE_MOUNT_POINT 588


/*  */
/* NWU Helps and Parameter Descriptions */
/*  */
#define MSG_NWU_DH_SET 5

/*  */
/* Description Messages */
/*  */
#define NWCM_PN_SHM_KEY 1
#define NWCM_PN_SHM_SIZE 2
#define NWCM_PN_SHM_ACCESS 3
#define NWCM_PN_MAX_CONNECTIONS 4
#define NWCM_PN_MAX_NCP_ENGINES 5
#define NWCM_PN_NCP_ENGINES_TO_START 6
#define NWCM_PN_MAX_VOLUMES 7
#define NWCM_PN_MAX_OPEN_FILES 8
#define NWCM_PN_MAX_CONTEXTS 9
#define NWCM_PN_HYBRID_USERS_ENABLED 11
#define NWCM_PN_HYBRID_ALLOW_NWUSER 12
#define NWCM_PN_NWU_DEFAULT 13
#define NWCM_PN_FILE_ACCESS_CONTROL 14
#define NWCM_PN_FILE_UMASK 15
#define NWCM_PN_NPFS_MIN_SYNC_INTERVAL 16
#define NWCM_PN_NPFS_PURGE_THRESHOLD 17
#define NWCM_PN_NPFS_MANDATORY_SYNC_INTERVAL 18
#define NWCM_PN_READ_AHEAD_ENABLED 20
#define NWCM_PN_READ_AHEAD_CACHE_BUFFER_SIZE 21
#define NWCM_PN_BURST_MODE_PROTOCOL 22
#define NWCM_PN_BURST_MODE_BUFFER_SIZE 23
#define NWCM_PN_BURST_MODE_CLIENTS 25
#define NWCM_PN_NEMUX_ERROR_ACTION 26
#define NWCM_PN_LOGIN 27
#define NWCM_PN_NATIVE_LOCKS 28

#define NWCM_PN_CONSOLE_DEVICE 31
#define NWCM_PN_ERR_LOG_FILE_SIZE 32
#define NWCM_PN_ALLOW_UNENCRYPTED_PASSWORDS 33
#define NWCM_PN_LOG_WATCHDOG_LOGOUTS 34
#define NWCM_PN_CONSOLE_DISPLAY_WATCHDOG_LOGOUTS 35
#define NWCM_PN_ALERT_NOTIFY_STRING 36

#define NWCM_PN_NUMBER_OF_WATCHDOG_PACKETS 51
#define NWCM_PN_DELAY_BETWEEN_WATCHDOG_PACKETS 52
#define NWCM_PN_DELAY_BEFORE_FIRST_WATCHDOG_PACKET 53
#define NWCM_PN_HOST_CODE_SET 54
#define NWCM_PN_CODE_PAGE 55
#define NWCM_PN_COUNTRY_CODE 56
#define NWCM_PN_ALLOW_CHANGE_TO_CLIENT_RIGHTS 57
#define NWCM_PN_HYBRID_SETUID_ENABLED 66
#define NWCM_PN_NWS_START_AT_BOOT 67
#define NWCM_PN_SIGNATURE_OPTION 68
#define NWCM_PN_CHECKSUMS 69
#define NWCM_PN_ALLOW_LIP 70
#define NWCM_PN_VOLTAB 71
#define NWCM_PN_DEBUG 72
#define NWCM_PN_COR_LIM 400
#define NWCM_PN_CPU_LIM 401
#define NWCM_PN_DAT_LIM 402
#define NWCM_PN_FSZ_LIM 403
#define NWCM_PN_STK_LIM 404
#define NWCM_PN_VMM_LIM 405
#define NWCM_PN_FNO_LIM 406

/*  */
/* Help Messages */
/*  */
#define NWCM_PH_SHM_KEY 101

#define NWCM_PH_SHM_SIZE 102
#define NWCM_PH_SHM_ACCESS 103
#define NWCM_PH_MAX_CONNECTIONS 104
#define NWCM_PH_MAX_NCP_ENGINES 105
#define NWCM_PH_NCP_ENGINES_TO_START 106
#define NWCM_PH_MAX_VOLUMES 107
#define NWCM_PH_MAX_OPEN_FILES 108
#define NWCM_PH_MAX_CONTEXTS 109
#define NWCM_PH_VOLTAB 110
#define NWCM_PH_HYBRID_USERS_ENABLED 111
#define NWCM_PH_HYBRID_ALLOW_NWUSER 112
#define NWCM_PH_FILE_ACCESS_CONTROL 114
#define NWCM_PH_FILE_UMASK 115
#define NWCM_PH_NPFS_MIN_SYNC_INTERVAL 116
#define NWCM_PH_NPFS_PURGE_THRESHOLD 117
#define NWCM_PH_NPFS_MANDATORY_SYNC_INTERVAL 118
#define NWCM_PH_READ_AHEAD_ENABLED 120
#define NWCM_PH_READ_AHEAD_CACHE_BUFFER_SIZE 121
#define NWCM_PH_BURST_MODE_PROTOCOL 122
#define NWCM_PH_BURST_MODE_BUFFER_SIZE 123
#define NWCM_PH_BURST_MODE_CLIENTS 125
#define NWCM_PH_NEMUX_ERROR_ACTION 126
#define NWCM_PH_LOGIN 127
#define NWCM_PH_NATIVE_LOCKS 128
#define NWCM_PH_SIGNATURE_OPTION 129
#define NWCM_PH_CHECKSUMS 130
#define NWCM_PH_CONSOLE_DEVICE 131
#define NWCM_PH_ERR_LOG_FILE_SIZE 132
#define NWCM_PH_ALLOW_UNENCRYPTED_PASSWORDS 133
#define NWCM_PH_LOG_WATCHDOG_LOGOUTS 134
#define NWCM_PH_CONSOLE_DISPLAY_WATCHDOG_LOGOUTS 135
#define NWCM_PH_ALERT_NOTIFY_STRING 136
#define NWCM_PH_ALLOW_LIP 139
#define NWCM_PH_NUMBER_OF_WATCHDOG_PACKETS 151
#define NWCM_PH_DELAY_BETWEEN_WATCHDOG_PACKETS 152
#define NWCM_PH_DELAY_BEFORE_FIRST_WATCHDOG_PACKET 153
#define NWCM_PH_HOST_CODE_SET 154
#define NWCM_PH_CODE_PAGE 155
#define NWCM_PH_COUNTRY_CODE 156
#define NWCM_PH_ALLOW_CHANGE_TO_CLIENT_RIGHTS 175
#define NWCM_PH_HYBRID_SETUID_ENABLED 176
#define NWCM_PH_NWS_START_AT_BOOT 177
#define NWCM_PH_DEBUG 178
#define NWCM_NCP_ENGINES_TO_START_INVALID 179
#define NWCM_PH_COR_LIM 450
#define NWCM_PH_CPU_LIM 451
#define NWCM_PH_DAT_LIM 452
#define NWCM_PH_FSZ_LIM 453
#define NWCM_PH_STK_LIM 454
#define NWCM_PH_VMM_LIM 455
#define NWCM_PH_FNO_LIM 456


#endif /* for __nwumsgtable_h__ */
/* ************************* end of file ********************* */
