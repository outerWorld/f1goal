/*
 * Author: huang chunping
 * Date  : 2013-01-13
 */

#ifndef __F1G_FL_MGR_H
#define __F1G_FL_MGR_H

#include "f1g_basic_types.h"
#include "f1g_buffer.h"
#include "f1g_template.h"

enum {
	STATIC_FILE = 0x00,
	DYN_FILE,	// which means the buffer should be parsed by template-engine.
};

/*
 *
 */
typedef struct _host_fl_s {
	// static file(0x00), dynamic file(0x01)
	i8_t			type;
	i8_t			fl_name[FL_NAME_SIZE];	//
	f1g_template_t	fl_tmpl;				//
}host_fl_t, *host_fl_p;

typedef struct _host_fl_list_s {
	i8_t		host[HOST_SIZE];
	i8_t		path[HOST_PATH_SIZE];
	i32_t		fl_num;
	i32_t		fl_cap;
	host_fl_p 	p_fls; //
}host_fl_list_t, *host_fl_list_p;

i32_t host_fl_list_enlarge_cap(host_fl_list_p p_hfl, i32_t new_cap);

// file manager
typedef struct _fl_mgr_s {
	i32_t			host_num;
	i32_t			host_cap;
	host_fl_list_p 	p_hfl; // host file list
}fl_mgr_t, *fl_mgr_p;

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
i32_t fl_mgr_load(fl_mgr_p p_fm, string_t host_paths);

i32_t fl_mgr_init(fl_mgr_p p_fm, i32_t host_cap, i32_t fl_cap);

fl_mgr_p fl_mgr_create(i32_t host_cap, i32_t fl_cap, string_t host_paths);

i32_t fl_mgr_enlarge_host_cap(fl_mgr_p p_fm, i32_t new_cap);

i32_t fl_mgr_addhost(fl_mgr_p p_fm, string_t host, string_path);

i32_t fl_mgr_chghost(fl_mgr_p p_fm, string_t host, string_path);

i32_t fl_mgr_delhost(fl_mgr_p p_fm, string_t host);

i32_t fl_mgr_add_fl(fl_mgr_p p_fm, string_t host, string_t file);

i32_t fl_mgr_reload_fl(fl_mgr_p p_fm, string_t host, string_t file);

i32_t fl_mgr_del_fl(fl_mgr_p p_fm, string_t host, string_t file);

/*
 * type: how to get the file content, copy file content to buffer or copy file content address to buffer.
 */
enum {
	GET_FL_COPY = 0x00, // get file content and content size.
	GET_FL_INFO,		// get file information: content address, content size.
};
i32_t fl_mgr_get_fl(fl_mgr_p p_fm, string_host, string_t file, buffer_p p_buf, i32_t type);

i32_t fl_mgr_clear(fl_mgr_p p_fm);

i32_t fl_mgr_destroy(fl_mgr_p p_fm);

#endif // __F1G_FL_MGR_H
