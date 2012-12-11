#ifndef _IQ_DATABASE_H_
#define _IQ_DATABASE_H_

#define IQ_DATABASE_LINEMAX     4096
#define IQ_DATABASE_NAMEMAX     4096

#include "iq_mempool.h"
#include "iq_trie.h"

typedef struct iq_database_s {
    const char      *name;
    FILE            *fp;
    int             line;
#ifdef IQ_MEMPOOL
    iq_mempool_t    *mempool;
#endif
    iq_trie_t       *trie;
} iq_database_t;

iq_database_t *iq_database_create(const char *db_file);
void iq_database_destroy(iq_database_t *db);
void iq_database_parse(iq_database_t *db);
void iq_database_parse_line(iq_database_t *db, char *line);
const char *iq_database_find(iq_database_t *db, const char *ip);

#endif //_IQ_DATABASE_H_
