#include "menu.h"


void main_menu()
{
    title("MAIN MENU");
    printf("Hello and welcome to the random sentences generator !\n\n");
    int user_input;
    printf("Please choose an option:\n- 1 to generate a random french sentence\n"
           "- 2 to obtain a random word from our dictionary\n"
           "- 3 to check if a word of your choice is in our dictionary\n"
           "- 0 to exit\n>>> ");
    scanf("%d", &user_input);
    t_tree * my_trees = fill_trees();
    switch (user_input)
    {
        case 1:
            generate_sentence(my_trees);
            break;
        case 2:
            extract_random_word();
            break;
        case 3:
        {
            //char word[30];
            //printf("Please enter the word you want to check (forme de base):\n>>> ");
            //gets(word);
            //search_fbase((*trees)[0].roots, "chat", 0);
            break;
        } 
        default:
            break;
    }
    printf("\nThanks for using our sentence generator !\nThis project was made by HATOUM Jade, RENNO Joseph, and BAUDET Quentin.\n");

}

void generate_sentence(t_tree * my_trees)
{
    title("GENERATE RANDOM SENTENCE");
    int sentence_model;
    int choice_form;
    printf("You can generate a sentence from three different models :\n"
           "1 - Noun - Adjective - Verb - Noun\n"
           "2 - Noun - 'Qui' - Verb - Verb - Noun - Adjective\n"
           "3 - Not yet implemented\n\n");
    printf("Please choose a model (enter anything else to return to the main menu):\n>>> ");
    scanf("%d", &sentence_model);
    switch (sentence_model)
    {
        
        case 1:
            
            printf("Please choose whether you want it in base form or contracted form:\n"
                   "1 - Base form\n"
                   "2 - Contracted form\n>>> ");

            scanf("%d", &choice_form);
            if(choice_form == 1)
            {
                generate_modele1(my_trees,0);
            }
            else if(choice_form == 2)
            {
                generate_modele1(my_trees,1);
            }
            else
            {
                printf("Invalid Input\n");
            }

            break;
        case 2:
            //generate_modele2();
            break;
        case 3:
            //generate_modele3();
            break;
        default:
            main_menu();
            break;
    }
}

void extract_random_word()
{
    printf("Here, you can get a random word from our dictionary.\nPlease choose the type of word you want : ");
    int word_type;
    printf("1 - Noun\n2 - Verb\n3 - Adjective\n4 - Adverb\n>>> ");
    scanf("%d", &word_type);
    switch (word_type)
    {
        case 1:
            //extract_random_fbase(noun_tree);
            break;
        case 2:
            //extract_random_fbase(verb_tree);
            break;
        case 3:
            //extract_random_fbase(adj_tree);
            break;
        case 4:
            //extract_random_fbase(adv_tree);
            break;
        default:
            main_menu();
            break;
    }
}    


void title(char *title)
{
    printf("\n\n================================ %s ================================\n\n", title);
}