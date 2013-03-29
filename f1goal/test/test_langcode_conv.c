// -*- encoding=utf-8 -*-
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "charset_code.h"

static char utf8_buf[2048];
static char utf8_buf_raw[2048];
static char ucs2_buf[2048];
static char gbk_buf[2048];

#define T_ADD(total, tb, te) \
{\
	te.tv_usec -= tb.tv_usec; \
	te.tv_sec -= tb.tv_sec; \
	if (te.tv_usec < 0) {\
		te.tv_usec += 1000000; \
		te.tv_sec--; \
	} \
	total += te.tv_usec + te.tv_sec * 1000000; \
}

#define T_SET(tt) gettimeofday(&tt, NULL)

/*
*  ./test_langcode_conv data
*  data is utf-8 encoded.
*/
int main(int argc, char *argv[])
{
	int				i;
	char			*p;
	int				ret;
	int				loop;
	int				len_gbk;
	int				len_ucs2;
	int				len_utf8;
	int				len_utf8_raw;
	int				conv_type;
	long long		t_total = 0;
	struct timeval	tv_b, tv_e;

	if (argc < 3) return 1;//p = "%E6%90%9C%E7%B4%A2";
	loop = atoi(argv[1]);
	p = argv[2];


	for (i = 0; i < loop; i++) {
		len_utf8_raw = sizeof(utf8_buf_raw);
		T_SET(tv_b);
		ret = str_esc_decode(p, strlen(p), CT_UTF8, utf8_buf_raw, &len_utf8_raw);	
		if (0 != ret) {
			fprintf(stderr, "str_esc_decode fail [%s] [%d]\n", p, ret);
			return 1;
		}
		T_SET(tv_e);
		T_ADD(t_total, tv_b, tv_e);
		fprintf(stderr, "str_esc_decode [%s] [%s]\n", p, utf8_buf_raw);


		// UTF8 --> GBK
		len_gbk = sizeof(gbk_buf);
		T_SET(tv_b);
		ret = lc_conv(utf8_buf_raw, len_utf8_raw, gbk_buf, &len_gbk, CONV_UTF8_GBK);
		if (0 != ret) {
			fprintf(stderr, "CONV_UTF8_GBK conv [%s] failed [%d]\n", p, ret);
			return 1;
		}
		T_SET(tv_e);
		T_ADD(t_total, tv_b, tv_e);
		fprintf(stdout, "CONV_UTF8_GBK conv [%s] to [%s]\n", utf8_buf_raw, gbk_buf);

		// GBK --> UCS2
		len_ucs2 = sizeof(ucs2_buf);
		T_SET(tv_b);
		ret = lc_conv(gbk_buf, len_gbk, ucs2_buf, &len_ucs2, CONV_GBK_UCS2);
		if (0 != ret) {
			fprintf(stderr, "CONV_GBK_UCS2 conv [%s] failed [%d]\n", gbk_buf, ret);
			return 1;
		}
		T_SET(tv_e);
		T_ADD(t_total, tv_b, tv_e);
		fprintf(stdout, "CONV_GBK_UCS2 conv [%s] to [%s]\n", gbk_buf, ucs2_buf);

		// UCS2 --> UTF8
		len_utf8 = sizeof(utf8_buf);
		T_SET(tv_b);
		ret = lc_conv(ucs2_buf, len_ucs2, utf8_buf, &len_utf8, CONV_UCS2_UTF8);
		if (0 != ret) {
			fprintf(stderr, "CONV_UCS2_UTF8 conv [%s] failed [%d]\n", ucs2_buf, ret);
			return 1;
		}
		T_SET(tv_e);
		T_ADD(t_total, tv_b, tv_e);
		fprintf(stderr, "CONV_UCS2_UTF8 conv [%s] to [%s]\n", ucs2_buf, utf8_buf);

		// UTF8 --> UCS2
		len_ucs2 = sizeof(ucs2_buf);
		T_SET(tv_b);
		ret = lc_conv(utf8_buf, len_utf8, ucs2_buf, &len_ucs2, CONV_UTF8_UCS2);
		if (0 != ret) {
			fprintf(stderr, "CONV_UTF8_UCS2 conv [%s] failed [%d]\n", p, ret);
			return 1;
		}
		T_SET(tv_e);
		T_ADD(t_total, tv_b, tv_e);
		fprintf(stderr, "CONV_UTF8_UCS2 conv [%s] to [%s]\n", utf8_buf, ucs2_buf);


		// UCS2 --> GBK
		len_gbk = sizeof(gbk_buf);
		T_SET(tv_b);
		ret = lc_conv(ucs2_buf, len_ucs2, gbk_buf, &len_gbk, CONV_UCS2_GBK);
		if (0 != ret) {
			fprintf(stderr, "CONV_UCS2_GBK conv [%s] failed [%d]\n", ucs2_buf, ret);
			return 1;
		}
		T_SET(tv_e);
		T_ADD(t_total, tv_b, tv_e);
		fprintf(stderr, "CONV_UCS2_GBK conv [%s] to [%s]\n", ucs2_buf, gbk_buf);


		// GBK --> UTF8
		len_utf8 = sizeof(utf8_buf);
		T_SET(tv_b);
		ret = lc_conv(gbk_buf, len_gbk, utf8_buf, &len_utf8, CONV_GBK_UTF8);
		if (0 != ret) {
			fprintf(stderr, "CONV_GBK_UTF8 conv [%s] failed [%d]\n", gbk_buf, ret);
			return 1;
		}
		T_SET(tv_e);
		T_ADD(t_total, tv_b, tv_e);
		fprintf(stderr, "CONV_GBK_UTF8 conv [%s] to [%s]\n", gbk_buf, utf8_buf);
	}

	fprintf(stdout, "total time consumed [%lld usec], loop[%d], aver[%lf]\n", t_total, loop, t_total*1.0/loop);

	return 0;
}
