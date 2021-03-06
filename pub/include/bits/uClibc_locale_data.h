#define __lc_time_data_LEN		28528
#define __lc_time_rows_LEN		8250
#define __lc_time_item_offsets_LEN		50
#define __lc_time_item_idx_LEN		3339
#define __lc_numeric_data_LEN		23
#define __lc_numeric_rows_LEN		42
#define __lc_numeric_item_offsets_LEN		3
#define __lc_numeric_item_idx_LEN		13
#define __lc_monetary_data_LEN		1606
#define __lc_monetary_rows_LEN		3718
#define __lc_monetary_item_offsets_LEN		22
#define __lc_monetary_item_idx_LEN		396
#define __lc_messages_data_LEN		1724
#define __lc_messages_rows_LEN		316
#define __lc_messages_item_offsets_LEN		4
#define __lc_messages_item_idx_LEN		179
#define __lc_ctype_data_LEN		51
#define __lc_ctype_rows_LEN		20
#define __lc_ctype_item_offsets_LEN		10
#define __lc_ctype_item_idx_LEN		20
#define __CTYPE_HAS_UTF_8_LOCALES			1
#define __LOCALE_DATA_CATEGORIES			6
#define __LOCALE_DATA_WIDTH_LOCALES			9
#define __LOCALE_DATA_NUM_LOCALES			314
#define __LOCALE_DATA_NUM_LOCALE_NAMES		167
#define __LOCALE_DATA_AT_MODIFIERS_LENGTH		18
#define __lc_names_LEN		69
#define __lc_collate_data_LEN  91141
#define __CTYPE_HAS_8_BIT_LOCALES		1

#define __LOCALE_DATA_Cctype_IDX_SHIFT	3
#define __LOCALE_DATA_Cctype_IDX_LEN		16
#define __LOCALE_DATA_Cctype_ROW_LEN		4
#define __LOCALE_DATA_Cctype_PACKED		1

#define __LOCALE_DATA_Cuplow_IDX_SHIFT	3
#define __LOCALE_DATA_Cuplow_IDX_LEN		16
#define __LOCALE_DATA_Cuplow_ROW_LEN		8

#define __LOCALE_DATA_Cc2wc_IDX_LEN		16
#define __LOCALE_DATA_Cc2wc_IDX_SHIFT		3
#define __LOCALE_DATA_Cc2wc_ROW_LEN		8

typedef struct {
	unsigned char idx8ctype[16];
	unsigned char idx8uplow[16];
	unsigned char idx8c2wc[16];
	unsigned char idx8wc2c[38];
} __codeset_8_bit_t;


#define __LOCALE_DATA_Cwc2c_DOMAIN_MAX	0x25ff
#define __LOCALE_DATA_Cwc2c_TI_SHIFT		4
#define __LOCALE_DATA_Cwc2c_TT_SHIFT		4
#define __LOCALE_DATA_Cwc2c_II_LEN		38
#define __LOCALE_DATA_Cwc2c_TI_LEN		1072
#define __LOCALE_DATA_Cwc2c_TT_LEN		3456


#define __LOCALE_DATA_Cwc2c_TBL_LEN		4528

#define __LOCALE_DATA_Cuplow_TBL_LEN		504


#define __LOCALE_DATA_Cctype_TBL_LEN		420


#define __LOCALE_DATA_Cc2wc_TBL_LEN		1760



#define __LOCALE_DATA_NUM_CODESETS		23
#define __LOCALE_DATA_CODESET_LIST \
	"\x18\x22\x28\x2f\x36\x42\x4d\x59" \
	"\x65\x71\x7d\x89\x94\x9f\xaa\xb5" \
	"\xc0\xcb\xd6\xe1\xe8\xef\xf6" \
	"\0" \
	"ARMSCII-8\0" \
	"ASCII\0" \
	"CP1251\0" \
	"CP1255\0" \
	"GEORGIAN-PS\0" \
	"ISO-8859-1\0" \
	"ISO-8859-10\0" \
	"ISO-8859-13\0" \
	"ISO-8859-14\0" \
	"ISO-8859-15\0" \
	"ISO-8859-16\0" \
	"ISO-8859-2\0" \
	"ISO-8859-3\0" \
	"ISO-8859-4\0" \
	"ISO-8859-5\0" \
	"ISO-8859-6\0" \
	"ISO-8859-7\0" \
	"ISO-8859-8\0" \
	"ISO-8859-9\0" \
	"KOI8-R\0" \
	"KOI8-T\0" \
	"KOI8-U\0" \
	"TIS-620\0"

