#include "syshead.h"
#include "utils.h"
#include "basic.h"

static int tuntap_fd;
static char* dev;

char *tuntap_ip = "10.0.0.5";
char *tuntap_route = "10.0.0.0/24";

#define DEV_MAX_SIZE 10

static int set_if_route(char *dev, char *cidr)
{
    return run_cmd("ip route add dev %s %s", dev, cidr);
}

static int set_if_address(char *dev, char *cidr)
{
    return run_cmd("ip address add dev %s local %s", dev, cidr);
}

static int set_if_up(char *dev)
{
    return run_cmd("ip link set dev %s up", dev);
}


static int tuntap_alloc(char *dev) {
    assert(NULL != dev);
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0) {
        print_err("ERR: open tap dev failed\n");
        return -1;
    }

    CLEAR(ifr);
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if ((err =ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        print_err("ERR: ioctl tap dev failed\n");
        close(fd);
        return err;
    }

    strncpy(dev, ifr.ifr_name, DEV_MAX_SIZE);
    return fd;
}

int tuntap_init() {
    dev = calloc(DEV_MAX_SIZE, 1);
    if (NULL == dev) {
        print_err("ERR: calloc failed\n");
        return -1;
    }

    if ((tuntap_fd = tuntap_alloc(dev)) < 0) {
        return -1;
    }
    if (set_if_up(dev) != 0) {
        print_err("ERR: when setting up if\n");
    }

    if (set_if_route(dev, tuntap_route) != 0) {
        print_err("ERR: when setting route for if\n");
    }

    if (set_if_address(dev, tuntap_ip) != 0) {
        print_err("ERR: when setting addr for if\n");
    }
    return 0;
}

void tuntap_free() {
    free(dev);
}

int tuntap_read(char *buf, int len) {
    return read(tuntap_fd, buf, len);
}

int tuntap_write(char *buf, int len) {
    return write(tuntap_fd, buf, len);
}

int tun_read(char *buf, int len)
{
    return read(tuntap_fd, buf, len);
}

int tun_write(char *buf, int len)
{
    return write(tuntap_fd, buf, len);
}