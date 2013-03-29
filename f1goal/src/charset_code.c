
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "charset_code.h"

static char hex_char_vmap[128] = { -1 };
static int  mod_init_flag = 0;
#define MAX_CODE_SIZE	0xFFFFU
static unsigned short ucs2_gbk[MAX_CODE_SIZE] = { 0xFFFFU };
static unsigned short gbk_ucs2[MAX_CODE_SIZE] = { 0xFFFFU};

static int hex_char_vmap_init()
{
    char i = 0;
    const char *hex_d_chars = "0123456789";
    const char *hex_c_chars = "abcdef"; 
    const char *hex_C_chars = "ABCDEF"; 

    for (i = 0; i < 10; i++) {
        hex_char_vmap[hex_d_chars[i]] = i;
    }

    for (i = 0; i < 6; i++) {
        hex_char_vmap[hex_c_chars[i]] = 10 + i;
    }

    for (i = 0; i < 6; i++) {
        hex_char_vmap[hex_C_chars[i]] = 10 + i;
    }

    return 0;
}

extern code_map_t ucs2_gbk_map[];
//#define UG_MAP_SIZE sizeof(ucs2_gbk_map)/sizeof(ucs2_gbk_map[0])
static int charset_map_init()
{
	int i = 0;
	unsigned short gbk, ucs2;

	memset(gbk_ucs2, 0xFF, sizeof(gbk_ucs2));
	memset(ucs2_gbk, 0xFF, sizeof(ucs2_gbk));

	while (0xFFFFU != ucs2_gbk_map[i].c1 || 0xFFFFU != ucs2_gbk_map[i].c2) {
	//for (i = 0; i < sizeof(ucs2_gbk_map)/sizeof(ucs2_gbk_map[0]); i++) {
		gbk = ucs2_gbk_map[i].c1;	
		ucs2 = ucs2_gbk_map[i].c2;
		ucs2_gbk[ucs2] = gbk;
		gbk_ucs2[gbk] = ucs2;
		i++;
	}

	return 0;
}

static int module_init()
{	
	hex_char_vmap_init();
	charset_map_init();

	return 0;
}

/*
    UCS-4                   UTF-8
 U+00000000--U+0000007F 0xxxxxxx
 U+00000080--U+000007FF 110xxxxx 10xxxxxx
 U+00000800--U+0000FFFF 1110xxxx 10xxxxxx 10xxxxxx
 U+00010000--U+001FFFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 U+00200000--U+03FFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 U+04000000--U+7FFFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

 input string must be visible ascii characters, ie. "123%e5%a3%a1".
*/
static int utf8_str_to_uint(const char *str, int str_len, unsigned char *d_arr, int *d_len)
{
    int             i       = 0;
    int             num     = 0;
    int             d_size  = *d_len;
    unsigned char   *pb     = (unsigned char*)str;
    unsigned char   *pe     = (unsigned char*)str + str_len;
    unsigned char   byte    = 0;
    int             byte_num = 0;

    while (pb < pe) {
        if (*pb >= 128U) return -2;
        else if (0x25U == *pb) { // *pb is '%'
            if (pb + 2 >= pe) return -1; //Not enough IN data
            pb += 1; // skip to the hex-data
            // %25, '0123456789ABCDEFabcdef'
            if (-1 == hex_char_vmap[*pb] || -1 == hex_char_vmap[*(pb+1)]) {
                return -2; // Error coded
            }
            byte = hex_char_vmap[*pb] << 4 | hex_char_vmap[*(pb+1)];
            if (byte < 128) {
                byte_num = 0;
            }
            else if ((byte & 0xE0) == 0xC0) byte_num = 1;
            else if ((byte & 0xF0) == 0xE0) byte_num = 2;
            else if ((byte & 0xF8) == 0xF0) byte_num = 3;
            else if ((byte & 0xFC) == 0xF8) byte_num = 4;
            else if ((byte & 0xFE) == 0xFC) byte_num = 5;
            else return -3;

            // store the first byte
            if (num >= d_size) return 0;
            d_arr[num++] = byte;
            pb += 2; // move to the next byte

            // two characters maps to one byte data, ie. '3E' to byte 0x3E, but a '%' prefixs it.
            if (pb+3*byte_num > pe) return -1; // not enough IN data 
            if (byte_num > d_size-num) return -1; // not enough OUT capacity
            while (byte_num > 0) {
                if (0x25U != *pb) return -2; // Error coded
                pb++; // move to the data
                if (-1 == hex_char_vmap[*pb] || -1 == hex_char_vmap[*(pb+1)]) return -2;
                byte = hex_char_vmap[*pb] << 4 | hex_char_vmap[*(pb+1)];
                d_arr[num++] = byte;
                pb += 2;
                byte_num -= 1;
            }
        } else {
            if (num >= d_size) return 0;
            d_arr[num++] = *pb;
            pb++; // move the next character.
        }

    }

    // Output the bytes decoded.
    d_arr[num] = 0;
    *d_len = num;

    return 0;
}

