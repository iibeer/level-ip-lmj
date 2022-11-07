#include "syshead.h"
#include "utils.h"
#include "tuntap_if.h"
#include "netdev.h"

int running = 1;

int main() {
    if (tuntap_init() < 0) {
        return -1;
    }

    if (netdev_init() < 0) {
        return -1;
    }
    
    while (1) {
        netdev_rx_loop();
    }
    // int tuntap_write(char *buf, int len);
    // tuntap_free();
    return 0;
}