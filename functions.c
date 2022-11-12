#include "functions.h"

void split_line(char line[110], char** f_flechie, char** f_base, char** category, char** type)
{
    int i, type_size;
    
    char * token = strtok(line, "\t");
    *f_flechie = token;

    token = strtok(NULL, "\t");
    *f_base = token;
    token = strtok(NULL, "\t");

    *category = malloc(3 + 1);

    for(i = 0; i < 3; i++)
        (*category)[i] = token[i];
    (*category)[i] = '\0';

    type_size = strlen(token) - 3;
    *type = malloc(type_size + 1);

    for(i = 3; i <= type_size+1; i++)
        (*type)[i-3] = token[i];
    (*type)[i-3] = '\0';

    return;
}



