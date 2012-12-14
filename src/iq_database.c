#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#include "ip_query.h"

static void iq_split_name_value(char *line, char **name, char **value);
static char *iq_get_addr(char **pvalue);
static void iq_pton_addr(char *pres, unsigned int *addr, unsigned int *bits);

iq_database_t *
iq_database_create(const char *db_file)
{
    iq_database_t   *db;
    FILE            *fp;

    fp = fopen(db_file, "r");
    if (fp == NULL) {
        goto error;
    }
    db = (iq_database_t *)calloc(1, sizeof(iq_database_t));
    db->name = strdup(db_file);
    db->fp = fp;
#ifdef IQ_MEMPOOL
    db->mempool = iq_mempool_create();
#endif
    db->trie = iq_trie_create();

    return db;
error:
    return NULL;
}

void
iq_database_destroy(iq_database_t *db)
{
#ifdef IQ_MEMPOOL
    iq_mempool_destroy(db->mempool);
#endif
    free((void *)db->name);
    iq_trie_destroy(db->trie);
    free(db);
}

void
iq_database_parse(iq_database_t *db)
{
    char    line[IQ_DATABASE_LINEMAX];

    while (fgets(line, IQ_DATABASE_LINEMAX, db->fp)) {
        iq_database_parse_line(db, line);
    }
    fclose(db->fp);
    db->fp = NULL;
}

void
iq_database_parse_line(iq_database_t *db, char *line)
{
    char            *name;
    char            *value;
    char            *addr_str;
    unsigned int    addr;
    unsigned int    bits;
    iq_trie_error_e error;

    db->line++;
    iq_split_name_value(line, &name, &value);
    name = iq_strstrip(name, " \t\r\n");
    name = strndup(name, IQ_DATABASE_NAMEMAX);
#ifdef IQ_MEMPOOL
    iq_mempool_add(db->mempool, name);
#endif
    while (value) {
        addr_str = iq_get_addr(&value);
        iq_pton_addr(addr_str, &addr, &bits);
        error = iq_trie_add(db->trie, name, addr, bits);
#ifdef IQ_DEBUG
        if (error == IQ_TRIE_UNUSED) {
            printf("%d: %s = %s unused!\n", db->line, name, addr_str);
            exit(EXIT_FAILURE);
        }
#endif
    }
}

const char *
iq_database_find(iq_database_t *db, const char *ip)
{
    iq_trie_node_t  *node;
    struct in_addr sin_addr;

    inet_pton(AF_INET, ip, &sin_addr);
    node = iq_trie_find(db->trie, sin_addr.s_addr);

    return node->name;
}

static void
iq_split_name_value(char *line, char **name, char **value)
{
    const char      *delim = "=";  
    char            *saveptr;

    *name = strtok_r(line, delim, &saveptr);
    *value = strtok_r(NULL, delim, &saveptr);
}

static char *
iq_get_addr(char **pvalue)
{
    char            *addr;
    char            *saveptr;
    const char      *delim = " \t\r\n";  
    char            *p;
    size_t          len;

    len = strlen(*pvalue);
    addr = strtok_r(*pvalue, delim, &saveptr);
    p = addr+strlen(addr);
    while (p - *pvalue < len && *p == '\0') {
        p++;
    }
    *pvalue = *p != '\0' ? p : NULL;

    return addr;
}

static void
iq_pton_addr(char *pres, unsigned int *addr, unsigned int *bits)
{
    char            *token;
    const char      *delim = "/";  
    char            *saveptr;
    struct in_addr  sin_addr;

    token = strtok_r(pres, delim, &saveptr);
    inet_pton(AF_INET, token, &sin_addr);
    *addr = sin_addr.s_addr;
    token = strtok_r(NULL, delim, &saveptr);
    if (token) {
        *bits = atoi(token);
    } else {
        *bits = 32;
    }
}
