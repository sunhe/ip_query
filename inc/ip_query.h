#ifndef _IP_QUERY_H_
#define _IP_QUERY_H_

#include "iq_mempool.h"
#include "iq_string.h"
#include "iq_database.h"
#include "iq_trie.h"

#define IQ_BUF_SIZE     4096
#define IQ_MAX_LINE     4096
#define IQ_MAX_NAME     256 

typedef struct ip_query_s {
    iq_database_t   *db;
} ip_query_t;

ip_query_t *ip_query_create(const char *db_file);
const char *ip_query_find(ip_query_t *ipq, const char *ip);
void ip_query_destroy(ip_query_t *ipq);

#endif //_IP_QUERY_H_