/*
 gbk汉字区：
    a. gb2312汉字区 GBK/2: B0A1-F7FE
    b. gb13000.1扩充汉字区 1)GBK/3: 8140-A0FE 2)GBK/4: AA40-FEA0
 gbk图形符号区：
    a. gb2312非汉字符号区 GBK/1: A1A1-A9FE
    b. gb13000.1扩充非汉字区 GBK/5: A840-A9A0
*/
#define GBK2_CHK(b0,b1) ((b0)>=0xB0 && (b0)<=0xF7 && (b1)>=0xA1 && (b1)<=0xFE)
#define GBK3_CHK(b0,b1) ((b0)>=0x81 && (b0)<=0xA0 && (b1)>=0x40 && (b1)<=0xFE)
#define GBK4_CHK(b0,b1) ((b0)>=0xAA && (b0)<=0xFE && (b1)>=0x40 && (b1)<=0xA0)
#define GBK1_CHK(b0,b1) ((b0)>=0xA1 && (b0)<=0xA9 && (b1)>=0xA1 && (b1)<=0xFE)
#define GBK5_CHK(b0,b1) ((b0)>=0xA8 && (b0)<=0xA9 && (b1)>=0x40 && (b1)<=0xA0)
static int gbk_str_to_uint(const char *str, int str_len, unsigned char *d_arr, int *d_len)
{
    int             num     = 0;
    int             byte0    = 0;
    int             byte1    = 0;
    int             d_size  = *d_len;
    unsigned char   *pb     = (unsigned char*)str;
    unsigned char   *pe     = (unsigned char*)str + str_len;


    while (pb < pe) {
        if (*pb >= 128U) return E_NASC;
        else if (0x25U == *pb) { // 0x25 --> '%'
            if (pb + 2 >= pe) return E_IDAT; //not enough IN data
            pb += 1; // skip '%' and move to the data position
            if (-1 == hex_char_vmap[*pb] || -1 == hex_char_vmap[*(pb+1)]) return E_CODE;
            byte0 = hex_char_vmap[*pb]<<4 | hex_char_vmap[*(pb+1)];
            pb += 2; // skip current byte data and move to next byte
            if ((byte0 & 0x80) == 0x80) { // chinese character
                    if (pb + 3 > pe) return E_IDAT; // not enough IN data
                    if (0x25U != *pb) return E_CODE;  // not '%'
                    pb += 1; // skip '%' and move to the data position 
                    if (-1 == hex_char_vmap[*pb] || -1 == hex_char_vmap[*(pb+1)]) return E_CODE;
                    byte1 = hex_char_vmap[*pb]<<4 | hex_char_vmap[*(pb+1)];
                    pb += 2; // move to the next byte.
                if (GBK2_CHK(byte0,byte1) || GBK3_CHK(byte0,byte1) || GBK4_CHK(byte0,byte1)) {// check GBK2, GBK3, GBK4, chinese letters
                    if (num+2 >= d_size) return E_OBUF;
                    d_arr[num++] = byte0;
                    d_arr[num++] = byte1; 
                } else if (GBK1_CHK(byte0,byte1) || GBK5_CHK(byte0,byte1)) { // check GBK1,GBK5, graphic letters
                    if (num+2 >= d_size) return E_OBUF;
                    d_arr[num++] = byte0;
                    d_arr[num++] = byte1; 
                } else { // invalid gbk letters
                    return E_CODE;
                }
            } else { // characters < 128
                if (num >= d_size) return E_OBUF;
                d_arr[num++] = byte0;
            }
        } else {
            if (num >= d_size) return E_OBUF;
            d_arr[num++] = *pb;
            pb += 1; 
        }
    }

    *d_len = num;
    d_arr[num] = 0;

    return E_OK;
}

