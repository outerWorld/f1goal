
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "f1g_basic_funcs.h"
#include "f1g_udp.h"

i8_t udp_object_init(string_t ip, u16_t port, udp_object_p p_obj)
{
	p_obj->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == p_obj->fd) {
		return F1G_ERR;
	}

	return F1G_OK;
}

udp_object_p udp_object_create(string_t ip, u16_t port)
{
	udp_object_p p_obj;

	p_obj = (udp_object_p)malloc(sizeof(udp_object_t));
	if (NULL == p_obj) {
		return NULL;
	}

	if (F1G_OK != udp_object_init(ip, port, p_obj) ) {
		free(p_obj);
		p_obj = NULL;
		return NULL;
	}

	return p_obj;
}


i8_t udp_object_destroy(udp_object_p p_obj)
{
}

