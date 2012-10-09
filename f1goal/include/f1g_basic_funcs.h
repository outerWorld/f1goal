
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

#ifdef __cplusplus
}
#endif

#endif
