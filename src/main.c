#include "syshead.h"
#include "utils.h"
#include "tuntap_if.h"

int main() {
    if (tuntap_init() < 0) {
        return -1;
    }
    
    char *buff = calloc(1, 1600);
    if (NULL == buff) {
        print_err("ERR: calloc buff failed\n")
    }
    while (1) {
        if (tuntap_read(buff, 1600) < 0) {
            print_err("ERR: tuntap_read failed\n");
        }
        print_dbg("DBG: buff(%s)\n", buff);
    }
    // int tuntap_write(char *buf, int len);
    // tuntap_free();
    return 0;
}