int str_esc_decode(const char *str, int str_len, int code_type, unsigned char *d_arr, int *d_len)
{
    int ret = 0;

    if (!mod_init_flag) {
        module_init();
        mod_init_flag = 1;
    }

    switch (code_type) {
        case CT_UTF8:
            ret = utf8_str_to_uint(str, str_len, d_arr, d_len);
            break;
        case CT_GBK:
            ret = gbk_str_to_uint(str, str_len, d_arr, d_len);
            break;
        default:
            return -1;
    }

    if (0 != ret) {
        return ret;
    }

    return 0;
}

static int gbk_to_utf8(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len)
{
	int	num	= 0;
	int d2_size = *d2_len;
	unsigned short gbk = 0;
	unsigned short ucs2 = 0;
	unsigned char *pb, *pe;

	pb = (unsigned char*)d1_arr;
	pe = (unsigned char*)d1_arr + d1_len;

	while (pb < pe) {
		if (pb + 1 >= pe) return E_IDAT;
		gbk = pb[0];
		gbk <<= 8;
		gbk |= pb[1];
		
		if (gbk_ucs2[gbk] == 0x0FFFFU) return E_CODE;

		ucs2 = gbk_ucs2[gbk];
		if (ucs2 < 0x80U) {
			if (num >= d2_size) return E_OBUF;
			d2_arr[num++] = ucs2 & 0xFF;
		} else if (ucs2 >= 0x80U && ucs2 <= 0x07FFU) {
			if (num + 2 >= d2_size) return E_OBUF;
			d2_arr[num++] = 0xC0 | ((ucs2>>6) & 0x1F);
			d2_arr[num++] = 0x80 | (ucs2&0x3F);
		} else if (ucs2 >= 0x0800U && ucs2 <= 0x0FFFFU) {
			if (num + 3 >= d2_size) return E_OBUF;
			d2_arr[num++] = 0xE0 | ((ucs2>>12) & 0x0F);
			d2_arr[num++] = 0x80 | ((ucs2>>6) & 0x3F);
			d2_arr[num++] = 0x80 | (ucs2 & 0x3F);
		} else {
			return E_CODE;
		}

		pb += 2;
	}

	*d2_len = num;
	d2_arr[num] = 0;

	return E_OK;
}

