NAME			EUC

MB_CUR_MAX 		3

STATE_DEPEND_ENCODING	FALSE

#if EUC32
WC_ENCODING_MASK	0x30000000
WC_SHIFT_BITS		7
#else
WC_ENCODING_MASK	0x00008080
WC_SHIFT_BITS		8
#endif

CODESET0
    GL
    INITIAL_STATE_GL
    LENGTH		1
    WC_ENCODING		0x00000000
    ENCODING
	ISO8859-1	GL
	JISX0201.1976-0	GL
    FONT
	ISO8859-1	GL
	JISX0201.1976-0	GL

CODESET1
    GR
    INITIAL_STATE_GR
    LENGTH		2
#if EUC32
    WC_ENCODING		0x30000000
#else
    WC_ENCODING		0x00008080
#endif
    ENCODING
	JISX0208.1983-0 GL
	JISX0208.1983-0 GR
	JISX0208.1983-1 GL
	JISX0208.1983-1 GR
    FONT
	JISX0208.1983-0 GL
	JISX0208.1983-1 GR

CODESET2
    GR
    LENGTH		1
    MB_ENCODING		<SS> 0x8e
#if EUC32
    WC_ENCODING		0x10000000
#else
    WC_ENCODING		0x00000080
#endif
    ENCODING
	JISX0201.1976-0 GR
    FONT
	JISX0201.1976-0 GR
