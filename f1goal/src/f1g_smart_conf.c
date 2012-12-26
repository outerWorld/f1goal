
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "f1g_smart_conf.h"


#define DFT_SEC_SIZE	20
#define DFT_ITEM_SIZE	20
i32_t smart_conf_init(smart_conf_p p_conf, i32_t sec_size, i32_t item_size)
{
	i32_t i, j;
	sec_p p_sec;
	item_p p_item;

	if (sec_size <= 0) { sec_size = DFT_SEC_SIZE; }
	if (item_size <= 0) { item_size = DFT_ITEM_SIZE; }

	p_sec = (sec_p)malloc(sizeof(sec_t)*sec_size);
	if (!p_sec) { return F1G_ERR; }
	memset(p_sec, 0x00, sizeof(sec_t)*sec_size);

	for (i=0; i<sec_size; i++) {
		p_item = (item_p)malloc(item_size*sizeof(item_t));
		if (!p_item) { return F1G_ERR; }
		memset(p_item, 0x00, item_size*sizeof(item_t));
		p_sec[i].p_items = p_item;
		p_sec[i].item_num = 0;
		p_sec[i].item_size = item_size;
	}

	p_conf->sec_num = 0;
	p_conf->sec_size = sec_size;
	p_conf->p_secs = p_sec;

	return F1G_OK;
}

static i32_t extend_sec_size(smart_conf_p p_conf, int ext_sec_size)
{
	sec_p p_sec = p_conf->p_secs;

	p_conf->p_secs = (sec_p)malloc(sizeof(sec_t)*ext_sec_size);
	if (!p_conf->p_secs) { return F1G_ERR; }
	memset(p_conf->p_secs, 0x00, sizeof(sec_t)*ext_sec_size);

	if (p_conf->sec_num > 0) { memcpy(p_conf->p_secs, p_sec, p_conf->sec_num*sizeof(sec_t)); }
	
	p_conf->sec_size = ext_sec_size;

	if (p_sec) { free(p_sec); }
	
	return F1G_OK;
}

static i32_t extend_item_size(sec_p p_sec, int ext_item_size)
{
	item_p p_item = p_sec->p_items;

	p_sec->p_items = (item_p)malloc(sizeof(item_t)*ext_item_size);
	if (!p_sec->p_items) { return F1G_ERR; }
	memset(p_sec->p_items, 0x00, sizeof(item_t)*ext_item_size);

	if (p_sec->item_num > 0) { memcpy(p_sec->p_items, p_item, p_sec->item_num*sizeof(item_t)); }
	
	p_sec->item_size = ext_item_size;

	if (p_item) { free(p_item); }
	
	return F1G_OK;
}


static i32_t get_line(i8_p p_data, i32_t data_len, i8_p p_line, i32_t line_size)
{
	return F1G_OK;
}

/*
 * support: "#" as comment
 */
