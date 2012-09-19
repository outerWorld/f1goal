
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
#include <errno.h>

#include "pcap.h"

#include "f1g_eth_scrapy.h"

static int run_flag = 0;
static pcap_t * sg_scraper = NULL;
pcap_handler sg_handler = 0;

int eth_scrapy_init(char *eth, char *filter)
{
	int ret;
	struct bpf_program fp;
	bpf_u_int32 mask;
	bpf_u_int32 net;
	char errbuf[128];

	ret = pcap_lookupnet(eth, &net, &mask, errbuf);
	if (-1 == ret) {
		fprintf(stderr, "[%s,%d] pcap_lookupnet fail[%s]!\n", __FUNCTION__, __LINE__, errbuf);
		return -1;
	}

	sg_scraper = pcap_open_live(eth, 65535, 0, 10, errbuf);
	if (NULL == sg_scraper) {
		fprintf(stderr, "open device [%s] fail!\n", eth);
		return -1;
	}
	
	ret = pcap_setnonblock(sg_scraper, 1, errbuf);
	if (-1 == ret) {
		pcap_close(sg_scraper);
		fprintf(stderr, "pcap_setnonblock [%s] fail!\n", errbuf);
		sg_scraper = NULL;
		return -1;
	}

	ret = pcap_compile(sg_scraper, &fp, filter, 0, mask);
	if (-1 == ret) {
		pcap_close(sg_scraper);
		fprintf(stderr, "pcap_compile [%s] fail!\n", filter);
		sg_scraper = NULL;
		return -1;
	}
	
	ret = pcap_setfilter(sg_scraper, &fp);
	if (-1 == ret) {
		pcap_close(sg_scraper);
		fprintf(stderr, "pcap_setfilter [%s] fail!\n", filter);
		sg_scraper = NULL;
		return -1;
	}
	
	pcap_freecode(&fp);
	run_flag = 1;

	return 0;
}

int eth_scrapy_reg(pcap_handler handler)
{
	sg_handler = handler;

	return 0;
}

static void * eth_scrapy_callback(void *arg)
{
	int ret;

	while (1 == run_flag) {
		ret = pcap_dispatch(sg_scraper, 0, sg_handler, NULL);
		if (-1 == ret) {
			fprintf(stderr, "[%s,%d] [errno=%d] pcap_dispatch fail!\n", __FUNCTION__, __LINE__, errno);
			break;
		}
	}

	fprintf(stderr, "[%s,%d] eth scraper exit!\n", __FUNCTION__, __LINE__);
	run_flag = 0;
	return NULL;
}

int eth_scrapy_start()
{
	int ret;
	pthread_t thread;
	ret = pthread_create(&thread, NULL, eth_scrapy_callback, NULL);
	if (0 != ret) {
		fprintf(stderr, "[%s,%d] [errno=%d] Create Thread Fail!\n", __FUNCTION__, __LINE__, errno);
		return -1;
	}

	return 0;
}

int eth_scrapy_control(int code)
{
	static const int wait_time = 1000;
	switch(code) {
		case ETH_SCRAPY_STOP:
			run_flag = 0;
			fprintf(stdout, "[%s,%d] please wait %s useconds to exit!\n", __FUNCTION__, __LINE__, wait_time); 
			usleep(wait_time);
			break;
		case ETH_SCRAPY_START:
			run_flag = 1;
			break;
		case ETH_SCRAPY_STATUS:
			return run_flag;
		default:
			break;
	}

	return 0;
}

int eth_scrapy_destroy()
{
	pcap_close(sg_scraper);
	sg_scraper = NULL;
	return 0;
}

