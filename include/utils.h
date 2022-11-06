#ifndef UTILS_H
#define UTILS_H

#define CMDBUFLEN 100

#define print_dbg(str, ...) \
    printf(str" - %s:%u\n", ##__VA_ARGS__, __FILE__, __LINE__);

#define print_err(str, ...) \
    fprintf(stderr, str, ##__VA_ARGS__);


int run_cmd(char *cmd, ...);

#endif