static int utf8_to_gbk(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len)
{
	int	num	= 0;
	int d2_size = *d2_len;
	unsigned char byte0, byte1;
	unsigned short ucs2 = 0;
	unsigned short gbk = 0;
	unsigned char *pb, *pe;

	pb = (unsigned char*)d1_arr;
	pe = (unsigned char*)d1_arr + d1_len;

	while (pb < pe) {
		ucs2 = 0;
		if (*pb < 0x80) {
			ucs2 = *pb;
			pb += 1;
		} else if ((*pb & 0xE0) == 0xC0) {
			if (pb + 1 >= pe) return E_IDAT;
			ucs2 = *pb & 0x1F;
			ucs2 = ucs2 << 6;
			ucs2 = ucs2 | (pb[1] & 0x3F);
			pb += 2;
		} else if ((*pb & 0xF0) == 0xE0) {
			if (pb + 2 >= pe) return E_IDAT;
			ucs2 = *pb & 0x0F;
			ucs2 = ucs2 << 6;
			ucs2 = ucs2 | (pb[1] & 0x3F);
			ucs2 = ucs2 << 6;
			ucs2 = ucs2 | (pb[2] & 0x3F);
			pb += 3;
		} else {
			return E_CODE;
		}

		if (ucs2_gbk[ucs2] == 0xFFFFU) return E_CODE;
		gbk = ucs2_gbk[ucs2];
		if (num+2 >= d2_size) return E_OBUF;
		d2_arr[num++] = gbk >> 8;
		d2_arr[num++] = gbk & 0x0FF;
	}

	*d2_len = num;
	d2_arr[num] = 0;

	return 0;
}

/*
    UCS-4                   UTF-8
 U+00000000--U+0000007F 0xxxxxxx
 U+00000080--U+000007FF 110xxxxx 10xxxxxx
 U+00000800--U+0000FFFF 1110xxxx 10xxxxxx 10xxxxxx
 U+00010000--U+001FFFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 U+00200000--U+03FFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 U+04000000--U+7FFFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

 it must consider the byte order.
*/
static int ucs2_to_utf8(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len)
{
	int				i;
	int				num = 0;
	int				d2_size = *d2_len;
	unsigned char	*pb, *pe;
	unsigned short	d16 = 0x00;

	pb = (unsigned char*)d1_arr;
	pe = (unsigned char*)d1_arr + d1_len;

	while (pb < pe) {
		if (pb+1 >= pe) return E_IDAT;
		d16 = pb[0];
		d16 <<= 8;
		d16 |= pb[1];

		if (d16 < 0x80) {
			if (num >= d2_size) return E_OBUF;
			d2_arr[num++] = d16 & 0xFF;
		} else if (d16 >= 0x80U && d16 <= 0x7FFU) {
			if (num + 2 >= d2_size) return E_OBUF;
			d2_arr[num++] = 0xC0 | ((d16>>6) & 0x1F);
			d2_arr[num++] = 0x80 | (d16&0x3F);
		} else if ((d16 >= 0x800U) && (d16 <= 0xFFFFU)) {
			if (num + 3 >= d2_size) return E_OBUF;
			d2_arr[num++] = 0xE0 | ((d16>>12) & 0x0F);
			d2_arr[num++] = 0x80 | ((d16>>6) & 0x3F);
			d2_arr[num++] = 0x80 | (d16 & 0x3F);
		} else {
			return E_CODE;
		}
		pb += 2;
	}

	*d2_len = num;
	d2_arr[num] = 0;

	return 0;
}

/*
    UCS-4                   UTF-8
 U+00000000--U+0000007F 0xxxxxxx
 U+00000080--U+000007FF 110xxxxx 10xxxxxx
 U+00000800--U+0000FFFF 1110xxxx 10xxxxxx 10xxxxxx
*/
static int utf8_to_ucs2(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len)
{
	unsigned char	*pb, *pe;
	int				d2_size = *d2_len;
	int				num = 0;
	unsigned short	d16 = 0;

	pb = (unsigned char*)d1_arr;
	pe = (unsigned char*)d1_arr + d1_len;

	while (pb < pe) {
		d16 = 0;
		if (*pb < 0x80) {
			d16 = *pb;
			pb += 1;
		} else if ((*pb & 0xE0) == 0xC0) {
			if (pb + 1 >= pe) return E_IDAT;
			d16 = *pb & 0x1F;
			d16 = d16 << 6;
			d16 = d16 | (*(pb+1) & 0x3F);
			pb += 2;
		} else if ((*pb & 0xF0) == 0xE0) {
			if (pb + 2 >= pe) return E_IDAT;
			d16 = *pb & 0x0F;
			d16 = d16 << 6;
			d16 = d16 | (pb[1] & 0x3F);
			d16 = d16 << 6;
			d16 = d16 | (pb[2] & 0x3F);
			pb += 3;
		} else {
			return E_CODE;
		}
		if (num+2 >= d2_size) return E_OBUF;
		d2_arr[num++] = d16 >> 8;
		d2_arr[num++] = d16 & 0xFF;
	}

	*d2_len = num;
	d2_arr[num] = 0;

	return 0;
}