#define __CTYPE_HAS_CODESET_ARMSCII_8
#define __CTYPE_HAS_CODESET_ASCII
#define __CTYPE_HAS_CODESET_CP1251
#define __CTYPE_HAS_CODESET_CP1255
#define __CTYPE_HAS_CODESET_GEORGIAN_PS
#define __CTYPE_HAS_CODESET_ISO_8859_1
#define __CTYPE_HAS_CODESET_ISO_8859_10
#define __CTYPE_HAS_CODESET_ISO_8859_13
#define __CTYPE_HAS_CODESET_ISO_8859_14
#define __CTYPE_HAS_CODESET_ISO_8859_15
#define __CTYPE_HAS_CODESET_ISO_8859_16
#define __CTYPE_HAS_CODESET_ISO_8859_2
#define __CTYPE_HAS_CODESET_ISO_8859_3
#define __CTYPE_HAS_CODESET_ISO_8859_4
#define __CTYPE_HAS_CODESET_ISO_8859_5
#define __CTYPE_HAS_CODESET_ISO_8859_6
#define __CTYPE_HAS_CODESET_ISO_8859_7
#define __CTYPE_HAS_CODESET_ISO_8859_8
#define __CTYPE_HAS_CODESET_ISO_8859_9
#define __CTYPE_HAS_CODESET_KOI8_R
#define __CTYPE_HAS_CODESET_KOI8_T
#define __CTYPE_HAS_CODESET_KOI8_U
#define __CTYPE_HAS_CODESET_TIS_620
#define __CTYPE_HAS_CODESET_UTF_8
#define __LOCALE_DATA_WC_TABLE_DOMAIN_MAX   0x2ffff

#define __LOCALE_DATA_WCctype_II_LEN        768
#define __LOCALE_DATA_WCctype_TI_LEN       1888
#define __LOCALE_DATA_WCctype_UT_LEN        948
#define __LOCALE_DATA_WCctype_II_SHIFT        5
#define __LOCALE_DATA_WCctype_TI_SHIFT        3


#define __LOCALE_DATA_WCuplow_II_LEN        384
#define __LOCALE_DATA_WCuplow_TI_LEN        576
#define __LOCALE_DATA_WCuplow_UT_LEN        720
#define __LOCALE_DATA_WCuplow_II_SHIFT        6
#define __LOCALE_DATA_WCuplow_TI_SHIFT        3


#define __LOCALE_DATA_WCuplow_diffs       98


/* #define __LOCALE_DATA_MAGIC_SIZE 64 */
#ifndef __WCHAR_ENABLED
#if 0
#warning WHOA!!! __WCHAR_ENABLED is not defined! defining it now...
#endif
#define __WCHAR_ENABLED
#endif

/* TODO - fix */
#ifdef __WCHAR_ENABLED
#define __LOCALE_DATA_WCctype_TBL_LEN		(__LOCALE_DATA_WCctype_II_LEN + __LOCALE_DATA_WCctype_TI_LEN + __LOCALE_DATA_WCctype_UT_LEN)
#define __LOCALE_DATA_WCuplow_TBL_LEN		(__LOCALE_DATA_WCuplow_II_LEN + __LOCALE_DATA_WCuplow_TI_LEN + __LOCALE_DATA_WCuplow_UT_LEN)
#define __LOCALE_DATA_WCuplow_diff_TBL_LEN (2 * __LOCALE_DATA_WCuplow_diffs)
/* #define WCcomb_TBL_LEN		(WCcomb_II_LEN + WCcomb_TI_LEN + WCcomb_UT_LEN) */
#endif

#undef __PASTE2
#define __PASTE2(A,B)		A ## B
#undef __PASTE3
#define __PASTE3(A,B,C)		A ## B ## C

#define __LOCALE_DATA_COMMON_MMAP(X) \
	unsigned char	__PASTE3(lc_,X,_data)[__PASTE3(__lc_,X,_data_LEN)];

