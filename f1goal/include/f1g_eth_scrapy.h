
#ifndef __F1G_ETH_SCRAPY_H
#define __F1G_ETH_SCRAPY_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
	ETH_SCRAPY_START = 0x01,
	ETH_SCRAPY_STOP,
	ETH_SCRAPY_STATUS
};
enum {
    SCRAPY_CARD = 0x00,
    SCRAPY_FILE,
};

enum {
    SCRAPY_INIT = 0x00,
    SCRAPY_RUNNING,
    SCRAPY_OFF,
};

int eth_scrapy_init(char *eth, char *filter, int type);
int eth_scrapy_reg(pcap_handler handler);
int eth_scrapy_start();
int eth_scrapy_control(int code);
int eth_scrapy_destroy();

#ifdef __cplusplus
}
#endif

#endif
