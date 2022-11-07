#include "skbuff.h"
#include "utils.h"

struct sk_buff *skb_alloc(unsigned int size) {
    struct sk_buff *skb = (struct sk_buff *)malloc(sizeof(struct sk_buff));
    if (NULL == skb) {
        print_err("ERR: malloc skb failed\n");
        return NULL;
    }
    memset(skb, 0, sizeof(struct sk_buff));
    skb->data = (uint8_t *)malloc(size);
    if (NULL == skb->data) {
        print_err("ERR: malloc skb->data failed\n");
        return NULL;
    }
    memset(skb->data, 0, size);

    skb->head = skb->data;
    skb->end = skb->data + size;
    skb->refcnt = 0;

    list_init(&skb->list);

    return skb;
}

void skb_free(struct sk_buff *skb) {
    if (skb->refcnt < 1) {
        free(skb->head);
        free(skb);
    }
}

uint8_t *skb_push(struct sk_buff *skb, unsigned int len) {
    skb->data -= len;
    skb->len += len;

    return skb->data;
}

uint8_t *skb_head(struct sk_buff *skb) {
    return skb->head;
}

void *skb_reserve(struct sk_buff *skb, unsigned int len) {
    skb->data += len;

    return skb->data;
}

void skb_reset_header(struct sk_buff *skb) {
    skb->data = skb->end - skb->dlen;
    skb->len = skb->dlen;
}