static i32_t ini_read(smart_conf_p p_conf, string_t conf_file) {
	i32_t fd = 0;
	i32_t i = 0;
	i8_p p_proc = NULL;
	i8_p line = NULL;
	i8_p p_cur = NULL;
	i8_p buf = NULL;
	i8_p buf_end = NULL;
	i32_t buf_size = 0;
	i32_t proc_left = 0;
	off_t read_remain = 0;
	struct stat st;
	i32_t read_flag = 0; // 1(complete reading file)
	i32_t len = 0;
	sec_p p_cur_sec = NULL;
	item_p p_item = NULL;
	i32_t sec_seq = 0;
	i32_t item_seq = 0;

	fd = open(conf_file, O_RDONLY);
	if (fd <= 0) { return F1G_ERR; }
	if (fstat(fd, &st) != 0) { return F1G_ERR; }
	read_remain = st.st_size;
	buf_size = st.st_blksize;

	buf = (i8_p)malloc(buf_size+1); // note that more 1 byte is appended.
	if (!buf) { return F1G_ERR; }

	while (read_remain > 0) {
		// read file.
		if (proc_left > 0) { memcpy(buf, buf+buf_size-proc_left, proc_left); }
		len = read(fd, buf+proc_left, buf_size-proc_left);
		if (0 == len) {
			// reach the end of file.
			read_flag = 1;
		} else if (-1 == len) {
			// process error conditions.
		} else {
			read_remain -= len;
			if (0 == read_remain) { read_flag = 1; }
		}
		buf_end = buf + len;
		proc_left = len;

		p_proc = buf;
		while (p_proc < buf_end) {
			line = p_proc;
			// get one line.
			while (p_proc < buf_end && '\n' != *p_proc) { p_proc++; }
			proc_left = buf_end - p_proc;
			if (p_proc == buf_end && 0 == read_flag) {
				break;
			}
			*p_proc = '\0'; // this process is must.
			p_proc++;

			// skip spaces
			while (' ' == *line && '\0' != *line) { line++; }
			// is comment or nothing in the line?
			if ('#' == *line || '\0' == *line) { continue; }

			// is section and valid one ?
			if ('[' == *line) {
				if (sec_seq >= p_conf->sec_size) {
					if (F1G_OK != extend_sec_size(p_conf, p_conf->sec_size*2)) { return F1G_ERR; }
				}

				p_cur_sec = &p_conf->p_secs[sec_seq];
				p_cur = ++line;
				// reach the ']'
				while (']' != *p_cur && '\0' != *p_cur) { p_cur++; }
				if (p_cur-line >= SEC_NAME_SIZE || ']' != *p_cur) { return F1G_ERR; }
				memcpy(p_cur_sec->sec_name, line, p_cur-line);
				p_cur_sec->sec_name[p_cur-line] = '\0';
				// find there is any same section in configure ?
				for (i=0; i<sec_seq; i++) {
					if (strcasecmp(p_cur_sec->sec_name, p_conf->p_secs[i].sec_name) == 0) { return F1G_ERR; }
				}
				sec_seq++;
				item_seq = 0;
			} else {
				// is item and valid one ?
				if (item_seq >= p_cur_sec->item_size) {
					if (extend_item_size(p_cur_sec, p_cur_sec->item_size*2)) { return F1G_ERR; }
				}

				p_item = &p_cur_sec->p_items[item_seq];
				p_cur = line;
				while (' ' != *p_cur && '=' != *p_cur && '\0' != *p_cur) { p_cur++; }
				// check valid item name.
				if (p_cur-line >= ITEM_NAME_SIZE  || (' ' != *p_cur && '=' != *p_cur)) { return F1G_ERR; }
				memcpy(p_item->item_name, line, p_cur-line);
				p_item->item_name[p_cur-line] = '\0';
				// find there is any same item in configure?
				for (i=0; i<item_seq; i++) {
					if (strcasecmp(p_item->item_name, p_cur_sec->p_items[i].item_name) == 0) { return F1G_ERR; }
				}
				// skip spaces
				while (' ' == *p_cur && '\0' != *p_cur) { p_cur++; }
				// check '='
				if ('=' != *p_cur) { return F1G_ERR; }
				p_cur++;
				// skip spaces
				while (' ' == *p_cur && '\0' != *p_cur) { p_cur++; }
				line = p_cur;
				// traverse the value
				while ('\n' != *p_cur && '\0' != *p_cur) { p_cur++; }
				// trip space in tail.
				p_cur--;
				while (' ' == *p_cur && line != p_cur) { p_cur--; }
				p_cur++;
				p_item->val = (i8_p)malloc(p_cur-line+1);// 1 more byte allocated.
				if (!p_item->val) { return F1G_ERR; }
				memcpy(p_item->val, line, p_cur-line);
				p_item->val[p_cur-line] = '\0';
				p_item->val_len = p_cur - line;

				item_seq++;
				p_cur_sec->item_num++;
			}
		}
	}

	p_conf->sec_num = sec_seq;
	
	close(fd);

	return F1G_OK;
}

i32_t smart_conf_read(smart_conf_p p_conf, string_t conf_file, i32_t conf_type)
{
	switch (conf_type) {
		case CFT_INI:
			return ini_read(p_conf, conf_file);
			break;
		case CFT_LINE:
			// not support now.
			break;
		default:
			break;
	}

	return F1G_OK;
}

static item_p find_item(smart_conf_p p_conf, string_t sec, string_t item_name)
{
	i32_t i, j;
	sec_p p_sec = NULL;
	item_p p_item = NULL;

	for (i=0; i<p_conf->sec_num; i++) {
		p_sec = &p_conf->p_secs[i];
		if (strcasecmp(p_sec->sec_name, sec) == 0) {
			for (j=0; j<p_sec->item_num; j++) {
				if (strcasecmp(p_sec->p_items[j].item_name, item_name) == 0) {
					p_item = &p_sec->p_items[j];
					break;
				}
			}
		}
	}

	return p_item;
}

i32_t smart_conf_get_u32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u32_t dft_val, u32_p u32_val)
{
	item_p p_item;
	i8_p p_end = NULL;

	p_item = find_item(p_conf, sec, item_name);
	if (!p_item || 0 == strlen(p_item->val)) {
		*u32_val = dft_val;
	} else {
		*u32_val = strtoul(p_item->val, &p_end, 10);
		if ('\0' != *p_end) { return F1G_ERR; }
	}
		
	return F1G_OK;
}

