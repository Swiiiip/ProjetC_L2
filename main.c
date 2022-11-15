#include "menu.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    srand(time(NULL));
   
    clock_t start, end;
    start = clock();
    
    main_menu();

    
    end = clock() ;
    printf("\n\n========================\n Time taken: %f s\n========================\n\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    return 0;
}