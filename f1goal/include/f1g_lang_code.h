
#ifndef __F1G_LANG_CODE_H
#define __F1G_LANG_CODE_H

#include "f1g_basic_types.h"

enum {
	LANG_CODE_UTF8 = 0x00,
	LANG_CODE_UNICODE,
	LANG_CODE_GBK2312,
	LANG_CODE_GBK,
	LANG_CODE_UNKNOWN
};

//language code map
typedef struct _lc_map_s {
	u16_t	code_size;
	u16_t	*gbk;
	u16_t	*ucs;
	u16_t	*utf8;
}lc_map_t, *lc_map_p;

#define URL_ESC_CHAR '%'

lc_map_p lc_map_create(string_t lc_name);

i32_t lang_code_init(string_t gbk_unic_map);

// transfer string to code value of true type, for example: %20%ab%cd to '0x20 0xab 0xcd'
i32_t str_decode(i8_p p_str, i32_t str_len, u8_p p_buf, i32_p data_len, u8_p code_type);

i32_t gbk_to_utf8(u8_p p_gbk, i32_t gbk_len, u8_p p_utf8_buf, i32_p data_len);
i32_t utf8_to_unicode(u8_p p_utf8, i32_t utf8_len, u16_p p_unic_buf, i32_p data_len);
i32_t unicode_to_utf8(u16_p p_unic, i32_t unic_len, u8_p p_utf8_buf, i32_p data_len);

#endif // __F1G_LANG_CODE_H