static int gbk_to_ucs2(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len)
{
	int				num		= 0;
	int				d2_size = *d2_len;
	unsigned short	gbk;
	unsigned short	ucs2;
	unsigned char	*pb, *pe;

	pb = (unsigned char*)d1_arr;
	pe = (unsigned char*)d1_arr + d1_len;

	while (pb < pe) {
		if (pb + 1 >= pe) return E_IDAT;	
		gbk = pb[0];
		gbk <<= 8;
		gbk |= pb[1];

		if (gbk_ucs2[gbk] == 0xFFFFU) return E_CODE;
		ucs2 = gbk_ucs2[gbk];

		if (num+2 >= d2_size) return E_OBUF;
		d2_arr[num++] = ucs2 >> 8;
		d2_arr[num++] = ucs2 & 0x0FF;

		pb += 2;
	}

	*d2_len = num;
	d2_arr[num] = 0;

	return 0;
}
static int ucs2_to_gbk(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len)
{
	int				num		= 0;
	int				d2_size = *d2_len;
	unsigned short	ucs2 = 0;
	unsigned short	gbk = 0;
	unsigned char *pb, *pe;

	pb = (unsigned char*)d1_arr;
	pe = (unsigned char*)d1_arr + d1_len;

	while (pb < pe) {
		if (pb+1 >= pe) return E_IDAT;
		ucs2 = pb[0];
		ucs2 <<= 8;
		ucs2 |= pb[1];

		if (ucs2_gbk[ucs2] == 0xFFFFU) return E_CODE;
		gbk = ucs2_gbk[ucs2];

		if (num+2 >= d2_size) return E_OBUF;
		d2_arr[num++] = gbk >> 8;
		d2_arr[num++] = gbk & 0xFF;

		pb += 2;
	}

	*d2_len = num;
	d2_arr[num] = 0;

	return 0;
}

/*
 * data flow is big-endian mode
 */
int lc_conv(const unsigned char *d1_arr, int d1_len, unsigned char *d2_arr, int *d2_len, int conv_type)
{
	int ret = 0;
	int __tmp_len = 0;

    if (!mod_init_flag) {
        module_init();
        mod_init_flag = 1;
    }

	switch (conv_type) {
		case CONV_GBK_UTF8:
			__tmp_len = *d2_len;
			ret = gbk_to_utf8(d1_arr, d1_len, d2_arr, &__tmp_len);
			break;

		case CONV_UTF8_GBK:
			__tmp_len = *d2_len;
			ret = utf8_to_gbk(d1_arr, d1_len, d2_arr, &__tmp_len);
			break;

		case CONV_UCS2_UTF8:
			__tmp_len = *d2_len;
			ret = ucs2_to_utf8(d1_arr, d1_len, d2_arr, &__tmp_len);
			break;

		case CONV_UTF8_UCS2:
			__tmp_len = *d2_len;
			ret = utf8_to_ucs2(d1_arr, d1_len, d2_arr, &__tmp_len);
			break;

		case CONV_GBK_UCS2:
			__tmp_len = *d2_len;
			ret = gbk_to_ucs2(d1_arr, d1_len, d2_arr, &__tmp_len);
			break;

		case CONV_UCS2_GBK:
			__tmp_len = *d2_len;
			ret = ucs2_to_gbk(d1_arr, d1_len, d2_arr, &__tmp_len);
			break;

		default:
			return -1;
	}

	*d2_len = __tmp_len;

	return ret;
}
