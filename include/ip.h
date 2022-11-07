#ifndef IP_H
#define IP_H

#include "syshead.h"

static inline uint32_t ip_parse(char *addr)
{
    uint32_t dst = 0;
    
    if (inet_pton(AF_INET, addr, &dst) != 1) {
        perror("ERR: Parsing inet address failed");
        exit(1);
    }

    return ntohl(dst);
}

#endif