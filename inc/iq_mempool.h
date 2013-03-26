#ifndef _IQ_MEMPOOL_H_
#define _IQ_MEMPOOL_H_

typedef struct iq_mempool_node_s {
    void                        *data;
    struct iq_mempool_node_s    *next;
} iq_mempool_node_t;

typedef struct iq_mempool_s {
    iq_mempool_node_t   *head;
} iq_mempool_t;

static inline iq_mempool_t *
iq_mempool_create()
{
    iq_mempool_t        *mempool;
    iq_mempool_node_t   *head;

    mempool = calloc(1, sizeof(iq_mempool_t));
    mempool->head = NULL;

    return mempool;
}

static inline void
iq_mempool_add(iq_mempool_t *mempool, void *data)
{
    iq_mempool_node_t   *node;

    node = calloc(1, sizeof(iq_mempool_node_t));
    node->data = data;
    node->next = mempool->head;
    mempool->head = node;
}

static inline void
iq_mempool_destroy(iq_mempool_t *mempool)
{
    iq_mempool_node_t   *node;


    for(node = mempool->head; node; node = mempool->head) {
        mempool->head = mempool->head->next; 
        free(node->data);
        free(node);
    }
    free(mempool);
}

#endif //_IQ_MEMPOOL_H_
