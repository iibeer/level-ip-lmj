#include "netdev.h"
#include "ip.h"
#include "skbuff.h"
#include "tuntap_if.h"

/*维护两个网络设备loop和netdev*/
static struct netdev *loop;
static struct netdev *netdev;
extern int running;

static struct netdev *netdev_alloc(char *addr, char *hw_addr, uint32_t mtu) {
    assert(NULL != addr);
    assert(NULL != hw_addr);

    struct netdev *dev = (struct netdev *)malloc(sizeof(struct netdev));
    if (NULL == dev) {
        print_err("ERR: malloc netdev failed\n");
        return NULL;
    }

    dev->addr = ip_parse(addr);
    /*hhx: 8位16进制*/
    sscanf(hw_addr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hw_addr[0],
                                                     &dev->hw_addr[1],
                                                     &dev->hw_addr[2],
                                                     &dev->hw_addr[3],
                                                     &dev->hw_addr[4],
                                                     &dev->hw_addr[5]);
    dev->addr_len = MAC_ADDR_LEN;
    dev->mtu = mtu;
    return dev;
}

int netdev_init() {
    loop = netdev_alloc("127.0.0.1", "00:00:00:00:00:00", 1500);
    if (NULL == loop) {
        print_err("ERR: alloc netdev loop failed\n");
        return -1;
    }
    netdev = netdev_alloc("10.0.0.4", "00:0c:29:6d:50:25", 1500);
    if (NULL == netdev) {
        print_err("ERR: alloc netdev netdev failed\n");
        return -1;
    }
    return 0;
}

void netdev_free() {
    free(loop);
    free(netdev);
}

int netdev_transmit(struct sk_buff *skb, uint8_t *dmac, uint16_t ethertype) {
    struct netdev *dev;
    struct eth_hdr *hdr;

    dev = skb->dev;
    hdr = (struct eth_hdr *)skb_push(skb, sizeof(ETH_HDR_LEN));
    memcpy(hdr->dmac, dmac, MAC_ADDR_LEN);
    memcpy(hdr->smac, dev->hw_addr, MAC_ADDR_LEN);
    hdr->ethertype = htons(ethertype);
    eth_dbg("out", hdr);

    return tuntap_write((char *)skb->data, skb->len);
}

static void netdev_recieve(struct sk_buff *skb) {
    struct eth_hdr *hdr = eth_hdr(skb);
    hdr = (struct eth_hdr *)skb->data;
    eth_dbg("in", hdr);

    switch(hdr->ethertype) {
        default:
            print_dbg("DBG: recv skb ethertype: %x\n", hdr->ethertype);
            free(skb);
            break;
    }
}

void netdev_rx_loop() {
    while (running) {
        struct sk_buff *skb = skb_alloc(BUFLEN);
        if (NULL == skb) {
            return;
        }
        if (tuntap_read((char *)skb->data, BUFLEN) < 0) {
            print_err("ERR: tuntap_read failed\n");
            free(skb);
            return;
        }
        netdev_recieve(skb);
    }

    return;
}