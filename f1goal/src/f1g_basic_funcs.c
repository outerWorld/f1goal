
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
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

i8_t cmp_str2(const i8_p pstr1, i32_t str1_len, const i8_p pstr2, i32_t str2_len)
{
	if (str1_len < str2_len) {
		return -1;
	}
	
	return cmp_str(pstr1, pstr2, str2_len);
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

	*ploc = NULL;

	return F1G_OK;
}

i32_t substr(const i8_p pstr, const i8_p pstr_end, const i8_p prev, const i8_p post, i8_p pdata, i32_t data_size)
{
	i8_p pb;
	i8_p pe;
	i32_t len;
	i32_t prev_len;

	find_str(pstr, pstr_end, prev, &pb);
	if (pb == NULL) {
		return 0;
	}

	prev_len = strlen(prev);
	find_str(pb+prev_len, pstr_end, post, &pe);
	if (NULL == pe) {
		pe = pstr_end;
	}

	len = pe-pb-prev_len;
	if (len > data_size) {
		return 0;
	}
	
	memcpy(pdata, pb+prev_len, len);

	return len;
}

i8_t skip_char(i8_p * p_pdata, i32_t len, i8_t ch)
{
	i8_p ptmp = *p_pdata + len;
	
	while (*p_pdata<ptmp && ch==*(*p_pdata)) {
		(*p_pdata)++;
	}

	return F1G_OK;
}

i8_t skip_chars(i8_p * p_pdata, i32_t len, i8_p chars, i32_t chars_len)
{
	return F1G_OK;
}

i8_t skip_space(i8_p * p_pdata, i32_t len)
{
	return skip_char(p_pdata, len, ' ');
}

i8_t skip_tab(i8_p * p_pdata, i32_t len)
{
	return skip_char(p_pdata, len, '\t');
}

i8_t strip_char(i8_p p_str, i32_t len, i8_t ch, i8_p *p_e)
{
	*p_e = p_str + len - 1;
	while (*p_e >= p_str && *(*p_e)==ch) {
		(*p_e)--;
	}

	return F1G_OK;
}


i32_t time_interval_tv(struct timeval *tvb, struct timeval *tve, struct timeval *tv_out)
{
	memset(tv_out, 0x00, sizeof(struct timeval));

	tv_out->tv_sec = tve->tv_sec - tvb->tv_sec;
	tv_out->tv_usec = tve->tv_usec - tvb->tv_usec;
	if (tv_out->tv_usec < 0) {
		tv_out->tv_usec += 1000000;
		tv_out->tv_sec--;
	}

	return F1G_OK;
}

i32_t time_interval(struct timeval *tvb, struct timeval *tve)
{
	struct timeval tv;

	time_interval_tv(tvb, tve, &tv);

	return tv.tv_sec*1000000+tv.tv_usec;
}

i32_t set_daemon(string_t exec_name)
{
	i32_t i = 0;
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		return F1G_ERR;
	} else if (0 != pid) {
		// parent process exit.
		//fprintf(stdout, "%s parent process %d exit!\n", exec_name, getpid());
		exit(0);
	}

	//fprintf(stdout, "exec_name subprocess %d running\n", exec_name, getpid());

	setsid();
	chdir("/");
	umask(0);
	
	for (i=0; i<3; i++) {
		close(i);
	}

	return F1G_OK;
}

