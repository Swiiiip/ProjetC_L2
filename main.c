#include "structures.h"
#include <time.h>

int main()
{
   
    clock_t start, end;
    start = clock();
   
    fill_trees();
    
    end = clock();
    printf("========================\n Time taken: %f s\n========================\n\n", (double)(end - start) / CLOCKS_PER_SEC);
    

    return 0;
}