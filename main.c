#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip_query.h"

int
main(int argc, char *argv[])
{
    ip_query_t  *ipq;
    const char  *name;

    ipq = ip_query_create("ip.txt");
    name = ip_query_find(ipq, argv[1]);
    if (name) {
        printf("%s\n", name);
    }
    ip_query_destroy(ipq);

    return 0;
}
