
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_accessor.h"

// paras format is
//	ip:127.0.0.1;port:8080;socket_size:100;read_timeout:10;write_timeout:10;
static i32_t epoller_init(epoller_p p_ep, i32_t sock_type, string_t paras)
{
	return F1G_OK;
}

i32_t accessor_init(accessor_p p_acc, i32_t link_type, i32_t sock_type, string_t paras)
{
	switch (link_type) {
		case LINKER_EPOLL:
			p_acc->linkers[LINKER_EPOLL] = malloc(sizeof(epoller_t));
			if (!p_acc->linkers[LINKER_EPOLL]) return F1G_ERR;
			if (F1G_OK != epoller_init((epoller_p)p_acc->linkers[LINKER_EPOLL], sock_type, paras)) return F1G_ERR;
			break;
		
		case LINKER_POLL:
			break;

		case LINKER_SELECT:
			break;

		case LINKER_PCAP:
			break;

		case LINKER_USER_DEF:
			break;

		default:
			return F1G_ERR;
	}

	return F1G_OK;
}

accessor_p accessor_create(i32_t type, i32_t sock_type, string_t paras)
{
	accessor_p p_acc = NULL;

	p_acc = (accessor_p)malloc(sizeof(accessor_t));
	if (!p_acc) return NULL;
	memset(p_acc, 0x00, sizeof(accessor_t));

	if (F1G_OK != accessor_init(p_acc, type, sock_type, paras)) {
		free(p_acc);
		return NULL;
	}

	return p_acc;
}

i32_t accessor_detect(accessor_p p_acc)
{
	return F1G_OK;
}

i32_t accessor_check_status(accessor_p p_acc)
{
	return F1G_OK;
}

i32_t accessor_clear(accessor_p p_acc)
{
	return F1G_OK;
}

i32_t accessor_destroy(accessor_p p_acc)
{
	return F1G_OK;
}

