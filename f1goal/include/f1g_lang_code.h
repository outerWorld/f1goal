
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

#define URL_ESC_CHAR '%'

i32_t lang_code_init();

// transfer string to code value of true type, for example: %20%ab%cd to '0x20 0xab 0xcd'
i32_t str_decode(i8_p p_str, i32_t str_len, u8_p p_buf, i32_p data_len, u8_p code_type);

#endif // __F1G_LANG_CODE_H
