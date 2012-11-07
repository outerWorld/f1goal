
#ifndef __F1G_BASIC_FUNCS_H
#define __F1G_BASIC_FUNCS_H

#include "f1g_basic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void hex_print(FILE* fd, u8_p data, u32_t data_len);

/*
 * Compare the two string, equal(=0) or not equal(!=0)
 */
i8_t cmp_str(const i8_p pstr1, const i8_p pstr2, i32_t len);

i8_t cmp_str2(const i8_p pstr1, i32_t str1_len, const i8_p pstr2, i32_t str2_len);

/*
 * Get location of sub string at pstr
 * for example: pstr="123abce23", p_sub="abc",
 * 	then ploc=pstr+3="abce23"
 */
i8_t find_str(const i8_p pstr, const i8_p pstr_end, const i8_p p_sub, i8_p *ploc);

/*
 * Get data which between pb and pe from pstr 
 * for example: pstr="#LINE=12;#SEP=\t;", pb="#LINE=", pe=";", then,
 * pdata="12"
 */
i32_t substr(const i8_p pstr, const i8_p pstr_end, const i8_p prev, const i8_p post, i8_p pdata, i32_t data_size);

i8_t skip_char(i8_p * p_pdata, i32_t len, i8_t ch);
i8_t skip_chars(i8_p * p_pdata, i32_t len, i8_p chars, i32_t chars_len);
i8_t skip_space(i8_p * p_pdata, i32_t len);
i8_t skip_tab(i8_p * p_pdata, i32_t len);

/*
 * strip the character at the end of p_str, and p_e points at the position before the last ch.
 * for example:
 *    p_str="123bbb", to remove the character 'b', after this operation, *p_e = "3bbb"
 *  and specially, when p_str="bbbb", 'b' is the character being removed, then *p_e=p_str-1, that is, *p_e < pstr
 */
i8_t strip_char(i8_p p_str, i32_t len, i8_t ch, i8_p *p_e);

// return interval by sec:usec
i32_t time_interval_tv(struct timeval *tvb, struct timeval *tve, struct timeval *tv_out);
// return interval by unit USeconds
i32_t time_interval(struct timeval *tvb, struct timeval *tve);

#ifdef __cplusplus
}
#endif

#endif
