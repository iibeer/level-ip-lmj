#ifndef SKBUFF_H
#define SKBUFF_H

#include "syshead.h"
#include "list.h"

/**
 * @brief sk_buff示意图，当前只实现线性部分
 * head ------------> ------------------    ------------------------------
 *                        headroom                                 |
 * data ------------> ------------------    ------------------     |
 *                                                   |             |
 *                         data                      |             |
 *                               -------    ----    len            |
 *                                 |          |      |             |
 *                               payload    dlen     |             |
 *                                 |          |      |             |
 * end  ------------> ------------------    ------------------------------
 */
struct sk_buff {
    struct list_head list;
    struct netdev *dev;
    int refcnt;
    uint16_t protocol;
    uint32_t len;
    uint32_t dlen;
    uint32_t seq;
    uint32_t end_seq;
    uint8_t *head;
    uint8_t *data;
    uint8_t *end;
    uint8_t *payload;
};

struct sk_buff *skb_alloc(unsigned int size);
void skb_free(struct sk_buff *skb);
uint8_t *skb_push(struct sk_buff *skb, unsigned int len);
uint8_t *skb_head(struct sk_buff *skb);
void *skb_reserve(struct sk_buff *skb, unsigned int len);
void skb_reset_header(struct sk_buff *skb);


struct sk_buff_head {
    struct list_head head;

    uint32_t qlen;
};

static inline uint32_t skb_queue_len(const struct sk_buff_head *list)
{
    return list->qlen;
}

static inline void skb_queue_init(struct sk_buff_head *list)
{
    list_init(&list->head);
    list->qlen = 0;
}

static inline void skb_queue_add(struct sk_buff_head *list, struct sk_buff *new, struct sk_buff *next)
{
    list_add_tail(&new->list, &next->list);
    list->qlen += 1;
}

static inline void skb_queue_head(struct sk_buff_head *list, struct sk_buff *new)
{
    list_add_tail(&new->list, &list->head);
    list->qlen += 1;
}

static inline struct sk_buff *skb_dequeue(struct sk_buff_head *list)
{
    struct sk_buff *skb = list_first_entry(&list->head, struct sk_buff, list);
    list_del(&skb->list);
    list->qlen -= 1;

    return skb;
}

static inline int skb_queue_empty(const struct sk_buff_head *list)
{
    return skb_queue_len(list) < 1;
}

static inline struct sk_buff *skb_peek(struct sk_buff_head *list)
{
    if (skb_queue_empty(list)) return NULL;
        
    return list_first_entry(&list->head, struct sk_buff, list);
}

static inline void skb_queue_free(struct sk_buff_head *list)
{
    struct sk_buff *skb = NULL;
    
    while ((skb = skb_peek(list)) != NULL) {
        skb_dequeue(list);
        skb->refcnt--;
        skb_free(skb);
    }
}

#endif