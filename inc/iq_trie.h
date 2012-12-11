#ifndef _IQ_TRIE_H_
#define _IQ_TRIE_H_

#define IQ_IP_TABLE_BITS        8
#define IQ_IP_TABLE_SIZE        (1 << IQ_IP_TABLE_BITS)

typedef enum {
    IQ_TRIE_OK = 0,
    IQ_TRIE_UNUSED
} iq_trie_error_e;

typedef struct iq_trie_node_s {
    const char              *name;
    unsigned int            bits;
    struct iq_trie_node_s   *child[IQ_IP_TABLE_SIZE];
} iq_trie_node_t;

typedef struct iq_trie_s {
    iq_trie_node_t      *root;
#ifdef IQ_MEMPOOL
    iq_mempool_t        *mempool;
#endif
} iq_trie_t;

iq_trie_t *iq_trie_create();
iq_trie_error_e iq_trie_add(iq_trie_t *tire, const char *name, unsigned int addr, unsigned int bits);
iq_trie_node_t *iq_trie_find(iq_trie_t *tire, unsigned int addr);
void iq_trie_destroy(iq_trie_t *tire);

#endif //_IP_TRIE_H_
