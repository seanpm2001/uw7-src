#ifndef _IO_TSM_TOKTSM_TOKTXT_H
#define	_IO_TSM_TOKTSM_TOKTXT_H

#ident	"@(#)toktxt.h	2.1"
#ident	"$Header$"

#define	TOKENTSM_FRAME_TOKEN_RING_TXTMSG	"TOKEN-RING"
#define	TOKENTSM_FRAME_SNAP_TXTMSG		"TOKEN-RING_SNAP"
#define	TOKENTSM_MACTOK_TXTMSG			"MACTOK"
#define	TOKENTSM_NODE_EQUAL_TXTMSG		"NODE=%s"
#define	TOKENTSM_FRAME_EQUAL_TXTMSG		"FRAME=%s"
#define	TOKENTSM_STATS_VERSION_ERROR_MSG				\
	"This version of the statistics table is not supported.\n"
#define	TOKENTSM_MEMORY_ALLOC_ERROR_MSG					\
	"Unable to allocate memory.\n\r"

#define TOKENTSM_STAT_TOTAL_TX_MSG		"MTotalTxPacketCount"
#define	TOKENTSM_STAT_TOTAL_RX_MSG		"MTotalRxPacketCount"
#define	TOKENTSM_STAT_NO_ECB_MSG		"MNoECBAvailableCount"
#define	TOKENTSM_STAT_TX_BIG_MSG		"MPacketTxTooBigCount"
#define	TOKENTSM_STAT_TX_SMALL_MSG		"MPacketTxTooSmallCount"
#define	TOKENTSM_STAT_RX_OVERFLOW_MSG		"MPacketRxOverflowCount"
#define	TOKENTSM_STAT_RX_BIG_MSG		"MPacketRxTooBigCount"
#define	TOKENTSM_STAT_RX_SMALL_MSG		"MPacketRxTooSmallCount"
#define	TOKENTSM_STAT_TX_MISC_MSG		"MTotalTxMiscCount"
#define	TOKENTSM_STAT_RX_MISC_MSG		"MTotalRxMiscCount"
#define	TOKENTSM_STAT_RETRY_TX_MSG		"MRetryTxCount"
#define	TOKENTSM_STAT_CHECKSUM_MSG		"MChecksumErrorCount"
#define	TOKENTSM_STAT_RX_MISMATCH_MSG		"MHardwareRxMismatchCount"
#define	TOKENTSM_STAT_TX_OK_BYTE_MSG		"MTotalTxOKByteCount"
#define	TOKENTSM_STAT_RX_OK_BYTE_MSG		"MTotalRxOKByteCount"
#define	TOKENTSM_STAT_GRP_ADDR_TX_MSG		"MTotalGroupAddrTxCount"
#define	TOKENTSM_STAT_GRP_ADDR_RX_MSG		"MTotalGroupAddrRxCount"
#define	TOKENTSM_STAT_ADAPTER_RESET_MSG		"MAdapterResetCount"
#define	TOKENTSM_STAT_ADAP_OPR_TIME_MSG		"MAdapterOprTimeStamp"
#define	TOKENTSM_STAT_QDEPTH_MSG		"MQdepth"

#define	TOKENTSM_STAT_AC_ERROR_MSG		"TRN_ACErrorCount"
#define	TOKENTSM_STAT_ABORT_DELIMITER_MSG	"TRN_AbortDelimiterCount"
#define TOKENTSM_STAT_BURST_ERROR_MSG		"TRN_BurstErrorCount"
#define	TOKENTSM_STAT_FRAME_COPY_ERROR_MSG	"TRN_FrameCopiedErrorCount"
#define	TOKENTSM_STAT_FREQ_ERROR_MSG		"TRN_FrequencyErrorCount"
#define	TOKENTSM_STAT_INTERN_ERROR_MSG		"TRN_InternalErrorCount"
#define	TOKENTSM_STAT_LAST_RING_STATUS_MSG	"TRN_LastRingStatusCount"
#define	TOKENTSM_STAT_LINE_EROR_MSG		"TRN_LineErrorCount"
#define	TOKENTSM_STAT_LOST_FRAME_MSG		"TRN_LostFrameCount"
#define	TOKENTSM_STAT_TOKEN_ERROR_MSG		"TRN_TokenErrorCount"
#define	TOKENTSM_STAT_VALID_MASK1_MSG		"TRN_ValidMask1"
#define	TOKENTSM_STAT_UPSTREAM_NODE_H_MSG	"TRN_UpstreamNodeAddressHigh"
#define	TOKENTSM_STAT_UPSTREAM_NODE_L_MSG	"TRN_UpstreamNodeAddressLow"
#define	TOKENTSM_STAT_LAST_RING_ID_MSG		"TRN_LastRingID"
#define	TOKENTSM_STAT_LAST_BEACON_TYPE_MSG	"TRN_LastBeaconType"
#define	TOKENTSM_GRP_BIT_OVERRIDE_MSG					\
	"The group bit in the node address override was cleared.\n"
#define	TOKENTSM_LOCAL_BIT_OVERRIDE_MSG					\
	"The local bit in the node address override was set.\n"
#define	TOKENTSM_WHITESPACE_MSG			" \t\n"
#define	TOKENTSM_LSB_MSG			"LSB"
#define	TOKENTSM_STAT_UPSTREAM_NODE_MSG		"TRN_UpstreamNodeAddress"
#define	TOKENTSM_STAT_VALID_MASK1_MSG		"TRN_ValidMask1"
#define	TOKENTSM_STAT_UPSTREAM_NODE_H_MSG	"TRN_UpstreamNodeAddressHigh"
#define	TOKENTSM_STAT_UPSTREAM_NODE_L_MSG	"TRN_UpstreamNodeAddressLow"
#define	TOKENTSM_ODI_SPECVER_MSG		"ODI_CSPEC_VERSION: 1.10"

#endif	/* _IO_TSM_TOKTSM_TOKTXT_H */
