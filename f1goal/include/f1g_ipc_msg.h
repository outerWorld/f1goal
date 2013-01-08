
#ifndef __F1G_IPC_MSG_H
#define __F1G_IPC_MSG_H

#include "f1g_basic_types.h"

enum {
	MSG_TYPE_DATA = 0x00,
	MSG_TYPE_NOTICE
};

typedef struct _ipc_msg_header_s {
	i32_t	msg_type;
	i32_t	msg_src;
	i32_t	msg_src_port;
	i32_t	msg_dst;
	i32_t	msg_dst_port;
	i32_t	msg_len;
}ipc_msg_header_t, *ipc_msg_header_p;

#endif // __F1G_IPC_MSG_H
