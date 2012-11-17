
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_lang_code.h"

#define ASC_CODE_SIZE	256
static u8_t sg_hex_array[ASC_CODE_SIZE] = {0xFF};
static i8_t	sg_hex_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
							'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F'
};
static i32_t _init_hex_array(u8_p hex_array, i16_t base, i8_p p_char_arr, i32_t char_num)
{
	i32_t i = 0;

	for (i=0; i<char_num; i++) {
		hex_array[p_char_arr[i]] = base + i;
	}

	return F1G_OK;
}

i32_t lang_code_init(string_t gbk_unic_map)
{
	_init_hex_array(sg_hex_array, 0, &sg_hex_chars[0], 16);
	_init_hex_array(sg_hex_array, 10, &sg_hex_chars[16], 6);
	
	return F1G_OK;
}

// Normal conditions: %3f%0fabc
// unNormal conditions: %%%ag12
i32_t str_decode(i8_p p_str, i32_t str_len, u8_p p_buf, i32_p data_size, u8_p code_type)
{
	i32_t esc_flag = 0; // escape character :'%'
	i32_t valid_hex_fst = 0; // the first hex char found
	i32_t valid_hex_sec = 0; // the second hex char found
	i32_t data_len = 0;
	i32_t buf_size = 0;
	u8_t  value = 0;
	
	buf_size = *data_size;
	*data_size = 0;			// it must be reset to 0, as it will store the data length for output.
	*code_type = LANG_CODE_UNKNOWN;
	i8_p p_str_end = p_str + str_len;

	while (p_str != p_str_end) {
		switch (*p_str) {
			case URL_ESC_CHAR:
				if (1 == esc_flag) {
					fprintf(stderr, "%s,%d not correct format, double escape characters\n", __FUNCTION__, __LINE__);
					return F1G_ERR;
				}
				esc_flag = 1;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				if (1 == esc_flag) {
					if (0 == valid_hex_fst) {
						value = sg_hex_array[*p_str];
						valid_hex_fst = 1;
					} else {
						if (0 == valid_hex_sec) {
							value = (value<<4) | sg_hex_array[*p_str];
							p_buf[data_len] = value;
							data_len++;
							valid_hex_fst = 0;
							esc_flag = 0;
						} else {
							// TODO nothing. and it seems it is impossible to be here.
						}
					}
				}
			
				break;
			default:
				if (1 == esc_flag) {
					fprintf(stderr, "%s,%d not correct format\n", __FUNCTION__, __LINE__);
					return F1G_ERR;
				}
				p_buf[data_len] = *p_str;
				data_len++;
				break;
		}

		p_str++;
	}

	*data_size = data_len;

	return F1G_OK;
}


i32_t gbk_to_utf8(u8_p p_gbk, i32_t gbk_len, u8_p p_utf8_buf, i32_p data_len)
{
	return F1G_OK;
}

i32_t utf8_to_unicode(u8_p p_utf8, i32_t utf8_len, u16_p p_unic_buf, i32_p data_len)
{
	u8_p p_cur = p_utf8;
	u8_p p_end = p_utf8 + utf8_len;
	u16_t ucs = 0;
	i32_t data_size = *data_len;

	memset(p_unic_buf, 0x00, data_size);
	*data_len = 0;
	while (p_cur < p_end) {
		if (*p_cur&0x80 == 0x00) {
			ucs = *p_cur;
		} else if (*p_cur&0xE0 == 0xC0) {
		} else if (*p_cur&0xF0 == 0xE0) {
		} else if (*p_cur&0xF8 == 0xF0) {
		} else if (*p_cur&0xFC == 0xF8) {
		} else if (*p_cur&0xFE == 0xFC) {
		} else {
		}
	}

	return F1G_OK;
}

/*
 * 		Unicode 			Utf8
 * 000000 ~ 00007F		0xxxxxxx
 * 000080 ~ 0007FF		110xxxxx 10xxxxxx
 * 000800 ~ 00FFFF		1110xxxx 10xxxxxx 10xxxxxx
 * 010000 ~ 10FFFF		11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 */
i32_t unicode_to_utf8(u16_p p_unic, i32_t unic_len, u8_p p_utf8_buf, i32_p data_len)
{
	u16_p p_cur = p_unic;
	u16_p p_end = p_unic + unic_len;
	u8_p p_utf8_cur = p_utf8_buf;
	u32_t val = 0;
	i32_t buf_size = *data_len;

	memset(p_utf8_buf, 0x00, buf_size);
	*data_len = 0;
	while (p_cur < p_end) {
		val = 0x00000000U;
		val |= *p_cur;
		if (*p_cur>=0x0000U && *p_cur<=0x007FU) {
			*(p_utf8_cur++) = *((u8_p)&val + 0);
			*data_len = 1;
		} else if (*p_cur>=0x0080U && *p_cur<=0x07FFU) {
			*(p_utf8_cur++) = val&0x3F | 0x80;
			*(p_utf8_cur++) = (val>>6)&0x1F | 0x0C0;
			*data_len = 2;
		} else if (*p_cur>=0x0800U && *p_cur<=0xFFFFU) {
			*(p_utf8_cur++) = val&0x3F | 0x80;
			*(p_utf8_cur++) = (val>>6)&0x3F | 0x80;
			*(p_utf8_cur++) = (val>>12)&0x1F | 0x0E0;
			*data_len = 3;
		} else {
		}
		p_cur++;
	}

	return F1G_OK;
}

