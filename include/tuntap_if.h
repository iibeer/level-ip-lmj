#ifndef TUNTAP_IF_H
#define TUNTAP_IF_H

int tuntap_init();
void tuntap_free();
int tuntap_read(char *buf, int len);
int tuntap_write(char *buf, int len);

#endif