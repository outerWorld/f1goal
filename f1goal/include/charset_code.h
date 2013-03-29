/*
 * Author: huang chunping
 * Date  : 2013-03-27
 * Desc  :
 */

#ifndef __LANG_CODE_H
#define __LANG_CODE_H

// code type
enum {
    CT_UNKNOWN = 0x00,
    CT_UTF8,
    CT_GBK,
    CT_UNICODE,
    CT_NUM
};

// definition of error code
enum {
    E_OK = 0x00, // no error
    E_OBUF, // not enough out buffer
    E_IDAT, // not enough in bytes
    E_CODE, // invalid code
    E_NASC, // not ascii
};

// convert type: from code type 1 to code type 2
enum {
	CONV_GBK_UTF8 = 0x00,	// gbk to utf
	CONV_UTF8_GBK,
	CONV_UTF8_UCS2,
	CONV_UCS2_UTF8,
	CONV_GBK_UCS2,
	CONV_UCS2_GBK
};

typedef struct _code_map_s {
	unsigned short c1;
	unsigned short c2;
}code_map_t, *code_map_p;

// %20%A89A --> 0x20 0xA8 0xA9
/*
 utf-8		%E6%B5%8B%E8%AF%95 (测试)
 gbk		%B2%E2%CA%D4 (测试)
*/
int str_esc_decode(const char *str, int str_len, int code_type, unsigned char * d_arr, int *d_len);
int lc_conv(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len, int conv_type);

#endif // __LANG_CODE_H
