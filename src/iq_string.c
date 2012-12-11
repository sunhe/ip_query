#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip_query.h"

char *
iq_strstrip(char *str, const char *chars)
{
    char            *saveptr;
    return strtok_r(str, chars, &saveptr);
}
