#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    
    clock_t start, end;
    start = clock();
    t_tree * my_trees =  fill_trees();//Creation of all the trees of words that will be used in the rest of the program
    end = clock();
    end = clock();
    printf("\n\n=============================================\n Time taken for tree generation : %f s\n=============================================\n\n", (double)(end - start) / CLOCKS_PER_SEC); 
    
    main_menu(my_trees);//Launch of the main menu

    printf("\nThanks for using our sentence generator !\nThis project was made by HATOUM Jade, RENNO Joseph, and BAUDET Quentin.\n\n");
    return 0;
}