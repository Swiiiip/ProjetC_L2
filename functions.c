#include "functions.h"


char * create_sub_lines(char line[110], char seperator, int * index_line){

    
    
    int index_string = 0;
    int size = 0;
    int temp = * index_line;
    while(line[temp]!= seperator)
        {
            temp++;
            size ++;//to determine the size of the sub_line
        }
    


    char * sub_line = (char*)malloc(sizeof(char)*size);//allocate the space for the sub_line
    
   

    while(line[*index_line]!= seperator)
        {

            
            sub_line[index_string] = line[*index_line];
            index_string ++;
            *index_line = *index_line + 1;
        }

        sub_line[index_string] = '\0';
        *index_line = *index_line + 1;

        return sub_line;


}



