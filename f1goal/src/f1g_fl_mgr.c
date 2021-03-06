
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_smart_conf.h"
#include "f1g_fl_mgr.h"


i32_t fl_mgr_enlarge_host_cap(fl_mgr_p p_fm, i32_t new_cap)
{
	host_fl_list_p p_tmp = NULL;

	if (new_cap < p_fm->host_cap) return F1G_ERR;

	if (new_cap <= 0) new_cap = 4;
	
	p_tmp = (host_fl_list_p)malloc(sizeof(host_fl_list_t)*new_cap);
	if (!p_tmp) return F1G_ERR;
	memset(p_tmp, 0x00, sizeof(host_fl_list_t)*new_cap);

	if (p_fm->host_num > 0) memcpy(p_tmp, p_fm->p_hfl, sizeof(host_fl_list_t)*p_fm->host_num);

	if (p_fm->p_hfl) free(p_fm->p_hfl);
	
	p_fm->p_hfl = p_tmp;
	p_fm->host_cap = new_cap;

	return F1G_OK;
}

i32_t host_fl_list_enlarge_cap(host_fl_list_p p_hfl, i32_t new_cap)
{
	host_fl_p p_tmp = NULL;

	if (new_cap < p_hfl->fl_cap) return F1G_ERR;

	if (new_cap <= 0) new_cap = 64;

	p_tmp = (host_fl_p)malloc(sizeof(host_fl_list_t)*new_cap);
	if (!p_tmp) return F1G_ERR;
	memset(p_tmp, 0x00, sizeof(host_fl_list_t)*new_cap);

	if (p_hfl->fl_num > 0) memcpy(p_tmp, p_hfl->p_fls, sizeof(host_fl_list_t)*p_hfl->fl_num);

	if (p_hfl->p_fls) free(p_hfl->p_fls);

	p_hfl->p_fls = p_tmp;
	p_hfl->fl_cap = new_cap;

	return F1G_OK;
}

/*  file format:
 *	[host_path]
 *	host_num = 2
 *  host[0] = exam1.com
 *  host[1] = exam2.com
 *	
 *	[exam1.com]
 *	path = /var/www/html/exam1
 *  [exam2.com]
 *	path = /var/www/html/exam2
 */
i32_t fl_mgr_load(fl_mgr_p p_fm, string_t host_paths)
{
	i32_t	ret = F1G_ERR;
	i32_t	host_num = 0;
	smart_conf_t sc;
	host_fl_list_p p_fll = NULL;
	i8_t	sec[SEC_NAME_SIZE+2];
	i8_t	item[ITEM_NAME_SIZE+2];

	if (F1G_OK != smart_conf_init(&sc, 0, 0)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_read(&sc, host_paths, CFG_INI)) {
		goto _load_exit;
	}

	if (F1G_OK != smart_conf_get_i32(&sc, "host_path", "host_num", 0, &host_num)) {
		goto _load_exit;
	}

	//
	if (p_fm->host_cap < host_num) {
		// enlarge
		if (F1G_OK != fl_mgr_enlarge_host_cap(p_fm, host_num + 5)) {
			goto _load_exit;
		}
	}

	for (i=0; i<host_num; i++) {
		p_fll = &p_fm->p_hfl[i];
		snprinf(item, ITEM_NAME_SIZE, "host[%d]", i);
		if (F1G_OK != smart_conf_get_str("host_path", item, "", p_fll->host, HOST_SIZE)) {
			goto _load_exit;
		}

		if (F1G_OK != smart_conf_get_str(p_fll->host, "path", "", p_fll->path, HOST_PATH_SIZE)) {
			goto _load_exit;
		}
	
		// traverse the path, get all the files into memory.
	}

	ret = F1G_OK;
_load_exit:
	smart_conf_destroy(&sc);

	return ret;
}

i32_t fl_mgr_init(fl_mgr_p p_fm, i32_t host_cap, i32_t fl_cap)
{
	i32_t	i;
	host_fl_p p_fl;

	memset(p_fm, 0x00, sizeof(fl_mgr_t));

	if (host_cap <= 0) host_cap = 8;

	p_fm->p_hfl = (host_fl_list_p)malloc(sizeof(host_fl_list_t)*host_cap);
	if (!p_fm->p_hfl) return F1G_ERR;
	memset(p_fm->p_hfl, 0x00, sizeof(host_fl_list_t)*host_cap);
	p_fm->host_cap = host_cap;

	if (fl_cap <= 0) fl_cap = 16;
	p_hfl = p_fm->p_hfl;
	for (i=0; i<host_cap; i++) {
		p_hfl[i].p_fls = (host_fl_p)malloc(sizeof(host_fl_t)*fl_cap);
		if (!p_hfl[i].p_fls) return F1G_ERR;
		memset(p_hfl[i].p_fls, 0x00, sizeof(host_fl_t)*fl_cap);
		p_hfl[i].fl_cap = fl_cap;
	}

	return F1G_OK;
}

fl_mgr_p fl_mgr_create(i32_t host_cap, i32_t fl_cap, string_t host_paths)
{
	fl_mgr_p p_fm;

	p_fm = (fl_mgr_p)malloc(sizeof(fl_mgr_t));
	if (!p_fm) return NULL;

	if (F1G_OK != fl_mgr_init(p_fm, host_cap, fl_cap)) return NULL;

	return p_fm;
}
