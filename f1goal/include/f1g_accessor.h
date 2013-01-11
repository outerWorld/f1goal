
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

// definition of socket status(sst)
enum {
	SST_INIT = 0x00,
	SST_DATA_IN,
	SST_DATA_OUT,
	SST_DISC, // disconnect
	SST_ERR,
	SST_UNKNOWN,
	SST_NUM
};
typedef struct _sock_info_s {
	i32_t	fd;
	u32_t 	src_ip;
	u16_t	src_port;
	i16_t	sock_status; // INIT(0x00), DATA IN(0x01), DISCONNECT(0x02), ERROR(0x03)
}sock_info_t, *sock_info_p;
i32_t sock_info_show(sock_info_p p_si);

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
	i32_t   next_event;  // the event to be processed
	struct epoll_event * events;
	//sock_info_p p_si; // for reducing memory copy, make a preallocation, implemented later.
}epoller_t, *epoller_p;

typedef struct _pcaper_s {
}pcaper_t, *pcaper_p;

typedef struct _accessor_s {
	i32_t 		link_mode;
	buffer_t	rdbuf;
	buffer_t	wrbuf;
	void *		linkers[LINKER_MAX_NUM];
}accessor_t, *accessor_p;

enum {
	RECV_INIT = 0x00,
	RECV_OK = 0x01,
	RECV_ERR = 0x02, // the lowest bit: 1 (ok), 0(error)
	RECV_TIMEOUT = 0x04,
	RECV_DISC = 0x08, // disconnected found
	RECV_MEM_LACK = 0x10 // maybe need more memory.
};
i32_t nonblk_recvfrom(i32_t fd, buffer_p p_buf, i32_t *recv_stat);
i32_t nonblk_sendto(i32_t fd, u32_t ip, u16_t port, buffer_p p_buf, i32_t *send_stat);

i32_t accessor_init(accessor_p p_acc, i32_t link_mode, i32_t sock_type, string_t paras);

accessor_p accessor_create(i32_t link_type, i32_t sock_type, string_t paras);

// it returns the event number
i32_t accessor_detect(accessor_p p_acc);

// it returns the next event to be processed
i32_t accessor_check_status(accessor_p p_acc, sock_info_p p_si);

i32_t accessor_get_data(accessor_p p_acc);

i32_t accessor_clear(accessor_p p_acc);

i32_t accessor_destroy(accessor_p p_acc);

#endif // __F1G_ACCESSOR_H
