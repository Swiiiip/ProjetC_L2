#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void split_line(char line[110], char** f_flechie, char** f_base, char** category, char** type); //splits line into 4 strings (f_flechie, f_base, category, type)
int intVerify(int min, int max, char input[]);
int conversion_type(char* type, int mode);
char * conversion_string(int type, int mode);
char * generate_random_type(int mode);