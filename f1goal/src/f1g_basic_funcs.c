
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_basic_funcs.h"

void hex_print(FILE* fd, u8_p data, u32_t data_len)
{
	u32_t i,j,line;

	line = data_len / 16;
	if (data_len%16 != 0) {
		line += 1;
	}

	for (i=0; i<line; i++) {
		fprintf(fd, "%08x    ", i*16);
		for (j=i*16; j<data_len && j<(i+1)*16;j++) {
			fprintf(fd, "%02x ", (u8_t)(data[j]));
		}
		for (; j<(i+1)*16; j++) {
			fprintf(fd, "   ");
		}
		fprintf(fd, "    ");

		fprintf(fd, "|");
		for (j=i*16; j<data_len && j<(i+1)*16;j++) {
			if (data[j]<=0x20 || data[j]>=0x7F) {
				fprintf(fd, ".");
			} else {
				fprintf(fd, "%c", data[j]);
			}
		}
		for (; j<(i+1)*16; j++) {
			fprintf(fd, " ");
		}
		fprintf(fd, "|");
		fprintf(fd, "\n");
	}
}

i8_t cmp_str(const i8_p pstr1, const i8_p pstr2, i32_t len)
{
	i32_t i;
	i32_t mid;
	i8_t ret1, ret2;

	if (len < 512) {
		for (i=0; i<len; i++) {
			if (pstr1[i] != pstr2[i]) {
				break;
			}
		}
		if (i >= len) {
			return 0;
		} else {
			return (i8_t)(pstr1[i]-pstr2[i]);
		}
	} else {
		mid = len>>1;	
		if (pstr1[mid] != pstr2[mid]) {
			return pstr1[mid]-pstr2[mid];
		}
		ret1 = cmp_str(pstr1, pstr2, mid-1);
		ret2 = cmp_str(pstr1+mid, pstr2+mid, mid-1);
	}

	if (0==ret1 && 0==ret2) {
		return 0;
	} else {
		return 1;
	}
}

i8_t find_str(const i8_p pstr, const i8_p pstr_end, const i8_p p_sub, i8_p *ploc)
{
	i8_p pb;
	i32_t str_len;
	i32_t substr_len;

	str_len = pstr_end - pstr;
	substr_len = strlen(p_sub);

	*ploc = NULL;
	pb = pstr;
	while ((pstr_end-pb) >= substr_len) {
		if (0 == cmp_str(pb, p_sub, substr_len)) {
			*ploc = pb;
			return F1G_OK;
		}
		pb++;
	}

	*ploc = pstr_end;

	return F1G_OK;
}

i32_t substr(const i8_p pstr, const i8_p pstr_end, const i8_p prev, const i8_p post, i8_p pdata, i32_t data_size)
{
	i8_p pb;
	i8_p pe;
	i32_t len;
	i32_t prev_len;

	if (find_str(pstr, pstr_end, prev, &pb) != F1G_OK) {
		return 0;
	}

	prev_len = strlen(prev);
	find_str(pb+prev_len, pstr_end, post, &pe);

	len = pe-pb-prev_len;
	if (len > data_size) {
		return 0;
	}
	
	memcpy(pdata, pb+prev_len, len);

	return len;
}
