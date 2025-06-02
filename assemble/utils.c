#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char* trimWS(char *str) {
    //Remove leading whitespaces
    while (isspace(*str)) str++;
    //Remove trailing whitespaces
    char *end = str + strlen(str) -1;
    while (end > str && isspace(*end)) end--;
    *(end +1) = '\0'; 

    return str;
}
