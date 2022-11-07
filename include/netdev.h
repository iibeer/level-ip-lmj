#ifndef NETDEV_H
#define NETDEV_H

#include "syshead.h"
#include "utils.h"
#include "ethernet.h"

#define BUFLEN 1600

struct sk_buff;

struct netdev {
    uint32_t addr;
    uint8_t addr_len;
    uint8_t hw_addr[MAC_ADDR_LEN];
    uint32_t mtu;
};

int netdev_init();
void netdev_free();
int netdev_transmit(struct sk_buff *skb, uint8_t *dmac, uint16_t ethertype);
void netdev_rx_loop();

#endif