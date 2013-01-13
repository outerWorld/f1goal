/*
 * Author: huang chunping
 * Date  : 2013-01-13
 */

#ifndef __F1G_FL_MGR_H
#define __F1G_FL_MGR_H

#include "f1g_basic_types.h"

#define FL_BASE_PATH	"resource"

// file manager
typedef struct _fl_mgr_s {
	i8_t	base_path[];
	i8_t	host[]; // host maps to local file path.
}fl_mgr_t, *fl_mgr_p;

i32_t fl_mgr_load(fl_mgr_p p_fm, i32_t fl_cap, string_t fpath);

fl_mgr_p fl_mgr_create(i32_t fl_cap, string_t fpath);

i32_t fl_mgr_add_fl(fl_mgr_p p_fm, string_t file);

i32_t fl_mgr_clear(fl_mgr_p p_fm);

i32_t fl_mgr_destroy(fl_mgr_p p_fm);

#endif // __F1G_FL_MGR_H
