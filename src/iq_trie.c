#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "ip_query.h"

static iq_trie_node_t *iq_trie_create_leaf(iq_trie_t *trie, const char *name, unsigned int bits);
static int iq_trie_is_leaf(iq_trie_node_t *node);
static iq_trie_node_t *iq_trie_grow(iq_trie_t *trie, iq_trie_node_t *child);
static unsigned int iq_trie_btom(unsigned int bits);
static int iq_trie_update(iq_trie_node_t **pnode, iq_trie_node_t *new_leaf);

iq_trie_t *
iq_trie_create()
{
    iq_trie_t       *trie;
    iq_trie_node_t  *leaf;

    trie = (iq_trie_t *)calloc(1, sizeof(iq_trie_t));
#ifdef IQ_MEMPOOL
    trie->mempool = iq_mempool_create();
#endif
    leaf = iq_trie_create_leaf(trie, NULL, 0);
    trie->root = iq_trie_grow(trie, leaf);

    return trie;
}

void
iq_trie_destroy(iq_trie_t *trie)
{
#ifdef IQ_MEMPOOL
    iq_mempool_destroy(trie->mempool);
#endif
    free(trie);
}

iq_trie_error_e
iq_trie_add(iq_trie_t *trie, const char *name, unsigned int addr, unsigned int bits)
{
    iq_trie_node_t  *node;
    iq_trie_node_t  *leaf;
    iq_trie_node_t  *new_leaf;
    unsigned int    mask;
    unsigned int    len;
    unsigned int    min;
    unsigned int    max;
    unsigned int    i;
    int             b;
    int             used = 0;

    mask = iq_trie_btom(bits);
    addr = ntohl(addr & mask);
    node = trie->root;
    new_leaf = iq_trie_create_leaf(trie, name, bits);
    len = bits;
    while (len > IQ_IP_TABLE_BITS) {
        b = addr >> (32 - IQ_IP_TABLE_BITS);
        leaf = node->child[b];
        if (iq_trie_is_leaf(leaf)) {
            node->child[b] = iq_trie_grow(trie, leaf);
        }
        node = node->child[b];
        addr <<= IQ_IP_TABLE_BITS;
        len -= IQ_IP_TABLE_BITS;
    }
    min = addr >> (32 - IQ_IP_TABLE_BITS);
    max = min + (1 << (IQ_IP_TABLE_BITS - len));
    for (i=min; i<max; i++) {
        used |= iq_trie_update(&node->child[i], new_leaf);
    }

    return used? IQ_TRIE_OK : IQ_TRIE_UNUSED;
}

iq_trie_node_t *
iq_trie_find(iq_trie_t *trie, unsigned int addr)
{
    iq_trie_node_t  *node;
    unsigned char   *b;
    int             i;

    node = trie->root;
    b = (unsigned char *)&addr;
    for (i=0; i<4; i++) {
        node = node->child[b[i]];
        if (node == NULL) {
            return NULL;
        }
    }

    return node;
}

static iq_trie_node_t *
iq_trie_create_leaf(iq_trie_t *trie, const char *name, unsigned int bits)
{
    iq_trie_node_t  *leaf;
    int             i;

    leaf = (iq_trie_node_t *)calloc(1, sizeof(iq_trie_node_t));
#ifdef IQ_MEMPOOL
    iq_mempool_add(trie->mempool, leaf);
#endif
    leaf->name = name;
    leaf->bits = bits;
    for (i=0; i<IQ_IP_TABLE_SIZE; i++) {
        leaf->child[i] = leaf;
    }

    return leaf;
}

static int
iq_trie_is_leaf(iq_trie_node_t *node)
{
    return node->child[0] == node;
}

static iq_trie_node_t *
iq_trie_grow(iq_trie_t *trie, iq_trie_node_t *child)
{
    iq_trie_node_t  *parent;
    int             i;

    parent = (iq_trie_node_t *)calloc(1, sizeof(iq_trie_node_t));
#ifdef IQ_MEMPOOL
    iq_mempool_add(trie->mempool, parent);
#endif
    for (i=0; i<IQ_IP_TABLE_SIZE; i++) {
        parent->child[i] = child;
    }

    return parent;
}

static unsigned int
iq_trie_btom(unsigned int bits)
{
    unsigned int    mask;

    if (bits > 32) {
#ifdef IQ_DEBUG
        printf("%d is too big!\n", bits);
#endif
        exit(EXIT_FAILURE);
    }
    mask = 1UL << (32 - bits);
    mask--;
    mask = ~mask;
    mask = htonl(mask);

    return mask;
}

static int
iq_trie_update(iq_trie_node_t **pnode, iq_trie_node_t *new_leaf)
{
    iq_trie_node_t  *node;
    iq_trie_node_t  *leaf;
    int             i;
    int             used = 0;

    node = *pnode;
    if (iq_trie_is_leaf(node)) {
        if (node->bits < new_leaf->bits) {
            *pnode = new_leaf;
            used = 1;
        }
        goto end;
    }
    for (i=0; i<IQ_IP_TABLE_SIZE; i++) {
        leaf = node->child[i];
        if (iq_trie_is_leaf(leaf)) {
            if (leaf->bits < new_leaf->bits) {
                node->child[i] = new_leaf;
                used = 1;
            }
        } else {
            used |= iq_trie_update(&node->child[i], new_leaf);
        }
    }
end:
    return used;
}
