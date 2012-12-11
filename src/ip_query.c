#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip_query.h"

ip_query_t *
ip_query_create(const char *db_file)
{
    ip_query_t      *ipq;
    iq_database_t   *db;

    db = iq_database_create(db_file);
    if (db == NULL) {
        goto error;
    }
    iq_database_parse(db);
    ipq = calloc(1, sizeof(ipq));
    ipq->db = db;

    return ipq;
error:
    return NULL;
}

const char *
ip_query_find(ip_query_t *ipq, const char *ip)
{
    iq_database_t   *db;

    db = ipq->db;
    
    return iq_database_find(db, ip);
}

void
ip_query_destroy(ip_query_t *ipq)
{
    iq_database_destroy(ipq->db);
    free(ipq);
}
