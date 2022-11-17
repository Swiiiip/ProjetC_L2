#include "menu.h"
#include <time.h>


void main_menu(int user_input)
{
    clock_t start, end;
    start = clock();
    t_tree * my_trees = fill_trees();
    int exit;
    end = clock() ;
    printf("\n\n=============================================\n Time taken for tree generation : %f s\n=============================================\n\n", (double)(end - start) / CLOCKS_PER_SEC); 
    switch (user_input)
    {
        case 1:
            generate_sentence(my_trees);
            break;
        case 2:
            extract_random_word(my_trees);
            break;
        case 3:
        {
            int choice_tree;
            char word[30];
            printf("Please choose the type of word you want to check:\n- 1 for a noun\n- 2 for a adjective\n- 3 for an adverb\n- 4 for an verb\n>>> ");
            scanf("%d", &choice_tree);
            printf("Please enter the word you want to check (forme de base):\n>>> ");
            scanf("%s",word);
            switch (choice_tree)
            {
                case 1:
                {
                    search_fbase(my_trees[NOM].roots, word, 0);
                    getchar();
                    break;
                }
                case 2:
                    search_fbase(my_trees[ADJ].roots, word, 0);
                    getchar();
                    break;
                case 3:
                    search_fbase(my_trees[ADV].roots, word, 0);
                    getchar();
                    break;
                case 4:
                    search_fbase(my_trees[VER].roots, word, 0);
                    getchar();
                    break;
                default:
                    printf("Invalid choice, please try again\n");
                    break;
            }
            break;
        } 
        default:
            break;
    }
}

void generate_sentence(t_tree * my_trees)
{
    title("GENERATE RANDOM SENTENCE");
    int sentence_model;
    int choice_form;
    printf("You can generate a sentence from three different models :\n"
           "1 - Noun - Adjective - Verb - Noun\n"
           "2 - Noun - 'Qui' - Verb - Verb - Noun - Adjective\n"
           "3 - Pronoun - Verb - Adverb - 'et' - Noun - Adjectif - Verb\n\n");
    printf("Please choose a model (enter anything else to return to the main menu):\n>>> ");
    scanf("%d", &sentence_model);

    printf("Please choose whether you want it in base form or contracted form:\n"
                   "1 - Base form\n"
                   "2 - Contracted form\n>>> ");

    scanf("%d", &choice_form);
    printf("\n");
    switch (sentence_model)
    {
        
        case 1:
            
           
            if(choice_form == 1)
            {
                generate_modele1(my_trees,0);
                getchar();
            }
            else if(choice_form == 2)
            {
                generate_modele1(my_trees,1);
                getchar();
            }
            else
            {
                printf("Invalid Input\n");
            }

            break;
        case 2:
            if(choice_form == 1)
            {
                generate_modele2(my_trees,0);
                getchar();
            }
            else if(choice_form == 2)
            {
                generate_modele2(my_trees,1);
                getchar();
            }
            else
            {
                printf("Invalid Input\n");
            }
            break;
        case 3:
            //generate_modele3();
            break;
        default:
            break;
        
    }
}

void extract_random_word(t_tree * my_trees)
{
    printf("\nHere, you can get a random word from our dictionary.\nPlease choose the type of word you want :\n");
    int word_type;
    printf("1 - Noun\n2 - Adjective\n3 - Adverb\n4 - Verb\n>>> ");
    scanf("%d", &word_type);
    switch (word_type)
    {
        case 1:
            extract_random_fbase(my_trees[NOM]);
            break;
        case 2:
            extract_random_fbase(my_trees[ADJ]);
            break;
        case 3:
            extract_random_fbase(my_trees[ADV]);
            break;
        case 4:
            extract_random_fbase(my_trees[VER]);
            break;
        default:
            break;
    }
}    


void title(char *title)
{
    printf("\n\n================================ %s ================================\n\n", title);
}