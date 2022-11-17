#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    title("MAIN MENU");
    printf("Hello and welcome to the random sentences generator !\n\n");
    int user_input;
    printf("Please choose an option:\n- 1 to generate a random french sentence\n"
           "- 2 to obtain a random word from our dictionary\n"
           "- 3 to check if a word of your choice is in our dictionary\n"
           "- 0 to exit\n>>> ");
    scanf("%d", &user_input);
    if(user_input == 0)
    {
        printf("\nThanks for using our sentence generator !\nThis project was made by HATOUM Jade, RENNO Joseph, and BAUDET Quentin.\n\n");
        return 0;
    }
    else
    {
        main_menu(user_input);
        return main();
    }
    return 0;
}