i32_t smart_conf_get_x32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u32_t dft_val, u32_p u32_val)
{
	item_p p_item;
	i8_p p_end = NULL;

	p_item = find_item(p_conf, sec, item_name);
	if (!p_item || 0 == strlen(p_item->val)) {
		*u32_val = dft_val;
	} else {
		*u32_val = strtoul(p_item->val, &p_end, 16);
		if ('\0' != *p_end) { return F1G_ERR; }
	}
		
	return F1G_OK;
}

i32_t smart_conf_get_i32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 i32_t dft_val, i32_p i32_val)
{
	item_p p_item;
	i8_p p_end = NULL;

	p_item = find_item(p_conf, sec, item_name);
	if (!p_item || 0 == strlen(p_item->val)) {
		*i32_val = dft_val;
	} else {
		*i32_val = strtol(p_item->val, &p_end, 10);
		if ('\0' != *p_end) { return F1G_ERR; }
	}
		
	return F1G_OK;
}

i32_t smart_conf_get_i64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 i64_t dft_val, i64_p i64_val)
{
	item_p p_item;
	i8_p p_end = NULL;

	p_item = find_item(p_conf, sec, item_name);
	if (!p_item || 0 == strlen(p_item->val)) {
		*i64_val = dft_val;
	} else {
		*i64_val = strtoll(p_item->val, &p_end, 10);
		if ('\0' != *p_end) { return F1G_ERR; }
	}

	return F1G_OK;
}


i32_t smart_conf_get_u64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u64_t dft_val, u64_p u64_val)
{
	item_p p_item;
	i8_p p_end = NULL;

	p_item = find_item(p_conf, sec, item_name);
	if (!p_item || 0 == strlen(p_item->val)) {
		*u64_val = dft_val;
	} else {
		*u64_val = strtoull(p_item->val, &p_end, 10);
		if ('\0' != *p_end) { return F1G_ERR; }
	}

	return F1G_OK;
}

i32_t smart_conf_get_x64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u64_t dft_val, u64_p u64_val)
{
	item_p p_item;
	i8_p p_end = NULL;

	p_item = find_item(p_conf, sec, item_name);
	if (!p_item || 0 == strlen(p_item->val)) {
		*u64_val = dft_val;
	} else {
		*u64_val = strtoull(p_item->val, &p_end, 16);
		if ('\0' != *p_end) { return F1G_ERR; }
	}

	return F1G_OK;
}

i32_t smart_conf_get_str(smart_conf_p p_conf, string_t sec, string_t item_name,
								 string_t dft_val, i8_p p_buf, i32_t buf_size)
{
	item_p p_item;
	i8_p p_end = NULL;

	p_item = find_item(p_conf, sec, item_name);
	if (!p_item || 0 == strlen(p_item->val)) {
		strncpy(p_buf, dft_val, buf_size-1);
		p_buf[buf_size-1] = '\0';
	} else {
		if (p_item->val_len >= buf_size) {
			return F1G_ERR;
		} else {
			memcpy(p_buf, p_item->val, p_item->val_len);
			p_buf[p_item->val_len] = '\0';
		}
	}

	return F1G_OK;
}

i32_t smart_conf_destroy(smart_conf_p p_conf)
{
	i32_t i, j;
	sec_p p_sec = NULL;
	
	for (i=0; i<p_conf->sec_num; i++) {
		p_sec = &p_conf->p_secs[i];
		
		for (j=0; j<p_sec->item_num; j++) {
			if (p_sec->p_items[j].val) {
				free(p_sec->p_items[j].val);
			}
		}
		free(p_sec->p_items);
	}
	
	if (p_conf->p_secs) {
		free(p_conf->p_secs);
	}

	memset(p_conf, 0x00, sizeof(smart_conf_t));

	return F1G_OK;
}

i32_t smart_conf_dbg(smart_conf_p p_conf)
{
	i32_t i, j;
	sec_p p_sec = NULL;
	
	fprintf(stdout, "sec size: %d, sec num: %d\n", p_conf->sec_size, p_conf->sec_num);
	for (i=0; i<p_conf->sec_num; i++) {
		p_sec = &p_conf->p_secs[i];
		fprintf(stdout, "\tsec name: %s, item_size: %d, item num: %d\n", 
				p_sec->sec_name, p_sec->item_size, p_sec->item_num);
		for (j=0; j<p_sec->item_num; j++) {
			fprintf(stdout, "\t\titem name: %s, item value: %s\n", p_sec->p_items[j].item_name, p_sec->p_items[j].val);
		}
	}
}
