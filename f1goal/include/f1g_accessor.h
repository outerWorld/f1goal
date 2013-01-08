
#ifndef __F1G_ACCESSOR_H
#define __F1G_ACCESSOR_H

#include "f1g_basic_types.h"
#include "f1g_buffer.h"

enum {
	LINKER_EPOLL = 0x00,
	LINKER_POLL,
	LINKER_SELECT,
	LINKER_PCAP,
	LINKER_USER_DEF,
	LINKER_MAX_NUM
};

enum {
	SOCK_TYPE_LTCP = 0x00, // long connection
	SOCK_TYPE_STCP = 0x00, // short connection
	SOCK_TYPE_UDP,
	SOCK_TYPE_RAW_TCP,
	SOCK_TYPE_RAW_UDP,
};

typedef struct _selector_s {
	i32_t	accessor_id; // epollfd or selectfd
	i32_t	listen_fd;   // socketfd for listening connections
}selector_t, *selector_p;

typedef struct _poller_s {
	i32_t	accessor_id; // epollfd or selectfd
	i32_t	listen_fd;   // socketfd for listening connections
}poller_t, *poller_p;

typedef struct _epoller_s {
	i32_t	accessor_id; // epollfd or selectfd
	i32_t	listen_fd;   // socketfd for listening connections
	i32_t	event_size;
	i32_t	event_num;
	struct epoll_event * events;
}epoller_t, *epoller_p;

typedef struct _pcaper_s {
}pcaper_t, *pcaper_p;

typedef struct _accessor_s {
	i32_t 		link_mode;
	buffer_t	rdbuf;
	buffer_t	wrbuf;
	void *		linkers[LINKER_MAX_NUM];
}accessor_t, *accessor_p;

i32_t accessor_init(accessor_p p_acc, i32_t link_mode, i32_t sock_type, string_t paras);

accessor_p accessor_create(i32_t link_type, i32_t sock_type, string_t paras);

// detect there is any data in accessor, 0x00(yes), 0x01(no), 0x02(other)
i32_t accessor_detect(accessor_p p_acc);

// it returns the number of data not fetched in accessor
i32_t accessor_check_status(accessor_p p_acc);

i32_t accessor_get_data(accessor_p p_acc);

i32_t accessor_clear(accessor_p p_acc);

i32_t accessor_destroy(accessor_p p_acc);

#endif // __F1G_ACCESSOR_H