#define __LOCALE_DATA_COMMON_MMIDX(X) \
	unsigned char	__PASTE3(lc_,X,_rows)[__PASTE3(__lc_,X,_rows_LEN)]; \
	uint16_t		__PASTE3(lc_,X,_item_offsets)[__PASTE3(__lc_,X,_item_offsets_LEN)]; \
	uint16_t		__PASTE3(lc_,X,_item_idx)[__PASTE3(__lc_,X,_item_idx_LEN)]; \


typedef struct {
#ifdef __LOCALE_DATA_MAGIC_SIZE
	unsigned char magic[__LOCALE_DATA_MAGIC_SIZE];
#endif /* __LOCALE_DATA_MAGIC_SIZE */

#ifdef __CTYPE_HAS_8_BIT_LOCALES
	const unsigned char tbl8ctype[__LOCALE_DATA_Cctype_TBL_LEN];
    const unsigned char tbl8uplow[__LOCALE_DATA_Cuplow_TBL_LEN];
#ifdef __WCHAR_ENABLED
	const uint16_t tbl8c2wc[__LOCALE_DATA_Cc2wc_TBL_LEN]; /* char > 0x7f to wide char */
	const unsigned char tbl8wc2c[__LOCALE_DATA_Cwc2c_TBL_LEN];
	/* translit  */
#endif /* __WCHAR_ENABLED */
#endif /* __CTYPE_HAS_8_BIT_LOCALES */
#ifdef __WCHAR_ENABLED
	const unsigned char tblwctype[__LOCALE_DATA_WCctype_TBL_LEN];
	const unsigned char tblwuplow[__LOCALE_DATA_WCuplow_TBL_LEN];
	const int16_t tblwuplow_diff[__LOCALE_DATA_WCuplow_diff_TBL_LEN];
/* 	const unsigned char tblwcomb[WCcomb_TBL_LEN]; */
	/* width?? */
#endif /* __WCHAR_ENABLED */

	__LOCALE_DATA_COMMON_MMAP(ctype);
	__LOCALE_DATA_COMMON_MMAP(numeric);
	__LOCALE_DATA_COMMON_MMAP(monetary);
	__LOCALE_DATA_COMMON_MMAP(time);
	/* collate is different */
	__LOCALE_DATA_COMMON_MMAP(messages);


#ifdef __CTYPE_HAS_8_BIT_LOCALES
	const __codeset_8_bit_t codeset_8_bit[__LOCALE_DATA_NUM_CODESETS];
#endif /* __CTYPE_HAS_8_BIT_LOCALES */

	__LOCALE_DATA_COMMON_MMIDX(ctype);
	__LOCALE_DATA_COMMON_MMIDX(numeric);
	__LOCALE_DATA_COMMON_MMIDX(monetary);
	__LOCALE_DATA_COMMON_MMIDX(time);
	/* collate is different */
	__LOCALE_DATA_COMMON_MMIDX(messages);

	const uint16_t collate_data[__lc_collate_data_LEN];

	unsigned char lc_common_item_offsets_LEN[__LOCALE_DATA_CATEGORIES];
    size_t lc_common_tbl_offsets[__LOCALE_DATA_CATEGORIES * 4];
	/* offsets from start of locale_mmap_t */
	/* rows, item_offsets, item_idx, data */

#ifdef __LOCALE_DATA_NUM_LOCALES
	unsigned char locales[__LOCALE_DATA_NUM_LOCALES * __LOCALE_DATA_WIDTH_LOCALES];
	unsigned char locale_names5[5*__LOCALE_DATA_NUM_LOCALE_NAMES];
	unsigned char locale_at_modifiers[__LOCALE_DATA_AT_MODIFIERS_LENGTH];
#endif /* __LOCALE_DATA_NUM_LOCALES */

	unsigned char lc_names[__lc_names_LEN];
#ifdef __CTYPE_HAS_8_BIT_LOCALES
	unsigned char codeset_list[sizeof(__LOCALE_DATA_CODESET_LIST)]; /* TODO - fix */
#endif /* __CTYPE_HAS_8_BIT_LOCALES */


} __locale_mmap_t;

extern const __locale_mmap_t *__locale_mmap;
